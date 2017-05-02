#include "RP1210MsgLogger.h"
#include "MessageDef.h"
#include <QFile>
#include <QDateTime>
#include <QMessageBox>
#include <QCoreApplication>

RP1210MsgLogger::RP1210MsgLogger(bool startLog /*= false*/, QObject *parent /*= 0*/)
	: QObject(parent)
	, logFile(0)
{
	if (startLog)
		StartLog();
}

RP1210MsgLogger::~RP1210MsgLogger()
{
	StopLog();
}

bool RP1210MsgLogger::IsLog() const
{
	return logFile ? true : false;
}

void RP1210MsgLogger::StartLog()
{
	StopLog();

	QDateTime currentTime = QDateTime::currentDateTime();
	QString strTime = currentTime.toString("dd-MM-yyyy_hh-mm-ss-zzz");
	QString path = QCoreApplication::applicationDirPath();
	path = path + "/" + strTime + ".log";

	logFile = new QFile(path,this);


	if (!logFile->open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::warning(0, tr("Failed to create file!"), tr("Failed to create file : %1").arg(path), QMessageBox::Default);
		//logFile->deleteLater();
		delete logFile;
		logFile = 0;
		return;
	}

	logFile->write(strTime.toUtf8());
}

void RP1210MsgLogger::StopLog()
{
	if (logFile)
	{
		if (logFile->isWritable())
		{
			QDateTime currentTime = QDateTime::currentDateTime();
			QString strTime = currentTime.toString("dd-MM-yyyy_hh-mm-ss-zzz");
			logFile->write(strTime.toUtf8());
			logFile->flush();
		}

		logFile->close();
		//logFile->deleteLater();
		delete logFile;
	}

	logFile = 0;
}

void RP1210MsgLogger::LogToFile(QString msg)
{
	if (logFile && logFile->isWritable())
	{
		logFile->write(msg.toUtf8());
		logFile->write("\n");
	}
}

void RP1210MsgLogger::LogToFile(J1939Message const* msg)
{
	logFile->write(msg->GetRawMsgString().toUtf8());
	logFile->write("\n");
}
