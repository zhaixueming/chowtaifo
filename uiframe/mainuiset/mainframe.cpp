/****************************************************************************
 *  @file     mainframe.cpp
 *  @brief    界面主显示框类
 *  @author   junjie.zeng
 *  @email    junjie.zeng@smartmore.com
 *  @version
 *  @date     2020.12.21
 *  Copyright (c) 2020 SmartMore All rights reserved.
 ****************************************************************************/
#include "mainframe.h"
#include "vimocommon/vimocommon.h"

#include "logwidget.h"
#include "datawidget.h"
#include "titlebarwidget.h"
#include "ui_mainframe.h"
#include <QMessageBox>
#include <QtDebug>

#pragma region hik回调
 //海康回调
void __stdcall ImageCallBackTop(unsigned char *pData, MV_FRAME_OUT_INFO_EX *pFrameInfo, void *pUser)
{
	if (pFrameInfo)
	{
		vimo::display::HikvisionSDK *camPtr = (vimo::display::HikvisionSDK *)pUser;
		Mat GetImage = Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC3, pData);
		camPtr->emitSignaltop(GetImage);
	}
}
void __stdcall ImageCallBackIn(unsigned char *pData, MV_FRAME_OUT_INFO_EX *pFrameInfo, void *pUser)
{
	if (pFrameInfo)
	{
		vimo::display::HikvisionSDK *camPtr = (vimo::display::HikvisionSDK *)pUser;
		Mat GetImage = Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC3, pData);
		camPtr->emitSignalin(GetImage);
	}
}
void __stdcall ImageCallBackSide1(unsigned char *pData, MV_FRAME_OUT_INFO_EX *pFrameInfo, void *pUser)
{
	if (pFrameInfo)
	{
		vimo::display::HikvisionSDK *camPtr = (vimo::display::HikvisionSDK *)pUser;
		Mat GetImage = Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC3, pData);
		camPtr->emitSignalside1(GetImage);
	}
}
void __stdcall ImageCallBackSide2(unsigned char *pData, MV_FRAME_OUT_INFO_EX *pFrameInfo, void *pUser)
{
	if (pFrameInfo)
	{
		vimo::display::HikvisionSDK *camPtr = (vimo::display::HikvisionSDK *)pUser;
		Mat GetImage = Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC3, pData);
		camPtr->emitSignalside2(GetImage);
	}
}
void __stdcall ImageCallBackSide3(unsigned char *pData, MV_FRAME_OUT_INFO_EX *pFrameInfo, void *pUser)
{
	if (pFrameInfo)
	{
		vimo::display::HikvisionSDK *camPtr = (vimo::display::HikvisionSDK *)pUser;
		Mat GetImage = Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC3, pData);
		camPtr->emitSignalside3(GetImage);
	}
}
void __stdcall ImageCallBackSide4(unsigned char *pData, MV_FRAME_OUT_INFO_EX *pFrameInfo, void *pUser)
{
	if (pFrameInfo)
	{
		vimo::display::HikvisionSDK *camPtr = (vimo::display::HikvisionSDK *)pUser;
		Mat GetImage = Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC3, pData);
		camPtr->emitSignalside4(GetImage);
	}
}
#pragma endregion

namespace vimo::display
{
	MainFrame::MainFrame(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainFrame)
	{
		//初始化
		init();
		//登录
		//load();

	}

	MainFrame::~MainFrame()
	{
		bclose = false;
		delete ui;

		try
		{
			m_sszn_cam->DisConnect();
		}
		catch (...)
		{

		}

		try
		{
			if (TopImageListener.joinable())
				TopImageListener.join();
			if (InImageListener.joinable())
				InImageListener.join();
			if (Side1ImageListener.joinable())
				Side1ImageListener.join();
			if (Side2ImageListener.joinable())
				Side2ImageListener.join();
			if (Side3ImageListener.joinable())
				Side3ImageListener.join();
			if (Side4ImageListener.joinable())
				Side4ImageListener.join();
			if (SImageListener.joinable())
				SImageListener.join();
		}
		catch (const std::exception&)
		{
		}

		try
		{
			_plc->disconnect();
		}
		catch (const std::exception&)
		{
		}
	}

