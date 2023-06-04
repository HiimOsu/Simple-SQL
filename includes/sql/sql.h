#ifndef SQL_H
#define SQL_H

#include <string>
#include <vector>
#include <iostream>

#include "../parser/parser.h"
#include "../table/table.h"
#include "../error/error_def.h"

using namespace std;


/*  
//Commands:
    <CREATE | MAKE> : {  <create | make> table <TABLE_NAME> fields <FIELD_NAME> [, <FIELD_NAME>...]  }
    <INSERT> : { insert <INTO> <TABLE_NAME> values <VALUE> [, <VALUE>...]      }
    <SELECT> : {  select <* | FIELD_NAME> [, ,FIELD_NAME>...]
                        from <TABLE_NAME>
                        where <FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE>
                            [<LOGICAL_OPERATOR>
                                <FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE>...]
                }


//Keywords:
    <VALUE>  : A string of alphanumeric characters, or a string of alphanumeric
                characters and spaces enclosed by double quotation marks:
                "Jean Luise", Finch, 1923
    <RELATIONAL OPERATOR> : [ = | > | < | >= | <= ]
    <LOGICAL OPERATOR>    : [and | or]
*/
class SQL{
public:
    Table command(string command);
    vector<long> select_recnos(){return _table.select_recnos();}
private:
    void debug_log(string log1, string log2);
    string log2(int event, mmap_ss* ptree = nullptr);

    Table command_make(mmap_ss& ptree);
    Table command_insert(mmap_ss& ptree);
    Table command_select(mmap_ss& ptree);

    Parser parser;
    vector<long> _last_rec;
    Table _table;
    Table _last_selected;
};

#endif