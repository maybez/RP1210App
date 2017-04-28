#include "RP1210Window.h"
#include "RP1210IniData.h"
#include "RP1210Core.h"
#include "J1939FilterWindow.h"
#include "RP1210ReadThread.h"
#include "RP1210MsgParser.h"

#include <QClipboard>
#include <QMessageBox>
#include <QScrollBar>

RP1210Window::RP1210Window(QWidget *parent)
	: QDialog(parent)
	,J1939FilterDialog(0)
{
	ui.setupUi(this);

	IniData = RP1210IniData::GetInistance();
	IniData->ReadMainIniFile();
	rp1210Core = RP1210Core::GetInstance();
	rp1210ReadThread = new RP1210ReadThread(rp1210Core, this);  

	SetUpTableView();

	connect(IniData, SIGNAL(LogMsg(QString)), this, SLOT(OnLogMsg(QString)));
	connect(rp1210Core, SIGNAL(LogMsg(QString)), this, SLOT(OnLogMsg(QString)),Qt::QueuedConnection);
	connect(ui.checkBoxAutoBaudRate, SIGNAL(toggled(bool)), this,SLOT(OnAutoBaudRate(bool)));

	connect(ui.pushButtonConnect, SIGNAL(clicked()), this, SLOT(OnConnect()));
	connect(ui.pushButtonDisConnect, SIGNAL(clicked()), this, SLOT(OnDisConnect()));
	connect(ui.pushButtonFilter, SIGNAL(clicked()), this, SLOT(OnFilterWindow()));
	connect(ui.pushButtonClearLog, SIGNAL(clicked()), this, SLOT(OnClearLog()));

	connect(ui.comboBoxVendor, SIGNAL(currentIndexChanged(QString)), IniData, SLOT(OnVenderChanged(QString)));
	connect(ui.comboBoxDevice, SIGNAL(currentIndexChanged(int)), IniData, SLOT(OnDeviceChanged(int)));
	connect(ui.comboBoxProtocol, SIGNAL(currentIndexChanged(int)), IniData, SLOT(OnProtocolChanged(int)));
	
	ui.comboBoxVendor->setModel(IniData->GetVenderModel());
	ui.comboBoxDevice->setModel(IniData->GetDeviceModel());
	ui.comboBoxProtocol->setModel(IniData->GetProtocolModel());
	ui.comboBoxBaudRate->setModel(IniData->GetBaudRateModel());

	ui.pushButtonConnect->setEnabled(true);
	ui.pushButtonDisConnect->setEnabled(false);

}

RP1210Window::~RP1210Window()
{
	if (IniData)
		delete IniData;
	if (rp1210Core)
		delete rp1210Core;

	IniData = 0;
	rp1210Core = 0;
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

	DWORD dwError = rp1210Core->LoadRp1210DLL(dllPath);

	if (!dwError)      // 4/16/2017 : ZH :  dwError == 0 --> Load Success
	{ 
		// 4/18/2017 : ZH : 设备ID和协议字符串
		int DeviceID = IniData->GetDeviceId(ui.comboBoxDevice->currentIndex());
		QString Protocol = GetProtocolString();		
		short ErrorCode = rp1210Core->ClientConnect(DeviceID, Protocol);
		if (ErrorCode != NO_ERRORS)			return;

		// 4/18/2017 : ZH : 如果是J1939协议 
		rp1210Core->ClaimJ1939Address(J1939_OFFBOARD_DIAGNOSTICS_TOOL_1);

		// 4/23/2017 : ZH : 设置过滤器为全部通过
		rp1210Core->SetAllFilterStatesToPass();
	
		// 4/23/2017 : ZH : 开启读取线程
		rp1210ReadThread->SetNeedExit(false);
		rp1210ReadThread->start();

		ui.pushButtonConnect->setEnabled(false);
		ui.pushButtonDisConnect->setEnabled(true);
	}
	else
	{
		QMessageBox::critical(this, tr("Can not load RP1210 DLL"), tr("Load %1 failed! Error code = %2").arg(dllPath).arg(dwError));
	}
}

void RP1210Window::OnDisConnect()
{
	rp1210ReadThread->SetNeedExit(true);
	short ErrorCode = rp1210Core->ClientDisconnect();

	if (ErrorCode == NO_ERRORS)
	{ 
		ui.pushButtonConnect->setEnabled(true);
		ui.pushButtonDisConnect->setEnabled(false);
	}
}

void RP1210Window::OnClearLog()
{
	ui.textBrowserLogMsg->clear();
	
	// 4/23/2017 : ZH : 测试代码
	char tmp3[] = { 0x04,0xF6,0xFE,0x50,0xF1,0xFE,0x00,0x06,0x00,0xFF,0xFC,0x00,0x00,0xFC,0xFF,0x00,0x00,0xFF }; 
	QByteArray arr3(tmp3, sizeof(tmp3));
	msgParser->OnMessage(arr3);
}

void RP1210Window::OnFilterWindow()
{
	if (!J1939FilterDialog)
	{
		SetUpFilterWindow();
	}

	J1939FilterDialog->show();
	J1939FilterDialog->raise();
	J1939FilterDialog->activateWindow();
}