	//初始化
	void MainFrame::init()
	{
		//初始化界面
		initWidgets();
		//初始化界面信号槽
		initConnects();
		//初始化配置
		initconfig();
		emit this->progresssignals(1);
		////初始化PLC
		//initPlc();
		//emit this->progresssignals(30);
		////初始化Hik相机
		//initHik();
		emit this->progresssignals(80);
		//3D相机初始化
		initStereoscopic();

		////网络通信初始化
		//initNet();
		////读码器初始化
		//initCode();
		////数据库初始化
		//initSQL();

		////线程初始化
		//initthread();
		//qRegisterMetaType<cv::Mat>("cv::Mat");
		//qRegisterMetaType<cv::Mat>("cv::Mat&");
		//qRegisterMetaType<QString>("QString");
		//emit this->progresssignals(90);
		////显示信号槽
		//ShowImage();
        
		emit this->progresssignals(100);

	}

	//登录
	void MainFrame::load()
	{
		m_button_widget->adminload();
	}

	//初始化配置
	void MainFrame::initconfig()
	{
		try
		{
			//读取plc、存图配置
			std::string systemparamDir = QCoreApplication::applicationDirPath().toStdString() + "/SysParamConfig.json";
			m_sys_param = m_config_set->GetSystemParamFromFile(systemparamDir);

			CamSaveTopPath = QString::fromStdString(m_sys_param.top_save);
			CamSaveInPath = QString::fromStdString(m_sys_param.in_save);
			CamSaveSide1Path = QString::fromStdString(m_sys_param.side1_save);
			CamSaveSide2Path = QString::fromStdString(m_sys_param.side2_save);
			CamSaveSide3Path = QString::fromStdString(m_sys_param.side3_save);
			CamSaveSide4Path = QString::fromStdString(m_sys_param.side4_save);
			CamSaveSPath = QString::fromStdString(m_sys_param.stereo_save);
			InfoLog("配置初始化成功！");
		}
		catch (...)
		{
			ErrorLog("配置初始化失败！");
			bstart = false;
		}
	}

	//初始化PLC
	void MainFrame::initPlc()
	{
		QString ip = QString::fromStdString(m_sys_param.plc_ipadd);
		_plc->init(ip);
	}

	//初始化Hik相机
	void MainFrame::initHik()
	{
		m_hikcam_top = new HikvisionSDK("HikCameraTop");
		m_hikcam_in = new HikvisionSDK("HikCameraIn");
		m_hikcam_side1 = new HikvisionSDK("HikCameraSide1");
		m_hikcam_side2 = new HikvisionSDK("HikCameraSide2");
		m_hikcam_side3 = new HikvisionSDK("HikCameraSide3");
		m_hikcam_side4 = new HikvisionSDK("HikCameraSide4");
		try
		{
			int resCode = 0;
			resCode = m_hikcam_top->connectCamera(m_sys_param.top_number);
			if (resCode == 0)
			{
				m_hikcam_top->setTriggerMode(1);
				m_hikcam_top->setTriggerSource(0);
				m_hikcam_top->RegisterImageCallBack(ImageCallBackTop);
				m_hikcam_top->startCamera();
				InfoLog("顶部初始化成功！");
			}
			else
			{
				ErrorLog("顶部初始化失败！");
			}
				
			resCode = m_hikcam_in->connectCamera(m_sys_param.in_number);
			if (resCode == 0)
			{
				m_hikcam_in->setTriggerMode(1);
				m_hikcam_in->setTriggerSource(0);
				m_hikcam_in->RegisterImageCallBack(ImageCallBackIn);
				m_hikcam_in->startCamera();
				InfoLog("内部初始化成功！");
			}
			else
			{
                ErrorLog("内部初始化失败！");
			}
			
			resCode = m_hikcam_side1->connectCamera(m_sys_param.side1_number);
			if (resCode == 0)
			{
				m_hikcam_side1->setTriggerMode(1);
				m_hikcam_side1->setTriggerSource(0);
				m_hikcam_side1->RegisterImageCallBack(ImageCallBackSide1);
				m_hikcam_side1->startCamera();
				InfoLog("侧面1初始化成功！");
			}
			else
			{
                ErrorLog("侧面1初始化失败！");
			}
			
			resCode = m_hikcam_side2->connectCamera(m_sys_param.side2_number);
			if (resCode == 0)
			{
				m_hikcam_side2->setTriggerMode(1);
				m_hikcam_side2->setTriggerSource(0);
				m_hikcam_side2->RegisterImageCallBack(ImageCallBackSide2);
				m_hikcam_side2->startCamera();
				InfoLog("侧面2初始化成功！");
			}
			else
			{
                ErrorLog("侧面2初始化失败！");
			}
				
			resCode = m_hikcam_side3->connectCamera(m_sys_param.side3_number);
			if (resCode == 0)
			{
				m_hikcam_side3->setTriggerMode(1);
				m_hikcam_side3->setTriggerSource(0);
				m_hikcam_side3->RegisterImageCallBack(ImageCallBackSide3);
				m_hikcam_side3->startCamera();
				InfoLog("侧面3初始化成功！");
			}
			else
			{
                ErrorLog("侧面3初始化失败！");
			}
			
			resCode = m_hikcam_side4->connectCamera(m_sys_param.side4_number);
			if (resCode == 0)
			{
				m_hikcam_side4->setTriggerMode(1);
				m_hikcam_side4->setTriggerSource(0);
				m_hikcam_side4->RegisterImageCallBack(ImageCallBackSide4);
				m_hikcam_side4->startCamera();
				InfoLog("侧面4初始化成功！");
			}
			else
			{
                ErrorLog("侧面4初始化失败！");
			}
			
		}
		catch (...)
		{
			ErrorLog("面阵相机初始化失败！");
		}
	}

