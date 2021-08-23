/****************************************************************************
 *  @file     dataservice.cpp
 *  @brief    运行data
 *  @author   xiaoyu.liu
 *  @email    xiaoyu.liu@smartmore.com
 *  @version
 *  @date     2020-12-28
 *  Copyright (c) 2020 SmartMore All rights reserved.
 ****************************************************************************/
#include "dataservice.h"
#include "vimocommon/vimocommon.h"
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <mutex>

namespace vimo::dataservice
{

    static std::mutex g_data_mutex;
    using Message = vimo::common::Message;

    QString Datager::getDefaultDataName()
    {
        QString data_name = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
        data_name.append("_Smore_Data_");
        data_name.append(m_filter.toString());
        data_name.append(".txt");
        return data_name;
    }

    void Datager::appendData(DataType type, QString content)
    {
        qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss:zzz") + "---" + content << endl;
        std::lock_guard<std::mutex> guard(g_data_mutex);
        m_tdatas.push_back({QDateTime::currentDateTime().toString("hh:mm:ss"), type, std::move(content)});

        if (m_filter.contains(type))
        {
            m_cdatas.push_back(&m_tdatas.back());
            emit signalAppendData(m_tdatas.back());
        }
    }

    void Datager::setFilter(DataFilter filter)
    {
        std::lock_guard<std::mutex> guard(g_data_mutex);

        m_filter = filter;

        m_cdatas.clear();
        for (auto &it : m_tdatas)
        {
            if (m_filter.contains(it.type))
            {
                m_cdatas.push_back(&it);
            }
        }

        emit signalReloadData(m_cdatas);
    }

    void Datager::exportToFile(QString file_path)
    {
        if (!file_path.isEmpty())
        {
            // 导出日志
            std::lock_guard<std::mutex> guard(g_data_mutex);

            QFile file(file_path);
            if (file.open(QIODevice::WriteOnly))
            {
                QTextStream stream(&file);
                stream << tr("Time") << "," << tr("Content") << endl;

                for (auto &it : m_cdatas)
                {
                    stream << it->ctime << "," << it->content << endl;
                }

                stream.flush();
                file.close();
            }
            else
            {
                Message::postWarningMessage(tr("Warning"), tr("Failed to create data file"));
            }
        }
    }

    QString DataFilter::toString()
    {
        if (m_filter_mode == None)
            return QString("All");
        if (m_filter_mode == Level)
        {
            switch (m_type.level)
            {
            case DataLevel::Info:
                return QString("Info");
            case DataLevel::Warning:
                return QString("Warning");
            case DataLevel::Error:
                return QString("Error");
            }
        }
        if (m_filter_mode == Source)
        {
            switch (m_type.source)
            {
            case DataSource::AI:
                return QString("AI");
            case DataSource::Camera:
                return QString("Camera");
            case DataSource::Others:
                return QString("Others");
            }
        }
        return QString("");
    }

} // namespace vimo::dataservice
