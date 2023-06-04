#ifndef SQL_PARSER_FUNCTIONS_H
#define SQL_PARSER_FUNCTIONS_H

#include <string>


#include "constants.h"
#include "../bplustree/multimap.h"
#include "../bplustree/map.h"

#include "../tokenizer/stokenize.h"

using namespace std;

string get_value_str(STokenizer& stker, bool& abnormal);



#endif