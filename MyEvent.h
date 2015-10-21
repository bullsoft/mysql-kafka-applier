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
    bool is_rows_event();
    int set_type(Log_event_type type);
    std::string get_type_str();

    unsigned long long int get_position() const {
        return m_position;
    }

    void set_position(unsigned long long int position) {
        m_position = position;
    }

private:
    Log_event_type m_type;
    unsigned long long m_position;

};


#endif //MYSQL_KAFKA_APPLIER_MYEVENT_H
