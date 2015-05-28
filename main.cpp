#include "MyBinlog.h"
#include "MyEvent.h"
#include "Kafka.h"
#include "MyBinlog.h"

#include <iomanip>
using std::cout;
using std::setw;
using std::left;

using namespace std;

int main() {
/*
    std::string kafka_brokers = "localhost";
    std::string kafka_topic = "test";

    Kafka *k;
    k = new Kafka(kafka_brokers);
*/
    MyBinlog *binlog;
    binlog = new MyBinlog;
    binlog->connect(std::string("mysql://root@127.0.0.1:3306"));
    binlog->get_raw()->set_position(3194);

    bool run = true;

    while(run) {
        std::string binlog_filename;
        unsigned long pos;

        binlog->get_raw()->get_position(binlog_filename);
        pos = binlog->get_raw()->get_position();

        std::cout << binlog_filename << ":" << pos << std::endl;
        std::string msg;

        MyEvent* event = new MyEvent();
        try {
            msg = binlog->get_next_event(event);
        }catch(const std::exception& e) {
            std::cout << e.what() << std::endl;
            run = false;
            binlog->disconnect();
        }

        std::cout << event->event_type_str << std::endl;

        if(event->is_data_affected()) {
            std::cout << event->message << std::endl;
            // k->produce(msg, kafka_topic);
        }

        delete event;
    }

    delete binlog;
    //delete k;
    return 1;
}