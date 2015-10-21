//
// Created by 顾伟刚 on 15/5/27.
//

#include "MyEvent.h"

std::string event_types[] = {
        "Unknown",
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

MyEvent::~MyEvent() {

}

bool MyEvent::is_rows_event() {
    if(m_type == binary_log::PRE_GA_WRITE_ROWS_EVENT ||
       m_type == binary_log::PRE_GA_UPDATE_ROWS_EVENT ||
       m_type == binary_log::PRE_GA_DELETE_ROWS_EVENT ||
       m_type == binary_log::WRITE_ROWS_EVENT ||
       m_type == binary_log::WRITE_ROWS_EVENT_V1 ||
       m_type == binary_log::UPDATE_ROWS_EVENT ||
       m_type == binary_log::UPDATE_ROWS_EVENT_V1 ||
       m_type == binary_log::DELETE_ROWS_EVENT ||
       m_type == binary_log::DELETE_ROWS_EVENT_V1) {
        return true;
    }
    return false;
}

std::string MyEvent::get_type_str() {
    return event_types[m_type];
}

MyEvent::MyEvent() {

}

int MyEvent::set_type(Log_event_type type) {
    m_type = type;
    return ERR_OK;
}
