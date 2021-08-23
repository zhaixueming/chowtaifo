/****************************************************************************
 *  @file     mainframe.h
 *  @brief    界面主显示框类
 *  @author   junjie.zeng
 *  @email    junjie.zeng@smartmore.com
 *  @version
 *  @date     2020.12.21
 *  Copyright (c) 2020 SmartMore All rights reserved.
 ****************************************************************************/
#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <QMainWindow>
#include <QQueue>
#include <QTranslator>
#include <QDir>
#include <QMutex>
#include <windows.h>
#include "titlebarwidget.h"
#include <QDesktopWidget>

#include "logservice/logservice.h"
#include "logservice/dataservice.h"
#include "uiframe/commonuiset/messagedialog/messagedialog.h"
#include "uiframe/commonuiset/messagedialog/progressbardialog.h"
#include "uiframe/configuiset/configdialog.h"
#include "uiframe/configuiset/camsetdialog.h"
#include "uiframe/configuiset/stereoscopicdialog.h"
#include "infowidget.h"
#include "buttonwidget.h"
#include "resultwidget.h"
#include "statisticswidget.h"
#include "datawidget.h"
#include "serializer/Config.h"
#include "singleviewwidget.h"
#include "imageviews/sixviewswidget.h"
#include "Flow/serialcommond.h"

#include "cameraservice/hikvisionsdk.h"
#include "cameraservice/SR_Sdk.h"
#include "SR7Link.h"

#include "communication/plccomm.h"

namespace Ui
{
	class MainFrame;
}

namespace vimo::display
{
	class TitleBarWidget;
	class LogWidget;
    class DataWidget;

	class MainFrame : public QMainWindow
	{
		Q_OBJECT

	public:
		explicit MainFrame(QWidget *parent = nullptr);
		~MainFrame();
		void setTranslator(QTranslator *);

	private:
		Ui::MainFrame *ui;
		TitleBarWidget *m_titlebar_widget{ nullptr };
		ButtonWidget *m_button_widget{ nullptr };
		StatisticsWidget *m_statistics_widget{ nullptr };
		InfoWidget *m_info_widget{ nullptr };
		SingleViewWidget *m_single_view{ nullptr };
		ResultWidget *m_result_widget{ nullptr };
		DataWidget *m_data_widget{ nullptr };
		LogWidget *m_log_widget{ nullptr };

		// 图片显式框
		QWidget *m_sixview_frame{ nullptr };
		QWidget *m_ocrview_frame{ nullptr };
		QWidget *m_singleview_frame{ nullptr };
		// 界面语言// 语言
		int m_lang{ 0 };
		// 界面语言
		QTranslator *m_translator{ nullptr };
		ConfigDialog *m_config_set{ nullptr };
		CamsetDialog *m_cam_set{ nullptr };
		StereoscopicDialog *m_stereoscopic_set{ nullptr };
		BarDialog *_bardialog = new BarDialog();

	private:
		//存图文件夹
		QString CamSaveTopPath;
		QString CamSaveInPath;
		QString CamSaveSide1Path;
		QString CamSaveSide2Path;
		QString CamSaveSide3Path;
		QString CamSaveSide4Path;
		QString CamSaveSPath;
		
		//配方
		SystemParam m_sys_param;
		
		//海康相机声明
		HikvisionSDK* m_hikcam_top;//在此声明，后在源文件中实例化
		HikvisionSDK* m_hikcam_in;
		HikvisionSDK* m_hikcam_side1;
		HikvisionSDK* m_hikcam_side2;
		HikvisionSDK* m_hikcam_side3;
		HikvisionSDK* m_hikcam_side4;

		//3d声明
		SR_Sdk *m_sszn_cam = new SR_Sdk();

		//获取图像队列
		QQueue<cv::Mat> QueueImageTop;
		QMutex MutexImageTop;
		QQueue<cv::Mat> QueueImageIn;
		QMutex MutexImageIn;
		QQueue<cv::Mat> QueueImageSide1;
		QMutex MutexImageSide1;
		QQueue<cv::Mat> QueueImageSide2;
		QMutex MutexImageSide2;
		QQueue<cv::Mat> QueueImageSide3;
		QMutex MutexImageSide3;
		QQueue<cv::Mat> QueueImageSide4;
		QMutex MutexImageSide4;
		QQueue<cv::Mat> QueueImageS;
		QMutex MutexImageS;

		//图像处理队列
		unsigned int __stdcall ImageTopThread();
		unsigned int __stdcall ImageInThread();
		unsigned int __stdcall ImageSide1Thread();
		unsigned int __stdcall ImageSide2Thread();
		unsigned int __stdcall ImageSide3Thread();
		unsigned int __stdcall ImageSide4Thread();
		unsigned int __stdcall ImageSThread();

		std::thread TopImageListener;
		std::thread InImageListener;
		std::thread Side1ImageListener;
		std::thread Side2ImageListener;
		std::thread Side3ImageListener;
		std::thread Side4ImageListener;
		std::thread SImageListener;

		//plc声明
		PLCComm *_plc = new PLCComm();
		//PLC通信线程
		unsigned int __stdcall PlcThread();
		std::thread PlcListener;

		//plc消息队列
		QQueue<bool> Queueallowrotate;
		QMutex Mutexallowrotate;
		QQueue<int> Queueresult;
		QMutex Mutexresult;
		QQueue<int> Queueaccomplish;
		QMutex Mutexaccomplish;

		//账号权限
		int adminstatus = 2;
		//初始化是否成功
		bool bstart = true;
		//是否在首页显示
		bool bshowcam = true;
		//关闭软件标志位
		bool bclose = true;

	private:
		//初始化
		void init();
		//登录
		void load();
		//界面初始化
		void initWidgets();
		//界面信号槽
		void initConnects();
		//初始化配置
		void initconfig();
		//初始化PLC
		void initPlc();
		//初始化hik相机
		void initHik();
		//3D相机初始化
		void initStereoscopic();
		//开启批处理
		void StereoscopicBatch();
		//网络通信初始化
		void initNet();
		//读码器初始化
		void initCode();
		//数据库初始化
		void initSQL();
		//线程初始化
		void initthread();
		//显示信号槽
		void ShowImage();
		//清理队列
		void clearqueue();

		//控制曝光
		bool CamControl();
		//创建文件夹
		bool isDirExist(QString fullPath);
		//日志
		void InfoLog(QString log);
		void WarningLog(QString log);
		void ErrorLog(QString log);
        //Data
        void InfoData(QString data);
        void WarningData(QString data);
        void ErrorData(QString data);


	signals:
		//进度条信号
		void progresssignals(int value);
		

	private slots:
		//修改界面语言	
		void slotChangeLang();
		//按钮控制
		void loadslot(int adminctrl);
		//打开3d设置界面
		void stereoscopicslot();	
		//设置曝光
		void SetSexposure(int exp);
		//设置细化点数
		void SetSrefined(int point);
		//设置批处理点数
		void SetSbatch(int batch);

		//打开2d设置界面
		void camslot();
		//载入系统配置
		void configslot();		
		//进度条
		void progressslots(int value);				
		//设置曝光
		void camslotE(float value, int index);		
		//设置增益
		void camslotG(float value, int index);
		
	};
}  // namespace vimo::display
#endif  // MAINFRAME_H
