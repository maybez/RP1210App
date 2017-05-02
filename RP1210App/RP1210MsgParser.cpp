#include "RP1210MsgParser.h"
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QCoreApplication>
#include <QMessageBox>

RP1210MsgParser::RP1210MsgParser(QObject *parent)
	: QAbstractTableModel(parent)
{
	j1939MsgList.clear();

	// 测试代码
	//char tmp1[] = { 0x04,0xF6,0xFE,0x29,0xF1,0xFE,0x00,0x06,0x00,0xFF,0xFC,0x00,0x00,0xFC,0xFF,0x00,0x00,0xFF };
	char tmp[] = { 0x00,0x00,0x00,0x00,0xF1,0xFE,0x00,0x06,0x00,0xFF,0xFC,0x00,0x00,0xFC,0xFF,0x00,0x00,0xFF };
	QByteArray arr(tmp, sizeof(tmp));
	for (int i = 0; i < 100; ++i)
	{
		j1939MsgList.append(J1939Message(arr, false));
		arr[3] = arr[3] + 1;
	}
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
	
	// 对齐方式
	if (role == Qt::TextAlignmentRole)
	{
		if (index.column() == 6)
			return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
		else
			return QVariant(Qt::AlignCenter);
	}

	if (role == Qt::DisplayRole)
	{
		const J1939Message& msg = j1939MsgList[index.row()];
		msg.GetTimeStamp();
		switch (index.column())
		{
		case 0:
			return QVariant(msg.GetTimeStamp());
		case 1:
			if (msg.GetEchoByte() == -1)
				return QVariant("*");
			else
				return QVariant(msg.GetEchoByte());
		case 2:
			return QVariant(msg.GetPGN());
		case 3:
			return QVariant(msg.GetPriority());
		case 4:
			return QVariant(tr("%1").arg(msg.GetSA(),2,16,QChar('0')).toUpper());
		case 5:
			return QVariant(tr("%1").arg(msg.GetTA(), 2,16,QChar('0')).toUpper());
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

	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
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

QString RP1210MsgParser::GetMessageString(int row)
{
	return j1939MsgList.at(row).GetRawMsgString();
}

void RP1210MsgParser::ClearAllMessage()
{
	beginResetModel();
	j1939MsgList.clear();
	endResetModel();
}

void RP1210MsgParser::DeleteMessage(QModelIndexList& msgs)
{	
	QMap<int, int>rowMap;
	for each(QModelIndex msg in msgs)
	{
		rowMap.insert(msg.row(), 0);
	}

	QMapIterator<int, int> it(rowMap);
	it.toBack();

	// 利用QMap是存储数据是有序的，并且不会重复存储key值。
	beginResetModel();
	while (it.hasPrevious())
	{
		it.previous();
		j1939MsgList.removeAt(it.key());
	}
	endResetModel();
} 

void RP1210MsgParser::OnMessage(QByteArray data)
{
	J1939Message msg(data, false);

	beginInsertRows(QModelIndex(), j1939MsgList.size(), j1939MsgList.size());
	j1939MsgList.push_back(msg);
	endInsertRows(); 
}
