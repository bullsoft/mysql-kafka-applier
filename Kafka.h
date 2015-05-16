//
// Created by 顾伟刚 on 15/5/11.
//

#ifndef MYSQL_KAFKA_APPLIER_KAFKA_H
#define MYSQL_KAFKA_APPLIER_KAFKA_H

#include <string>
#include <iostream>
#include <exception>

#include "librdkafka/rdkafkacpp.h"

class MyDeliveryReportCb : public RdKafka::DeliveryReportCb {
public:
    void dr_cb (RdKafka::Message &message) {
        std::cout << "Message delivery for (" << message.len() << " bytes): " <<
        message.errstr() << std::endl;
    }
};


class MyEventCb : public RdKafka::EventCb {
public:
    void event_cb (RdKafka::Event &event) {
        switch (event.type())
        {
            case RdKafka::Event::EVENT_ERROR:
                std::cerr << "ERROR (" << RdKafka::err2str(event.err()) << "): " <<
                event.str() << std::endl;
                if (event.err() == RdKafka::ERR__ALL_BROKERS_DOWN)
                    // run = false;
                break;

            case RdKafka::Event::EVENT_STATS:
                std::cerr << "\"STATS\": " << event.str() << std::endl;
                break;

            case RdKafka::Event::EVENT_LOG:
                fprintf(stderr, "LOG-%i-%s: %s\n",
                        event.severity(), event.fac().c_str(), event.str().c_str());
                break;

            default:
                std::cerr << "EVENT " << event.type() <<
                " (" << RdKafka::err2str(event.err()) << "): " <<
                event.str() << std::endl;
                break;
        }
    }
};

class Kafka {
public:
    Kafka(std::string brokers, std::string topic);
    ~Kafka();

    int produce(std::string msg);

private:
    RdKafka::Topic *k_topic;
    RdKafka::Producer *k_producer;
    int32_t m_partition;
    std::string m_brokers;
    std::string m_topic;
};


#endif //MYSQL_KAFKA_APPLIER_KAFKA_H
