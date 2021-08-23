#include "SR_Sdk.h"
#include <Windows.h>

SR_Sdk::SR_Sdk()
{
	VaribleParamInit();
}

SR_Sdk::~SR_Sdk()
{

}

/********************图像显示函数*******************************/
void SR_Sdk::BatchDataShow(int *_BatchData, double max_height, double min_height,
	int _ColorMax, int img_w, int img_h, int _xscale, int _yscale, int _scaleW, int _scaleH)
{
	if (_BatchData == NULL || img_h == 0 || img_w == 0)
		return;

	/* 数据转换 */
	double mSub = max_height - min_height;
	if (mSub <= 0.000001)
		mSub = 1;

	double fscale = double(_ColorMax) / mSub;   //颜色区间与高度区间比例

	/* 抽帧抽点显示 */
	int imgW = _scaleW;
	int imgH = _scaleH;

	int TmpX = 0;
	int Tmppx = 0;

	if (img_h < imgH)
		imgH = img_h;

	if (img_w < imgW)
		imgW = img_w;

	int TT = (imgW * 8 + 31) / 32;   //图像宽度4字节对齐
	TT = TT * 4;

	int dwDataSize = TT * imgH;
	unsigned char* BatchImage = new unsigned char[dwDataSize];
	memset(BatchImage, 0, sizeof(BatchImage));

	for (int i = 0; i < imgH; i++)
	{
		TmpX = i * _yscale * img_w;
		Tmppx = i * TT;
		for (int j = 0; j < imgW; j++)
		{
			double Tmp = double(_BatchData[TmpX + j * _xscale]) * 0.00001;
			if (Tmp < min_height)
				BatchImage[Tmppx + j] = 0;
			else if (Tmp > max_height)
				BatchImage[Tmppx + j] = 0xff;
			else
			{
				unsigned char tmpt = unsigned char((Tmp - min_height) * fscale);
				BatchImage[Tmppx + j] = tmpt;
			}
		}
	}

	//转成图像显示
	QImage heightImage = QImage(BatchImage, imgW, imgH, QImage::Format_Indexed8);
	QVector<QRgb> grayTable;
	for (int i = 0; i < 256; i++)
	{
		grayTable.push_back(qRgb(i, i, i));
	}
	heightImage.setColorTable(grayTable);
	//发送显示高度图
	emit BatchDataShowsignal(heightImage);
	if (BatchImage)
	{
		delete[] BatchImage;
		BatchImage = NULL;
	}
}

void SR_Sdk::GrayDataShow(unsigned char* _grayData,
	int img_w, int img_h, int _xscale, int _yscale, int _scaleW, int _scaleH)
{
	if (_grayData == NULL || img_h == 0 || img_w == 0)
		return;

	/* 抽帧抽点显示 */
	int imgW = _scaleW;
	int imgH = _scaleH;

	int TmpX = 0;
	int Tmppx = 0;

	if (img_h < imgH)
		imgH = img_h;

	if (img_w < imgW)
		imgW = img_w;

	int TT = (imgW * 8 + 31) / 32;  //图像4字节对齐
	TT = TT * 4;

	unsigned char* BatchImage = new unsigned char[TT * imgH];
	memset(BatchImage, 0, sizeof(BatchImage));

	for (int i = 0; i < imgH; i++)
	{
		TmpX = i * _yscale * img_w;
		Tmppx = i * TT;
		for (int j = 0; j < imgW; j++)
		{
			BatchImage[Tmppx + j] = _grayData[TmpX + j * _xscale];
		}
	}

	//转成图像显示
	QImage heightImage = QImage(BatchImage, imgW, imgH, QImage::Format_Indexed8);
	QVector<QRgb> grayTable;
	for (int i = 0; i < 256; i++)
	{
		grayTable.push_back(qRgb(i, i, i));
	}
	heightImage.setColorTable(grayTable);
	//发送显示灰度图
	emit GrayDataShowsignal(heightImage);
	if (BatchImage)
	{
		delete[] BatchImage;
		BatchImage = NULL;
	}
}

