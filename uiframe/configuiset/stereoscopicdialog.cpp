/****************************************************************************
 *  @file     stereoscopicdialog.cpp
 *  @brief    3D设置界面
 *  @author
 *  @email
 *  @version
 *  @date     2020.12.21
 *  Copyright (c) 2020 SmartMore All rights reserved.
 ****************************************************************************/
#include "stereoscopicdialog.h"
#include "configtitlebar.h"
#include "ui_stereoscopicdialog.h"
#include "uiframe/commonuiset/messagedialog/messagedialog.h"
#include <QDebug>
#include <QFile>
#include <QGraphicsDropShadowEffect>


namespace vimo::display
{
	StereoscopicDialog::StereoscopicDialog(QWidget *parent) : QDialog(parent), ui(new Ui::StereoscopicDialog)
	{
		ui->setupUi(this);
		setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
		setWindowModality(Qt::ApplicationModal);
		initWidgets();
        ReadingData();
	}

	StereoscopicDialog::~StereoscopicDialog()
	{
		delete ui;
	}

	void StereoscopicDialog::initWidgets()
	{
		m_titleBar = new ConfigTitleBar(this);
		m_titleBar->m_pTitleContent->setText(tr("3d相机设置"));
		ui->titlebar_layout->addWidget(m_titleBar);
		connect(m_titleBar, SIGNAL(signalClose()), this, SLOT(slotCheckAndClose()));
	}

	void StereoscopicDialog::slotCheckAndClose()
	{
		accept();
	}

	//读取文件
	StereoscopicParam StereoscopicDialog::GetStereoscopicParamFromFile(std::string path)
    {
		StereoscopicParam param;
        std::ifstream fi(path.c_str());
        if (fi.good())
        {
            nlohmann::json j;
            fi >> j;
            param = j.get<StereoscopicParam>();
        }
        else
        {
            MessageDialog message_box_dialog(DialogType::errorDialog, tr("Get Stereoscopic Param File faild!"));
            message_box_dialog.exec();
        }
        return param;
    }

	void StereoscopicDialog::SaveStereoscopic()
	{
        std::string sysparamDir = QCoreApplication::applicationDirPath().toStdString() + "/StereoscopicParam.json";
        std::ofstream fo(sysparamDir.c_str());
        nlohmann::json j = _stereoscopicparam;
        fo << std::setw(4) << j << std::endl;
        fo.flush();
	}

	void StereoscopicDialog::ReadingData()
	{
        std::string paramDir = QCoreApplication::applicationDirPath().toStdString() + "/StereoscopicParam.json";
        _stereoscopicparam = GetStereoscopicParamFromFile(paramDir);

		ui->lineEditexposure->setText(QString::fromStdString(_stereoscopicparam.exposure));
        ui->lineEditrefined->setText(QString::fromStdString(_stereoscopicparam.refined));
        ui->lineEditfrequency->setText(QString::fromStdString(_stereoscopicparam.frequency));
        ui->lineEditbatch->setText(QString::fromStdString(_stereoscopicparam.batch));
	}

	//保存按钮
	void StereoscopicDialog::on_ButtonSave_clicked()
	{		
		bool bsave;
		_stereoscopicparam.exposure = ui->lineEditexposure->text().toDouble(&bsave);
		if (!bsave)
		{
			MessageDialog message_box_dialog(DialogType::warningDialog, tr("数据错误!"));
			message_box_dialog.exec();
			return;
		}
		_stereoscopicparam.refined = ui->lineEditrefined->text().toDouble(&bsave);
		if (!bsave)
		{
			MessageDialog message_box_dialog(DialogType::warningDialog, tr("数据错误!"));
			message_box_dialog.exec();
			return;
		}
		_stereoscopicparam.frequency = ui->lineEditfrequency->text().toDouble(&bsave);
		if (!bsave)
		{
			MessageDialog message_box_dialog(DialogType::warningDialog, tr("数据错误!"));
			message_box_dialog.exec();
			return;
		}
		_stereoscopicparam.batch = ui->lineEditbatch->text().toDouble(&bsave);
		if (!bsave)
		{
			MessageDialog message_box_dialog(DialogType::warningDialog, tr("数据错误!"));
			message_box_dialog.exec();
			return;
		}
       
		SaveStereoscopic();
		MessageDialog message_box_dialog(DialogType::infoDialog, tr("保存成功!"));
        message_box_dialog.exec();
	}

	//取消按钮
	void StereoscopicDialog::on_pushButtonCancel_clicked()
	{
		accept();
	}

	//测试按钮
	void StereoscopicDialog::on_Buttonexposuretest_clicked()
	{

	}
	void StereoscopicDialog::on_Buttonrefinedtest_clicked()
	{

	}
	void StereoscopicDialog::on_Buttonfrequencytest_clicked()
	{

	}
	void StereoscopicDialog::on_Buttonbatchtest_clicked()
	{

	}

	//3d显示
	void StereoscopicDialog::on_PBtn3DShow_clicked()
	{

	}

	//保存数据
	void StereoscopicDialog::on_pBtnGSave_clicked()
	{

	}
	void StereoscopicDialog::on_pBtnHSave_clicked()
	{

	}
	void StereoscopicDialog::on_pBtnESave_clicked()
	{

	}






}  // namespace vtk::display
