//
// Created by 顾伟刚 on 15/5/11.
//

#include "Kafka.h"

Kafka::Kafka(std::string brokers, std::string topic)
{
    std::string err_str;
    m_partition = RdKafka::Topic::PARTITION_UA;

    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

    conf->set("metadata.broker.list", brokers, err_str);

    MyEventCb my_event_cb;
    conf->set("event_cb", &my_event_cb, err_str);

    MyDeliveryReportCb my_dr_cb;
    conf->set("dr_cb", &my_dr_cb, err_str);

    k_producer = RdKafka::Producer::create(conf, err_str);
    if (!k_producer) {
        std::cerr << "Failed to create producer: " << err_str << std::endl;
        exit(1);
    }
    std::cout << "% Created producer " << k_producer->name() << std::endl;
    k_topic = RdKafka::Topic::create(k_producer, topic, tconf, err_str);
    if (!k_topic) {
        std::cerr << "Failed to create topic: " << err_str << std::endl;
        exit(1);
    }
}

int Kafka::produce(std::string msg) {
    RdKafka::ErrorCode resp =
            k_producer->produce(k_topic, m_partition,
                                RdKafka::Producer::RK_MSG_COPY /* Copy payload */,
                                const_cast<char *>(msg.c_str()), msg.size(),
                                NULL, NULL);
    if (resp != RdKafka::ERR_NO_ERROR) {
        std::cerr << "% Produce failed: " << RdKafka::err2str(resp) << std::endl;
        return 1;
    } else {
        std::cerr << "% Produced message (" << msg.size() << " bytes)" << std::endl;
    }
    return 0;
}

Kafka::~Kafka() {
    while (k_producer->outq_len() > 0) {
        std::cerr << "Waiting for " << k_producer->outq_len() << std::endl;
        k_producer->poll(1000);
    }
    RdKafka::wait_destroyed(5000);
}