	//3D相机初始化
	void MainFrame::initStereoscopic()
	{
		QString cam_ip = QString::fromStdString(m_sys_param.stereo_ip);
		QList iplist = cam_ip.split(".");
		if (iplist.size() == 4)
		{
			bool bint;
			int ip1 = iplist.at(0).toInt(&bint);
			if (!bint)
			{
				ErrorLog("3D相机IP错误！");
				return;
			}
			int ip2 = iplist.at(1).toInt(&bint);
			if (!bint)
			{
				ErrorLog("3D相机IP错误！");
				return;
			}
			int ip3 = iplist.at(2).toInt(&bint);
			if (!bint)
			{
				ErrorLog("3D相机IP错误！");
				return;
			}
			int ip4 = iplist.at(3).toInt(&bint);
			if (!bint)
			{
				ErrorLog("3D相机IP错误！");
				return;
			}

			bool binit = m_sszn_cam->Connect(ip1, ip2, ip3, ip4);
			if (binit)
			{
                InfoLog("3D相机初始化成功！");
			}
			else
			{
                ErrorLog("3D相机初始化失败！");
			}
		}
		else
		{
            ErrorLog("3D相机初始化错误！");
		}		
	}

	//开启批处理
	void MainFrame::StereoscopicBatch()
	{
		m_sszn_cam->StartOneCall();
	}

	//网络通信初始化
	void MainFrame::initNet()
	{

	}

	//读码器初始化
	void MainFrame::initCode()
	{

	}

	//数据库初始化
	void MainFrame::initSQL()
	{

	}

	//线程初始化
	void MainFrame::initthread()
	{		
		std::thread CamTopthread = std::thread(&MainFrame::ImageTopThread, this);
		TopImageListener.swap(CamTopthread);
		std::thread CamInthread = std::thread(&MainFrame::ImageInThread, this);
		InImageListener.swap(CamInthread);
		std::thread CamSide1thread = std::thread(&MainFrame::ImageSide1Thread, this);
		Side1ImageListener.swap(CamSide1thread);
		std::thread CamSide2thread = std::thread(&MainFrame::ImageSide2Thread, this);
		Side2ImageListener.swap(CamSide2thread);
		std::thread CamSide3thread = std::thread(&MainFrame::ImageSide3Thread, this);
		Side3ImageListener.swap(CamSide3thread);
		std::thread CamSide4thread = std::thread(&MainFrame::ImageSide4Thread, this);
		Side4ImageListener.swap(CamSide4thread);
		std::thread CamSthread = std::thread(&MainFrame::ImageSThread, this);
		SImageListener.swap(CamSthread);
		std::thread plcthread = std::thread(&MainFrame::PlcThread, this);
		PlcListener.swap(plcthread);
		InfoLog("线程初始化！");
	}

