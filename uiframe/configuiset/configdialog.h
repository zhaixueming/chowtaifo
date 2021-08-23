/****************************************************************************
 *  @file     configdialog.h
 *  @brief    设置界面
 *  @author
 *  @email
 *  @version
 *  @date     2020.12.21
 *  Copyright (c) 2020 SmartMore All rights reserved.
 ****************************************************************************/
#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QWidget>
#include "serializer/Config.h"
#include "configtitlebar.h"

namespace Ui
{
	class ConfigDialog;
}

namespace vimo::display
{
	class ConfigDialog : public QDialog
	{
		Q_OBJECT

	public:
		explicit ConfigDialog(QWidget *parent = nullptr);
		~ConfigDialog();

	private slots:
		void slotCheckAndClose();
		
		//加载地址按钮
		void on_Buttonstereo_clicked();
		void on_Buttontop_clicked();
		void on_Buttonin_clicked();
		void on_Buttonside1_clicked();
		void on_Buttonside2_clicked();
		void on_Buttonside3_clicked();
		void on_Buttonside4_clicked();
		
		//保存按钮
		void on_ButtonSave_clicked();
		//取消按钮
		void on_ButtonCancel_clicked();
	public:
		//读取文件
		SystemParam GetSystemParamFromFile(std::string path);

	private:
		void initWidgets();
		//读取配置
		void ReadingData();
		//配置实例化
		SystemParam _systemparam;		
		//保存文件
		void SaveConfig();
	private:
		Ui::ConfigDialog *ui;
		ConfigTitleBar *m_titleBar{ nullptr };
	};
}  // namespace vtk::display
#endif  // CONFIGDIALOG_H
