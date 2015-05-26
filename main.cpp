#include "binlog.h"
#include "Kafka.h"
#include "BinlogEvent.h"


#include <iomanip>
#include <iostream>
using std::cout;
using std::setw;
using std::left;

using namespace std;

int main() {

    std::string kafka_brokers = "localhost";
    std::string kafka_topic = "test";

    Kafka *k;
    k = new Kafka(kafka_brokers);

    BinlogEvent *binlog;
    binlog = new BinlogEvent;
    binlog->connect(std::string("mysql://root@127.0.0.1:3306"));
    binlog->get_raw()->set_position(3194);

    while(true) {
        std::string binlog_filename;
        unsigned long pos;

        binlog->get_raw()->get_position(binlog_filename);
        pos = binlog->get_raw()->get_position();

        std::cout << binlog_filename << ":" << pos << std::endl;

        std::string msg;

        try {
            msg = binlog->get_next_event();
            k->produce(msg, kafka_topic);
        }catch(const std::exception& e) {
            std::cout << e.what() << std::endl;
            binlog->disconnect();
            return 1;
        }
        std::cout << msg << std::endl;
    }

}