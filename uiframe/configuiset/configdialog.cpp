/****************************************************************************
 *  @file     configdialog.cpp
 *  @brief    设置界面
 *  @author
 *  @email
 *  @version
 *  @date     2020.12.21
 *  Copyright (c) 2020 SmartMore All rights reserved.
 ****************************************************************************/
#include "configdialog.h"
#include "configtitlebar.h"
#include "ui_configdialog.h"
#include "uiframe/commonuiset/messagedialog/messagedialog.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsDropShadowEffect>

namespace vimo::display
{
	ConfigDialog::ConfigDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ConfigDialog)
	{
		ui->setupUi(this);
		setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
		setWindowModality(Qt::ApplicationModal);

		initWidgets();
		ReadingData();
	}

	ConfigDialog::~ConfigDialog()
	{
		delete ui;
	}

	void ConfigDialog::initWidgets()
	{
		m_titleBar = new ConfigTitleBar(this);
		m_titleBar->m_pTitleContent->setText(tr("设置"));
		ui->titlebar_layout->addWidget(m_titleBar);
		connect(m_titleBar, SIGNAL(signalClose()), this, SLOT(slotCheckAndClose()));
	}

	void ConfigDialog::slotCheckAndClose()
	{		
		accept();
	}

	//读取文件
	SystemParam ConfigDialog::GetSystemParamFromFile(std::string path)
	{
		SystemParam param;
		std::ifstream fi(path.c_str());
		if (fi.good())
		{
			nlohmann::json j;
			fi >> j;
			param = j.get<SystemParam>();
		}
		else
		{
			MessageDialog message_box_dialog(DialogType::errorDialog, tr("Get System Param File faild!"));
			message_box_dialog.exec();
		}
		return param;
	}

	//保存配置
	void ConfigDialog::SaveConfig()
	{
		std::string sysparamDir = QCoreApplication::applicationDirPath().toStdString() + "/SysParamConfig.json";
		std::ofstream fo(sysparamDir.c_str());
		nlohmann::json j = _systemparam;
		fo << std::setw(4) << j << std::endl;
		fo.flush();
	}

	//加载到控件中
	void ConfigDialog::ReadingData()
	{
		//init
		std::string systemparamDir = QCoreApplication::applicationDirPath().toStdString() + "/SysParamConfig.json";
		_systemparam = GetSystemParamFromFile(systemparamDir);

		ui->lineEditPLCIP->setText(QString().fromStdString(_systemparam.plc_ipadd));
		
		ui->lineEditrotate->setText(QString().fromStdString(_systemparam.plc_rotate));
		ui->lineEditrestoration->setText(QString().fromStdString(_systemparam.plc_restoration));
		ui->lineEditallow->setText(QString().fromStdString(_systemparam.plc_allow));
		ui->lineEditstate->setText(QString().fromStdString(_systemparam.plc_state));
		ui->lineEditAllowed->setText(QString().fromStdString(_systemparam.plc_Allowed));
		ui->lineEditresult->setText(QString().fromStdString(_systemparam.plc_result));
		ui->lineEditaccomplish->setText(QString().fromStdString(_systemparam.plc_accomplish));

		ui->lineEditstereo->setText(QString().fromStdString(_systemparam.stereo_ip));
		ui->lineEdittop->setText(QString().fromStdString(_systemparam.top_number));
		ui->lineEditin->setText(QString().fromStdString(_systemparam.in_number));
		ui->lineEditside1->setText(QString().fromStdString(_systemparam.side1_number));
		ui->lineEditside2->setText(QString().fromStdString(_systemparam.side2_number));
		ui->lineEditside3->setText(QString().fromStdString(_systemparam.side3_number));
		ui->lineEditside4->setText(QString().fromStdString(_systemparam.side4_number));
		
		ui->lineEditstereoSave->setText(QString().fromStdString(_systemparam.stereo_save));
		ui->lineEdittopSave->setText(QString().fromStdString(_systemparam.top_save));
		ui->lineEditinSave->setText(QString().fromStdString(_systemparam.in_save));
		ui->lineEditside1Save->setText(QString().fromStdString(_systemparam.side1_save));
		ui->lineEditside2Save->setText(QString().fromStdString(_systemparam.side2_save));
		ui->lineEditside3Save->setText(QString().fromStdString(_systemparam.side3_save));
		ui->lineEditside4Save->setText(QString().fromStdString(_systemparam.side4_save));
	}
	//加载地址按钮
	void ConfigDialog::on_Buttonstereo_clicked()
	{
		QString StringFile = QFileDialog::getExistingDirectory(this, "选择目录", "E://");
		ui->lineEditstereo->setText(StringFile);
	}
	void ConfigDialog::on_Buttontop_clicked()
	{
		QString StringFile = QFileDialog::getExistingDirectory(this, "选择目录", "E://");
		ui->lineEdittop->setText(StringFile);
	}
	void ConfigDialog::on_Buttonin_clicked()
	{
		QString StringFile = QFileDialog::getExistingDirectory(this, "选择目录", "E://");
		ui->lineEditin->setText(StringFile);
	}
	void ConfigDialog::on_Buttonside1_clicked()
	{
		QString StringFile = QFileDialog::getExistingDirectory(this, "选择目录", "E://");
		ui->lineEditside1->setText(StringFile);
	}	
	void ConfigDialog::on_Buttonside2_clicked()
	{
		QString StringFile = QFileDialog::getExistingDirectory(this, "选择目录", "E://");
		ui->lineEditside2->setText(StringFile);
	}
	void ConfigDialog::on_Buttonside3_clicked()
	{
		QString StringFile = QFileDialog::getExistingDirectory(this, "选择目录", "E://");
		ui->lineEditside3->setText(StringFile);
	}
	void ConfigDialog::on_Buttonside4_clicked()
	{
		QString StringFile = QFileDialog::getExistingDirectory(this, "选择目录", "E://");
		ui->lineEditside4->setText(StringFile);
	}
	
	//保存按钮
	void ConfigDialog::on_ButtonSave_clicked()
	{
		_systemparam.plc_ipadd = ui->lineEditPLCIP->text().toStdString();
		
		_systemparam.plc_rotate = ui->lineEditrotate->text().toStdString();
		_systemparam.plc_restoration = ui->lineEditrestoration->text().toStdString();
		_systemparam.plc_allow = ui->lineEditallow->text().toStdString();
		_systemparam.plc_state = ui->lineEditstate->text().toStdString();
		_systemparam.plc_Allowed = ui->lineEditAllowed->text().toStdString();
		_systemparam.plc_result = ui->lineEditresult->text().toStdString();
		_systemparam.plc_accomplish = ui->lineEditaccomplish->text().toStdString();

		_systemparam.stereo_ip = ui->lineEditstereo->text().toStdString();
		_systemparam.top_number = ui->lineEdittop->text().toStdString();
		_systemparam.in_number = ui->lineEditin->text().toStdString();
		_systemparam.side1_number = ui->lineEditside1->text().toStdString();
		_systemparam.side2_number = ui->lineEditside2->text().toStdString();
		_systemparam.side3_number = ui->lineEditside3->text().toStdString();
		_systemparam.side4_number = ui->lineEditside4->text().toStdString();
		
		_systemparam.stereo_save = ui->lineEditstereoSave->text().toStdString();
		_systemparam.top_save = ui->lineEdittopSave->text().toStdString();
		_systemparam.in_save = ui->lineEditinSave->text().toStdString();
		_systemparam.side1_save = ui->lineEditside1Save->text().toStdString();
		_systemparam.side2_save = ui->lineEditside2Save->text().toStdString();
		_systemparam.side3_save = ui->lineEditside3Save->text().toStdString();
		_systemparam.side4_save = ui->lineEditside4Save->text().toStdString();
		
		SaveConfig();
		MessageDialog message_box_dialog(DialogType::infoDialog, tr("保存成功!"));
		message_box_dialog.exec();
	}
	//取消按钮
	void ConfigDialog::on_ButtonCancel_clicked()
	{
		accept();
	}
}  // namespace vtk::display
