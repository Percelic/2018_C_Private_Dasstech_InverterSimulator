#include "global.h"
#include "COMPort_Comm.h"
#include "UIDrawer.h"

__COMPort_property stCOMPortProp = { 0, };
char sWriteBuffer[MAX_COMM_LENGTH] = {0, };
char sReadBuffer[MAX_COMM_LENGTH] = {0, };
char sGenerateProtocol[200] = { 0, };
char sRequestID[5] = { 0, };
char sGen[100] = { 0, };
float fGen = 0.0f;
float fAccuGen = 0.0f;
int nChecksum = 0;
char sDebugMsg[200] = { 0, };

HANDLE hSerial;
DCB dcb;
COMMTIMEOUTS commTimeouts;
SYSTEMTIME stTimer_COMPort;

DWORD dwBytesRead = 0;

void COMPort_Init()
{

}

void COMPort_SetConfig(const char* _COMPort, int _Baudrate, const char* _DataByteText, const char* _ParityText, const char* _StopBitText, const char* _FlowCtrlText, float _fGenerate)
{
	

	sprintf_s(stCOMPortProp.COM_Port,"%s", _COMPort);
	stCOMPortProp.Baudrate = _Baudrate;
	
	if (strcmp(_DataByteText, "7 Bit") == 0)
		stCOMPortProp.DataByte = 7;
	else if (strcmp(_DataByteText, "8 Bit") == 0)
		stCOMPortProp.DataByte = 8;

	if (strcmp(_ParityText, "none") == 0) // none, odd, even, mark ,space
		stCOMPortProp.ParityBitKey = 0;
	else if (strcmp(_ParityText, "odd") == 0)
		stCOMPortProp.ParityBitKey = 1;
	else if (strcmp(_ParityText, "even") == 0)
		stCOMPortProp.ParityBitKey = 2;
	else if (strcmp(_ParityText, "mark") == 0)
		stCOMPortProp.ParityBitKey = 3;
	else if (strcmp(_ParityText, "space") == 0)
		stCOMPortProp.ParityBitKey = 4;

	if (strcmp(_StopBitText, "1 Bit") == 0)
		stCOMPortProp.StopBitKey = 0;
	if (strcmp(_StopBitText, "1.5 Bit") == 0)
		stCOMPortProp.StopBitKey = 1;
	if (strcmp(_StopBitText, "2 Bit") == 0)
		stCOMPortProp.StopBitKey = 2;
	

	fGen = _fGenerate;

	//stCOMPortProp.FlowCtrlKey = ;

	
}

void COMPort_Connect()
{
	char COMoverAdder[25] = "\\\\.\\";
	DWORD status;
	memset(sDebugMsg, 0, sizeof(sDebugMsg));


	//dcb.BaudRate = a;
	if (strlen(stCOMPortProp.COM_Port) >= 5)	{
		strcat_s(COMoverAdder, sizeof(COMoverAdder), stCOMPortProp.COM_Port);
		sprintf_s(stCOMPortProp.COM_Port,sizeof(stCOMPortProp.COM_Port),"%s",COMoverAdder);
	}
	if ((hSerial = CreateFile(stCOMPortProp.COM_Port, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0))) 
	{
		printf("COM 접속 성공.\n");
	}

	//if (!DeviceIoControl(hSerial, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &status, NULL)) 
	//{
	//	DWORD err = GetLastError();
	//	sprintf_s(sDebugMsg,sizeof(sDebugMsg),"[Debug] Dismount volume > %d occured! \n",err);
	//	UIDrawer_AppendLog(sDebugMsg);
	//}

	//if (!DeviceIoControl(hSerial, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &status, NULL)) 
	//{
	//	//DWORD err = GetLastError();
	//	sprintf_s(sDebugMsg, sizeof(sDebugMsg), "[Debug] Lock volume > %d occured! \n", GetLastError());
	//	UIDrawer_AppendLog(sDebugMsg);
	//}

	dcb.BaudRate = stCOMPortProp.Baudrate;
	dcb.ByteSize = stCOMPortProp.DataByte;
	dcb.Parity = stCOMPortProp.ParityBitKey;
	dcb.StopBits = stCOMPortProp.StopBitKey;

	if (SetCommState(hSerial, &dcb) == -1)
	{
		printf("포트정보 세팅 실패\n");
	}

	commTimeouts.ReadIntervalTimeout = 18;
	//commTimeouts.ReadTotalTimeoutConstant = 10;
	//commTimeouts.ReadTotalTimeoutMultiplier = 50;

	commTimeouts.ReadTotalTimeoutConstant = 10;
	commTimeouts.ReadTotalTimeoutMultiplier = 50;
	
	commTimeouts.WriteTotalTimeoutConstant = 50;
	commTimeouts.WriteTotalTimeoutMultiplier = 10;

	if (SetCommTimeouts(hSerial, &commTimeouts) == -1)
	{
		printf("타임아웃 세팅 실패\n");
	}

}

void COMPort_Disconnect()
{
	CloseHandle(hSerial);
}

