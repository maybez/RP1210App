#include "RP1210Core.h"

#include <QMessageBox>

RP1210Core::RP1210Core(QObject *parent)
	: QObject(parent)
	, pRP1210_ClientConnect(0)
	, pRP1210_ClientDisconnect(0)
	, pRP1210_ReadMessage(0)
	, pRP1210_SendMessage(0)
	, pRP1210_SendCommand(0)
	, pRP1210_ReadVersion(0)
	, pRP1210_ReadDetailedVersion(0)
	, pRP1210_GetHardwareStatus(0)
	, pRP1210_GetErrorMsg(0)
	, pRP1210_GetLastErrorMsg(0)
	, hRP1210DLL(0)
{
}

RP1210Core* RP1210Core::GetInstance()
{
	if (!theRp1210Instance)
		theRp1210Instance = new RP1210Core(0);

	return theRp1210Instance;
}

RP1210Core::~RP1210Core()
{
	UnLoadRp1210DLL();
}

DWORD RP1210Core::LoadRp1210DLL(QString DLLPath)
{

#ifdef UNICODE
	hRP1210DLL = LoadLibrary(DLLPath.toStdWString().c_str());
	DWORD dwError = GetLastError();
#else
	hRP1210DLL = LoadLibrary(DLLPath.toStdString().c_str());
#endif

	if(hRP1210DLL == NULL)
	{
		emit LogMsg(tr("Failed to load %1,dwError = %2.").arg(DLLPath).arg(dwError));
		return dwError;
	}
	
	// 4/16/2017 : ZH : 导出函数
	pRP1210_ClientConnect       = (fxRP1210_ClientConnect)GetProcAddress(hRP1210DLL, "RP1210_ClientConnect");
	pRP1210_ClientDisconnect    = (fxRP1210_ClientDisconnect)GetProcAddress(hRP1210DLL, "RP1210_ClientDisconnect");
	pRP1210_ReadMessage         = (fxRP1210_ReadMessage)GetProcAddress(hRP1210DLL, "RP1210_ReadMessage");
	pRP1210_SendMessage         = (fxRP1210_SendMessage)GetProcAddress(hRP1210DLL, "RP1210_SendMessage");
	pRP1210_SendCommand         = (fxRP1210_SendCommand)GetProcAddress(hRP1210DLL, "RP1210_SendCommand");
	pRP1210_ReadVersion         = (fxRP1210_ReadVersion)GetProcAddress(hRP1210DLL, "RP1210_ReadVersion");
	pRP1210_ReadDetailedVersion = (fxRP1210_ReadDetailedVersion)GetProcAddress(hRP1210DLL, "RP1210_ReadDetailedVersion");
	pRP1210_GetHardwareStatus   = (fxRP1210_GetHardwareStatus)GetProcAddress(hRP1210DLL, "RP1210_GetHardwareStatus");
	pRP1210_GetErrorMsg         = (fxRP1210_GetErrorMsg)GetProcAddress(hRP1210DLL, "RP1210_GetErrorMsg");
	pRP1210_GetLastErrorMsg     = (fxRP1210_GetLastErrorMsg)GetProcAddress(hRP1210DLL, "RP1210_GetLastErrorMsg");
	
	if (NULL == pRP1210_ClientConnect)       { emit LogMsg( "\nError:    Could not find procedure RP1210_ClientConnect       in DLL!\n"); FreeLibrary(hRP1210DLL);hRP1210DLL = 0 ; return dwError; }
	if (NULL == pRP1210_ClientDisconnect)    { emit LogMsg( "\nError:    Could not find procedure RP1210_ClientDisconnect    in DLL!\n"); FreeLibrary(hRP1210DLL);hRP1210DLL = 0 ; return dwError; }
	if (NULL == pRP1210_ReadMessage)         { emit LogMsg( "\nError:    Could not find procedure RP1210_ReadMessage         in DLL!\n"); FreeLibrary(hRP1210DLL);hRP1210DLL = 0 ; return dwError; }
	if (NULL == pRP1210_SendMessage)         { emit LogMsg( "\nError:    Could not find procedure RP1210_SendMessage         in DLL!\n"); FreeLibrary(hRP1210DLL);hRP1210DLL = 0 ; return dwError; }
	if (NULL == pRP1210_SendCommand)         { emit LogMsg( "\nError:    Could not find procedure RP1210_SendCommand         in DLL!\n"); FreeLibrary(hRP1210DLL);hRP1210DLL = 0 ; return dwError; }
	if (NULL == pRP1210_ReadVersion)         { emit LogMsg( "\nError:    Could not find procedure RP1210_ReadVersion         in DLL!\n"); FreeLibrary(hRP1210DLL);hRP1210DLL = 0 ; return dwError; }
	if (NULL == pRP1210_GetHardwareStatus)   { emit LogMsg( "\nError:    Could not find procedure RP1210_GetHardwareStatus   in DLL!\n"); FreeLibrary(hRP1210DLL);hRP1210DLL = 0 ; return dwError; }
	if (NULL == pRP1210_GetErrorMsg)         { emit LogMsg( "\nError:    Could not find procedure RP1210_GetErrorMsg         in DLL!\n"); FreeLibrary(hRP1210DLL);hRP1210DLL = 0 ; return dwError; }
	if (NULL == pRP1210_ReadDetailedVersion) { emit LogMsg( "\nWarning:  Could not find procedure RP1210_ReadDetailedVersion in DLL.\n"); FreeLibrary(hRP1210DLL);hRP1210DLL = 0 ; return dwError; }
	if (NULL == pRP1210_GetLastErrorMsg)     { emit LogMsg( "\nWarning:  Could not find procedure RP1210_GetLastErrorMsg     in DLL.\n"); FreeLibrary(hRP1210DLL);hRP1210DLL = 0 ; return dwError; }

	emit LogMsg(tr("load %1 success!").arg(DLLPath));
	return 0;
} 

