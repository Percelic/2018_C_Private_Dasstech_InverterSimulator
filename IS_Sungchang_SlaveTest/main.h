#include <Windows.h>
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"" )
#pragma comment(lib, "comctl32.lib")
#include <commctrl.h>
#include <process.h>	// _beginThread, _exitThread 관련 라이브러리

#define __WINDOW_MAX_X_SIZE 780
#define __WINDOW_MIN_X_SIZE 780

#define __WINDOW_MAX_Y_SIZE 340
#define __WINDOW_MIN_Y_SIZE 340

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);		//윈도우 프로시저 콜백 함수 정의
HINSTANCE g_hInst;
LPCTSTR lpszClass = "Inverter Simulator-Serial v0.22";			// Define Window name
HANDLE hUpdateThread;										// update thread Handler
HANDLE hMsgSendThread;
DWORD dwThreadId;											// store Thread Id 

HANDLE hSerialReadThread;
unsigned int WINAPI Serial_Read(void *arg);