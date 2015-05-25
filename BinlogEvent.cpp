//
// Created by 顾伟刚 on 15/5/13.
//

#include "BinlogEvent.h"

#include <iomanip>

using std::string;
using std::map;
using std::cout;
using std::setw;
using std::left;

using binary_log::Binary_log;
using binary_log::system::create_transport;
using binary_log::system::Binary_log_driver;

typedef std::vector<Value >::iterator Row_of_fields_Iterator;

std::pair<unsigned char *, size_t> buffer_buflen;
Decoder decode;

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

std::string get_event_type_str(Log_event_type type) {
    return event_types[type];
}

int get_number_of_events() {
    return sizeof(event_types) / sizeof(event_types[0]);
}

int BinlogEvent::connect(std::string uri) {
    m_drv = create_transport(uri.c_str());
    m_binlog = new Binary_log(m_drv);

    int error_number = m_binlog->connect();

    if (const char* msg = str_error(error_number)) {
        throw new std::runtime_error(std::string(msg));
    }

    if (error_number != ERR_OK) {
        throw new std::runtime_error("Unable to setup conneciton");
    }
}

int BinlogEvent::disconnect() {
    m_binlog->disconnect();
    return 0;
}

int BinlogEvent::set_position(unsigned long start_pos) {
    if (start_pos > MAX_BINLOG_POSITION) {
        throw new std::runtime_error("Illegal binlog start position");
    };
    if(m_binlog->set_position(start_pos) != ERR_OK) {
        throw new std::runtime_error("The specified position can not be set");
    }
    m_start_position = start_pos;
}

int BinlogEvent::get_next_event() {
    Binary_log_event *event;
    string database_dot_table;

    int error_number;
    const char *error = NULL;

    error_number = m_drv->get_next_event(&buffer_buflen);

    if (error_number == ERR_OK) {
        const char *error = NULL;
        if (!(event = decode.decode_event((char*)buffer_buflen.first, buffer_buflen.second, &error, 1))) {
            throw std::runtime_error("can not decode event: " + std::string(error));
        }
    } else {
        const char* msg = str_error(error_number);
        std::cerr << msg << std::endl;
        throw std::runtime_error("get next event error");
    }

    if (event->get_event_type() == binary_log::INCIDENT_EVENT ||
        (event->get_event_type() == binary_log::ROTATE_EVENT &&
         event->header()->log_pos == 0 ||
         event->header()->log_pos == 0))
    {
        /*
          If the event type is an Incident event or a Rotate event
          which occurs when a server starts on a fresh binlog file,
          the event will not be written in the binlog file.
        */
        m_now_position = 0;
    } else {
        m_now_position = (event->header()->log_pos) - (event->header())->data_written;
    }

    if(event->get_event_type() == binary_log::TABLE_MAP_EVENT ||
       event->get_event_type() == binary_log::PRE_GA_WRITE_ROWS_EVENT ||
       event->get_event_type() == binary_log::PRE_GA_UPDATE_ROWS_EVENT ||
       event->get_event_type() == binary_log::PRE_GA_DELETE_ROWS_EVENT ||
       event->get_event_type() == binary_log::WRITE_ROWS_EVENT ||
       event->get_event_type() == binary_log::WRITE_ROWS_EVENT_V1 ||
       event->get_event_type() == binary_log::UPDATE_ROWS_EVENT ||
       event->get_event_type() == binary_log::UPDATE_ROWS_EVENT_V1 ||
       event->get_event_type() == binary_log::DELETE_ROWS_EVENT ||
       event->get_event_type() == binary_log::DELETE_ROWS_EVENT_V1)
    {
        map<int, string>::iterator tb_it;

        if (event->get_event_type() == binary_log::TABLE_MAP_EVENT) {
            m_tm_event = static_cast<Table_map_event*>(event);
            database_dot_table = m_tm_event->get_db_name();
            database_dot_table.append(".");
            database_dot_table.append(m_tm_event->get_table_name());
            m_tid_tname[m_tm_event->get_table_id()]= database_dot_table;
        } else {
            // It is a row event
            Rows_event *row_event= static_cast<Rows_event*>(event);
            m_tid_tname.begin();
            tb_it = m_tid_tname.find(row_event->get_table_id());
            if (tb_it != m_tid_tname.end())
            {
                database_dot_table = tb_it->second;
                if (row_event->get_flags() == Rows_event::STMT_END_F) {
                    m_tid_tname.erase(tb_it);
                }
            } else {
                throw new std::runtime_error("can not find db.table");
            }

            Converter converter;
            Row_event_set rows(row_event, m_tm_event);
            try
            {
                Row_event_set::iterator it = rows.begin();
                do
                {
                    Row_of_fields fields = *it;
                    if (row_event->get_event_type() == binary_log::WRITE_ROWS_EVENT ||
                        row_event->get_event_type() == binary_log::WRITE_ROWS_EVENT_V1) {
                        Row_of_fields_Iterator field_it = fields.begin();
                        std::cout << " , database.table: " << database_dot_table << ", data: ";
                        do {

                            std::string str;
                            converter.to(str, *field_it);
                            std::cout << str << "\t";

                        } while(++field_it != fields.end());
                        std::cout << std::endl;
                    }
                } while (++it != rows.end());
            }
            catch (const std::logic_error& le)
            {
                std::cerr << "MySQL Data Type error: " << le.what() << '\n';
            }
        }


        cout << setw(17) << left
        << m_start_position
        << setw(15) << left
        << (event->header())->log_pos
        << setw(15) << left
        << (event->header())->data_written
        << event_types[(event->get_event_type())]
        << "[" << event->get_event_type()
        << "]"
        << std::endl;
        cout << "Event Info: ";
        event->print_long_info(cout);
    }
    return 0;
}

BinlogEvent::~BinlogEvent() {
    delete m_drv;
    delete m_binlog;
    m_binlog = NULL;
    m_drv = NULL;
}

BinlogEvent::BinlogEvent() {

}

Binary_log *BinlogEvent::get_raw() {
    return m_binlog;
}