void COMPort_Write(byte addTime,const char* _writeText) 
{
	DWORD dwByte;

	printf("attempt to WriteFile %d occured ! \n", WriteFile(hSerial, _writeText, strlen(_writeText), &dwByte, 0));
		printf("send > %s \n", _writeText);
		
		if (addTime) 
		{
			char sTimeSend[100] = { 0, };

			GetLocalTime(&stTimer_COMPort);
			sprintf_s(sTimeSend, sizeof(sTimeSend), "[ %02d:%02d:%02d-%02d:%02d:%02d :: Send] >>", stTimer_COMPort.wYear - 2000, stTimer_COMPort.wMonth, stTimer_COMPort.wDay, stTimer_COMPort.wHour, stTimer_COMPort.wMinute, stTimer_COMPort.wSecond);
			UIDrawer_AppendLog(sTimeSend);
		}
		else
		{
			UIDrawer_AppendLog("[Send] >> ");
		}

		UIDrawer_AppendLog(_writeText);
		UIDrawer_AppendLog("\n");
}

void COMPort_ConfigNotify() 
{
	printf("COM Port is %s\n",stCOMPortProp.COM_Port);
	printf("Baudrate is %d\n",stCOMPortProp.Baudrate);
	printf("DataByte is %d\n",stCOMPortProp.DataByte);
	printf("ParityBit is %d\n",stCOMPortProp.ParityBitKey);
	printf("StopBits is %d\n",stCOMPortProp.StopBitKey);
	printf("Generate is %d\n", fGen);
	printf("==================\n");
	printf("\n");
}

void COMPort_Read(byte addTime) 
{
	nChecksum = 0;
	memset(sReadBuffer, 0, sizeof(sReadBuffer));
	ReadFile(hSerial, sReadBuffer, 64, &dwBytesRead, NULL);
	
	if (dwBytesRead > 0)
	{
		if (addTime)
		{
			char sTimeSend[100] = { 0, };

			GetLocalTime(&stTimer_COMPort);
			sprintf_s(sTimeSend, sizeof(sTimeSend), "[ %02d:%02d:%02d-%02d:%02d:%02d :: Recv] <<", stTimer_COMPort.wYear - 2000, stTimer_COMPort.wMonth, stTimer_COMPort.wDay, stTimer_COMPort.wHour, stTimer_COMPort.wMinute, stTimer_COMPort.wSecond);
			UIDrawer_AppendLog(sTimeSend);
		}

		else
		{
			UIDrawer_AppendLog("[Recv] << ");
		}
		UIDrawer_AppendLog(sReadBuffer);
		UIDrawer_AppendLog("\n");
		
		if (!strncmp(sReadBuffer, "^P", 2) && !strncmp((sReadBuffer + 5), "ST4", 3))
		{

			/* variable Initialize */
			/*UIDrawer_AppendLog("[DEBUG] Step00 Enter done\n");*/
			memset(sDebugMsg, 0, sizeof(sDebugMsg));
			_strnset_s(sGenerateProtocol,sizeof(sGenerateProtocol), 0, sizeof(sGenerateProtocol));
			strncpy_s(sRequestID, sizeof(sRequestID), (sReadBuffer + 2), 3);

			printf("Controller sent Request Generate Protocol!\n");
			printf("sRequestID is %s\n", sRequestID);

			fAccuGen += (fGen / 10.0f);
			/* end variable Initialize */

			/*UIDrawer_AppendLog("[DEBUG] Step01 Init done\n");*/

			/* calculate checksum  */
			sprintf_s(sGenerateProtocol, sizeof(sGenerateProtocol), "419%s%04d%07d", sRequestID, (int)fGen, (int)fAccuGen, nChecksum);

			for (int i = 0; i < strlen(sGenerateProtocol); i++) {
				int _checksumadd = 0;
				_checksumadd = (sGenerateProtocol[i] - 0x30);
				nChecksum += _checksumadd;
			}
			/* end calculate checksum */

			/*UIDrawer_AppendLog("[DEBUG] Step02 calculate CS done\n");*/
			
			/* send Protocol */
			sprintf_s(sGenerateProtocol, sizeof(sGenerateProtocol), "^D419%s,%04d,%07d,%2d", sRequestID, (int)fGen, (int)fAccuGen, nChecksum);

			/*sprintf_s(sDebugMsg, sizeof(sDebugMsg),"[DEBUG] Step03 : Send Protocol : %s\n", sGenerateProtocol);
			UIDrawer_AppendLog(sDebugMsg);

			sprintf_s(sDebugMsg, sizeof(sDebugMsg), "[DEBUG] Step04  : Send Protocol size [ %d / %d ]\n",strlen(sGenerateProtocol) ,sizeof(sGenerateProtocol));
			UIDrawer_AppendLog(sDebugMsg);*/

			printf("sGenerateProtocol is > %s\n",sGenerateProtocol);
			printf("sGenerateProtocol size is %d / %d\n", strlen(sGenerateProtocol),sizeof(sGenerateProtocol));

			COMPort_Write(TRUE,sGenerateProtocol);

			/* end send Protocol */
		}
	}
	
}