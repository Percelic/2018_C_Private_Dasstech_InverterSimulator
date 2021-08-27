#include "global.h"
#include "UIDrawer.h"

HWND hLabel[MAX_LABEL] = { 0, };
HWND hEdit[MAX_EDITBOX] = { 0, };
HWND hCombo[MAX_COMBOBOX] = { 0, };
HWND hButton[MAX_BUTTON] = { 0, };

HWND hDebugButton[2] = { 0, };

HFONT hFont[10] = { 0, };


char sReady_COMPort[MAX_COMPORT][10];
char* sCombobox_Contents[MAX_COMBOBOX][16] = {  { },
												{ "110","300","600","1200","2400","4800","9600","14400","19200","38400","57600","115200","230400","460800","921600"},			// baud rate		
												{ "7 Bit", "8 Bit"},																											// data bit
												{ "none", "odd", "even", "mark", "space"},																						// parity bit
												{ "1 bit", "1.5 bit", "2 bit"},																									// stop bit
												{ "Xon/Xoff", "hardware", "none"} };																							// flow ctrl
char sComPort[MAX_COMPORT][10];
char sResult[15000] = {0,};

int nLogSize = 0;

void UIDrawer_Init() {
	hFont[0] = CreateFont(26, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "arial");
	hFont[1] = CreateFont(16, 0, 0, 0, NULL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "arial");
}

