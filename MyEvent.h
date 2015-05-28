//
// Created by 顾伟刚 on 15/5/27.
//

#ifndef MYSQL_KAFKA_APPLIER_MYEVENT_H
#define MYSQL_KAFKA_APPLIER_MYEVENT_H

#include <string>
#include "binlog.h"

class MyEvent {
public:
    MyEvent();
    ~MyEvent();

    bool is_data_affected();

    std::string event_type_str;
    Log_event_type event_type;
    std::string table = "";
    std::string message = "";
    unsigned long long position;
};


#endif //MYSQL_KAFKA_APPLIER_MYEVENT_H
