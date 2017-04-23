#include "RP1210MsgParser.h"

RP1210MsgParser::RP1210MsgParser(QObject *parent)
	: QAbstractTableModel(parent)
{
	j1939MsgList.clear();
}

RP1210MsgParser::~RP1210MsgParser()
{
	j1939MsgList.clear();
}

int RP1210MsgParser::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return j1939MsgList.size();
}

int RP1210MsgParser::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	return 7;
}

QVariant RP1210MsgParser::data(const QModelIndex& index, int role) const
{
	if (index.row() >= j1939MsgList.size())
		return QVariant();
	
	if (role == Qt::DisplayRole)
	{
		const J1939Message& msg = j1939MsgList[index.row()];
		msg.GetTimeStamp();
		switch (index.column())
		{
		case 0:
			return QVariant(msg.GetTimeStamp());
		case 1:
			return QVariant(msg.GetEchoByte());
		case 2:
			return QVariant(msg.GetPGN());
		case 3:
			return QVariant(msg.GetPriority());
		case 4:
			return QVariant(msg.GetSA());
		case 5:
			return QVariant(msg.GetTA());
		case 6:
			return QVariant(msg.GetMsgDataString());
		default:
			return QVariant();
		}		 
	}
	return QVariant();
}

QVariant RP1210MsgParser::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Vertical)
		return QVariant();

	if (orientation == Qt::Horizontal)
	{
		switch (section)
		{
		case 0:
			return QVariant("TimeStamp");
		case 1:
			return QVariant("Echo");
		case 2:
			return QVariant("PGN");
		case 3:
			return QVariant("Priority");
		case 4:
			return QVariant("SA");
		case 5:
			return QVariant("TA");
		case 6:
			return QVariant("DATA");
		default:
			return QVariant();
			break;
		}
	}
	return QVariant();
}

void RP1210MsgParser::OnMessage(QByteArray data)
{
	J1939Message msg(data, false);
	j1939MsgList.push_back(msg);
}
