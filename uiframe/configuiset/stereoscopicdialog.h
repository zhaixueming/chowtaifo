/****************************************************************************
 *  @file     stereoscopicdialog.h
 *  @brief    3d设置界面
 *  @author
 *  @email
 *  @version
 *  @date     2020.12.21
 *  Copyright (c) 2020 SmartMore All rights reserved.
 ****************************************************************************/
#ifndef STEREOSCOPICDIALOG_H
#define STEREOSCOPICDIALOG_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <QDialog>
#include <QLabel>
#include <QWidget>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QFile>
#include <QFileDialog>
#include "configtitlebar.h"
#include "serializer/Stereoscopic.h"

namespace Ui
{
    class StereoscopicDialog;
}

namespace vimo::display
{
    class StereoscopicDialog : public QDialog
    {
        Q_OBJECT

      public:
        explicit StereoscopicDialog(QWidget *parent = nullptr);
        ~StereoscopicDialog();

	signals:
		//设置曝光
		void SetSexposuresignal(int exp);
		//设置细化点数
		void SetSrefinedsignal(int point);
		//设置批处理点数
		void SetSbatchsignal(int batch);
		
      private slots:
        void slotCheckAndClose();
        //保存按钮
        void on_ButtonSave_clicked();
        //取消按钮
        void on_pushButtonCancel_clicked();
		//测试按钮
		void on_Buttonexposuretest_clicked();
		void on_Buttonrefinedtest_clicked();
		void on_Buttonfrequencytest_clicked();
		void on_Buttonbatchtest_clicked();
		//3d显示
		void on_PBtn3DShow_clicked();
		//保存数据
		void on_pBtnGSave_clicked();
		void on_pBtnHSave_clicked();
		void on_pBtnESave_clicked();

      private:
        void initWidgets();
        void ReadingData();		
		StereoscopicParam GetStereoscopicParamFromFile(std::string path);
		void SaveStereoscopic();

      private:
        Ui::StereoscopicDialog *ui;
        ConfigTitleBar *m_titleBar{nullptr};
		StereoscopicParam _stereoscopicparam;
    };
} // namespace vimo::display
#endif // CONFIGDIALOG_H