void UIDrawer_Draw(HWND ParentHwnd) {
	
	UIDrawer_Init();



	// 1. Label Drawing Part
	
	hLabel[0] = CreateWindow("Static", "Inverter Simulator v0.23",WS_VISIBLE | WS_CHILD , 30, 0, 280, 30, ParentHwnd,(HMENU)ID_LABEL1, NULL, NULL);
	hLabel[1] = CreateWindow("Static", "COM Port", WS_VISIBLE | WS_CHILD | SS_RIGHT, 10, 40, 100, 30, ParentHwnd, (HMENU)ID_LABEL2, NULL, NULL);
	hLabel[2] = CreateWindow("Static", "Baud Rate", WS_VISIBLE | WS_CHILD | SS_RIGHT, 10, 70, 100, 30, ParentHwnd, (HMENU)ID_LABEL3, NULL, NULL);
	hLabel[3] = CreateWindow("Static", "Data Bit", WS_VISIBLE | WS_CHILD | SS_RIGHT, 10, 100, 100, 30, ParentHwnd, (HMENU)ID_LABEL4, NULL, NULL);
	hLabel[4] = CreateWindow("Static", "Parity Bit", WS_VISIBLE | WS_CHILD | SS_RIGHT, 10, 130, 100, 30, ParentHwnd, (HMENU)ID_LABEL5, NULL, NULL);
	hLabel[5] = CreateWindow("Static", "Stop Bit", WS_VISIBLE | WS_CHILD | SS_RIGHT, 10, 160, 100, 30, ParentHwnd, (HMENU)ID_LABEL6, NULL, NULL);
	hLabel[6] = CreateWindow("Static", "Flow Control", WS_VISIBLE | WS_CHILD | SS_RIGHT, 10, 190, 100, 30, ParentHwnd, (HMENU)ID_LABEL7, NULL, NULL);
	hLabel[7] = CreateWindow("Static", "Generate", WS_VISIBLE | WS_CHILD | SS_RIGHT, 10, 223, 80, 30, ParentHwnd, (HMENU)ID_LABEL8, NULL, NULL);
	hLabel[8] = CreateWindow("Static", "(0.1kW)", WS_VISIBLE | WS_CHILD | SS_RIGHT, 170, 223, 60, 30, ParentHwnd, (HMENU)ID_LABEL9, NULL, NULL);

	SendMessage(hLabel[0],WM_SETFONT,(WPARAM)hFont[0], NULL);
	
	for (int i = 1; i < MAX_LABEL; i++) {
		SendMessage(hLabel[i], WM_SETFONT, (WPARAM)hFont[1], NULL);
	}

	// 2. Editbox Drawing Part


	hEdit[0] = CreateWindow("edit","",WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY | ES_MULTILINE| WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL , 250, 40, 500,215, ParentHwnd, (HMENU)ID_EDITBOX1, NULL, NULL);
	hEdit[1] = CreateWindow("edit", "0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, 220, 80, 25, ParentHwnd, (HMENU)ID_EDITBOX2, NULL, NULL);

	SendMessage(hEdit[0], WM_SETFONT, (WPARAM)hFont[1], NULL);
	SendMessage(hEdit[1], WM_SETFONT, (WPARAM)hFont[1], NULL);
	

	// 3. Combobox Drawing Part
	
	hCombo[0] = CreateWindow("Combobox", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWN, 130, 40, 100, 30, ParentHwnd, (HMENU)ID_COMBOBOX1, NULL, NULL);
	hCombo[1] = CreateWindow("Combobox", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 130, 70, 100, 30, ParentHwnd, (HMENU)ID_COMBOBOX2, NULL, NULL);
	hCombo[2] = CreateWindow("Combobox", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 130, 100, 100, 30, ParentHwnd, (HMENU)ID_COMBOBOX3, NULL, NULL);
	hCombo[3] = CreateWindow("Combobox", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 130, 130, 100, 30, ParentHwnd, (HMENU)ID_COMBOBOX4, NULL, NULL);
	hCombo[4] = CreateWindow("Combobox", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 130, 160, 100, 30, ParentHwnd, (HMENU)ID_COMBOBOX5, NULL, NULL);
	hCombo[5] = CreateWindow("Combobox", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 130, 190, 100, 30, ParentHwnd, (HMENU)ID_COMBOBOX6, NULL, NULL);

	for (int i = 0; i < MAX_COMBOBOX; i++) {
		SendMessage(hCombo[i], WM_SETFONT, (WPARAM)hFont[1], NULL);

		for (int j = 0; sCombobox_Contents[i][j] != 0; j++) {
			SendMessage(hCombo[i], CB_ADDSTRING, (WPARAM)j , (LPARAM)sCombobox_Contents[i][j]);
		}
	}

	SendMessage(hCombo[1], CB_SETCURSEL, (WPARAM)6, NULL);
	SendMessage(hCombo[2], CB_SETCURSEL, (WPARAM)1, NULL);
	SendMessage(hCombo[3], CB_SETCURSEL, (WPARAM)0, NULL);
	SendMessage(hCombo[4], CB_SETCURSEL, (WPARAM)0, NULL);
	SendMessage(hCombo[5], CB_SETCURSEL, (WPARAM)2, NULL);

	// 4. Button Drawing Part

	hButton[0] = CreateWindow("Button", "½ÃÀÛ", WS_VISIBLE | WS_CHILD , 40, 250, 190, 35, ParentHwnd, (HMENU)ID_BUTTON1,NULL,NULL);
	SendMessage(hButton[0], WM_SETFONT, (WPARAM)hFont[1], NULL);

	//hDebugButton[0] = CreateWindow("Button", "Debug1", WS_VISIBLE | WS_CHILD, 250, 0, 80, 30, ParentHwnd, (HMENU)ID_DEBUGBUTTON1, NULL, NULL);
}

void UIDrawer_ScanCOMPort() {
	int count = 0;
	memset(sReady_COMPort, 0, sizeof(sReady_COMPort));
	SendMessage(hCombo[0], CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	for (int i = 0; i < MAX_COMPORT; i++) {
		memset(sResult, 0, sizeof(sResult));
		sprintf_s(sComPort[i], "COM%d", i + 1);
		QueryDosDevice(sComPort[i], (LPSTR)sResult, 50);

		if (strlen(sResult) != 0)
		{
			sprintf_s(sReady_COMPort[count++], "COM%d", i + 1);
		}	
	}

	for (int j = 0; strlen(sReady_COMPort[j]) != 0; j++)
	{
		SendMessage(hCombo[0], CB_ADDSTRING, (WPARAM)j, (LPARAM)sReady_COMPort[j]);
	}
}

void UIDrawer_AppendLog(const char* LogText) 
{

		nLogSize = SendMessage(hEdit[0], WM_GETTEXTLENGTH, (WPARAM)NULL, (LPARAM)NULL);

		if (nLogSize >= 8192)
		{
			SendMessage(hEdit[0], WM_SETTEXT, 0, (LPARAM)"");
			InvalidateRect(g_hWnd, NULL, 0);
		}

		SendMessage(hEdit[0], EM_SETSEL, SendMessage(hEdit[0], WM_GETTEXTLENGTH, 0, 0), -1);
		SendMessage(hEdit[0], EM_REPLACESEL, 0, (LPARAM)LogText);
}