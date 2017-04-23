

#ifndef RP1210ReadThread_h__
#define RP1210ReadThread_h__

#include <QThread>

// 4/22/2017 : ZH : 消息读取线程

class RP1210Core;

class RP1210ReadThread : public QThread
{
	Q_OBJECT

public:
	RP1210ReadThread(RP1210Core* core,QObject *parent);
	~RP1210ReadThread();

	void SetNeedExit(bool needExit);

protected:
	void run() Q_DECL_OVERRIDE;

private:
	bool NeedExit;                     // 是否需要退出
	RP1210Core* rp1210Core;            // rp1210
									
signals:							
	void MsgReady(QByteArray data);    // 有消息
	void ErrorOccurred(short ErrorCode);   // 读取失败 
};



#endif // RP1210ReadThread_h__

