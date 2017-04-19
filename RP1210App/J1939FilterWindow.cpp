#include "J1939FilterWindow.h"
#include "RP1210Core.h"

J1939FilterWindow::J1939FilterWindow(QWidget *parent)
	: QDialog(parent)
	,rp1210Core(0)
{
	ui.setupUi(this);
	InitSignalConnection();
	OnCheckBoxChanged(true);
}

J1939FilterWindow::~J1939FilterWindow()
{
}

void J1939FilterWindow::SetRp1210Core(class RP1210Core* core)
{
	rp1210Core = core;
}
 

void J1939FilterWindow::OnSetFilterType()
{
	if (rp1210Core && ui.groupBoxFilterType->isChecked())
	{
		unsigned char FilterType;
		if (ui.radioButtonInclusive->isChecked())
			FilterType = FILTER_INCLUSIVE;
		else
			FilterType = FILTER_EXCLUSIVE;

		rp1210Core->SetJ1919FilterType(FilterType);
	}
}

void J1939FilterWindow::OnSetPassAll()
{
	if (rp1210Core)
	{
		rp1210Core->SetAllFilterStatesToPass();
	}
}

void J1939FilterWindow::OnSetDiscardAll()
{
	if (rp1210Core)
	{
		rp1210Core->SetAllFilterStatesToDiscard();
	}
}

void J1939FilterWindow::OnSetFilter()
{
	if (rp1210Core)
	{
		unsigned char Flags = GetFilterFlags();

		unsigned char PGN[3] = { 0 };
		memset(PGN, 0x00, sizeof(PGN));
		PGN[0] = ui.lineEditPGNByte0->text().toInt(0, 16);
		PGN[1] = ui.lineEditPGNByte1->text().toInt(0, 16);
		PGN[2] = ui.lineEditPGNByte2->text().toInt(0, 16);

		unsigned char Priority = ui.lineEditPriority->text().toInt();
		unsigned char SA = ui.lineEditSA->text().toInt();
		unsigned char TA = ui.lineEditTA->text().toInt();

		rp1210Core->SetMessageFilterForJ1939(Flags, PGN, Priority, SA, TA);
	}

}

void J1939FilterWindow::OnCancel()
{
	close();
}

void J1939FilterWindow::OnCheckBoxChanged(bool )
{
	if (ui.checkBoxPGN->isChecked())
		ui.groupBoxPGN->setEnabled(true);
	else
		ui.groupBoxPGN->setEnabled(false);

	if (ui.checkBoxPriority->isChecked())
		ui.lineEditPriority->setEnabled(true);
	else
		ui.lineEditPriority->setEnabled(false);

	if (ui.checkBoxSA->isChecked())
		ui.lineEditSA->setEnabled(true);
	else
		ui.lineEditSA->setEnabled(false);

	if (ui.checkBoxTA->isChecked())
		ui.lineEditTA->setEnabled(true);
	else
		ui.lineEditTA->setEnabled(false); 
}

void J1939FilterWindow::InitSignalConnection()
{
	connect(ui.pushButtonSetType, SIGNAL(clicked()), this, SLOT(OnSetFilterType()));
	connect(ui.pushButtonPassAll, SIGNAL(clicked()), this, SLOT(OnSetPassAll()));
	connect(ui.pushButtonDiscardAll, SIGNAL(clicked()), this, SLOT(OnSetDiscardAll()));
	connect(ui.pushButtonSetFilter, SIGNAL(clicked()), this, SLOT(OnSetFilter()));
	connect(ui.pushButtonCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
}

unsigned char J1939FilterWindow::GetFilterFlags()
{
	unsigned char Flags = 0x00;

	if (ui.checkBoxPGN->isChecked())
		Flags |= FILTER_PGN;

	if (ui.checkBoxPriority->isChecked())
		Flags |= FILTER_PRIORITY;

	if (ui.checkBoxSA->isChecked())
		Flags |= FILTER_SOURCE;

	if (ui.checkBoxTA->isChecked())
		Flags |= FILTER_DESTINATION;

	return Flags;
}
