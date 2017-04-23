#include "RP1210ReadThread.h"
#include "RP1210Core.h"
  
RP1210ReadThread::RP1210ReadThread(RP1210Core* core, QObject *parent)
	:QThread(parent)
	,rp1210Core(core)
	,NeedExit(false)
{

}

RP1210ReadThread::~RP1210ReadThread()
{
}

void RP1210ReadThread::SetNeedExit(bool needExit)
{
	NeedExit = needExit;
}

void RP1210ReadThread::run() 
{
	unsigned char RxBuffer[MAX_J1939_MESSAGE_LENGTH + 256] = { 0 };
	short retVal = 0;

	while (true)
	{
		if (!rp1210Core) break;

		if (NeedExit) break;

		memset(RxBuffer, 0x00, sizeof(RxBuffer));

		retVal = rp1210Core->ReadMessge((char*)&RxBuffer[0], sizeof(RxBuffer), NON_BLOCKING_IO);

		if (retVal > 0)
		{ // 4/23/2017 : ZH : 读取到消息
			emit MsgReady(QByteArray((char*)(&RxBuffer[0]), retVal));
		}
		else if (retVal == 0)
		{ // 4/23/2017 : ZH : 暂时没有消息
			QThread::msleep(10);
		}
		else
		{ // 4/23/2017 : ZH : 发生错误
			emit ErrorOccurred(retVal*(-1));
			break;
		}
	} 
}
 