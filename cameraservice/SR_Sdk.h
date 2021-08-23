#ifndef SR_SDKSAMPLE_H
#define SR_SDKSAMPLE_H

#include <QMainWindow>
#include <QMessageBox>
#include <SR7Link.h>
#include <cameraservice/SR3dexe.h>
#include <fstream>
#include <QFileDialog>
#include <QTextEdit>
#include "cameraservice/CallOneTimes.h"

#define DEVICEID 0

class CallOneTimes;

class SR_Sdk : public QObject
{
	Q_OBJECT

public:
	explicit SR_Sdk();
	~SR_Sdk();

	/*******信号槽响应******/
private:
	///
	/// \brief CallOneTimes_DataShowFunc  一次性回调数据显示
	/// \param _width                     宽
	/// \param _height                    高
	///
	void CallOneTimes_DataShowFunc(int _width, int _height);
	/*********界面按钮响应********/
public:
	bool Connect(int address1, int address2, int address3, int address4);  //连接
	void DisConnect();       //断开
	void Export(QString fileName);           //相机配置导出
	bool Load(QString fileName);             //相机配置导入
	void StopMea();          //停止批处理
	void HeightImg();        //选择显示高度图
	void GrayImg();          //选择显示灰度图
	void CamA();             //选择显示相机A数据
	void CamB();             //选择显示相机B数据
	bool HSave(QString fileName);            //高度数据保存
	bool GSave(QString fileName);            //灰度（亮度）数据保存
	bool ESave(QString fileName);            //编码器数据保存
	void YInterV(double arg1);    //Y间距设置
	void _3DShow();                       //3D显示
	void Upper(double arg1);      //设置高度显示上限
	void Lower(double arg1);      //设置高度显示下限
	bool StartOneCall();                 //一次性回调开始批处理
	bool SoftTrigger();                  //一次性回调软触发
	
private:
	///
	/// \brief VaribleParamInit   变量初始化
	///
	void VaribleParamInit();
	///
	/// \brief saveIpAddress       保存IP地址
	/// \param _SREthernetConFig   IP配置参数
	///
	void saveIpAddress(SR7IF_ETHERNET_CONFIG _SREthernetConFig);
	///
	/// \brief getHeightUpperLower 高度区间显示上下限获取
	/// \param _upper              上限
	/// \param _lower              下限
	///
	void getHeightUpperLower(double& _upper, double& _lower);
	///
	/// \brief InitConfigBeforeDisConnect    断开连接前的处理
	///
	void InitConfigBeforeDisConnect();
	///
	/// \brief InitCallOneTimesBeforeDisConnect  断开 一次性回调设置
	///
	void InitCallOneTimesBeforeDisConnect();	
	///
	/// \brief BatchDataShow  高度图显示
	/// \param _BatchData     高度数据缓存
	/// \param max_height     高度显示区间上限
	/// \param min_height     高度显示区间下限
	/// \param _ColorMax      高度区间范围对应的灰度最大值 255
	/// \param img_w          图像宽度-----轮廓宽度
	/// \param img_h          图像高度-----批处理行数
	/// \param _xscale        图像宽度缩放倍数
	/// \param _yscale        图像高度缩放倍数
	/// \param _scaleW        缩放图像宽
	/// \param _scaleH        缩放图像高
	///
	void BatchDataShow(int *_BatchData,
		double max_height,
		double min_height,
		int _ColorMax,
		int img_w,
		int img_h,
		int _xscale,
		int _yscale,
		int _scaleW,
		int _scaleH);
	///
	/// \brief GrayDataShow  灰度(亮度)图显示
	/// \param _grayData     灰度数据缓存
	/// \param img_w         图像宽度-----轮廓宽度
	/// \param img_h         图像高度-----批处理行数
	/// \param _xscale       图像宽度缩放倍数
	/// \param _yscale       图像高度缩放倍数
	/// \param _scaleW       缩放图像宽
	/// \param _scaleH       缩放图像高
	///
	void GrayDataShow(unsigned char* _grayData,
		int img_w,
		int img_h,
		int _xscale,
		int _yscale,
		int _scaleW,
		int _scaleH);
signals:
	void BatchDataShowsignal(QImage batchimage);
	void GrayDataShowsignal(QImage grayimage);

private:
	bool bConnectFlag;                      //连接标志
	bool b_HighSpeedInitFail;               //多次回调高速数据通信初始化失败标志
	bool b_BatchStartFlag;                  //开始批处理标志
	CallOneTimes* _callOneTimes;            //一次性回调

	//显示相关
	double d_HeightUpper;                   //高度区间显示上限
	double d_HeightLower;                   //高度区间显示下限
	int mCurDispType;                       //当前显示类型 0:高度 1:灰度
	int mCurCamId;                          //当前选择显示的相机
};

#endif // SR_SDKSAMPLE_H
