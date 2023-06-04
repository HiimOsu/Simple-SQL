#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <iostream>
#include <iomanip>

#include "constants.h"
#include "../table/typedefs.h"
#include "../error/error.h"

#include "../tokenizer/stokenize.h"
#include "../util/My_utilities.h"
#include "sql_parser_functions.h"
using namespace std;

// struct func_ptr;

class Parser{
public:
    Parser(string str = ""): _cmd(str){ init_map();}

    //setter
    void set_command(const string& str);
    void set_command(const char* str);
    void set_string(const string& str){set_command(str);}

    //getter
    mmap_ss parse_tree() throw(const Error_msg);
    string get_string(){return _cmd;}

    
    static Map<int, string> _type_table; //given a event type(int), return the event name
    static int event_type(string str);//return the event type based of the event type
private:
    string _cmd;
    static map_sl _event_table; // key = string(command), value = long(event type)
    static Fsm _table;
    static void init_map();

    void print_token();
    void state_function(int state, string str, mmap_ss& parse_tree);
};

// struct func_ptr{
//     func_ptr(){}
//     func_ptr(void (Parser::*_ptr)(string)): ptr(_ptr){}

//     void (Parser::*ptr)(string);
// };
#endif