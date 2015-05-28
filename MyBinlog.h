//
// Created by 顾伟刚 on 15/5/13.
//

#ifndef MYSQL_KAFKA_APPLIER_BINLOGEVENT_H
#define MYSQL_KAFKA_APPLIER_BINLOGEVENT_H

#include "binlog.h"
#include "MyEvent.h"

using binary_log::system::Binary_log_driver;

#define MAX_BINLOG_SIZE 1073741824
#define MAX_BINLOG_POSITION MAX_BINLOG_SIZE/4

class MyBinlog {
public:
    MyBinlog();
    ~MyBinlog();

    std::string get_event_type_str(Log_event_type type);
    int get_number_of_events();

    int connect(std::string uri);
    int disconnect();
    std::string get_next_event(MyEvent *my_event);
    Binary_log *get_raw();

private:
    Binary_log_driver *m_drv;
    Binary_log *m_binlog;
    std::map<int, std::string> m_tid_tname;
    Table_map_event *m_tm_event;
};


#endif //MYSQL_KAFKA_APPLIER_BINLOGEVENT_H