void RP1210Core::UnLoadRp1210DLL()
{
	if (hRP1210DLL)
	{
		pRP1210_ClientConnect = 0;
		pRP1210_ClientDisconnect = 0;
		pRP1210_ReadMessage = 0;
		pRP1210_SendMessage = 0;
		pRP1210_SendCommand = 0;
		pRP1210_ReadVersion = 0;
		pRP1210_ReadDetailedVersion = 0;
		pRP1210_GetHardwareStatus = 0;
		pRP1210_GetErrorMsg = 0;
		pRP1210_GetLastErrorMsg = 0;

		FreeLibrary(hRP1210DLL);
		hRP1210DLL = 0;
	}
}

short RP1210Core::ClientConnect(short DeviceId, QString Protocol, long SendBufferLen /*= 0*/, long ReceiveBufferLen /*= 0*/, bool IsAppPacketizingIncomingMsgs /*= false*/)
{
	short temp = 0;
	if (IsAppPacketizingIncomingMsgs)
		temp = 1;

	ClientID = pRP1210_ClientConnect(NULL_WINDOW, DeviceId, (char*)(Protocol.toStdString().c_str()), SendBufferLen, ReceiveBufferLen, temp);
	if (ClientID >= 0 && ClientID <= 127)    
	{
		emit LogMsg(tr("RP1210_ClientConnect with DeviceID = %1,%2 success! ClientID = %3.").arg(DeviceId).arg(Protocol).arg(ClientID));
		return NO_ERRORS;
	} 

	QString ErrorString = QString(tr("Call RP1210_ClientConnect with DeviceID = %1,%2 failded!\r\n%3.").arg(DeviceId).arg(Protocol).arg(GetErrorMsg(ClientID)));
	emit LogMsg(ErrorString);
	QMessageBox::critical(0, tr("RP1210 API failed!"), ErrorString);

	return ClientID;
}

short RP1210Core::ClientDisconnect()
{
	short ErrorCode = pRP1210_ClientDisconnect(ClientID);
	if (ErrorCode != NO_ERRORS)
	{
		QString ErrorString = QString(tr("Call RP1210_ClientDisconnect with ClientID = %1 failed!\r\n%2.").arg(ClientID).arg(GetErrorMsg(ErrorCode)));
		emit LogMsg(ErrorString);
		QMessageBox::critical(0, tr("RP1210 API failed!"),ErrorString);
		return ErrorCode;
	} 

	emit LogMsg("RP1210_ClientDisconnect...");
	return ErrorCode;
}

