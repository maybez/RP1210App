
#ifndef RP1210MsgParser_h__
#define RP1210MsgParser_h__

#include <QAbstractTableModel>
#include "MessageDef.h"

// 4/23/2017 : ZH : 消息保存与解析 --> 目前只支持J1939消息
class RP1210MsgParser : public QAbstractTableModel
{
	Q_OBJECT

public:
	RP1210MsgParser(QObject *parent);
	~RP1210MsgParser();

	// 4/23/2017 : ZH : QAbstractTableModel重载
	int rowCount(const QModelIndex &parent)const override;
	int columnCount(const QModelIndex &parent) const override;
	QVariant data(const QModelIndex& index, int role)const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

public:
	QString GetMessageString(int row);
	void ClearAllMessage();
	void DeleteMessage(QModelIndexList& msgs);


public slots:
    // 4/23/2017 : ZH : 接收并解析消息
	void OnMessage(QByteArray data);

private:
	// 4/23/2017 : ZH : 保存消息
	QList<J1939Message> j1939MsgList;
};





#endif // RP1210MsgParser_h__

