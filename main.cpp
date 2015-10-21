#include <iomanip>

#include "MyBinlog.h"
#include "Kafka.h"
#include "MyBinlog.h"

using namespace std;

int main() {
/*
    std::string kafka_brokers = "localhost";
    std::string kafka_topic = "test";

    Kafka *k;
    k = new Kafka(kafka_brokers);
*/
    MyBinlog *binlog;
    binlog = new MyBinlog(std::string("mysql://root@127.0.0.1:3306"));
    int err_code;
    err_code = binlog->connect();
    binlog->set_position(16425);
    std::cout << err_code << std::endl;

    bool run = true;

    while(run) {
        MyEvent* event = new MyEvent();
        int result;
        try {
            result = binlog->get_next_event(event);
            if (result != ERR_OK) {
                throw new std::runtime_error("failed to get next event");
            }
            std::cout << "filename and position: " << binlog->get_filename() << "\t" << binlog->get_position() << std::endl;

        }catch(const std::exception& e) {
            std::cout << e.what() << std::endl;
            run = false;
            binlog->disconnect();
        }
    }

    delete binlog;
    //delete k;
    return 1;
}
