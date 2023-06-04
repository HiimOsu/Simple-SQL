#ifndef TABLETOKEN_CONSTANTS_H
#define TABLETOKEN_CONSTANTS_H


const int MAX_COLUMNS = 256;
const int MAX_ROWS = 30;
const int MAX_BUFFER = 200;

const char DIGITS[] = "";
const char ALFA[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-";
const char SPACES[] = {' ', '\t', '\n', '\0'};
const char OPERATORS[] = "><";
const char PUNC[] = "?.,:;'`~!+%&|\"";


//token types:
 enum MYTOKEN_TYPE{
    TOKEN_END = -2,
    TOKEN_UNKNOWN,
    DEFAULT,
    TOKEN_NUMBER,
    TOKEN_ALPHA,
    TOKEN_SPACE,
    TOKEN_OPERATOR,
    TOKEN_PUNC,

    SQL_TOKEN_OPERATOR,
    SQL_TOKEN_LOGICAL,
    SQL_TOKEN_RELATIONAL,
    SQL_TOKEN_TKSTR,
    SQL_TOKEN_LP,
    SQL_TOKEN_RP,
    SQL_TOKEN_RESULT,
};

enum TOKEN_PRIORIT{
    LOWER,
    HIGHER
};
#endif