/****************************************************************************
 *  @file     dataservice.h
 *  @brief    运行日志
 *  @author   xiaoyu.liu
 *  @email    xiaoyu.liu@smartmore.com
 *  @version
 *  @date     2020-12-28
 *  Copyright (c) 2020 SmartMore All rights reserved.
 ****************************************************************************/

#ifndef DataGER_H
#define DataGER_H

#include <QObject>
#include <QString>
#include <list>

namespace vimo::dataservice
{

    enum class DataLevel //枚举
    {
        Error,
        Info,
        Warning
    };

    enum class DataSource
    {
        AI,
        Camera,
        Others
    };

    struct DataType
    {
        DataLevel level;
        DataSource source;
    };

    class DataFilter
    {
      public:
        DataFilter()
        {
            m_filter_mode = None;
        }

        DataFilter(DataLevel level)
        {
            m_filter_mode = Level;
            m_type.level = level;
        }

        DataFilter(DataSource source)
        {
            m_filter_mode = Source;
            m_type.source = source;
        }

        bool contains(const DataType &type)
        {
            return m_filter_mode == None || (m_filter_mode == Level && m_type.level == type.level) ||
                   (m_filter_mode == Source && m_type.source == type.source);
        }

        QString toString();

      private:
        enum
        {
            None,
            Level,
            Source
        } m_filter_mode;
        DataType m_type;
    };

    struct DataItem
    {
        QString ctime;
        DataType type;
        QString content;
    };

    class Datager : public QObject
    {
        Q_OBJECT
      public:
        static Datager &getInstance()
        {
            static Datager Datager;
            return Datager;
        }
        void appendData(DataType type, QString content);
        void setFilter(DataFilter filter);
        void exportToFile(QString file_path);
        QString getDefaultDataName();

      signals:
        void signalAppendData(const DataItem &item);
        void signalReloadData(const std::list<DataItem *> &Datas);

      public:
        Datager() = default;

        DataFilter m_filter;
        std::list<DataItem> m_tdatas;
        std::list<DataItem *> m_cdatas;
    };

} // namespace vimo::dataservice

Q_DECLARE_METATYPE(vimo::dataservice::DataItem);

#endif // DATAGER_H
