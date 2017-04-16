#include "RP1210Window.h"
#include "RP1210IniData.h"
#include "RP1210Core.h"

RP1210Window::RP1210Window(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	IniData = RP1210IniData::GetInistance();
	IniData->ReadMainIniFile();

	rp1210Core = RP1210Core::GetInstance();


	connect(ui.checkBoxAutoBaudRate, SIGNAL(toggled(bool)), this,SLOT(OnAutoBaudRate(bool)));
	connect(ui.pushButtonConnect, SIGNAL(clicked()), this, SLOT(OnConnect()));
	connect(ui.comboBoxVendor, SIGNAL(currentIndexChanged(QString)), IniData, SLOT(OnVenderChanged(QString)));
	connect(ui.comboBoxDevice, SIGNAL(currentIndexChanged(int)), IniData, SLOT(OnDeviceChanged(int)));
	connect(ui.comboBoxProtocol, SIGNAL(currentIndexChanged(int)), IniData, SLOT(OnProtocolChanged(int)));
	
	//connect(ui.comboBoxDevice, SIGNAL(currentIndexChanged(int)),IniData)

	ui.comboBoxVendor->setModel(IniData->GetVenderModel());
	ui.comboBoxDevice->setModel(IniData->GetDeviceModel());
	ui.comboBoxProtocol->setModel(IniData->GetProtocolModel());
	ui.comboBoxBaudRate->setModel(IniData->GetBaudRateModel());
}

void RP1210Window::OnAutoBaudRate(bool bAuto)
{
	if (ui.checkBoxAutoBaudRate->isChecked())
	{
		ui.comboBoxBaudRate->setEnabled(false);
	}
	else
	{
		ui.comboBoxBaudRate->setEnabled(true);
	}
}

void RP1210Window::OnConnect()
{
	QString dllPath = IniData->GetVenderDllPath(ui.comboBoxVendor->currentIndex());
	bool bRe = rp1210Core->LoadRp1210DLL(dllPath);
	if (bRe)
	{
		int a = 100;
		int b = 100;
	}

}