void RP1210Window::OnSelectionChanged(QItemSelection selected, QItemSelection deSelected)
{
	QItemSelectionModel* selectionModel = ui.tableViewMsg->selectionModel();
	QModelIndexList selectedRows = selectionModel->selectedRows();

	if (!selectedRows.empty())     //4/28/2017 ZH :有选中项
	{
		copyAction->setEnabled(true);
		deleteAction->setEnabled(true);
		logtoFileAction->setEnabled(true);
	}
	else                           //4/28/2017 ZH :无选中项
	{
		copyAction->setEnabled(false);
		deleteAction->setEnabled(false);
		logtoFileAction->setEnabled(false);
	}
}

void RP1210Window::OnScrollRangeChanged(int min, int max)
{
	ui.tableViewMsg->verticalScrollBar()->setValue(max);
}

void RP1210Window::OnSelectAll()
{
	QItemSelectionModel* selectionModel = ui.tableViewMsg->selectionModel();

	QModelIndex topLeft = msgParser->index(0, 0, QModelIndex());
	QModelIndex bottomRight = msgParser->index(msgParser->rowCount(QModelIndex())-1, msgParser->columnCount(QModelIndex())-1,QModelIndex());
	QItemSelection selection(topLeft, bottomRight);

	selectionModel->select(selection, QItemSelectionModel::Select);
}

void RP1210Window::OnClearAll()
{
	msgParser->ClearAllMessage(); 
}

void RP1210Window::OnDelete()
{
	QItemSelectionModel* selectionModel = ui.tableViewMsg->selectionModel();
	

	QModelIndexList selectedRows = selectionModel->selectedRows(); 

	msgParser->DeleteMessage(selectedRows);
}

void RP1210Window::OnCopy()
{
	QItemSelectionModel* selectionModel = ui.tableViewMsg->selectionModel();

	QModelIndexList selectedRows = selectionModel->selectedRows();


	QString strTemp = "";

	for each (QModelIndex item in selectedRows)
	{
		strTemp += (msgParser->GetMessageString(item.row())+"\r\n");
	}

	QClipboard* clipBoard = QApplication::clipboard();
	clipBoard->setText(strTemp);
} 

void RP1210Window::OnLogtoFile()
{
	bool isChecked = logtoFileAction->isChecked();
	if (isChecked)
	{

	}
	else
	{

	}
}
void RP1210Window::OnLogMsg(QString Msg)
{
	ui.textBrowserLogMsg->append(Msg);
}

QString RP1210Window::GetProtocolString()
{
	QString ProtocolName = IniData->GetProtocolName(ui.comboBoxProtocol->currentIndex());
	if (!(ui.checkBoxAutoBaudRate->isChecked()))
	{
		QString BaudRate = IniData->GetBaudRate(ui.comboBoxBaudRate->currentIndex());
		return QString("%1:Baud=%2").arg(ProtocolName).arg(BaudRate);
	}
	else
	{
		return QString("%1:Baud=Auto").arg(ProtocolName);
	}
}

void RP1210Window::SetUpFilterWindow()
{
	J1939FilterDialog = new J1939FilterWindow(this);
	J1939FilterDialog->SetRp1210Core(rp1210Core);
} 

void RP1210Window::SetUpTableView()
{
	// 绑定model 和view
	msgParser = new RP1210MsgParser(this);
	ui.tableViewMsg->setModel(msgParser);

	connect(rp1210ReadThread, &RP1210ReadThread::MsgReady, msgParser, &RP1210MsgParser::OnMessage, Qt::QueuedConnection);

	//4/27/2017 ZH :表格视图的上下文菜单
	selectAllAction = new QAction(tr("Select All"), this);
	clearAllAction = new QAction(tr("Clear All"), this); 
	copyAction = new QAction(tr("Copy"), this);
	deleteAction = new QAction(tr("Delete"), this); 
	logtoFileAction = new QAction(tr("Log to file"), this);
	logtoFileAction->setCheckable(true);
	logtoFileAction->setChecked(true);

	separator01Action = new QAction(this);
	separator02Action = new QAction(this);
	separator01Action->setSeparator(true);
	separator02Action->setSeparator(true);

	ui.tableViewMsg->addAction(selectAllAction);
	ui.tableViewMsg->addAction(clearAllAction);
	ui.tableViewMsg->addAction(separator01Action);
	ui.tableViewMsg->addAction(copyAction);
	ui.tableViewMsg->addAction(deleteAction);
	ui.tableViewMsg->addAction(separator02Action);
	ui.tableViewMsg->addAction(logtoFileAction);
	ui.tableViewMsg->setContextMenuPolicy(Qt::ActionsContextMenu);

	connect(selectAllAction, &QAction::triggered, this, &RP1210Window::OnSelectAll);
	connect(clearAllAction, &QAction::triggered, this, &RP1210Window::OnClearAll);
	connect(copyAction, &QAction::triggered, this, &RP1210Window::OnCopy);
	connect(deleteAction, &QAction::triggered, this, &RP1210Window::OnDelete);
	connect(logtoFileAction, &QAction::triggered, this, &RP1210Window::OnLogtoFile);

	connect(ui.tableViewMsg->verticalScrollBar(), &QScrollBar::rangeChanged, this, &RP1210Window::OnScrollRangeChanged);	
	connect(ui.tableViewMsg->selectionModel(), &QItemSelectionModel::selectionChanged, this, &RP1210Window::OnSelectionChanged); 
}

