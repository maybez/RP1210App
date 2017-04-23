
#ifndef MessageDef_h__
#define MessageDef_h__

// 4/23/2017 : ZH : RP1210协议中涉及到的各种消息的定义

#include <QString>
#include <QByteArray>

// 4/23/2017 : ZH : 消息基类
class MessageBase
{
public:
	MessageBase(QByteArray rawMsg,bool useEcho = false);
	~MessageBase(); 


	QString ByteArrayToQString(QByteArray arr) const;

	// 4/23/2017 : ZH : 把RawMsg中的每个字节都格式化为16进制形式组成的字符串
	QString GetRawMsgString();
	QString GetRawMsgString()const;

	// 4/23/2017 : ZH : 子类必须实现的接口，子类可能有不同的转成字符串的样子
	virtual QString GetMsgDataString()const = 0;
	
	// 4/23/2017 : ZH : 获取时间戳
	unsigned int GetTimeStamp() const;

	// 4/23/2017 : ZH : 获取Echo字节
	char GetEchoByte() const;	

protected:
	unsigned int TimeStamp;      // 时间戳  : 4字节，大端存储
	unsigned char Echo;          // echo标记：只有当IsEcho为True的时候才有效
	QByteArray Data;             // 消息载荷

	bool UseEcho;                // 是否含使用echo标记 	
	QByteArray RawMsg;           // 最原始的消息
	QString    RawMsgStr;        // 字符串形式
};

class J1939Message : public MessageBase
{
public:
	J1939Message(QByteArray rawMsg, bool useEcho = false);
	~J1939Message();

public:
	int GetPGN() const;
	unsigned char GetPriority() const;
	unsigned char GetSA() const;
	unsigned char GetTA() const;
	QByteArray GetJ1939Data() const;
	QString GetMsgDataString()const override; 

protected:
	int PGN;                      // PGN：三个字节，小端存储
	unsigned char Priority;       // Priority:一个字节
	unsigned char SA;             // SA：一个字节   
	unsigned char TA;             // TA：一个字节
	QByteArray    J1939Data;      // J1939消息数据
};







#endif // MessageDef_h__