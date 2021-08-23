#ifndef HIKVISIONSDK_H
#define HIKVISIONSDK_H
#include <QObject>
#include <QSTring>
#include <windows.h>
#include "opencv2/opencv.hpp"
#include "iostream"
#include <MvCameraControl.h>

using namespace cv;

namespace vimo::display
{
	class HikvisionSDK : public QObject
	{
		Q_OBJECT
	public:
		HikvisionSDK() = delete;
		HikvisionSDK(QString camName);
		~HikvisionSDK();

		// ch:连接设备
		int connectCamera(std::string id);

		// 注册图像数据回调
		int RegisterImageCallBack(void(__stdcall* cbOutput)(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser));

		// 注册消息异常回调
		int RegisterExceptionCallBack(void(__stdcall* cbException)(unsigned int nMsgType, void* pUser));

		//设置是否为触发模式
		int setTriggerMode(unsigned int TriggerModeNum);

		//开启相机采集
		int startCamera();

		// 设置显示窗口句柄
		int Display(void* hWnd);

		// 保存图片
		int SaveImage(MV_SAVE_IMAGE_PARAM_EX* pstParam);

		//关闭相机采集
		int stopCamera();

		//关闭相机
		int closeCamera();

		//软触发
		int softTrigger();

		//读取buffer
		int ReadBuffer(cv::Mat &image);

		//设置图像高度
		int setHeight(unsigned int height);

		//设置图像ROI宽度
		int setWidth(unsigned int width);

		//获取图像高度值
		int getHeight();

		//获取图像宽度值
		int getWidth();

		//获取相机曝光时间
		float getExposureTime();

		//获取相机增益
		float getGain();

		//设置图像水平偏移OffsetX
		int setOffsetX(unsigned int offsetX);

		//设置图像竖直偏移OffsetY
		int setOffsetY(unsigned int offsetY);

		//设置触发源
		int setTriggerSource(unsigned int TriggerSourceNum);

		//设置帧率控制使能
		int setFrameRateEnable(bool comm);

		//设置心跳时间
		int setHeartBeatTime(unsigned int time);

		//设置曝光时间
		int setExposureTime(float ExposureTimeNum);

		//设置增益
		int setGain(float GainNum);

		//关闭自动曝光
		int setExposureAuto(bool exposureAutoFlag);

		//关闭自动增益
		int setGainAuto(bool gainAutoFlag);

		//清理相机缓存
		void clearCamera();

		//发射信号
		void emitSignaltop(const cv::Mat& image);
		void emitSignalin(const cv::Mat& image);
		void emitSignalside1(const cv::Mat& image);
		void emitSignalside2(const cv::Mat& image);
		void emitSignalside3(const cv::Mat& image);
		void emitSignalside4(const cv::Mat& image);

	signals:
		void imageshowsignaltop(const cv::Mat& image);
		void imagesaveignaltop(const cv::Mat& image);
		void imageoperationignaltop(const cv::Mat& image);

		void imageshowsignalin(const cv::Mat& image);
		void imagesaveignalin(const cv::Mat& image);
		void imageoperationignalin(const cv::Mat& image);

		void imageshowsignalside1(const cv::Mat& image);
		void imagesaveignalside1(const cv::Mat& image);
		void imageoperationignalside1(const cv::Mat& image);

		void imageshowsignalside2(const cv::Mat& image);
		void imagesaveignalside2(const cv::Mat& image);
		void imageoperationignalside2(const cv::Mat& image);

		void imageshowsignalside3(const cv::Mat& image);
		void imagesaveignalside3(const cv::Mat& image);
		void imageoperationignalside3(const cv::Mat& image);

		void imageshowsignalside4(const cv::Mat& image);
		void imagesaveignalside4(const cv::Mat& image);
		void imageoperationignalside4(const cv::Mat& image);

	private:
		void* m_hDevHandle;

	public:
		// 用于保存图像的缓存
		unsigned int    m_nBufSizeForSaveImage;
		// 用于从驱动获取图像的缓存
		unsigned int    m_nBufSizeForDriver;

		QString m_camName;		
	};
}
#endif // HIKVISIONSDK_H
