# Copyright (C) 2015 BullSoft

project(mysql-kafka-applier)

cmake_minimum_required(VERSION 2.8)

message(STATUS "Running cmake version ${CMAKE_VERSION}")

set(MYSQL_KAFKA_APPLIER_VERSION "0.0.1")
set(CMAKE_COLOR_MAKEFILE   ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${CMAKE_CURRENT_SOURCe_DIR})

if (BINLOG_PATH)
    include_directories(${BINLOG_PATH}/include)
    link_directories(${BINLOG_PATH}/lib)
else (BINLOG_PATH)
    include_directories("/usr/local/include/mysql-binlog-events")
endif (BINLOG_PATH)

if (MYSQL_PATH)
    include_directories("${MYSQL_PATH}/include" "${MYSQL_PATH}/include/mysql")
    link_directories(${MYSQL_PATH}/lib)
else (MYSQL_PATH)
    include_directories("/usr/local/mysql/include/mysql" "/usr/local/mysql/include")
endif (MYSQL_PATH)

link_directories("/usr/local/lib" "/usr/lib")

set(SOURCE_FILES main.cpp MyContentHandler.cpp MyContentHandler.h ComparisonValues.h)
add_executable(mysql_kafka_applier ${SOURCE_FILES} Kafka.cpp Kafka.h MyBinlog.cpp MyBinlog.h MyEvent.cpp MyEvent.h)

target_link_libraries(mysql_kafka_applier binlogevents mysqlstream rdkafka++ pthread z)

install(TARGETS mysql_kafka_applier DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
