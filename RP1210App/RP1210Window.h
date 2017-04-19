#pragma once

#include <QtWidgets/QDialog>
#include "ui_RP1210Window.h"

class RP1210Window : public QDialog
{
    Q_OBJECT

public:
    RP1210Window(QWidget *parent = Q_NULLPTR);
	~RP1210Window();

private:
    Ui::RP1210WindowClass ui;
	class J1939FilterWindow* J1939FilterDialog;

private:
	// 4/16/2017 : ZH : ini文件中获取到的数据
	class RP1210IniData* IniData;

	// 4/16/2017 : ZH : rp1210动态库
	class RP1210Core* rp1210Core;


protected slots:
	void OnAutoBaudRate(bool bAuto);
	void OnConnect();
	void OnDisConnect();
	void OnClearLog();
	void OnFilterWindow();

public slots:
	void OnLogMsg(QString Msg);


private:
	// 4/18/2017 : ZH : 私有，辅助函数，根据客户选择拼接协议字符串，用来和硬件建立链接
	QString GetProtocolString();

	// 4/19/2017 : ZH : 私有，辅助函数，创建过滤器窗口并连接好各种信号槽
	void SetUpFilterWindow();
};