	//显示信号槽
	void MainFrame::ShowImage()
	{
		//显示3d连接
		QObject::connect(m_sszn_cam, &SR_Sdk::BatchDataShowsignal, this,
			[=](const QImage &image)
		{
			if (bshowcam)
			{
				((SingleViewWidget *)this->m_singleview_frame)->updateImage(image);//m_singleview_frame
			}
		});

		//显示链接
		QObject::connect(m_hikcam_top, &HikvisionSDK::imageshowsignaltop, this,
			[=](const cv::Mat &image)
		{
			if (bshowcam)
			{
				QImage showimage = vimo::common::matToQImage(image);
				((SixViewsWidget *)this->m_sixview_frame)->updateImage1(showimage);
			}
		});
		QObject::connect(m_hikcam_in, &HikvisionSDK::imageshowsignalin, this,
			[=](const cv::Mat &image)
		{
			if (bshowcam)
			{
				QImage showimage = vimo::common::matToQImage(image);
				((SixViewsWidget *)this->m_sixview_frame)->updateImage2(showimage);
			}
		});

		QObject::connect(m_hikcam_side1, &HikvisionSDK::imageshowsignalside1, this,
			[=](const cv::Mat &image)
		{
			if (bshowcam)
			{
				QImage showimage = vimo::common::matToQImage(image);
				((SixViewsWidget *)this->m_sixview_frame)->updateImage3(showimage);
			}
		});
		QObject::connect(m_hikcam_side2, &HikvisionSDK::imageshowsignalside2, this,
			[=](const cv::Mat &image)
		{
			if (bshowcam)
			{
				QImage showimage = vimo::common::matToQImage(image);
				((SixViewsWidget *)this->m_sixview_frame)->updateImage4(showimage);
			}
		});
		QObject::connect(m_hikcam_side3, &HikvisionSDK::imageshowsignalside3, this,
			[=](const cv::Mat &image)
		{
			if (bshowcam)
			{
				QImage showimage = vimo::common::matToQImage(image);
				((SixViewsWidget *)this->m_sixview_frame)->updateImage5(showimage);
			}
		});
		QObject::connect(m_hikcam_side4, &HikvisionSDK::imageshowsignalside4, this,
			[=](const cv::Mat &image)
		{
			if (bshowcam)
			{
				QImage showimage = vimo::common::matToQImage(image);
				((SixViewsWidget *)this->m_sixview_frame)->updateImage6(showimage);
			}
		});
	}

	//清理队列
	void MainFrame::clearqueue()
	{
		QueueImageTop.clear();
		QueueImageIn.clear();
		QueueImageSide1.clear();
		QueueImageSide2.clear();
		QueueImageSide3.clear();
		QueueImageSide4.clear();
		QueueImageS.clear();
	}

	//相机设置
	bool MainFrame::CamControl()
	{
		return true;
	}

	//plc线程
	unsigned int __stdcall MainFrame::PlcThread()
	{
		while (bstart)
		{
			try
			{
				//读取旋转模式 false：非固定时间旋转 true:固定时间旋转
				QString rotationstring = QString::fromStdString(m_sys_param.plc_rotate);
				bool rotationmode;
				bool brread = _plc->boolread(rotationstring, rotationmode);
				if (brread)
				{
					//读取成功

				}
				else
				{
					//读取失败
				}

				//设备复位是否完成 false：未复位 true：复位
				QString restorationstring = QString::fromStdString(m_sys_param.plc_restoration);
				bool restorationbool;
				bool breread = _plc->boolread(restorationstring, restorationbool);
				if (breread)
				{
					//读取成功

				}
				else
				{
					//读取失败
				}
				//是否允许读取RFID 1默认 2允许读取
				QString allowstring = QString::fromStdString(m_sys_param.plc_allow);
				bool allowbool;
				bool baread = _plc->boolread(allowstring, allowbool);
				if (baread)
				{
					//读取成功

				}
				else
				{
					//读取失败
				}

				//设备状态 1自动 2待机 3报警
				QString statestring = QString::fromStdString(m_sys_param.plc_state);
				bool statebool;
				bool bsread = _plc->boolread(statestring, statebool);
				if (bsread)
				{
					//读取成功

				}
				else
				{
					//读取失败
				}

				//允许旋转 false不允许 true允许				 
				if (Queueallowrotate.size() > 0)
				{
					InfoLog("allow rotate");
					Mutexallowrotate.lock();
					//获取dalsadown队列第一个图像
					bool allowrotate = Queueallowrotate.front();
					Queueallowrotate.pop_front();
					Mutexallowrotate.unlock();

					QString allowrotatestring = QString::fromStdString(m_sys_param.plc_Allowed);
					bool barread = _plc->boolwrite(allowrotatestring, allowrotate);
					if (barread)
					{
						//发送成功

					}
					else
					{
						//发送失败
					}
				}

				//检测结果 0默认 1ng 2ok
				if (Queueresult.size() > 0)
				{
					InfoLog("result");
					Mutexresult.lock();
					//获取dalsadown队列第一个图像
					int result = Queueresult.front();
					Queueresult.pop_front();
					Mutexresult.unlock();

					QString resultstring = QString::fromStdString(m_sys_param.plc_result);
					bool breread = _plc->intwrite(resultstring, result);
					if (breread)
					{
						//发送成功

					}
					else
					{
						//发送失败
					}

				}

				//读取RFID是否成功 1成功 2读取失败
				if (Queueaccomplish.size() > 0)
				{
					InfoLog("RFID accomplish");
					Mutexaccomplish.lock();
					//获取dalsadown队列第一个图像
					int accomplish = Queueaccomplish.front();
					Queueaccomplish.pop_front();
					Mutexaccomplish.unlock();

					QString accomplishstring = QString::fromStdString(m_sys_param.plc_accomplish);
					bool bacread = _plc->intwrite(accomplishstring, accomplish);
					if (bacread)
					{
						//发送成功

					}
					else
					{
						//发送失败
					}

				}
			}
			catch (...)
			{
				qDebug() << "--plc error" << endl;
			}
		}
		return 0;
	}

