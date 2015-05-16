//
// Created by 顾伟刚 on 15/5/13.
//

#ifndef MYSQL_KAFKA_APPLIER_BINLOGEVENT_H
#define MYSQL_KAFKA_APPLIER_BINLOGEVENT_H

#include "binlog.h"
using binary_log::Binary_log;
using binary_log::system::create_transport;
using binary_log::system::Binary_log_driver;

#define MAX_BINLOG_SIZE 1073741824
#define MAX_BINLOG_POSITION MAX_BINLOG_SIZE/4

class BinlogEvent {
public:
    BinlogEvent();
    ~BinlogEvent();

    std::string get_event_type_str(Log_event_type type) {
        return m_event_types[type];
    }

    int get_number_of_events() {
        return sizeof(m_event_types) / sizeof(m_event_types[0]);
    }

    int connect(std::string uri);

    int disconnect();

private:
    Binary_log_driver *m_drv;
    Binary_log *m_binlog;
    std::string m_event_types[]=
            {"Unknown",
             "Start_v3",
             "Query",
             "Stop",
             "Rotate",
             "Intvar",
             "Load",
             "Slave",
             "Create_file",
             "Append_block",
             "Exec_load",
             "Delete_file",
             "New_load",
             "RAND",
             "User var",
             "Format_desc",
             "Xid",
             "Begin_load_query",
             "Execute_load_query",
             "Table_map",
             "Write_rows_event_old",
             "Update_rows_event_old",
             "Delete_rows_event_old",
             "Write_rows_v1",
             "Update_rows_v1",
             "Delete_rows_v1",
             "Incident",
             "Heartbeat",
             "Ignorable",
             "Rows_query",
             "Write_rows",
             "Update_rows",
             "Delete_rows",
             "Gtid",
             "Anonymous_Gtid",
             "Previous_gtids",
             "User Defined"
            };
};


#endif //MYSQL_KAFKA_APPLIER_BINLOGEVENT_H
