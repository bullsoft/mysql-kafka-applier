//
// Created by 顾伟刚 on 15/5/13.
//

#include "MyBinlog.h"
#include "MyContentHandler.h"

#include <iomanip>
#include <stdio.h>
#include <stdlib.h>

using std::string;
using std::map;
using std::cout;
using std::setw;
using std::left;

using binary_log::Binary_log;
using binary_log::system::create_transport;
using binary_log::system::Binary_log_driver;

MyContentHandler contentHandler;

int MyBinlog::connect() {
    m_drv = create_transport(m_uri.c_str());
    m_binlog = new Binary_log(m_drv);

    int error_number;
    error_number = m_binlog->connect();

    if (const char* msg = str_error(error_number)) {
        throw new std::runtime_error(std::string(msg));
    }

    if (error_number != ERR_OK) {
        throw new std::runtime_error("Unable to setup conneciton");
    }

    m_handler.add_listener(contentHandler);

    std::cout << "Connected ... " << std::endl;

    return ERR_OK;
}

int MyBinlog::disconnect() {
    m_binlog->disconnect();
    return ERR_OK;
}

int MyBinlog::get_next_event(MyEvent *my_event) {

    Binary_log_event *event;

    int result;
    std::pair<unsigned char *, size_t> buffer_buflen;
    result = m_drv->get_next_event(&buffer_buflen);
    if (result == ERR_OK) {
        if (!(event = m_decode.decode_event((char *) buffer_buflen.first, buffer_buflen.second, NULL, 1))) {
            return ERR_FAIL;
        }
    }
    else {
        return ERR_FAIL;
    }

    my_event->set_type(event->get_event_type());
    my_event->set_position(event->header()->log_pos);

    if (event->header()->type_code == binary_log::TABLE_MAP_EVENT) {
        m_handler.handle_event(&event);
        return ERR_OK;
    }

    m_handler.handle_event(&event);

    if(my_event->is_rows_event()) {
        for(Row_map::iterator row_it = contentHandler.rows_val.begin();
            row_it != contentHandler.rows_val.end();
            ++ row_it)
        {
            for(Row_Fields_map::iterator filed_it = row_it->begin();
                filed_it != row_it->end();
                ++ filed_it)
            {
                std::cout << filed_it->second << "\t";
            }
            std::cout << std::endl;
        }
    }

    if (event->header()->type_code == binary_log::QUERY_EVENT) {
        binary_log::Query_event *qev = dynamic_cast<binary_log::Query_event *>(event);

        std::cout << "Query = "
        << qev->query
        << " DB = "
        << qev->db
        << std::endl;
    }

    delete event;

    return ERR_OK;
}

MyBinlog::~MyBinlog() {
    delete m_drv;
    delete m_binlog;
    m_binlog = NULL;
    m_drv = NULL;
}

MyBinlog::MyBinlog(std::string uri) {
    m_uri = uri;
}

int MyBinlog::set_position(unsigned long position, std::string filename) {
    if(m_drv == NULL) {
        throw new std::runtime_error("not connected yet");
    }
    if (position < MIN_BINLOG_POSITION || position > MAX_BINLOG_POSITION) {
        throw new std::runtime_error("invalid position");
    }

    if (filename.empty()) {
        return m_binlog->set_position(position);
    } else {
        return m_binlog->set_position(static_cast<const std::string>(filename), position);
    }
}