	//
	unsigned int __stdcall MainFrame::ImageTopThread()
	{
		while (bclose)
		{
			try
			{
				if (QueueImageTop.size() > 0)
				{
					InfoLog("TopSave");
                    InfoData("TopSave");
					MutexImageTop.lock();
					//获取dalsadown队列第一个图像
					cv::Mat Image = QueueImageTop.front();
					QueueImageTop.pop_front();
					MutexImageTop.unlock();



				}
			}
			catch (...)
			{
				qDebug() << "--LFSave error" << endl;
			}
		}
		return 0;
	}

	//
	unsigned int __stdcall MainFrame::ImageInThread()
	{
		while (bclose)
		{
			try
			{
				if (QueueImageIn.size() > 0)
				{
					InfoLog("InSave");
                    InfoData("InSave");
					MutexImageIn.lock();
					//获取dalsadown队列第一个图像
					cv::Mat Image = QueueImageIn.front();
					QueueImageIn.pop_front();
					MutexImageIn.unlock();

				}
			}
			catch (...)
			{
				qDebug() << "--LBSave error" << endl;
			}
		}
		return 0;
	}

	//
	unsigned int __stdcall MainFrame::ImageSide1Thread()
	{
		while (bclose)
		{
			try
			{
				if (QueueImageSide1.size() > 0)
				{
					InfoLog("Side1Save");
                    InfoData("Side1Save");
					MutexImageSide1.lock();
					//获取dalsadown队列第一个图像
					cv::Mat Image = QueueImageSide1.front();
					QueueImageSide1.pop_front();
					MutexImageSide1.unlock();

				}
			}
			catch (...)
			{
				qDebug() << "--RFSave error" << endl;
			}
		}
		return 0;
	}

	//
	unsigned int __stdcall MainFrame::ImageSide2Thread()
	{
		while (bclose)
		{
			try
			{
				if (QueueImageSide2.size() > 0)
				{
					MutexImageSide2.lock();
					//获取dalsadown队列第一个图像
					cv::Mat Image = QueueImageSide2.front();
					QueueImageSide2.pop_front();
					MutexImageSide2.unlock();

				}
			}
			catch (...)
			{
				qDebug() << "--RBSave error" << endl;
			}
		}
		return 0;
	}

	//
	unsigned int __stdcall MainFrame::ImageSide3Thread()
	{
		while (bclose)
		{
			try
			{
				if (QueueImageSide3.size() > 0)
				{
					InfoLog("RFSave");
                    InfoData("RFSave");
					MutexImageSide3.lock();
					//获取dalsadown队列第一个图像
					cv::Mat Image = QueueImageSide3.front();
					QueueImageSide3.pop_front();
					MutexImageSide3.unlock();

				}
			}
			catch (...)
			{
				qDebug() << "--RFSave error" << endl;
			}
		}
		return 0;
	}

