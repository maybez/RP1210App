#pragma once

#include <QDialog>
#include "ui_J1939FilterWindow.h"

class J1939FilterWindow : public QDialog
{
	Q_OBJECT

public:
	J1939FilterWindow(QWidget *parent = Q_NULLPTR);
	~J1939FilterWindow();

	void SetRp1210Core(class RP1210Core* core);

private:
	Ui::J1939FilterWindow ui;
	class RP1210Core* rp1210Core;

protected slots:
	void OnSetFilterType();
	void OnSetPassAll();
	void OnSetDiscardAll();
	void OnSetFilter();
	void OnCancel();
	void OnCheckBoxChanged(bool);

private:
	// 4/19/2017 : ZH : 私有，辅助函数，建立信号槽连接
	void InitSignalConnection();
	unsigned char GetFilterFlags();
};
