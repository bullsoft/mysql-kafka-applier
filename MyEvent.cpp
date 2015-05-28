//
// Created by 顾伟刚 on 15/5/27.
//

#include "MyEvent.h"

MyEvent::MyEvent() {

}

MyEvent::~MyEvent() {

}

bool MyEvent::is_data_affected() {
    Log_event_type event_type = this->event_type;
    if(event_type == binary_log::PRE_GA_WRITE_ROWS_EVENT ||
       event_type == binary_log::PRE_GA_UPDATE_ROWS_EVENT ||
       event_type == binary_log::PRE_GA_DELETE_ROWS_EVENT ||
       event_type == binary_log::WRITE_ROWS_EVENT ||
       event_type == binary_log::WRITE_ROWS_EVENT_V1 ||
       event_type == binary_log::UPDATE_ROWS_EVENT ||
       event_type == binary_log::UPDATE_ROWS_EVENT_V1 ||
       event_type == binary_log::DELETE_ROWS_EVENT ||
       event_type == binary_log::DELETE_ROWS_EVENT_V1) {
        return true;
    }
    return false;
}
