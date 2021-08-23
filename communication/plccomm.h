#ifndef PLCCOMM_H
#define PLCCOMM_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include "snap7.h"
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable

//重复发送次数
static constexpr int DB_RW_RETRY_TIMES = 0;
//间隔时间
static constexpr int DB_RW_RETRY_INTERVAL_MSEC = 10; // ms

//架号
static constexpr int RackID = 0;
//槽号
static constexpr int SlotID = 2;

class PLCComm : public QObject
{
	Q_OBJECT

public:
	PLCComm();
	~PLCComm();

private:
	//通信实例
	TS7Client *client;
	//plc ip地址
	const char *IP_PLC = "172.16.2.121";
	//是否运行
	bool m_running;
	QMutex m_rw_mutex;

public:
	//初始化
	bool init(QString ip);
	//断开
	void disconnect();
	//读bool 例：地址输入20.0 内容输出0、1
	bool boolread(QString address, bool &result);
	//写bool 例：地址输入20.0 内容输入0、1
	bool boolwrite(QString address, bool value);
	//读int 例：地址输入20.0 内容输出数值（int型）
	bool intread(QString address, int &result);
	//写int 例：地址输入20.0 内容输入数值（int型）
	bool intwrite(QString address, int value);

private:
	//wraper
	//写入DB
	bool WraperDBWrite(int DBNumber, int Start, int Size, void *pUsrData);
	//读取DB
	bool WraperDBRead(int DBNumber, int Start, int Size, void *pUsrData);
	//写入多个值
	void WraperWriteMultiVars(PS7DataItem Item, int ItemsCount);
	//连接
	bool WraperConnect();
	//是否连接
	bool WraperIsConnected();
	//断开连接
	bool WraperDisConnect();
	//int转byte
	void intToByte(int i, byte *bytes, int size = 4);
};

#endif // PLCCOMM_H
