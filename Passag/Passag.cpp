#include <Windows.h>
#include <tchar.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <sstream>
#include <string>

#include "SharedPassagStruct.h"
#include "PassengerFunction.h"

using namespace std;

#define TSTRING std::basic_string<TCHAR>

#ifdef UNICODE
#define tcout wcout
#define tcin wcin
#define tstringstream wstringstream
#else
#define tcout cout
#define tcin cin
#define tstringstream stringstream
#endif


int _tmain(int argc, TCHAR** argv) {
#ifdef UNICODE
	int val = _setmode(_fileno(stdin), _O_WTEXT);
	val = _setmode(_fileno(stdout), _O_WTEXT);
	val = _setmode(_fileno(stderr), _O_WTEXT);
#endif

	if (argc < 4) {
		tcout << _T("Invalid Arguments : person_name origin_port destiny_port (optional)max_wait_time_seconds") << endl;
		return -1;
	}

	TSTRING person_name = argv[1];
	TSTRING origin_port = argv[2];
	TSTRING destiny_port = argv[3];
	int max_waiting_time_in_seconds = -1;
	if (argc > 4) {
		max_waiting_time_in_seconds = _ttoi(argv[4]);
	}

	DWORD process_id = GetProcessId(GetCurrentProcess());

	tcout  <<_T("Process Id : ")  << process_id << endl;
	
	TSTRING pipe_name(PIPE_NAME_PREFIX);
	{
		tstringstream oss;
		oss << process_id;
		pipe_name.append(oss.str());
	}

	const HANDLE passenger_named_pipe = CreateNamedPipe(pipe_name.c_str(), PIPE_ACCESS_INBOUND,
														PIPE_WAIT | PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
														1, 0, sizeof(PassengerMessage), 1000, nullptr);
	if (passenger_named_pipe == INVALID_HANDLE_VALUE) {
		tcout << _T("Error creating named pipe -> ") << GetLastError() << endl;
		return -1;
	}


	const HANDLE control_named_pipe = CreateFile(CONTROL_PIPE_MAIN, GENERIC_WRITE, 0, nullptr,
												 OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (control_named_pipe == INVALID_HANDLE_VALUE) {
		tcout << _T("Error opening control named pipe -> ") << GetLastError() << endl;
		CloseHandle(passenger_named_pipe);
		return -1;
	}

	PassengerMessage message;
	message.id = process_id;
	message.type = PASSENGER_TYPE_NEW_PASSENGER;
	memcpy(message.data.new_passenger.name, person_name.c_str(), (person_name.size() + 1) * sizeof(TCHAR));
	memcpy(message.data.new_passenger.origin, origin_port.c_str(), (origin_port.size() + 1) * sizeof(TCHAR));
	memcpy(message.data.new_passenger.destiny, destiny_port.c_str(), (destiny_port.size() + 1) * sizeof(TCHAR));

	DWORD bytes_written;
	WriteFile(control_named_pipe, &message, sizeof(message), &bytes_written, nullptr);

	if (!read_pipe(passenger_named_pipe, message)) {
		tcout << _T("Error connecting to pipe reader -> ") << GetLastError() << endl;
		CloseHandle(passenger_named_pipe);
		CloseHandle(control_named_pipe);
		exit(-1);
	}

	if (message.type == PASSENGER_TYPE_GOOD_AIRPORTS) {
		tcout << _T("You are now waiting to be boarded on the plane") << endl;

		//TODO max_waiting_time_in_seconds thread to wait for the shits
		//create_thread();

		receive_control_updates(passenger_named_pipe, destiny_port);

	} else if (message.type == PASSENGER_TYPE_BAD_AIRPORTS) {
		tcout << _T("Invalid airports!") << endl;
	} else {
		tcout << _T("Invalid Message from control! type -> ") << message.type << endl;
	}

	tcout << _T("Exiting...") << endl;


	if (!CloseHandle(control_named_pipe)) {
		tcout << _T("Error closing the control named pipe -> ") << GetLastError() << endl;
	}
	if (!CloseHandle(passenger_named_pipe)) {
		tcout << _T("Error closing the passenger named pipe -> ") << GetLastError() << endl;
	}

	//TODO Funcionamento: 
	//	O passageiro � atribu�do ao aeroporto origem, ficando a aguardar que exista um avi�o dispon�vel para o aeroporto destino. 
	//		Quando tal avi�o existir, o passageiro embarca automaticamente e, ao chegar ao aeroporto destino, desembarca e o programa termina.
	//		Caso tenha sido indicado um tempo de espera m�ximo, o passageiro desiste automaticamente de viajar se o tempo indicado passar 
	//			e n�o for atribu�do a nenhum avi�o, sai do aeroporto e o programa termina.

	//TODO Intera��o:
	//	O utilizador � automaticamente informado de quando embarca, da posi��o em que est� quando est� em voo, e quando chega
	//	O utilizador pode sempre interagir com esta aplica��o para a terminar. Se o fizer, considera - se que o passageiro deixou de existir

}