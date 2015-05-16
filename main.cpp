#include <iostream>

#include "binlog.h"
#include "Kafka.h"


using binary_log::Binary_log;
using binary_log::system::create_transport;

using namespace std;


int main() {
    std::string kafka_brokers = "localhost";
    std::string kafka_topic = "test";

    Kafka k(kafka_brokers, kafka_topic);
    k.produce("hfasdfadf12e2341234");
    k.produce("1223r231343");
    k.produce("32454356203-rldmfladlk;dl");

    return 0;
}