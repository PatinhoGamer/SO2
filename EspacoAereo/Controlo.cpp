#include <windows.h>
#include <tchar.h>
#include <fcntl.h>
#include <io.h>
#include <synchapi.h>
#include <iostream>
#include <string>
#include "RegistryStuff.h"
#include "Controlo.h"
#include "Utils.h"
#include "SharedStructContents.h"
#include "TextInterface.h"
#include "ControlMain.h"
#include "ControlFunctions.h"

using namespace std;

#ifdef UNICODE
#define tcout std::wcout
#else
#define tcout std::cout
#endif


bool exit_bool = false;

BOOL WINAPI console_handler(DWORD dwType) {
	switch (dwType) {
	case CTRL_C_EVENT:
		tcout << _T("Ctrl + C\n");
		exit_bool = true;
		//TODO make da ting go down when it should
		break;
	case CTRL_BREAK_EVENT:
		tcout << _T("Ctrl + break\n");
		break;
	default:
		tcout << _T("Non Defined Event\n");
	}
	return false;
}



int _tmain(int argc, TCHAR** argv) {

#ifdef UNICODE
	int val = _setmode(_fileno(stdin), _O_WTEXT);
	val = _setmode(_fileno(stdout), _O_WTEXT);
	val = _setmode(_fileno(stderr), _O_WTEXT);
#endif

	//Check if is already running --------------------------------------------
	HANDLE process_lock_mutex = CreateMutexW(0, FALSE, _T("Airport_Control"));
	// Tries to create a mutex with the specified name
	// If the application is already running it cant create another mutex with the same name
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		tcout << _T("Process already running\n");
		return -1;
	}
	// -----------------------------------------------------------------------


	//Get max planes amount from registry ------------------------------------
	int max_planes = get_max_planes_from_registry();
	tcout << _T("Max planes from registry : ") << max_planes << endl;
	//Create Semaphore that control max planes at a moment
	HANDLE planes_semaphore = CreateSemaphoreW(nullptr, max_planes, max_planes, SEMAPHORE_MAX_PLANES);
	if (planes_semaphore == nullptr) {
		tcout << _T("Semaphore already exists -> ") << GetLastError() << endl;
		return -1;
	}
	// -----------------------------------------------------------------------


	if (!SetConsoleCtrlHandler(console_handler, true)) {
		tcout << _T("Unable to set console handler!\n");
		return -1;
	}

	//Create Shared Memory ---------------------------------------------------
	const DWORD shared_memory_size = sizeof(SharedControl) + sizeof(Plane) * max_planes; //Soma do espa�o necess�rio a alocar

	HANDLE handle_mapped_file;
	void* shared_mem_pointer = allocate_shared_memory(handle_mapped_file, shared_memory_size);
	if (shared_mem_pointer == NULL) {
		return -1;
	}


	ControlMain* control_main = nullptr;
	{
		SharedControl* shared_control = (SharedControl*)shared_mem_pointer;
		Plane* planes_start = (Plane*)&shared_control[1];


		shared_control->max_plane_amount = max_planes;
		control_main = new ControlMain(shared_control, planes_start, handle_mapped_file);

		memset(control_main->shared_control->map, MAP_EMPTY, sizeof(control_main->shared_control->map));
	}


	control_main->receiving_thread = create_thread(receive_updates, control_main);
	control_main->heartbeat_thread = create_thread(heartbeat_checker, control_main);


	enter_text_interface(control_main, &exit_bool);

	exit_everything(control_main);

	//TODO Pode assumir que existe um n�mero m�ximo tanto de aeroportos como de avi�es. 
	// Estas quantidades dever�o estar definidas no Registry.Quando os valores m�ximos s�o atingidos, os
	// novos avi�es que se tentem ligar ao sistema ser�o ignorados at� que este tenha espa�o para eles.

	//TODO Interface com utilizador: interface gr�fica Win32 que apresenta todo o espa�o a�reo e os seus elementos. Os 
	//	aeroportos e os avi�es s�o representados graficamente de forma distinta.Esta informa��o estar� permanentemente vis�vel e sempre atualizada
	//		
	//		TODO Numa vers�o inicial (Meta 1) ser� aceite uma vers�o simplificada da interface segundo o 
	//			paradigma consola.N�o � para desenhar uma matriz, mas sim para apresentar mensagens de texto tais como �avi�o ID na posi��o x, y�.

	//TODO O controlador a�reo considerar� que um avi�o que n�o d� sinal de vida durante 3 segundos deixou de existir
	//	quer esteja em voo ou parado no aeroporto.O avi�o � identificado perante o controlador a�reo atrav�s do seu ID de processo

	//TODO Comandos:
	//	Encerrar todo o sistema (todas as aplica��es s�o notificadas).
	//	Suspender / ativar a aceita��o de novos avi�es por parte dos utilizadores.
	//	Listar todos os aeroportos, avi�es e passageiros existentes no sistema(com os detalhes de cada um, por	exemplo, destino, no caso dos passageiros).
}
