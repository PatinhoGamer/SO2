#include <windows.h>
#include <tchar.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <string>

using namespace std;

#ifdef UNICODE
#define tcout wcout
#define tcin wcin
#define tstring wstring
#define tstringstream wstringstream
#else
#define tcout cout
#define tcin cin
#define tstring string
#define tstringstream stringstream
#endif

#define BUFFER_SIZE 512

int _tmain(int argc, LPTSTR argv[]) {

#ifdef UNICODE
	int val = _setmode(_fileno(stdin), _O_WTEXT);
	val = _setmode(_fileno(stdout), _O_WTEXT);
	val = _setmode(_fileno(stderr), _O_WTEXT);
#endif

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