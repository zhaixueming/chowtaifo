/****************************************************************************
 *  @file     datawidget.cpp
 *  @brief    日志列表类
 *  @author   junjie.zeng
 *  @email    junjie.zeng@smartmore.com
 *  @version
 *  @date     2020.12.21
 *  Copyright (c) 2020 SmartMore All rights reserved.
 ****************************************************************************/
#include "datawidget.h"
#include "logservice/dataservice.h"
#include "ui_datawidget.h"
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>

namespace vimo::display
{

using namespace vimo::dataservice;

DataWidget::DataWidget(const QString &title, QWidget *parent) : QWidget(parent), ui(new Ui::DataWidget), m_title(title)
{
    init();
}

DataWidget::~DataWidget()
{
    delete ui;
}

void DataWidget::init()
{
    initWidgets();
    initConnects();
}

void DataWidget::initWidgets()
{
    ui->setupUi(this);
    ui->data_list_widget->setTitle(m_title);

    std::vector<TableWidget::HeadData> head_data_list;
    TableWidget::HeadData head_data = {tr("DateTime"), 120};
    head_data_list.emplace_back(head_data);
    head_data = {tr("Contents"), 220};
    head_data_list.emplace_back(head_data);
    ui->data_list_widget->initTable(m_title, head_data_list);
    QStringList list;
    list << "All"
         << "Error"
         << "Info"
         << "Warning"
         << "AI"
         << "Camera"
         << "Others";
    ui->data_list_widget->resetCombox(list);
}

void DataWidget::initConnects()
{
    connect(&Datager::getInstance(), &Datager::signalAppendData, this, [&](const DataItem &data_item) {
        QList<QVariant> list;
        QVariant item;
        item = data_item.ctime;
        list.append(item);

        item = data_item.content;
        list.append(item);
        ui->data_list_widget->appendRow(list);	
    });
    connect(&Datager::getInstance(), &Datager::signalReloadData, this, [&](const std::list<DataItem *> &datas) {
        ui->data_list_widget->deleteAllRows();
        QList<QList<QVariant>> lists;
        for (const auto &data_item : datas)
        {
            QList<QVariant> list;
            QVariant item;
            item = data_item->ctime;
            list.append(item);

            item = data_item->content;
            list.append(item);

            lists.append(list);
        }
        ui->data_list_widget->appendRows(lists);
    });

    connect(ui->data_list_widget, &TableWidget::signalComboxTextChange, this, [&](const QString &text) {
        if (text == "All")
        {
            DataFilter filter;
            Datager::getInstance().setFilter(filter);
        }
        else if (text == "Error")
        {
            Datager::getInstance().setFilter(DataLevel::Error);
        }
        else if (text == "Info")
        {
            Datager::getInstance().setFilter(DataLevel::Info);
        }
        else if (text == "Warning")
        {
            Datager::getInstance().setFilter(DataLevel::Warning);
        }
        else if (text == "AI")
        {
            Datager::getInstance().setFilter(DataSource::AI);
        }
        else if (text == "Camera")
        {
            Datager::getInstance().setFilter(DataSource::Camera);
        }
        else if (text == "Others")
        {
            Datager::getInstance().setFilter(DataSource::Others);
        }
    });
}

void DataWidget::setTitle(QString title)
{
    ui->data_list_widget->setTitle(title);
}
//设置日志格式
void DataWidget::setHeader(QStringList header_list)
{
    ui->data_list_widget->setHeader(header_list);
}
}  // namespace vimo::display
