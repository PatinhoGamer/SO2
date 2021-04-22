#include <windows.h>
#include <tchar.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <string>
#include <vector>
#include "../EspacoAereo/Utils.h"
#include "../EspacoAereo/SharedStructContents.h"


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

	if (argc < 4) {
		tcout << _T("Missing arguments\n");
		return -1;
	}
	int capacity = _ttoi(argv[1]);
	int velocity = _ttoi(argv[2]);
	TCHAR* startingPort = argv[3];


	HANDLE semaphore_plane_counter = OpenSemaphoreW(SEMAPHORE_ALL_ACCESS, FALSE, SEMAPHORE_NAME_MAX_PLANES);
	{
		if (semaphore_plane_counter == NULL) {
			tcout << _T("Error opening semaphore -> ") << GetLastError() << endl;
			return -1;
		}
		DWORD result = WaitForSingleObject(semaphore_plane_counter, INFINITE);
		if (result != WAIT_OBJECT_0) {
			tcout << _T("Error waiting for the semaphore -> ") << GetLastError() << endl;
			return -1;
		}
	}

	while (true) {
		tcout << _T("> ");
		tstring input;
		tcin >> input;
		vector<tstring> input_parts = stringSplit(input, _T(" "));
		auto command = input_parts[0];

		if (command == _T("destiny")) {
			// put result[1] on menuset_destiny(result[1]);
		}
		else if (command == _T("board")) {

		}
		else if (command == _T("fly")) {
			// put result[1] on 
		}
		else if (command == _T("exit")) {
			// put result[1] on 
		}
		else {
			tcout << _T("----- Comands available ----- \n destiny <name>\n board\n fly\n exit") << endl;
		}
	}



	ReleaseSemaphore(semaphore_plane_counter, 1, NULL);

	//TODO Intera��o / Comandos:
	//	Definir o pr�ximo destino(depois de iniciada a viagem este n�o pode ser alterado).
	//	Embarcar passageiros(todos os que estiverem nesse aeroporto para esse destino) e que caibam no avi�o.
	//	Iniciar viagem(pode iniciar viagem sem embarcar ningu�m).
	//	Quando em viagem, o piloto n�o pode fazer nada a n�o ser terminar o programa.
	//	O piloto pode terminar o programa a qualquer altura
	//		Se o fizer a meio do voo, considera-se que houve um acidente e os passageiros perdem-se.
	//		Se o fizer num aeroporto, considera-se que o piloto se reformou.
	//		Em ambos os casos, o avi�o deixa de existir no sistema.


	//TODO Funcionalidades principais:
	//	Movimenta-se no espa�o a�reo, partindo sempre de um aeroporto em dire��o a outro.A l�gica desta
	//		movimenta��o n�o � aqui apresentada, j� que ser� fornecida uma DLL com a sua implementa��o feita
	//	Quando chega a um novo aeroporto, deve comunicar a sua chegada ao controlador a�reo.

}