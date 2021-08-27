#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <CommCtrl.h>

/*

Receive Protocol from (Tracker) Controller :	^P###ST4  ( ### is Inverter ID )

Send Protocol to (Tracker) Controller :			^D419###, ( ### is Inverter ID )
													AAAA, ( AAAA is generated power[0.1kW] )
													BBBBBBB, ( BBBBBBB is total generated power[kWh] )
													CC  ( CC is Checksum ; sum of value between ^D and ,CC )

*/

#define ID_LABEL1		51			// Inverter Simulator Title
#define ID_LABEL2		52			// select COM PORT 
#define ID_LABEL3		53			// Baud Rate setting.
#define ID_LABEL4		54			// Data Flow setting.
#define ID_LABEL5		55			// Parity Bit setting.
#define ID_LABEL6		56			// Stop Bit setting.
#define ID_LABEL7		57			// Flow Control setting.
#define ID_LABEL8		58			// Generate setting.
#define ID_LABEL9		59			// W indicate

#define ID_EDITBOX1		101			// Log Editbox
#define ID_EDITBOX2		102			// Generate Editbox

#define ID_COMBOBOX1	151			// COM PORT set
#define ID_COMBOBOX2	152			// Baud Rate set
#define ID_COMBOBOX3	153			// Data Flow set
#define ID_COMBOBOX4	154			// Parity Bit set
#define ID_COMBOBOX5	155			// Stop Bit set
#define ID_COMBOBOX6	156			// Flow Control set

#define ID_BUTTON1		201			// Communication Start Button

#define ID_DEBUGBUTTON1 251
#define ID_DEBUGBUTTON2	252

#define MAX_COMPORT		255

#define MAX_LABEL		ID_LABEL9 - (ID_LABEL1-1)
#define MAX_EDITBOX		ID_EDITBOX2 - (ID_EDITBOX1-1)
#define MAX_COMBOBOX	ID_COMBOBOX6 - (ID_COMBOBOX1-1)
#define MAX_BUTTON		ID_BUTTON1 - (ID_BUTTON1-1)

#define MAX_COMM_LENGTH 64

static HWND g_hWnd;
static byte g_IsRun;

typedef struct __COMPort_property {
	char COM_Port[30];
	int Baudrate;
	int DataByte;
	int ParityBitKey;
	int StopBitKey;
	int FlowCtrlKey;
	int Generate;
}__COMPort_property;