	//
	unsigned int __stdcall MainFrame::ImageSide4Thread()
	{
		while (bclose)
		{
			try
			{
				if (QueueImageSide4.size() > 0)
				{
					MutexImageSide4.lock();
					//获取dalsadown队列第一个图像
					cv::Mat Image = QueueImageSide4.front();
					QueueImageSide4.pop_front();
					MutexImageSide4.unlock();

				}
			}
			catch (...)
			{
				qDebug() << "--RBSave error" << endl;
			}
		}
		return 0;
	}

	//
	unsigned int __stdcall MainFrame::ImageSThread()
	{
		while (bclose)
		{
			try
			{
				if (QueueImageS.size() > 0)
				{
					MutexImageS.lock();
					//获取dalsadown队列第一个图像
					cv::Mat Image = QueueImageS.front();
					QueueImageS.pop_front();
					MutexImageS.unlock();

				}
			}
			catch (...)
			{
				qDebug() << "--RBSave error" << endl;
			}
		}
		return 0;
	}

#pragma region 界面
	//界面初始化
	void MainFrame::initWidgets()
	{
		ui->setupUi(this);
		setWindowFlags(Qt::FramelessWindowHint);
		/*QDesktopWidget *deskdop = QApplication::desktop();
		move((deskdop->width() - this->width()) / 2, (deskdop->height() - this->height()) / 2);*/
		qRegisterMetaType<logservice::LogItem>("LogItem");
        qRegisterMetaType<dataservice::DataItem>("DataItem");

		m_titlebar_widget = new TitleBarWidget("思谋", "----", "", this);
		ui->titlebar_layout->addWidget(m_titlebar_widget);

		m_button_widget = new ButtonWidget();
		m_info_widget = new InfoWidget();
		m_statistics_widget = new StatisticsWidget();
		m_result_widget = new ResultWidget();

		ui->buttonLayout->addWidget(m_button_widget);
		ui->infoLayout->addWidget(m_info_widget);//m_info_widget控件放置在infoLayout布局位置
		ui->numLayout->addWidget(m_statistics_widget);
		ui->resultLayout->addWidget(m_result_widget);

		std::vector<ImageView::ViewInfo> info_listOcr;// ViewInfo中    QUuid view_uuid;   QString view_name;
		info_listOcr.push_back(ImageView::ViewInfo{ QUuid(), QString("OCR") });//在info_listOcr末尾添加元素
		m_singleview_frame = new SingleViewWidget(info_listOcr); //由SingleViewWidget构造函数new一个SingleViewWidget类对象
		ui->OCRLayout->addWidget(m_singleview_frame);

		std::vector<ImageView::ViewInfo> info_liststereoscopic;
		info_liststereoscopic.push_back(ImageView::ViewInfo{ QUuid(), QString("3D相机") });
		m_ocrview_frame = new SingleViewWidget(info_liststereoscopic);
		ui->stereoscopiccamLayout->addWidget(m_ocrview_frame);

		std::vector<ImageView::ViewInfo> info_list;
		info_list.push_back(ImageView::ViewInfo{ QUuid(), QString("相机-上") });
		info_list.push_back(ImageView::ViewInfo{ QUuid(), QString("相机-内") });
		info_list.push_back(ImageView::ViewInfo{ QUuid(), QString("相机-侧1") });
		info_list.push_back(ImageView::ViewInfo{ QUuid(), QString("相机-侧2") });
		info_list.push_back(ImageView::ViewInfo{ QUuid(), QString("相机-侧3") });
		info_list.push_back(ImageView::ViewInfo{ QUuid(), QString("相机-侧4") });
		m_sixview_frame = new SixViewsWidget(info_list);
		ui->cam_layout->addWidget(m_sixview_frame);

		m_data_widget = new DataWidget(tr("RFID"));//m_data_widget title
		ui->dataLayout->addWidget(m_data_widget);

		dataservice::Datager::getInstance();

		m_log_widget = new LogWidget(tr("日志"));//m_log_widget title
		ui->log_layout->addWidget(m_log_widget);

		logservice::Logger::getInstance();

	}

