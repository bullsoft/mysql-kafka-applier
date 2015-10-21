//
// Created by 顾伟刚 on 15/5/13.
//

#ifndef MYSQL_KAFKA_APPLIER_BINLOGEVENT_H
#define MYSQL_KAFKA_APPLIER_BINLOGEVENT_H

#include "binlog.h"
#include "MyEvent.h"

using binary_log::system::Binary_log_driver;

#define MAX_BINLOG_SIZE 1073741824
#define MIN_BINLOG_POSITION 4
#define MAX_BINLOG_POSITION MAX_BINLOG_SIZE/4

class MyBinlog {
public:
    MyBinlog(std::string uri);
    ~MyBinlog();

    int connect();
    int disconnect();

    int set_position(unsigned long position, std::string filename = "");

    int get_next_event(MyEvent *my_event);

    const std::string get_filename() const {
        std::string filename;
        m_binlog->get_position(filename);
        return filename;
    }

    const long get_position() const {
        return m_binlog->get_position();
    }

private:
    std::string m_uri;
    Binary_log_driver *m_drv;
    Binary_log *m_binlog;
    binary_log::Decoder m_decode;
    Content_stream_handler m_handler;
};


#endif //MYSQL_KAFKA_APPLIER_BINLOGEVENT_H
