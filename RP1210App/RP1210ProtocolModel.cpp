#include "RP1210ProtocolModel.h"
#include <QSettings>

RP1210ProtocolModel::RP1210ProtocolModel(QObject *parent)
	: QAbstractListModel(parent)
{
	RawProtocolMap.clear();
	IndexList.clear();
}

RP1210ProtocolModel::~RP1210ProtocolModel()
{
	RawProtocolMap.clear();
	IndexList.clear();
}

Rp1210Protocol* RP1210ProtocolModel::GetProtocol(int index)
{
	if (0 <= index && index < IndexList.size())
	{
		int protocolId = IndexList[index];
		return &(RawProtocolMap[protocolId]);
	}
	return 0;
}

void RP1210ProtocolModel::IniProtocolList(class QSettings* VenderIni, QList<Rp1210Device>& DeviceList)
{
	RawProtocolMap.clear();
	IndexList.clear();

	QStringList ProtocolStrList = VenderIni->value("/VendorInformation/Protocols").toStringList();
	for (int i = 0; i < ProtocolStrList.size(); ++i)
	{
		QString temp = "/ProtocolInformation" + ProtocolStrList[i];
		Rp1210Protocol protocol;
		protocol.ProtocolID = ProtocolStrList[i].toInt();

		protocol.ProtocolName = VenderIni->value(temp + "/ProtocolString").toString();
		protocol.ProtocolParam = VenderIni->value(temp + "/ProtocolParams").toString();
		protocol.ProtocolSpeed = VenderIni->value(temp + "/ProtocolSpeed").toStringList();

		QStringList DeviceIdList = VenderIni->value(temp + "/Devices").toStringList();
		for (int i = 0; i < DeviceIdList.size(); ++i)
		{
			int DeviceId = DeviceIdList[i].toInt();
			protocol.DeviceIDs.append(DeviceId);

			for (int j = 0; j < DeviceList.size(); ++j)
			{
				if (DeviceList[j].DeviceID == DeviceId)
					DeviceList[j].Protocols.append(protocol.ProtocolID);
			}
		}

		RawProtocolMap.insert(protocol.ProtocolID, protocol);
	} 
}

void RP1210ProtocolModel::SetProtocolList(QList<int> list)
{
	beginResetModel();
	IndexList = list;
	endResetModel();
}

int RP1210ProtocolModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	return IndexList.size();
}

QVariant RP1210ProtocolModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (index.row() >= IndexList.count())
		return QVariant();


	if (role == Qt::DisplayRole)
	{
		int protocolId = IndexList[index.row()];
		return RawProtocolMap[protocolId].ProtocolName;
	}
	else
		return QVariant();
}
