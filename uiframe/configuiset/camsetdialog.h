/****************************************************************************
 *  @file     pointdialog.h
 *  @brief    点位设置界面
 *  @author
 *  @email
 *  @version
 *  @date     2020.12.21
 *  Copyright (c) 2020 SmartMore All rights reserved.
 ****************************************************************************/
#ifndef CAMSETDIALOG_H
#define CAMSETDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QWidget>
#include <QDateTime>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <tchar.h>
#include <fstream>
#include <iostream>
#include <windows.h>
#include "configtitlebar.h"
#include "vimocommon/vimocommon.h"
#include "uiframe/mainuiset/imageviews/sixviewswidget.h"
#include "serializer/camparam.h"

namespace Ui
{
	class CamsetDialog;
}

namespace vimo::display
{
	class CamsetDialog : public QDialog
	{
		Q_OBJECT

	public:
		explicit CamsetDialog(QWidget *parent = nullptr);
		~CamsetDialog();

	private slots:
		void slotCheckAndClose();			
		//保存按钮
		void on_ButtonSave_clicked();
		//取消按钮
		void on_ButtonCancel_clicked();	

		//单张取图
		void on_Buttononetop_clicked();
		void on_Buttononein_clicked();
		void on_Buttononeside1_clicked();
		void on_Buttononeside2_clicked();
		void on_Buttononeside3_clicked();
		void on_Buttononeside4_clicked();
		//实时取图
		void on_Buttonlivetop_clicked();		
		void on_Buttonlivein_clicked();
		void on_Buttonliveside1_clicked();
		void on_Buttonliveside2_clicked();
		void on_Buttonliveside3_clicked();
		void on_Buttonliveside4_clicked();
		//测试参数
		void on_Buttontesttop_clicked();
		void on_Buttontestin_clicked();
		void on_Buttontestside1_clicked();
		void on_Buttontestside2_clicked();
		void on_Buttontestside3_clicked();
		void on_Buttontestside4_clicked();
		//存图
		void on_Buttonsaveimagetop_clicked();
		void on_Buttonsaveimagein_clicked();
		void on_Buttonsaveimageside1_clicked();
		void on_Buttonsaveimageside2_clicked();
		void on_Buttonsaveimageside3_clicked();
		void on_Buttonsaveimageside4_clicked();

	signals:
		void camparamsignal();
		//单张采图
		void sendcamtoplivesignal();
		void sendcaminlivesignal();
		void sendcamside1livesignal();
		void sendcamside2livesignal();
		void sendcamside3livesignal();
		void sendcamside4livesignal();
		//实时取图
		void sendcamtoponesignal();
		void sendcaminonesignal();
		void sendcamside1onesignal();
		void sendcamside2onesignal();
		void sendcamside3onesignal();
		void sendcamside4onesignal();
		//设置曝光
		void sendcamtopesignal(float exp);
		void sendcaminesignal(float exp);
		void sendcamside1esignal(float exp);
		void sendcamside2esignal(float exp);
		void sendcamside3esignal(float exp);
		void sendcamside4esignal(float exp);
		//设置增益
		void sendcamtopgsignal(float gain);
		void sendcamingsignal(float gain);
		void sendcamside1gsignal(float gain);
		void sendcamside2gsignal(float gain);
		void sendcamside3gsignal(float gain);
		void sendcamside4gsignal(float gain);
	private:
		void initWidgets();
		
		//获取参数
		void getparam();
		//设置参数
		void setparam();
		//读取文件
		CamParam GetCamParamFromFile(std::string path);
		//保存文件
		void Savecamparam();

	private:
		Ui::CamsetDialog *ui;
		CamParam _camparam;
		ConfigTitleBar *m_titleBar{ nullptr };
		std::vector<ImageView::ViewInfo> info_list;
		// 图片显式框
		QWidget *m_view_frame{ nullptr };
	};
}  // namespace vtk::display
#endif  // MATERIALSETDIALOG_H
