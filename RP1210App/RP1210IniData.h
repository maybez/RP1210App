
#ifndef RP1210IniData_h__
#define RP1210IniData_h__


// 4/16/2017 : ZH : 负责RP1210协议涉及到的所有ini文件的数据读取工作

#include <QObject>
#include <QSettings>
#include <QStringListModel>

#include "RP1210DeviceModel.h"
#include "RP1210ProtocolModel.h"
 
class RP1210IniData : public QObject
{
	Q_OBJECT

public:
	~RP1210IniData();
	static RP1210IniData* GetInistance();

public:
	void ReadMainIniFile();
	QStringListModel* GetVenderModel();
	RP1210DeviceModel* GetDeviceModel();
	RP1210ProtocolModel* GetProtocolModel();
	QStringListModel* GetBaudRateModel();

public:
	QString GetVenderDllPath(int index);
	int GetDeviceId(int index);
	QString GetProtocolName(int index);
	QString GetBaudRate(int index);


public slots:
	void OnVenderChanged(QString VenderName);
	void OnDeviceChanged(int index);
	void OnProtocolChanged(int index);

private:
	RP1210IniData(QObject *parent);

	QString MainIniPath;
	QString VenderIniPath;
	QString VenderDllPath;

	QSettings* MainIni;
	QSettings* VenderIni;

	QStringListModel* VenderModel;	
	RP1210DeviceModel* DeviceModel; 
	RP1210ProtocolModel* ProtocolModel;
	QStringListModel* BaudRateModel;

	static RP1210IniData* TheInstance;
};

#endif // RP1210IniData_h__