short RP1210Core::SendCommand(short CommandNumber, char* ClientCommand, short MsgSize)
{
	short ErrorCode = pRP1210_SendCommand(CommandNumber, ClientID, ClientCommand, MsgSize);
	if (ErrorCode != NO_ERRORS)
	{
		QString ErrorString = QString(tr("Call RP1210_SendCommand with CommandNumber = %1 ，MsgSize = %2 failed!\r\n%3.").arg(CommandNumber).arg(MsgSize).arg(GetErrorMsg(ErrorCode)));
		emit LogMsg(ErrorString);
		QMessageBox::critical(0, tr("RP1210 API failed!"), ErrorString);
		return ErrorCode;
	}

	emit LogMsg(tr("RP1210_SendCommand with CommandNumber %1,MsgSize %2").arg(CommandNumber).arg(MsgSize));
	return ErrorCode;
}

QString RP1210Core::GetErrorMsg(short ErrorID)
{
	char  szTemp[100] = { 0 };
	memset(szTemp, 0x00, sizeof(szTemp));
	if (ErrorID < 0)
		ErrorID = (-1 * ErrorID);

	pRP1210_GetErrorMsg(ErrorID, szTemp);

	return QString("ErrorID = %1;Msg = %2").arg(ErrorID).arg(szTemp);
}

// 4/19/2017 : ZH : claim address for j1939 protocol
short RP1210Core::ClaimJ1939Address(unsigned char ToolAddress)
{
	// 4/19/2017 : ZH : 别特么问为啥这样，我特么也是从示例代码中抄来的，没时间研究J1939/81文档了~~~
	// J1939 "NAME" for this sample source code application ( see J1939/81 )
	//    Self Configurable       =   0 = NO
	//    Industry Group          =   0 = GLOBAL
	//    Vehicle System          =   0 = Non-Specific
	//    Vehicle System Instance =   0 = First Diagnostic PC
	//    Reserved                =   0 = Must be zero
	//    Function                = 129 = Offboard Service Tool
	//    Function Instance       =   0 = First Offboard Service Tool
	//    Manufacturer Code       =  11 = Dearborn Group, Inc. 
	//    Manufacturer Identity   =   0 = Dearborn Group, Inc. Sample Source Code
	const unsigned char ucJ1939Name[8] = { 0x00, 0x00, 0x60, 0x01, 0x00, 0x81, 0x00, 0x00 };

	unsigned char TxBuffer[32] = {0};
	memset(TxBuffer, 0x00, sizeof(TxBuffer));

	TxBuffer[0] = ToolAddress;           // 设备地址
	TxBuffer[1] = ucJ1939Name[0];        // 名称？
	TxBuffer[2] = ucJ1939Name[1];
	TxBuffer[3] = ucJ1939Name[2];
	TxBuffer[4] = ucJ1939Name[3];
	TxBuffer[5] = ucJ1939Name[4];
	TxBuffer[6] = ucJ1939Name[5];
	TxBuffer[7] = ucJ1939Name[6];
	TxBuffer[8] = ucJ1939Name[7];
	TxBuffer[9] = BLOCK_UNTIL_DONE;      // 调用标记

	// 4/19/2017 : ZH : 发送数据
	short ErrorCode = SendCommand(RP1210_Protect_J1939_Address, (char*)TxBuffer, 10);
	return ErrorCode;
}

// 4/19/2017 : ZH : Set filters for j1939 protocol
short RP1210Core::SetMessageFilterForJ1939(unsigned char flag, unsigned char* PGN, unsigned char Priority, unsigned char SA, unsigned char TA)
{
	unsigned char TX[32] = { 0 };
	memset(TX, 0x00, sizeof(TX));
	TX[0] = flag;
	TX[1] = PGN[0];
	TX[2] = PGN[1];
	TX[3] = PGN[2];
	TX[4] = Priority;
	TX[5] = SA;
	TX[6] = TA;

	short ErrorCode = SendCommand(RP1210_Set_Message_Filtering_For_J1939, (char*)TX, 7);
	return ErrorCode;
}

short RP1210Core::SetJ1919FilterType(unsigned char FilterType)
{
	short ErrorCode = SendCommand(RP1210_Set_J1939_Filter_Type, (char*)(&FilterType), 1);
	return ErrorCode;
}

short RP1210Core::SetAllFilterStatesToPass()
{
	short ErrorCode = SendCommand(RP1210_Set_All_Filters_States_to_Pass, 0, 0);
	return ErrorCode;
}

short RP1210Core::SetAllFilterStatesToDiscard()
{
	short ErrorCode = SendCommand(RP1210_Set_All_Filters_States_to_Discard, 0, 0);
	return ErrorCode;
}

RP1210Core* RP1210Core::theRp1210Instance = 0;
