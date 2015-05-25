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
    /*
    std::string kafka_brokers = "localhost";
    std::string kafka_topic = "test";

    Kafka k(kafka_brokers, kafka_topic);
    k.produce("hfasdfadf12e2341234");
    k.produce("1223r231343");
    k.produce("32454356203-rldmfladlk;dl");
    */

    BinlogEvent binlog;
    binlog.connect(std::string("mysql://root@127.0.0.1:3306"));
    binlog.set_position(2392);

    cout << setw(17) << left
    << "Start Position"
    << setw(15) << left
    << "End Position"
    << setw(15) << left
    << "Event Length"
    << setw(25) << left
    << "Event Type"
    << endl;
    while(true) {
        if(binlog.get_next_event() != ERR_OK) break;
    }
    binlog.disconnect();
    return 0;
}