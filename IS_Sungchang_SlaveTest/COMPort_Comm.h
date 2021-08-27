void COMPort_Init();
void COMPort_Connect();
void COMPort_Disconnect();
void COMPort_SetConfig(const char* _COMPort, int _Baudrate, const char* _DataByteText, const char* _ParityText, const char* _StopBitText, const char* _FlowCtrlText, float _fGenerate);
void COMPort_Update();
void COMPort_Write(byte addTime, const char* _writeText);
void COMPort_Read(byte addTime);
void COMPort_ConfigNotify();
