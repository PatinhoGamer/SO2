#include <windows.h>
#include <tchar.h>

#include "ControlFunctions.h"
#include "MainBreakdown.h"
#include "resource.h" //1� passo
#include "StartException.h"

#define ADD_AIRPORT 1
#define LIST_AIRPORTS 2
#define LIST_PLANES 3
#define LIST_PASSANGERS 4
#define ACCEPT 5


//LRESULT CALLBACK TrataEventos(HWND, UINT, WPARAM, LPARAM)

typedef struct {
	HINSTANCE hInstance;

	HWND airport_name_text_field;
	HWND map_area;

	ControlMain* control;
} HANDLES_N_STUFF;


HANDLES_N_STUFF stuff;

LRESULT CALLBACK window_procedure(HWND, UINT, WPARAM, LPARAM);

void AddControls(HWND, HANDLES_N_STUFF*);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
	HWND hWnd;
	MSG msg = { 0 };
	WNDCLASSEX window_app = { 0 };

	window_app.cbSize = sizeof(WNDCLASSEX);
	window_app.hbrBackground = (HBRUSH)COLOR_WINDOW;
	window_app.hCursor = LoadCursor(nullptr, IDC_ARROW);
	window_app.hInstance = hInst;
	window_app.lpszClassName = _T("windowClass");
	window_app.lpfnWndProc = window_procedure;

	window_app.cbWndExtra = sizeof(HANDLES_N_STUFF*);

	if (!RegisterClassEx(&window_app))
		return -1;

	hWnd = CreateWindow(_T("windowClass"), _T("Controlo"), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						-10, 0, 1500, 1050, HWND_DESKTOP, NULL, hInst, 0);

	HANDLE process_lock_mutex = nullptr;
	try {
		HANDLE process_lock_mutex = lock_process();

		ControlMain* control_main = main_start();

		startAllThreads(control_main);


		if (hWnd != nullptr) {
			//SetWindowLongPtr(hWnd, 0, (LONG_PTR)&structure); // TODO make this works
			stuff.control = control_main;

			while (GetMessage(&msg, NULL, NULL, NULL)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		exit_everything(control_main);
		waitForThreadsToFinish(control_main);

		exitAndSendSentiment(control_main);

		delete control_main;
	} catch (StartException* e) {
		MessageBox(hWnd, e->get_message(), _T("Error"), MB_OK);
		delete e;
	}
	
	CloseHandle(process_lock_mutex);

	return 0;
}

LRESULT CALLBACK window_procedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	HANDLES_N_STUFF* main_struct = nullptr;

	//main_struct = (HANDLES_N_STUFF*)GetWindowLongPtr(hWnd, 0); // TODO ERROR FIX - o GetWindowLongPtr est� a retornar NULL
	main_struct = &stuff;
	main_struct->hInstance = GetModuleHandle(NULL);

	switch (msg) {
		case WM_COMMAND:
			switch (wParam) {
				case ADD_AIRPORT:
					TCHAR text[100];
					GetWindowTextW(main_struct->airport_name_text_field, text, sizeof(text));
					SetWindowTextW(main_struct->map_area, text);
					break;
			}

			break;
		case WM_CLOSE:
			if (MessageBox(hWnd, _T("Are you sure you want to exit?"), _T("Exit"), MB_YESNO) == IDYES) {
				PostQuitMessage(0);
			}
			break;
		case WM_CREATE:
			AddControls(hWnd, main_struct);
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

void AddControls(HWND hWnd, HANDLES_N_STUFF* main_struct) {

	HINSTANCE hInstance = main_struct->hInstance;

	//TODO adicionar os handles e merdas na struct e meter pa dentro
	main_struct->map_area =
		CreateWindowW(_T("static"), _T("isto vai ser o mapa e esta feito 1000x1000 "), WS_VISIBLE | WS_CHILD | WS_BORDER,
					  20, 1, 1000, 1000, hWnd, NULL, NULL, NULL);

	main_struct->airport_name_text_field =
		CreateWindowW(_T("EDIT"), 0, WS_VISIBLE | WS_CHILD | WS_BORDER, 1172, 10, 250, 30, hWnd, NULL, hInstance, NULL);

	CreateWindowW(_T("Button"), _T("Add Airport"), WS_VISIBLE | WS_CHILD, 1050, 10, 120, 30, hWnd, NULL, NULL, NULL);

	CreateWindowW(_T("EDIT"), 0, WS_VISIBLE | WS_CHILD | WS_BORDER, 1050, 50, 380, 440, hWnd, NULL, hInstance, NULL);

	CreateWindowW(_T("Button"), _T("List Airports"), WS_VISIBLE | WS_CHILD, 1050, 500, 120, 30, hWnd, NULL, NULL, NULL);
	CreateWindowW(_T("Button"), _T("List Planes"), WS_VISIBLE | WS_CHILD, 1172, 500, 120, 30, hWnd, NULL, NULL, NULL);
	CreateWindowW(_T("Button"), _T("List Passangers"), WS_VISIBLE | WS_CHILD, 1294, 500, 120, 30, hWnd, NULL, NULL, NULL);
	CreateWindowW(_T("Button"), _T("Accept"), WS_VISIBLE | WS_CHILD, 1050, 532, 120, 30, hWnd, NULL, NULL, NULL);
}

//
//LRESULT CALLBACK TrataEventos(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
//	static TCHAR letra = 'a';
//	HDC hdc, auxdc;
//	static HBITMAP bmp, bmp2, foto, double_dc; 
//	PAINTSTRUCT paint;
//	switch (messg) {
//	case WM_DESTROY:
//		DeleteObject(bmp);
//		PostQuitMessage(0);
//		break;
//
//	case WM_PAINT:
//
//		
//		hdc = BeginPaint(hWnd, &paint);
//		
//		auxdc = CreateCompatibleDC(hdc);
//		
//		SelectObject(auxdc, bmp);
//		
//		BitBlt(hdc, 10, 10, 100, 100, auxdc, 0, 0, SRCCOPY);
//		DeleteDC(auxdc);
//		EndPaint(hWnd, &paint);
//		break;
//	case WM_CREATE:
//		
//		bmp = LoadBitmap(hInstanceGlobal, MAKEINTRESOURCE(IDB_BITMAP1));
//
//		hdc = GetDC(hWnd);
//		double_dc = CreateCompatibleDC(hdc);
//		foto = CreateCompatibleBitmap(hdc, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
//		SelectObject(double_dc, foto);
//		ReleaseDC(hWnd, hdc);
//		DeleteObject(foto);
//		break;
//	case WM_KEYDOWN:
//		switch (wParam) {
//		case VK_UP:
//			letra = 'c';
//			break;
//
//		case VK_DOWN:
//			letra = 'b';
//			break;
//		}
//		break;
//
//	case WM_CHAR:
//		letra = wParam;
//		break;
//
//	case WM_LBUTTONDOWN:
//		hdc = GetDC(hWnd);
//		TextOut(hdc, LOWORD(lParam), HIWORD(lParam), &letra, 1);
//		ReleaseDC(hWnd, hdc);
//		break;
//	case WM_COMMAND:
//
//		break;
//	default:
//		
//		return(DefWindowProc(hWnd, messg, wParam, lParam));
//		break; 
//	}
//	return(0);
//}
