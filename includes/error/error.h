#ifndef ERROR_H
#define ERROR_H
#include <string>
#include "error_def.h"
using namespace std;

struct Error_msg{
    std::string _msg;
    SQL_error_t e_t; 
    Error_msg(const char* m, SQL_error_t error_type = ERROR_T_DEFAULT): e_t(error_type){_msg = m;}
    Error_msg(const std::string s, SQL_error_t error_type = ERROR_T_DEFAULT): e_t(error_type){_msg = s;}

};


#endif