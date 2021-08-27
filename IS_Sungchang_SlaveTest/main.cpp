#define _CRT_SECURE_NO_WARNINGS			//VS 구문 사용 안전성을 위해 발생하는 오류 무시

#include "global.h"
#include "main.h"
#include "UIDrawer.h"
#include "COMPort_Comm.h"

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console" )
#endif

//HWND g_hWnd;
SYSTEMTIME stTimer;

WSADATA wsaData;
__COMPort_property stCOMPortProp1 = { 0, };

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam, int ncmdShow) {
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, ncmdShow);
	InitCommonControls();

	while (GetMessage(&Message, NULL, 0, 0)) {
		if (IsDialogMessage(hWnd, &Message) == 0) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}

	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	static HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	switch (iMessage) {

		
	case WM_CREATE:
	{
		//AllocConsole();		//콘솔 창 띄우기
		ShowWindow(hWnd, 0);
		g_hWnd = hWnd;
		g_IsRun = FALSE;
		UIDrawer_Draw(hWnd);

		//CreateFile("COM",FILE_GENERIC_READ, );
	}
		return 0;

	case WM_GETMINMAXINFO:
	{
		((MINMAXINFO*)lParam)->ptMaxTrackSize.x = __WINDOW_MAX_X_SIZE;
		((MINMAXINFO*)lParam)->ptMaxTrackSize.y = __WINDOW_MAX_Y_SIZE;
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = __WINDOW_MIN_X_SIZE;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = __WINDOW_MIN_Y_SIZE;
	}
		return 0;

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(0,0,0));
		SetBkColor(hdcStatic, RGB(255, 255, 255));
		return (INT_PTR)hBrush;
	}

	case WM_CTLCOLORLISTBOX:
	{
		HDC hdcLbx = (HDC)wParam;
		SetTextColor(hdcLbx, RGB(0, 0, 0));
		SetBkMode(hdcLbx, TRANSPARENT);
		//SetBkColor(hdcLbx, RGB(255, 255, 255));
		return (INT_PTR)hBrush;
	}

	case WM_CTLCOLOREDIT:
	{
		HDC hdcEdit = (HDC)wParam;
		srand(GetTickCount());
		SetTextColor(hdcEdit, RGB(0, 0, 0));
		//SetTextColor(hdcEdit, RGB((rand() % 255), (rand() % 255), (rand() % 255)));
		SetBkColor(hdcEdit, RGB(255, 255, 255));
		return (INT_PTR)hBrush;
	}

	case WM_COMMAND:		//버튼 클릭 시 이벤트 발생 (HMENU)Number
		if (HIWORD(wParam) == BN_CLICKED) 
		{
			switch (LOWORD(wParam)) 
			{
			case ID_BUTTON1:
				if (g_IsRun) 
				{
					DWORD dwThreadExitCode = 0;
					g_IsRun = FALSE;
					
					GetExitCodeThread(hSerialReadThread, &dwThreadExitCode);

					//while (!dwThreadExitCode);
						
					if (dwThreadExitCode) {
						CloseHandle(hSerialReadThread);
						COMPort_Disconnect();
						printf("Success to Close Handle.\n");
					}
					SendMessage(GetDlgItem(g_hWnd, ID_BUTTON1),WM_SETTEXT,(WPARAM)NULL,(LPARAM)"시작");

					EnableWindow(GetDlgItem(hWnd, ID_COMBOBOX1), TRUE);
					EnableWindow(GetDlgItem(hWnd, ID_COMBOBOX2), TRUE);
					EnableWindow(GetDlgItem(hWnd, ID_COMBOBOX3), TRUE);
					EnableWindow(GetDlgItem(hWnd, ID_COMBOBOX4), TRUE);
					EnableWindow(GetDlgItem(hWnd, ID_COMBOBOX5), TRUE);
					EnableWindow(GetDlgItem(hWnd, ID_COMBOBOX6), TRUE);
					EnableWindow(GetDlgItem(hWnd, ID_EDITBOX2), TRUE);
				}
				
				else
				{
					g_IsRun = TRUE;

					
					char sCOMPortConfig[5][10] = { 0, };
					int nCOMPortBaudrate = 0;
					
					char sGenerate[100] = { 0, };
					float fGenerate = 0.0f;

					GetDlgItemText(hWnd, ID_COMBOBOX1, sCOMPortConfig[0], sizeof(sCOMPortConfig[0]));
					nCOMPortBaudrate = GetDlgItemInt(hWnd, ID_COMBOBOX2, NULL, NULL);
					GetDlgItemText(hWnd, ID_COMBOBOX3, sCOMPortConfig[1], sizeof(sCOMPortConfig[1]));
					GetDlgItemText(hWnd, ID_COMBOBOX4, sCOMPortConfig[2], sizeof(sCOMPortConfig[2]));
					GetDlgItemText(hWnd, ID_COMBOBOX5, sCOMPortConfig[3], sizeof(sCOMPortConfig[3]));
					GetDlgItemText(hWnd, ID_COMBOBOX6, sCOMPortConfig[4], sizeof(sCOMPortConfig[4]));
					GetDlgItemText(hWnd, ID_EDITBOX2, sGenerate, sizeof(sGenerate));

					fGenerate = atof(sGenerate);

					EnableWindow(GetDlgItem(hWnd, ID_COMBOBOX1), FALSE);
					EnableWindow(GetDlgItem(hWnd, ID_COMBOBOX2), FALSE);
					EnableWindow(GetDlgItem(hWnd, ID_COMBOBOX3), FALSE);
					EnableWindow(GetDlgItem(hWnd, ID_COMBOBOX4), FALSE);
					EnableWindow(GetDlgItem(hWnd, ID_COMBOBOX5), FALSE);
					EnableWindow(GetDlgItem(hWnd, ID_COMBOBOX6), FALSE);
					EnableWindow(GetDlgItem(hWnd, ID_EDITBOX2), FALSE);

					COMPort_SetConfig(sCOMPortConfig[0],
						nCOMPortBaudrate,
						sCOMPortConfig[1],
						sCOMPortConfig[2],
						sCOMPortConfig[3],
						sCOMPortConfig[4],
						fGenerate);

					COMPort_ConfigNotify();
					COMPort_Connect();

					hSerialReadThread = (HANDLE)_beginthreadex(NULL, 0, Serial_Read, NULL, 0, (unsigned*)&dwThreadId);
					SendMessage(GetDlgItem(g_hWnd, ID_BUTTON1), WM_SETTEXT, (WPARAM)NULL, (LPARAM)"정지");

				}
				break;
			/*case ID_DEBUGBUTTON1:
			{
				COMPort_Write(TRUE,"^P419100ST4");
			}
				break;*/
			}
	
		}

		if (HIWORD(wParam) == CBN_DROPDOWN) 
		{
			switch (LOWORD(wParam)) {
				case ID_COMBOBOX1:
					UIDrawer_ScanCOMPort();
					//printf("take focus to ID_COMBOBOX1");
				break;
			}
		}

		return 0;


	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

unsigned int WINAPI Serial_Read(void *arg) {
	DWORD returnValue = 1;
	//do {
	while (g_IsRun) {
		COMPort_Read(TRUE);
	}
	//} while (_dwBytesRead > 0);

	return returnValue;
}

unsigned int WINAPI DataSend_Scheduler(void *arg) {
	DWORD returnValue = 1;

	while (g_IsRun) {
	//	Sleep( * 1000);

	}

	return returnValue;
}