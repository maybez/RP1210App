#include "RP1210IniData.h"

#include <QStringListModel>

RP1210IniData::RP1210IniData(QObject *parent)
	: QObject(parent)
{
	MainIniPath = "C:\\Windows\\RP121032.ini";
	VenderIniPath = "";
	VenderDllPath = "";

	MainIni = new QSettings(MainIniPath, QSettings::IniFormat, this);
	VenderIni = 0;

	VenderModel = new QStringListModel(this);
	DeviceModel = new RP1210DeviceModel(this);
	ProtocolModel = new RP1210ProtocolModel(this);
	BaudRateModel = new QStringListModel(this);
}

RP1210IniData* RP1210IniData::TheInstance = 0;

RP1210IniData::~RP1210IniData()
{
}

RP1210IniData* RP1210IniData::GetInistance()
{
	if (!TheInstance)
		TheInstance = new RP1210IniData(0);

	return TheInstance;
}

void RP1210IniData::ReadMainIniFile()
{
	QStringList VenderList = MainIni->value("/RP1210Support/APIImplementations").toStringList();
	VenderModel->setStringList(VenderList);
}

QStringListModel* RP1210IniData::GetVenderModel()
{
	return VenderModel;
}

RP1210DeviceModel* RP1210IniData::GetDeviceModel()
{
	return DeviceModel;
}

RP1210ProtocolModel* RP1210IniData::GetProtocolModel()
{
	return ProtocolModel;
}

QStringListModel* RP1210IniData::GetBaudRateModel()
{
	return BaudRateModel;
}

QString RP1210IniData::GetVenderDllPath(int index)
{
	QModelIndex ModelIndex = VenderModel->index(index);
	QString VenderName = VenderModel->data(ModelIndex, Qt::DisplayRole).toString();

	QString VenderDllPath;
	QString CpuAbi = QSysInfo::currentCpuArchitecture();
	if (CpuAbi == "i386")
		VenderDllPath = "C:\\Windows\\System32\\" + VenderName + ".dll";
	else
		VenderDllPath = "C:\\Windows\\SysWOW64\\" + VenderName + ".dll";

	return VenderDllPath; 
}

//QString RP1210IniData::GetVenderName(int index)
//{
//	QModelIndex ModelIndex = VenderModel->index(index);
//	return VenderModel->data(ModelIndex, Qt::DisplayRole).toString();
//}

int RP1210IniData::GetDeviceId(int index)
{
	return DeviceModel->GetDeviceList().at(index).DeviceID;
}

QString RP1210IniData::GetProtocolName(int index)
{
	QModelIndex ModelIndex = ProtocolModel->index(index);
	return ProtocolModel->data(ModelIndex, Qt::DisplayRole).toString();
}

QString RP1210IniData::GetBaudRate(int index)
{
	QModelIndex ModelIndex = BaudRateModel->index(index);
	return BaudRateModel->data(ModelIndex, Qt::DisplayRole).toString();
}

void RP1210IniData::OnVenderChanged(QString VenderName)
{
	VenderIniPath = "C:\\Windows\\" + VenderName + ".ini";
	VenderDllPath = "C:\\Windows\\SysWOW64" + VenderName + ".dll";

	if (VenderIni)
	{
		delete VenderIni;
		VenderIni = 0;
	}
	VenderIni = new QSettings(VenderIniPath, QSettings::IniFormat, this);
	DeviceModel->InitDeviceList(VenderIni);
	ProtocolModel->IniProtocolList(VenderIni, DeviceModel->GetDeviceList());
}

void RP1210IniData::OnDeviceChanged(int index)
{
	QList<int>* protocolList = DeviceModel->GetProtocolList(index);
	ProtocolModel->SetProtocolList(protocolList);
}

void RP1210IniData::OnProtocolChanged(int index)
{
	Rp1210Protocol* protocol = ProtocolModel->GetProtocol(index);
	if(protocol)
		BaudRateModel->setStringList(protocol->ProtocolSpeed);
}
