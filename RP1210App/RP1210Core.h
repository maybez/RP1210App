#ifndef RP1210Core_h__
#define RP1210Core_h__

// 2017/04/16 : ZH :  RP1210协议实体

#include <QObject>
#include "RP1210.h"

class RP1210Core : public QObject
{
	Q_OBJECT

private:
	RP1210Core(QObject *parent);

public:
	static RP1210Core* GetInstance(); 
	~RP1210Core();

	DWORD LoadRp1210DLL(QString DLLPath);
	void UnLoadRp1210DLL(); 


public:
	// 4/19/2017 : ZH : RP1210 API 指针的再次简单封装调用，我不想直接使用裸函数指针来做操作.
	short ClientConnect(short DeviceId, QString Protocol,long SendBufferLen = 0,long ReceiveBufferLen = 0,bool IsAppPacketizingIncomingMsgs = false);
	short ClientDisconnect();
	short SendCommand(short CommandNumber,char* ClentCommand,short MsgSize);
	short ReadMessge(char* RxBuffer, short BufferSize, short BlockOnSend);




	QString GetErrorMsg(short ErrorID);

public:
	// 4/19/2017 : ZH : 简单封装后的RP1210_SendCommand函数的具体命令函数
	short ClaimJ1939Address(unsigned char ToolAddress);
	short SetMessageFilterForJ1939(unsigned char flag,unsigned char* PGN,unsigned char Priority,unsigned char SA,unsigned char TA);
	short SetJ1919FilterType(unsigned char FilterType);
	short SetAllFilterStatesToPass();
	short SetAllFilterStatesToDiscard();
	//Set_Message_Filtering_For_J1939

signals:
	void LogMsg(QString Msg);

private:	
	// 4/16/2017 : ZH : 动态库导出函数指针定义
	fxRP1210_ClientConnect           pRP1210_ClientConnect;
	fxRP1210_ClientDisconnect        pRP1210_ClientDisconnect;
	fxRP1210_ReadMessage             pRP1210_ReadMessage;
	fxRP1210_SendMessage             pRP1210_SendMessage;
	fxRP1210_SendCommand             pRP1210_SendCommand;
	fxRP1210_ReadVersion             pRP1210_ReadVersion;
	fxRP1210_ReadDetailedVersion     pRP1210_ReadDetailedVersion;
	fxRP1210_GetHardwareStatus       pRP1210_GetHardwareStatus;
	fxRP1210_GetErrorMsg             pRP1210_GetErrorMsg;
	fxRP1210_GetLastErrorMsg         pRP1210_GetLastErrorMsg;

private:
	HINSTANCE                        hRP1210DLL;     // 4/16/2017 : ZH : 动态库句柄
	static RP1210Core*               theRp1210Instance;


private:
	short     ClientID;    // RP1210_ClientConnect调用成功后返回的客户端id,后面动态库的调用都需要使用到此函数。
};


#endif // RP1210Core_h__


