
#include "MessageDef.h"

MessageBase::MessageBase(QByteArray rawMsg, bool useEcho /*= false*/)
	:RawMsg(rawMsg)
	,UseEcho(useEcho)
	,TimeStamp(0) 
{
	// 4/23/2017 : ZH : 至少要4个字节
	if (RawMsg.size() >= 4)
	{
		TimeStamp = (RawMsg[0] << 24) | (RawMsg[1] << 16) | (RawMsg[2] << 8) | (RawMsg[3]);      // 时间戳，大端存储
	}

	if (UseEcho)
	{
		if (RawMsg.size() >= 5)
		{
			Echo = RawMsg[4];
			RawMsg.mid(5);
		}
	}
	else
	{
		Echo = -1;
		Data = RawMsg.mid(4);
	}
}

MessageBase::~MessageBase()
{
	Data.clear();
	RawMsg.clear();
	RawMsgStr.clear();
}

QString MessageBase::ByteArrayToQString(QByteArray arr) const
{
	int len = arr.size() * 3 + 1;
	char* temp = new char[len];
	memset(temp, 0x00, len);

	for (int i = 0, index = 0; i < arr.size(); ++i, index += 3)
	{
		sprintf_s(&(temp[index]), 3, "%02X", arr.at(i));
	}

	QString str(temp);
	delete[] temp;
	return str;
}

QString MessageBase::GetRawMsgString()
{
	if (RawMsgStr.isEmpty())
	{ 
		RawMsgStr = ByteArrayToQString(RawMsg);
	}

	return RawMsgStr;
}

QString MessageBase::GetRawMsgString() const
{
	return RawMsgStr;
}

unsigned int MessageBase::GetTimeStamp() const
{
	return TimeStamp;
}

char MessageBase::GetEchoByte() const
{
	if (UseEcho)
		return Echo;      // 使用了Echo
	else
		return -1;        // 未使用Echo 
}

J1939Message::J1939Message(QByteArray rawMsg, bool useEcho /*= false*/)
	:MessageBase(rawMsg,useEcho)
	,PGN(0)
{
	if (Data.size() >= 6)
	{
		PGN = (Data[2] << 16) | (Data[1] << 8) | (Data[0]);   //PNG小端存储
		Priority = Data[3];
		SA = Data[4];
		TA = Data[5];
		J1939Data = Data.mid(6);
	}
}

J1939Message::~J1939Message()
{
	J1939Data.clear();
}

int J1939Message::GetPGN()const
{
	return PGN;
}

unsigned char J1939Message::GetPriority()const
{
	return Priority;
}

unsigned char J1939Message::GetSA()const
{
	return SA;

}

unsigned char J1939Message::GetTA()const
{
	return TA;
}

QByteArray J1939Message::GetJ1939Data()const
{
	return J1939Data;
}

QString J1939Message::GetMsgDataString() const
{
	//return GetRawMsgString();

	return ByteArrayToQString(J1939Data);
}