/*************************信号响应函数*************************************/
void SR_Sdk::CallOneTimes_DataShowFunc(int _width, int _height)
{
	//缩放显示系数计算---_width X _height 缩放为 800 X 560 大小图像
	int mScaleW = 800;
	int mScaleH = 560;
	int mXsacle = int(double(_width) / mScaleW);    /* X方向缩放倍数 */
	int mYscale = int(double(_height) / mScaleH);   /* Y方向缩放倍数 */

	//显示的相机ID
	int mCamId = 0;
	BatchDataShow(_callOneTimes->getBatchData(mCamId), d_HeightUpper, d_HeightLower,
		255, _width, _height, mXsacle, mYscale, mScaleW, mScaleH);
	GrayDataShow(_callOneTimes->getIntensityData(mCamId), _width, _height,
		mXsacle, mYscale, mScaleW, mScaleH);
}

/***************************其他***********************************/
//保存IP地址
void SR_Sdk::saveIpAddress(SR7IF_ETHERNET_CONFIG _SREthernetConFig)
{
	/* 保存当前IP */
	std::fstream file("IpSet.bin", std::ios::out);
	file << (int)_SREthernetConFig.abyIpAddress[0]
		<< ","
		<< (int)_SREthernetConFig.abyIpAddress[1]
		<< ","
		<< (int)_SREthernetConFig.abyIpAddress[2]
		<< ","
		<< (int)_SREthernetConFig.abyIpAddress[3]
		<< "," << "\n";
	file.close();
}
//高度区间显示上下限获取
void SR_Sdk::getHeightUpperLower(double& _upper, double& _lower)
{
	const char* _version = SR7IF_GetModels(DEVICEID);   //型号获取
	QString str_Version;
	str_Version = QString(_version);

	double m_dHeightRange = 8.4;
	if (str_Version == tr("SR7050") || str_Version == tr("SR7060D"))
	{
		m_dHeightRange = 3;
	}
	else if (str_Version == tr("SR7080"))
	{
		m_dHeightRange = 9;
	}
	else if (str_Version == tr("SR7140"))
	{
		m_dHeightRange = 15;
	}
	else if (str_Version == tr("SR7240"))
	{
		m_dHeightRange = 24;
	}
	else if (str_Version == tr("SR7400"))
	{
		m_dHeightRange = 60;
	}
	else if (str_Version == tr("SR7300"))
	{
		m_dHeightRange = 150;
	}
	else if (str_Version == tr("SR6060"))
	{
		m_dHeightRange = 15;
	}
	else if (str_Version == tr("SR6030"))
	{
		m_dHeightRange = 8;
	}
	else if (str_Version == tr("SR6070"))
	{
		m_dHeightRange = 16;
	}
	else if (str_Version == tr("SR6071"))
	{
		m_dHeightRange = 40;
	}
	else if (str_Version == tr("SR6130"))
	{
		m_dHeightRange = 100;
	}
	else if (str_Version == tr("SR6260"))
	{
		m_dHeightRange = 240;
	}
	else if (str_Version == tr("SR8020"))
	{
		m_dHeightRange = 6;
	}
	else if (str_Version == tr("SR8060"))
	{
		m_dHeightRange = 20;
	}
	else if (str_Version == tr("SR7900"))
	{
		m_dHeightRange = 450;
	}
	else if (str_Version == tr("SR7060"))
	{
		m_dHeightRange = 6;
	}
	else if (str_Version == tr("SR71600"))
	{
		m_dHeightRange = 1500;
	}

	_upper = m_dHeightRange;
	_lower = -m_dHeightRange;
}
//断开连接前的处理
void SR_Sdk::InitConfigBeforeDisConnect()
{
	if (b_BatchStartFlag)
	{
		//停止批处理
		SR7IF_StopMeasure(DEVICEID);
		b_BatchStartFlag = false;
	}

	InitCallOneTimesBeforeDisConnect();
}
//断开 一次性回调设置
void SR_Sdk::InitCallOneTimesBeforeDisConnect()
{
	if (_callOneTimes)
	{
		//退出图像显示线程
		_callOneTimes->exitImageDiap();
		//数据缓存释放
		_callOneTimes->deleteDataMemory();
	}
}
//变量初始化
void SR_Sdk::VaribleParamInit()
{
	bConnectFlag = false;
	b_HighSpeedInitFail = false;
	b_BatchStartFlag = false;

	mCurDispType = 0;
	mCurCamId = 0;
	d_HeightUpper = 8.4;
	d_HeightLower = -8.4;

	_callOneTimes = NULL;
}