	//界面信号槽
	void MainFrame::initConnects()
	{
		//切换语言
		connect(m_titlebar_widget, SIGNAL(signalChangeLang()), this, SLOT(slotChangeLang()));
		//进度条
		connect(this, SIGNAL(progresssignals(int)), this, SLOT(progressslots(int)));
		//登录
		connect(m_button_widget, SIGNAL(adminsignal(int)), this, SLOT(loadslot(int)));
		//3D设置
		connect(m_button_widget, SIGNAL(stereoscopicsignal()), this, SLOT(stereoscopicslot()));
		//2D设置
		connect(m_button_widget, SIGNAL(camsignal()), this, SLOT(camslot()));
		//配置设置
		connect(m_button_widget, SIGNAL(configsignal()), this, SLOT(configslot()));
	}

	//界面设置中文/英文 的转译器
	void MainFrame::setTranslator(QTranslator *translator)
	{
		m_translator = translator;
	}

	//界面设置中文/英文
	void MainFrame::slotChangeLang()
	{
		qApp->removeTranslator(m_translator);
		m_lang = 1 - m_lang;
		if (m_lang)
			m_translator->load("://uiframe/translations/smore_vimo_en.qm");
		else
			m_translator->load("://uiframe/translations/smore_vimo_zh-cn.qm");
		qApp->installTranslator(m_translator);
		ui->retranslateUi(this);

		m_data_widget->setTitle(tr("RFID"));
        QStringList data_list_header;
        data_list_header << tr("DataTime") << tr("Contents");
        m_data_widget->setHeader(data_list_header);

		m_log_widget->setTitle(tr("Log"));
		QStringList log_list_header;
		log_list_header << tr("DateTime") << tr("Contents");
		m_log_widget->setHeader(log_list_header);
	}

	//进度条
	void MainFrame::progressslots(int value)
	{
		if (value == 1)
		{
			_bardialog->show();
		}
		else if (value == 100)
		{
			_bardialog->accept();
		}
		else
		{
			_bardialog->setValue(value);
		}
	}

	//按钮控制
	void MainFrame::loadslot(int adminctrl)
	{
		adminstatus = adminctrl;
		if (adminctrl == 1)
		{
			m_button_widget->buttonopen();
		}
		else if (adminctrl == 2)
		{
			m_button_widget->buttonclose();
		}
	}

	//打开3d设置界面
	void MainFrame::stereoscopicslot()
	{
		//关闭首页显示
		bshowcam = false;
		m_stereoscopic_set = new StereoscopicDialog();	
		//设置曝光
		connect(m_stereoscopic_set, SIGNAL(SetSexposuresignal(int)), this, SLOT(SetSexposure(int)));
		//设置细化点数
		connect(m_stereoscopic_set, SIGNAL(SetSrefinedsignal(int)), this, SLOT(SetSrefined(int)));
		//设置批处理点数
		connect(m_stereoscopic_set, SIGNAL(SetSbatchsignal(int)), this, SLOT(SetSbatch(int)));
		m_stereoscopic_set->exec();
		bshowcam = true;
	}
	//设置曝光
	void MainFrame::SetSexposure(int exp)
	{

	}
	//设置细化点数
	void MainFrame::SetSrefined(int point)
	{

	}
	//设置批处理点数
	void MainFrame::SetSbatch(int batch)
	{

	}

	//载入面阵相机设置
	void MainFrame::camslot()
	{
		//关闭首页显示
		bshowcam = false;
		m_cam_set = new CamsetDialog();
		//实时显示
		//connect(m_cam_set, SIGNAL(camlivesignal(bool)), this, SLOT(camliveslot(bool)));
		
		//设置曝光
		connect(m_cam_set, SIGNAL(sendcamtopesignal(float)), this, SLOT(camslotE(float, 1)));
		connect(m_cam_set, SIGNAL(sendcaminesignal(float)), this, SLOT(camslotE(float, 2)));
		connect(m_cam_set, SIGNAL(sendcamside1esignal(float)), this, SLOT(camslotE(float, 3)));
		connect(m_cam_set, SIGNAL(sendcamside2esignal(float)), this, SLOT(camslotE(float, 4)));
		connect(m_cam_set, SIGNAL(sendcamside3esignal(float)), this, SLOT(camslotE(float, 5)));
		connect(m_cam_set, SIGNAL(sendcamside4esignal(float)), this, SLOT(camslotE(float, 6)));
		//设置增益
		connect(m_cam_set, SIGNAL(sendcamtopgsignal(float)), this, SLOT(camslotG(float, 1)));
		connect(m_cam_set, SIGNAL(sendcagingsignal(float)), this, SLOT(camslotG(float, 2)));
		connect(m_cam_set, SIGNAL(sendcagside1gesignal(float)), this, SLOT(camslotG(float, 3)));
		connect(m_cam_set, SIGNAL(sendcagside2gsignal(float)), this, SLOT(camslotG(float, 4)));
		connect(m_cam_set, SIGNAL(sendcagside3gsignal(float)), this, SLOT(camslotG(float, 5)));
		connect(m_cam_set, SIGNAL(sendcagside4gsignal(float)), this, SLOT(camslotG(float, 6)));
		
		m_cam_set->exec();
		bshowcam = true;
	}

