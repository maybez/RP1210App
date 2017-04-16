#include "RP1210DeviceModel.h"
#include <QSettings>

RP1210DeviceModel::RP1210DeviceModel(QObject *parent)
	: QAbstractListModel(parent)
{
	DeviceList.clear();
}

RP1210DeviceModel::~RP1210DeviceModel()
{
	DeviceList.clear();
}

QList<Rp1210Device>& RP1210DeviceModel::GetDeviceList()
{
	return DeviceList;
}

QList<int>& RP1210DeviceModel::GetProtocolList(int index)
{
	if (0 <= index && index < DeviceList.size())
	{
		return DeviceList[index].Protocols;
	}

	return QList<int>();
}

void RP1210DeviceModel::InitDeviceList(QSettings* VenderIni)
{
	beginResetModel();
	DeviceList.clear();
	
	QStringList DeviceStrList = VenderIni->value("/VendorInformation/Devices").toStringList();

	for (int i = 0; i < DeviceStrList.count(); ++i)
	{
		QString temp = "/DeviceInformation" + DeviceStrList[i];

		Rp1210Device device;
		device.DeviceID = VenderIni->value(temp + "/DeviceID").toInt();
		device.DeviceName = VenderIni->value(temp + "/DeviceName").toString();
		device.DeviceParam = VenderIni->value(temp + "/DeviceParams").toString();
		device.MultiCANChannels = VenderIni->value(temp + "/MultiCANChannels").toInt();
		device.MultiJ1939Channels = VenderIni->value(temp + "/MultiJ1939Channels").toInt();
		device.MultiISO15765Channels = VenderIni->value(temp + "/MultiISO15765Channels").toInt();

		DeviceList.append(device);
	}
	endResetModel();
}

int RP1210DeviceModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	return DeviceList.count();
}

QVariant RP1210DeviceModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();
	if (index.row() >= DeviceList.count())
		return QVariant();
	if (role == Qt::DisplayRole)
		return  QString("%1 -- %2").arg(DeviceList.at(index.row()).DeviceID).arg(DeviceList.at(index.row()).DeviceName);    
	else
		return QVariant();
}
 
