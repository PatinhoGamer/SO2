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

	//TODO Funcionamento: 
	//	� indicado por argumento da linha de comandos o aeroporto de origem, o aeroporto de destino, o nome do passageiro, e, opcionalmente,
	//	o tempo(em segundos) que fica a aguardar at� embarcar.Os nomes dos aeroportos origem e destino devem existir no sistema, caso contr�rio o programa termina de imediato
	//	O passageiro � atribu�do ao aeroporto origem, ficando a aguardar que exista um avi�o dispon�vel para o aeroporto destino. 
	//		Quando tal avi�o existir, o passageiro embarca automaticamente e, ao chegar ao aeroporto destino, desembarca e o programa termina.
	//		Caso tenha sido indicado um tempo de espera m�ximo, o passageiro desiste automaticamente de viajar se o tempo indicado passar 
	//			e n�o for atribu�do a nenhum avi�o, sai do aeroporto e o programa termina.

	//TODO Intera��o:
	//	O utilizador � automaticamente informado de quando embarca, da posi��o em que est� quando est� em voo, e quando chega
	//	O utilizador pode sempre interagir com esta aplica��o para a terminar. Se o fizer, considera - se que o passageiro deixou de existir
	
	
	//TODO Comandos: 
	//	Deve adicionar os comandos que considerar necess�rios para a utiliza��o do programa tendo em vista a funcionalidade requerida
}