	//载入系统配置
	void MainFrame::configslot()
	{
		//读取plc、存图、数据库、服务器配置
		std::string systemparamDir = QCoreApplication::applicationDirPath().toStdString() + "/SysParamConfig.json";
		m_sys_param = m_config_set->GetSystemParamFromFile(systemparamDir);

		CamSaveTopPath = QString::fromStdString(m_sys_param.top_save);
		CamSaveInPath = QString::fromStdString(m_sys_param.in_save);
		CamSaveSide1Path = QString::fromStdString(m_sys_param.side1_save);
		CamSaveSide2Path = QString::fromStdString(m_sys_param.side2_save);
		CamSaveSide3Path = QString::fromStdString(m_sys_param.side3_save);
		CamSaveSide4Path = QString::fromStdString(m_sys_param.side4_save);
		CamSaveSPath = QString::fromStdString(m_sys_param.stereo_save);
	}

#pragma region 相机设置
	//设置曝光
	void MainFrame::camslotE(float value, int index)
	{
		switch (index)
		{
		case 1:
			m_hikcam_top->setExposureTime(value);
			break;
		case 2:
			m_hikcam_in->setExposureTime(value);
			break;
		case 3:
			m_hikcam_side1->setExposureTime(value);
			break;
		case 4:
			m_hikcam_side2->setExposureTime(value);
			break;
		case 5:
			m_hikcam_side3->setExposureTime(value);
			break;
		case 6:
			m_hikcam_side4->setExposureTime(value);
			break;
		default:
			break;
		}		
	}	
	//设置增益
	void MainFrame::camslotG(float value, int index)
	{
		switch (index)
		{
		case 1:
			m_hikcam_top->setGain(value);
			break;
		case 2:
			m_hikcam_in->setGain(value);
			break;
		case 3:
			m_hikcam_side1->setGain(value);
			break;
		case 4:
			m_hikcam_side2->setGain(value);
			break;
		case 5:
			m_hikcam_side3->setGain(value);
			break;
		case 6:
			m_hikcam_side4->setGain(value);
			break;
		default:
			break;
		}
	}
#pragma endregion

	//日志
	void MainFrame::InfoLog(QString log)
	{
		logservice::Logger::getInstance().appendLog({ logservice::LogLevel::Info, logservice::LogSource::AI }, log);
	}
	void MainFrame::WarningLog(QString log)
	{
		logservice::Logger::getInstance().appendLog({ logservice::LogLevel::Warning, logservice::LogSource::Camera }, log);
	}
	void MainFrame::ErrorLog(QString log)
	{
		logservice::Logger::getInstance().appendLog({ logservice::LogLevel::Error, logservice::LogSource::Others }, log);
	}

    //Data
    void MainFrame::InfoData(QString data)
    {
        dataservice::Datager::getInstance().appendData({dataservice::DataLevel::Info, dataservice::DataSource::AI}, data);
    }
    void MainFrame::WarningData(QString data)
    {
        dataservice::Datager::getInstance().appendData({dataservice::DataLevel::Warning, dataservice::DataSource::Camera}, data);
    }
    void MainFrame::ErrorData(QString data)
    {
        dataservice::Datager::getInstance().appendData({dataservice::DataLevel::Error, dataservice::DataSource::Others}, data);
    }

	//创建文件夹
	bool MainFrame::isDirExist(QString fullPath)
	{
		QDir dir(fullPath);
		if (dir.exists())
		{
			return true;
		}
		else
		{
			bool ok = dir.mkpath(fullPath);//创建多级目录
			return ok;
		}
	}
#pragma endregion
}
// namespace vimo::display
