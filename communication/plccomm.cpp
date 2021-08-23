#include "plccomm.h"
#include <QDebug>
#include <QMessageBox>
#include <QThread>
#include <chrono>

PLCComm::PLCComm()
{
	// Connect to PLC
	client = new TS7Client;
}

PLCComm::~PLCComm()
{
	delete client;
}

//��ʼ��
bool PLCComm::init(QString ip)
{
	IP_PLC = ip.toStdString().c_str();
	bool binit = WraperConnect();
	if (binit)
		return true;
	else
		return false;
}
//�Ͽ�
void PLCComm::disconnect()
{
	WraperDisConnect();
}
//��bool ������ַ����20.0 �������0��1
bool PLCComm::boolread(QString address, bool &result)
{
	QStringList stringlist = address.split(".");
	int numberint = stringlist.at(0).toInt();
	int startint = stringlist.at(1).toInt();
	byte buffer[1] = { 0 };
	bool bread = WraperDBRead(numberint, startint, 1, buffer);
	if (bread)
	{
		if (buffer[0] == 1)
			result = true;
		else
			result = false;

		return true;
	}
	else
	{
		return false;
	}
}
//дbool ������ַ����20.0 ��������0��1
bool PLCComm::boolwrite(QString address, bool value)
{
	QStringList stringlist = address.split(".");
	int numberint = stringlist.at(0).toInt();
	int startint = stringlist.at(1).toInt();

	byte buffer[1] = { 0 };
	if (value)
		buffer[1] = 1;
	else
		buffer[1] = 0;

	bool bwrite = WraperDBWrite(numberint, startint, 1, buffer);
	if (bwrite)
		return true;
	else
		return false;
}
//��int ������ַ����20.0 ���������ֵ��int�ͣ�
bool PLCComm::intread(QString address, int &result)
{
	QStringList stringlist = address.split(".");
	int numberint = stringlist.at(0).toInt();
	int startint = stringlist.at(1).toInt();
	byte buffer[4] = { 0 };
	bool bread = WraperDBRead(numberint, startint, 4, buffer);
	if (bread)
	{
		result = buffer[1];
		return true;
	}
	else
	{
		return false;
	}
}
//дint ������ַ����20.0 ����������ֵ��int�ͣ�
bool PLCComm::intwrite(QString address, int value)
{
	QStringList stringlist = address.split(".");
	int numberint = stringlist.at(0).toInt();
	int startint = stringlist.at(1).toInt();

	byte buffer[4] = { 0 };
	intToByte(value, buffer);

	bool bwrite = WraperDBWrite(numberint, startint, 4, buffer);
	if (bwrite)
		return true;
	else
		return false;
}

//д��DB
bool PLCComm::WraperDBWrite(int DBNumber, int Start, int Size, void *pUsrData)
{
	QMutexLocker locker(&m_rw_mutex);

	if (!client || !client->Connected()) return false;

	int retry_count = DB_RW_RETRY_TIMES;
	int ret = client->DBWrite(DBNumber, Start, Size, pUsrData);
	if (ret)
	{
		qCritical() << "WraperDBRead Failed, ret=" << ret;
	}
	while (ret && retry_count > 0)
	{
		qCritical() << "WraperDBWrite Failed, ret=" << ret << ", retry_count=" << retry_count << ", Start=" << Start;
		if (DB_RW_RETRY_INTERVAL_MSEC > 0)
			QThread::msleep(DB_RW_RETRY_INTERVAL_MSEC);
		ret = client->DBWrite(DBNumber, Start, Size, pUsrData);
		--retry_count;
	}

	if (ret)
		return false;
	else
		return true;	
}
//��ȡDB
bool PLCComm::WraperDBRead(int DBNumber, int Start, int Size, void *pUsrData)
{
	QMutexLocker locker(&m_rw_mutex);

	if (!client || !client->Connected()) return false;

	int retry_count = DB_RW_RETRY_TIMES;
	int ret = client->DBRead(DBNumber, Start, Size, pUsrData);
	if (ret)
	{
		qCritical() << "WraperDBRead Failed, ret=" << ret;
	}
	while (ret && retry_count > 0)
	{
		qCritical() << "WraperDBRead Failed, ret=" << ret << ", retry_count=" << retry_count << ", Start=" << Start;
		if (DB_RW_RETRY_INTERVAL_MSEC > 0)
			QThread::msleep(DB_RW_RETRY_INTERVAL_MSEC);
		ret = client->DBRead(DBNumber, Start, Size, pUsrData);
		--retry_count;
	}

	if (ret)
		return false;
	else
		return true;
}
//д����ֵ
void PLCComm::WraperWriteMultiVars(PS7DataItem Item, int ItemsCount)
{
	QMutexLocker locker(&m_rw_mutex);

	if (!client || !client->Connected()) return;

	int retry_count = DB_RW_RETRY_TIMES;
	int ret = client->WriteMultiVars(Item, ItemsCount);
	if (ret)
	{
		qCritical() << "WraperDBRead Failed, ret=" << ret;
	}
	while (ret && retry_count > 0)
	{
		qCritical() << "WraperWriteMultiVars Failed, ret=" << ret << ", retry_count=" << retry_count;
		if (DB_RW_RETRY_INTERVAL_MSEC > 0)  QThread::msleep(DB_RW_RETRY_INTERVAL_MSEC);
		ret = client->WriteMultiVars(Item, ItemsCount);
		--retry_count;
	}
}
//�Ƿ�����
bool PLCComm::WraperIsConnected()
{
	QMutexLocker locker(&m_rw_mutex);
	if (!client)
	{
		return false;
	}
	return client->Connected();
}
//����
bool PLCComm::WraperConnect()
{
	if (client->ConnectTo(IP_PLC, RackID, SlotID))
	{
		qCritical() << "Conntect to PLC failed";
		return false;
	}
	return true;
}
//�Ͽ�����
bool PLCComm::WraperDisConnect()
{
	QMutexLocker locker(&m_rw_mutex);
	if (client && client->Connected())
	{
		client->Disconnect();
	}
	qDebug() << "client disconnect";

	return true;
}
//intתbyte
void PLCComm::intToByte(int i, byte *bytes, int size)
{
	memset(bytes, 0, sizeof(byte) *  size);
	bytes[0] = (byte)(0xff & i);
	bytes[1] = (byte)((0xff00 & i) >> 8);
	bytes[2] = (byte)((0xff0000 & i) >> 16);
	bytes[3] = (byte)((0xff000000 & i) >> 24);
	return;
}