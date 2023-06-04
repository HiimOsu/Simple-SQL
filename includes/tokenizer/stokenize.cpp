#ifndef STOKENIZE_CPP
#define STOKENIZE_CPP

#include "stokenize.h"
#include <string.h>

//Static member
Fsm STokenizer::_table;

//PRE: there are still more() token in the buffer
//POST: 1.position = the last success position + 1
//      2.state = the last state before leaving the FSM
//      3.if return [success]: t = the first valid success tk
//          else [false]: t = the first invalid char  
bool dumb_get_token(const char* buff, int& pos, int& state, Fsm& table, std::string& t){

    int walker = pos;

    std::string hold(""), tk("");

    while (walker < MAX_BUFFER && buff[walker] > '\0' && buff[walker] < MAX_COLUMNS && table[state][buff[walker]] != -1)
    {   
        hold += buff[walker];
        state = table[state][buff[walker]];

        if(table.is_success(state)){
            tk += hold;
            hold = "";
        }
        walker++;    
    }

    if(tk == ""){
        t = buff[pos++];
        return false;
    }
    else{
        t = tk;
        pos += tk.length();
        return true;
    }
}


STokenizer::STokenizer(const char str[]){
    set_string(str);
    make_table();
}
STokenizer::STokenizer(){
    char arr[] = "";
    strcpy(_buffer, arr);
    _pos = 0;
    make_table();
}

bool STokenizer::done(){
    return _pos > strlen(_buffer);
}
bool STokenizer::more(){

    return _pos <= strlen(_buffer);

}            

void STokenizer::set_string(char str[]){
    // assert(strlen(str) <= MAX_BUFFER);
    strcpy(_buffer, str);
    if(strlen(str) > MAX_BUFFER)
        _buffer[MAX_BUFFER] = '\0';
    _pos = 0;
}

void STokenizer::set_string(const char str[]){
    // assert(strlen(str) <= MAX_BUFFER);
    strcpy(_buffer, str);
    if(strlen(str) > MAX_BUFFER)
        _buffer[MAX_BUFFER] = '\0';
    _pos = 0;
}


void STokenizer::make_table(){
    if(_table.is_made()){
        return; 
    }
 
    STokenizer::_table.set_made();

    _table.init_table();

    //Default State Path
    _table.mark_cells(0, DIGITS, 1);
    _table.mark_cells(0, ALFA, 2);
    _table.mark_cells(0, SPACES, 3);
    _table.mark_cells(0, OPERATORS, 4);
    _table.mark_cell(0, '=', 8);
    _table.mark_cells(0, PUNC, 5);



    //Mark Fail/Success
    int success_state[] = {1,2,3,4,5,7, 8, 9};
    int fail_state[] = {0,6};
    _table.mark_fail(fail_state,2);
    _table.mark_success(success_state, 8);


    //Digits State Path
    _table.mark_cells(1, DIGITS, 1);
    _table.mark_cell(1, '.', 6);
    _table.mark_cells(6, DIGITS, 7);
    _table.mark_cells(7, DIGITS, 7);

    //SQL_Relational
    _table.mark_cell(4, '=', 9);

    //ALPH State Path
    _table.mark_cells(2, ALFA, 2);


}

bool STokenizer::get_token(int& start_state, std::string& token){
    // if(more()){
        // std::cout << "[Debug]: There is more token. Getting More Token NOW!  pos = " << _pos << ", state = " << start_state << std::endl;
    return dumb_get_token(_buffer, _pos, start_state, _table, token);
    // }

    // token = "";
    // start_state = TOKEN_END;
    // return false;
}


STokenizer& operator >> (STokenizer& s, Token& t){
    std::string token = "";
    int state = 0;

    if(s.more()){
        s.get_token(state, token);
    }
    else state = -2;
    
    t = Token(token, state); 
    return s;
}


#endif