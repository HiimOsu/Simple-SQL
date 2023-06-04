#ifndef SQL_PARSER_FUNCTIONS_CPP
#define SQL_PARSER_FUNCTIONS_CPP

#include "sql_parser_functions.h"

string get_value_str(STokenizer& stker, bool& abnormal){
    Token tk;
    string str;
    do
    {
        str += tk.token_str();
        stker >> tk;

        // cout << "DEBUG | more = " << stker.more() << " | str = " <<  tk.token_str() << endl;
    }while(stker.more() && tk.token_str() != "\"");

    if(tk.token_str() != "\"") abnormal = true;
    else abnormal = false;

    return str;
}


#endif