/***********************响应函数*************************************/
//连接
bool SR_Sdk::Connect(int address1, int address2, int address3, int address4)
{
	//sdk dll 连接参数配置
	SR7IF_ETHERNET_CONFIG SREthernetConFig;

	/* 获取IP控件地址4个值 */
	/* 从界面获取IP地址--界面IP输入值 */
	SREthernetConFig.abyIpAddress[0] = address1;
	SREthernetConFig.abyIpAddress[1] = address2;
	SREthernetConFig.abyIpAddress[2] = address3;
	SREthernetConFig.abyIpAddress[3] = address4;

	/* 连接相机 */
	int reT = SR7IF_EthernetOpen(DEVICEID, &SREthernetConFig);
	if (reT < 0)   //失败    
		return false;

	//成功
	bConnectFlag = true;
	saveIpAddress(SREthernetConFig);
	getHeightUpperLower(d_HeightUpper, d_HeightLower);

	//相机B是否在线
	bool bCamBOnline = false;
	int RT = SR7IF_GetOnlineCameraB(DEVICEID);
	if (RT == 0)
		bCamBOnline = true;

	int mBatchW = SR7IF_ProfileDataWidth(DEVICEID, NULL);

	//新建模式
	if (_callOneTimes == NULL)
	{
		_callOneTimes = new CallOneTimes();
		connect(_callOneTimes, &CallOneTimes::SignalDataShow, this, &SR_Sdk::CallOneTimes_DataShowFunc);
	}

	//初始化
	_callOneTimes->VariableInit();
	_callOneTimes->setDeviceId(DEVICEID);
	_callOneTimes->DataMemoryInit(mBatchW);
	mCurCamId = 0;
	return true;
}
//断开
void SR_Sdk::DisConnect()
{
	//断开连接前做相关处理
	InitConfigBeforeDisConnect();

	/* 断开连接 */
	int reT = SR7IF_CommClose(DEVICEID);
	if (reT < 0)
		return;

	bConnectFlag = false;
	b_HighSpeedInitFail = false;
}
//相机配置导出
void SR_Sdk::Export(QString fileName)
{
	if (!bConnectFlag)
		return;

	if (b_BatchStartFlag)
		return;

	if (fileName.isEmpty() || !fileName.contains(tr(".ssp")))
		return;

	unsigned int filesize = 0;
	char * SettingParam = (char*)SR7IF_ExportParameters(DEVICEID, &filesize);

	if (SettingParam == NULL)
		return;

	//保存为文件
	/* 定义一个句柄 */
	HANDLE hFile;
	const wchar_t *sLinkName = reinterpret_cast<const wchar_t *>(fileName.utf16());
	hFile = CreateFile(sLinkName,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_FLAG_WRITE_THROUGH,
		NULL);

	/* 定义一个变量来接收实际写入的字符数 */
	DWORD realWriteSize;
	WriteFile(hFile, SettingParam, sizeof(char) * filesize, &realWriteSize, NULL);
	CloseHandle(hFile); /*关闭文件 关闭句柄。*/
}
//相机配置导入
bool SR_Sdk::Load(QString fileName)
{
	if (!bConnectFlag)	
		return false;
	
	if (b_BatchStartFlag)	
		return false;
	
	if (fileName.isEmpty() || !fileName.contains(tr(".ssp")))
		return false;

	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly))
	{
		int fileSize = file.size();
		char *_data = new char[fileSize];
		file.read(_data, fileSize);
		file.close();

		int reT = SR7IF_LoadParameters(DEVICEID, _data, fileSize);
		if (reT < 0)
			return false;
		else
			return true;
	}
	else
		return false;
}
//选择显示高度图
void SR_Sdk::HeightImg()
{
	if (mCurDispType != 0)
	{
		mCurDispType = 0;
		if (!b_BatchStartFlag)   //批处理结束后
		{
			//一次性回调
			if (_callOneTimes)
			{
				_callOneTimes->ImageUpdate();
			}
		}
	}
}
//选择显示灰度图
void SR_Sdk::GrayImg()
{
	if (mCurDispType != 1)
	{
		mCurDispType = 1;
		if (!b_BatchStartFlag)  //批处理结束后
		{
			//一次性回调
			if (_callOneTimes)
			{
				_callOneTimes->ImageUpdate();
			}
		}
	}
}
//选择显示相机A数据
void SR_Sdk::CamA()
{
	if (mCurCamId != 0)
	{
		mCurCamId = 0;
		if (!b_BatchStartFlag)  //批处理结束后
		{
			//一次性回调		
			if (_callOneTimes)
			{
				_callOneTimes->ImageUpdate();
			}
		}
	}
}
//选择显示相机B数据
void SR_Sdk::CamB()
{
	if (mCurCamId != 1)
	{
		mCurCamId = 1;
		if (!b_BatchStartFlag)  //批处理结束后
		{
			//一次性回调			
			if (_callOneTimes)
			{
				_callOneTimes->ImageUpdate();
			}
		}
	}
}
//高度数据保存
bool SR_Sdk::HSave(QString fileName)
{
	if (fileName.isEmpty())
		return false;

	if (_callOneTimes)
	{
		int reT = _callOneTimes->saveHeightData(fileName, 0);
		if (reT < 0)
			return false;
		else
			return true;
	}
	return false;
}
//灰度（亮度）数据保存
bool SR_Sdk::GSave(QString fileName)
{
	if (fileName.isEmpty())
		return false;
	if (_callOneTimes)
	{
		int reT = _callOneTimes->saveIntensityData(fileName, 0);
		if (reT < 0)
			return false;
		else
			return true;
	}
	return false;
}
//编码器数据保存
bool SR_Sdk::ESave(QString fileName)
{
	if (fileName.isEmpty())
		return false;
	if (_callOneTimes)
	{
		int reT = _callOneTimes->saveEncoderData(fileName, 0);
		if (reT < 0)
			return false;
		else
			return true;
	}
	return false;
}
//Y间距设置
void SR_Sdk::YInterV(double arg1)
{
	if (_callOneTimes)
	{
		_callOneTimes->set3DYinterval(arg1);
	}
}
//设置高度显示上限
void SR_Sdk::Upper(double arg1)
{
	d_HeightUpper = arg1;
}
//设置高度显示下限
void SR_Sdk::Lower(double arg1)
{
	d_HeightLower = arg1;
}
//3D显示
void SR_Sdk::_3DShow()
{
	double y_true_step = 0.1;
	double z_scale = 1;
	double Ho = d_HeightUpper;
	double Lo = d_HeightLower;
	if (_callOneTimes)
	{
		int mCamId = 0;
		if (_callOneTimes->getBatchData(mCamId)
			&& _callOneTimes->getProfileWidth() != 0
			&& _callOneTimes->getBatchHeight() != 0)
		{
			SR_3D_EXE_Show(_callOneTimes->getBatchData(mCamId),
				_callOneTimes->getXpitch(), y_true_step,
				_callOneTimes->getProfileWidth(),
				_callOneTimes->getBatchHeight(),
				z_scale, Ho, Lo);
		}		
	}
}
//一次性回调开始批处理
bool SR_Sdk::StartOneCall()
{
	if (!bConnectFlag)	
		return false;
	
	if (_callOneTimes == NULL)	
		return false;
	
	//等待开始批处理变量设置
	_callOneTimes->setBatchWait(1);

	//设置回调函数
	int reT = _callOneTimes->setBatchOneTimeDataHandler();
	if (reT < 0)	
		return false;
	
	//开始批处理标志
	_callOneTimes->setBatchFinishFlagInit();

	//开始批处理
	reT = _callOneTimes->startMeasureWithCallback();
	if (reT < 0)	
		return false;
	
	//启动图像显示线程
	_callOneTimes->startImageDisp();
	b_BatchStartFlag = true;
	return true;
}
//一次性回调软触发
bool SR_Sdk::SoftTrigger()
{
	if (!bConnectFlag)	
		return false;
	
	if (_callOneTimes == NULL)
		return false;
	
	int reT = _callOneTimes->SoftTrigger();
	if (reT < 0)
		return false;
	return true;
}
//停止批处理
void SR_Sdk::StopMea()
{
	if (!bConnectFlag)	
		return;
	
	int reT = SR7IF_StopMeasure(DEVICEID);
	if (reT < 0)	
		return;
	
	//退出图像显示线程
	if (_callOneTimes)	
		_callOneTimes->exitImageDiap();
	
	b_BatchStartFlag = false;
}
