#pragma once

#include <QtWidgets/QDialog>
#include "ui_RP1210Window.h"

class RP1210Window : public QDialog
{
    Q_OBJECT

public:
    RP1210Window(QWidget *parent = Q_NULLPTR);

private:
    Ui::RP1210WindowClass ui;
		
private:
	// 4/16/2017 : ZH : ini文件中获取到的数据
	class RP1210IniData* IniData;

	// 4/16/2017 : ZH : rp1210动态库
	class RP1210Core* rp1210Core;


protected slots:
	void OnAutoBaudRate(bool bAuto);
	void OnConnect(); 
};
