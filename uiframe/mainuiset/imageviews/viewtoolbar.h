﻿/****************************************************************************
 *  @file     viewtoolbar.h
 *  @brief    图像视窗工具条类
 *  @author   junjie.zeng
 *  @email    junjie.zeng@smartmore.com
 *  @version
 *  @date     2020.12.21
 *  Copyright (c) 2020 SmartMore All rights reserved.
 ****************************************************************************/
#ifndef VIEWTOOLBAR_H
#define VIEWTOOLBAR_H

#include "vimocommon/vimocommon.h"
#include "imageview.h"
#include <QWidget>

namespace Ui
{
class ViewToolBar;
}

namespace vimo::display
{

class ViewToolBar : public QWidget
{
    Q_OBJECT

public:
    explicit ViewToolBar(QWidget *parent = nullptr);
    ~ViewToolBar();

    void setViewInfo(const ImageView::ViewInfo &info);
    void setScalefactor(int factor);
    void updateDeviceIconToolTip();

signals:
    void signalChangeDisplayImageType(bool is_ai_image);
    void signalZoomoutImage();
    void signalZoominImage();
    void signalRevertImage();
    void signalUpdateImage(const cv::Mat &image);

private slots:
    //void on_device_btn_clicked();
    void on_zoomout_btn_clicked();
    void on_zoomin_btn_clicked();
    void on_revert_btn_clicked();
    //void on_image_dispaly_btn_toggled(bool checked);

private:
    void init();
    void initWidgets();
    void initConnects();
    void updateCameraStatus();
    void updateCameraStatus(vimo::common::CameraState state);
    void updateDeviceIcon();
    void changeEvent(QEvent *e);

private:
    Ui::ViewToolBar *ui;
    ImageView::ViewInfo m_view_info;
};
}  // namespace vimo::display
#endif  // VIEWTOOLBAR_H
