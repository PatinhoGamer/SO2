#include <windows.h>
#include <tchar.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <string>
#include "RegistryStuff.h"
#include "Controlo.h"
#include "Utils.h"
#include "CircularBuffer.h"
#include "SharedConstants.h"

using namespace std;

#ifdef UNICODE
#define tcout std::wcout
#define tcin std::wcin
#define tstring std::wstring
#define tstringstream std::wstringstream
#else
#define tcout std::cout
#define tcin std::cin
#define tstring std::string
#define tstringstream std::stringstream
#endif


int _tmain(int argc, TCHAR** argv) {

#ifdef UNICODE
	int val = _setmode(_fileno(stdin), _O_WTEXT);
	val = _setmode(_fileno(stdout), _O_WTEXT);
	val = _setmode(_fileno(stderr), _O_WTEXT);
#endif

	HANDLE process_lock_mutex, planes_semaphore;

	//Check if is already running --------------------------------------------
	process_lock_mutex = CreateMutexW(0, FALSE, _T("Airport_Control"));
	// Tries to create a mutex with the specified name
	// If the application is already running it cant create another mutex with the same name
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		tcout << _T("Process already running\n");
		return -1;
	}
	// -----------------------------------------------------------------------


	//Get max planes amount from registry ------------------------------------
	int max_planes = get_max_planes_from_registry();
	tcout << "Max planes from registry : " << max_planes << endl;
	//Create Semaphore that control max planes at a moment
	planes_semaphore = CreateSemaphoreW(NULL, 0, max_planes, _T(SEMAPHORE_NAME_MAX_PLANES));
	if (planes_semaphore == NULL) {
		tcout << _T("Semaphore already exists -> ") << GetLastError() << endl;
		return -1;
	}
	// -----------------------------------------------------------------------


	// -----------------------------------------------------------------------
	HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
	WaitForSingleObject(timer, INFINITE);
	// -----------------------------------------------------------------------


	while (1) {
		tstring input;
		tcin >> input;

		if (input == _T("newport")) {

		}
	}


	//TODO o controlo vai ter sempre uma maneira de comunicar os os avi�es e passageiros. Talvez criar classes para esconder a merda

	//TODO Permite a cria��o de aeroportos, mediante indica��o pela interface com o utilizador do nome do aeroporto e das suas coordenadas
	//	N�o dever� existir nenhum outro aeroporto num raio de 10 �posi��es� nem nenhum outro aeroporto com esse nome. N�o existe o conceito de �remover aeroportos�

	//TODO Pode assumir que existe um n�mero m�ximo tanto de aeroportos como de avi�es. 
	// Estas quantidades dever�o estar definidas no Registry.Quando os valores m�ximos s�o atingidos, os
	// novos avi�es que se tentem ligar ao sistema ser�o ignorados at� que este tenha espa�o para eles.

	//TODO Interface com utilizador: interface gr�fica Win32 que apresenta todo o espa�o a�reo e os seus elementos. Os 
	//	aeroportos e os avi�es s�o representados graficamente de forma distinta.Esta informa��o estar� permanentemente vis�vel e sempre atualizada

	//TODO Interface com utilizador: interface gr�fica Win32 que apresenta todo o espa�o a�reo e os seus elementos. Os 
	//	aeroportos e os avi�es s�o representados graficamente de forma distinta.Esta informa��o estar� permanentemente vis�vel e sempre atualizada
	//		
	//		TODO Numa vers�o inicial (Meta 1) ser� aceite uma vers�o simplificada da interface segundo o 
	//			paradigma consola.N�o � para desenhar uma matriz, mas sim para apresentar mensagens de texto tais como �avi�o ID na posi��o x, y�.

	//TODO O controlador a�reo considerar� que um avi�o que n�o d� sinal de vida durante 3 segundos deixou de existir
	//	quer esteja em voo ou parado no aeroporto.O avi�o � identificado perante o controlador a�reo atrav�s do seu ID de processo

	//TODO Comandos:
	//	Encerrar todo o sistema (todas as aplica��es s�o notificadas).
	//	Criar novos aeroportos.
	//	Suspender / ativar a aceita��o de novos avi�es por parte dos utilizadores.
	//	Listar todos os aeroportos, avi�es e passageiros existentes no sistema(com os detalhes de cada um, por	exemplo, destino, no caso dos passageiros).
}
