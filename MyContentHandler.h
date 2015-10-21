//
// Created by 顾伟刚 on 15/10/18.
//

#ifndef MYSQL_KAFKA_APPLIER_MYCONTENTHANDLER_H
#define MYSQL_KAFKA_APPLIER_MYCONTENTHANDLER_H

#include "binlog.h"
#include <iostream>
#include <stdlib.h>

typedef std::pair<enum_field_types, std::string> Row_Fields_pair;
typedef std::vector<Row_Fields_pair> Row_Fields_map;
typedef std::vector<Row_Fields_map> Row_map;

class MyContentHandler : public Content_handler
{
public:
    /*
      These methods override the process_event method
      defined in the class  Content_handler.
    */
    binary_log::Binary_log_event *process_event(binary_log::Rows_event *ev);
    binary_log::Binary_log_event *process_event(binary_log::Table_map_event *event);
    binary_log::Binary_log_event *process_event(binary_log::Query_event *event)
    {
        return event;
    }

    binary_log::Binary_log_event *process_event(binary_log::Intvar_event *event)
    {
        return event;
    }

    binary_log::Binary_log_event *process_event(binary_log::User_var_event *event)
    {
        return event;
    }

    binary_log::Binary_log_event *process_event(binary_log::Xid_event *event)
    {
        return event;
    }

    binary_log::Binary_log_event *process_event(binary_log::Format_description_event *event)
    {
        return event;
    }

    binary_log::Binary_log_event *process_event(binary_log::Incident_event *event)
    {
        return event;
    }

    binary_log::Binary_log_event *process_event(binary_log::Rotate_event *event)
    {
        return event;
    }

    ~MyContentHandler ()
    {
        Int2event_map::iterator it= m_table_index.begin();
        do
        {
            if (it->second != NULL)
            {
                delete it->second;
            }
        } while (++it != m_table_index.end());
    }

    Row_Fields_map row_fields_val;
    Row_map rows_val;

    int get_m_table_id() const {
        return m_table_id;
    }

private:
    int m_table_id;
    typedef std::map<long int, binary_log::Table_map_event *> Int2event_map;
    Int2event_map m_table_index;
};


#endif //MYSQL_KAFKA_APPLIER_MYCONTENTHANDLER_H
