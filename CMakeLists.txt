cmake_minimum_required(VERSION 3.2)
project(mysql_kafka_applier)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("." "/usr/local/include/mysql-binlog-events" "/usr/local/mysql/include/mysql" "/usr/local/mysql/include" "/usr/local/include")

link_directories("/usr/local/lib" "/usr/lib")

set(SOURCE_FILES main.cpp)
add_executable(mysql_kafka_applier ${SOURCE_FILES} Kafka.cpp Kafka.h BinlogEvent.cpp BinlogEvent.h)

target_link_libraries(mysql_kafka_applier binlogevents mysqlstream rdkafka++ pthread z)
