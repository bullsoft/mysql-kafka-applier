//
// Created by 顾伟刚 on 15/10/18.
//

#ifndef MYSQL_KAFKA_APPLIER_COMPARISONVALUES_H
#define MYSQL_KAFKA_APPLIER_COMPARISONVALUES_H

#include "binlog.h"
#include <iostream>
#include <cstdlib>
#include <map>
#include <string>
#include <stdio.h>

extern const char* uri_arg;

class ComparisonValues
{
public:
    typedef std::map<enum_field_types, std::string> Map;
    typedef std::map<std::string, std::string> VarMap;
    Map expected_field_val;
    VarMap expected_server_var;

    void assign_field_value();
};

void ComparisonValues::assign_field_value()
{
    expected_field_val[MYSQL_TYPE_TINY]= "127";
    expected_field_val[MYSQL_TYPE_SHORT]= "32767";
    expected_field_val[MYSQL_TYPE_LONG]= "2147483647";
    expected_field_val[MYSQL_TYPE_FLOAT]= "4.50933e+27";
    expected_field_val[MYSQL_TYPE_DOUBLE]= "4.17201e-309";
    expected_field_val[MYSQL_TYPE_TIMESTAMP]= "2038-01-19 03:14:07";
    expected_field_val[MYSQL_TYPE_LONGLONG]= "9223372036854775807";
    expected_field_val[MYSQL_TYPE_DATETIME]= "9999-12-31 23:59:59";
    expected_field_val[MYSQL_TYPE_VARCHAR]= "BAPI";
    // Not implemented yet in the API
    expected_field_val[MYSQL_TYPE_INT24]= "8388607";
    expected_field_val[MYSQL_TYPE_DATE]= "9999-12-31";
    expected_field_val[MYSQL_TYPE_TIME]= "838:59:59";
    expected_field_val[MYSQL_TYPE_BIT]= "b'11111111'";
}

#endif //MYSQL_KAFKA_APPLIER_COMPARISONVALUES_H
