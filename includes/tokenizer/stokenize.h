#ifndef STOKENIZER_H
#define STOKENIZER_H

#include <string>

#include "constants.h"
#include "token.h"
#include "FSM/fsm.h"

class STokenizer
{
public:
    STokenizer();
    //Pre : Receive a cstring
    // post: Tokenizer containing the string( if the string's length is longer than MAX BUFFER, excess char will be discarded);
    STokenizer(const char str[]);
    
    //Post:
    //  true: The last null token is extracted (_pos passes through the '\0' character)
    //  false: else
    bool done();      
    //Post:
    //  true:The last null token is not extracted yet(_pos is still within strlen(buff));
    //  false: else
    bool more();            
    
    //---------------
    //Pre: more is true
    //Post: extract token from buffer,
        //  Empty token
    friend STokenizer& operator >> (STokenizer& s, Token& t);

    //set a new string as the input string
    void set_string(char str[]);
    void set_string(const char str[]);

    int  pos(){return _pos;}

private:
    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    static void make_table();

    //[Main functions of the object]
    //extract the longest string that match
    //     one of the acceptable token types
    bool get_token(int& start_state, std::string& token);
    //---------------------------------
    char _buffer[MAX_BUFFER + 1];       //input string
    int _pos;                       //current position in the string
    static Fsm _table;
};

#endif