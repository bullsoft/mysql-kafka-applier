# Copyright (C) 2015 BullSoft

project(mysql-kafka-applier)

cmake_minimum_required(VERSION 2.8)

message(STATUS "Running cmake version ${CMAKE_VERSION}")

set(MYSQL_KAFKA_APPLIER_VERSION "0.0.1")
set(CMAKE_COLOR_MAKEFILE   ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("." "/usr/local/include/mysql-binlog-events" "/usr/local/mysql/include/mysql" "/usr/local/mysql/include" "/usr/local/include")

link_directories("/usr/local/lib" "/usr/lib")

set(SOURCE_FILES
    main.cpp
    Kafka.cpp
    MyBinlog.cpp
    MyEvent.cpp
   )

add_executable(mysql_kafka_applier ${SOURCE_FILES})

target_link_libraries(mysql_kafka_applier binlogevents mysqlstream rdkafka++ pthread z)

install(TARGETS mysql_kafka_applier DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
