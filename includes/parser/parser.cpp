#ifndef PARSER_CPP
#define PARSER_CPP

#include "parser.h"

Fsm Parser::_table;
map_sl Parser::_event_table;
Map<int, string> Parser::_type_table;
void Parser::set_command(const string& str){
    _cmd = str;
}
void Parser::set_command(const char* str){
    _cmd = str;
}


mmap_ss Parser::parse_tree() throw(const Error_msg){
    bool debug = false;
    if(debug) cout << "[Debug] [ParseTree] Entering parse Tree && cmd = " << _cmd << endl;

    STokenizer stker; 
    Token tk;                   //token extract from stker
    mmap_ss map;                //out put
    string str,                 //holder for tk._str
    last_str;                   //last_string

    int state = ST_GET_COMMAND, // current state
     last_state;                //last state;
    int type;                   //holder for tk._type

    bool get_new_buffer = false;
    bool continue_value_str = false;

    int pos = 0;
    while(pos < _cmd.size()){
        stker.set_string(_cmd.c_str() + pos);
        pos += MAX_BUFFER;

        stker >> tk;
        
        //situation: cmd size > buffer size
        if(get_new_buffer){
            type = tk.type();
            str = tk.token_str();
            if(debug) {
                cout << "[Debug] [ParseTree] Get New Buf,"
                << "\n\tcurrent post: " << pos
                << "\n\tLast string: " << last_str 
                << "\n\tcurrent str: " << str
                << "\n\tcontinue_value_str: " << continue_value_str
                << "\n\n";  
            }


            //concatenate the first token of the new buffer & the last token of the last buffer (if it is needed)
            //else parse the last token from the old buffer before parsing the latest token
            if(continue_value_str){ //If get_value_str is not done yet from the last buffer...

                if(str != "\"")
                    last_str += str + get_value_str(stker, continue_value_str);
            
            }
            else
            {//else 3 cases

                if(str == "\"" || str == "," ){ //case 1 & 2
                    //Parse the old token first
                    last_state = state;
                    state =  _table[state][event_type(last_str)];
                    state_function(state, last_str, map);
                    
                    if(str == "\""){//case 1: new token is a value_string
                        last_str = get_value_str(stker, continue_value_str);
                    }
                    else // case2: if(str == ",") new token is just a comma
                        last_str = str; 
                    
                    
                    if(state == -1) break;

                }else if(type != TOKEN_SPACE){//case 3: token is not a space
                    last_str += str; // concat old and new token
                }
            }
            


            stker >> tk;
            if(stker.done() && pos < _cmd.size()){//if it need to get a new buff
                get_new_buffer = true;
                break;
            }
            //parse the first_token from the lattest buffer
            last_state = state;
            state =  _table[state][event_type(last_str)];
            state_function(state, last_str, map);

        }

        //parsing current buffer
        do
        {    
            // assert(state != -1);       //current state does not exist ()

            if(state == -1) break;

            type = tk.type();
            str = tk.token_str();
            if(type != TOKEN_SPACE){
                
                //string enclosed by quotational mark
                if(str == "\""){
                    continue_value_str = false;
                    str = get_value_str(stker, continue_value_str);

                    if(stker.done() && pos < _cmd.size()){
                        last_str = str;
                        get_new_buffer = true;
                        break;
                    }
                }

                //transit to next state

                stker >> tk;
                if(stker.done() && pos < _cmd.size()){ // if reach the end of buff er
                    last_str = str;
                    get_new_buffer = true;
                    break;
                }
                
                last_state = state;
                state = _table[state][event_type(str)]; //based on the event types
                state_function(state, str, map);

                if(debug && false)
                    cout << "Token: \"" << str << "\"  Type: " << _type_table[event_type(str)] << " State: " << state << endl;
            }
            else {
                //next tok
                stker >> tk;
                if(stker.done() && pos < _cmd.size()){ // if reach the end of buff er
                    last_str = str;
                    get_new_buffer = true;
                    break;
                }
            }
        
        }while(stker.more());

    }
    


    if(state == ST_SELECT_GET_WHERE) map["where"].push_back("no");


    // if(state == -1){
        
    // }
    // else if(_table[state][0] == 0){
        
    // }
    
    return map;
}

void Parser::print_token(){
    STokenizer stker(_cmd.c_str());
    Token tk;
    while (stker.more())
    {   
        stker >> tk;
        cout << left << "Token: " << setw(15) << tk.token_str() << "type: " << setw(15) << tk.type()  << endl;

    }
    cout << "last token as int: " << (int) (tk.token_str()[0]) << endl;
}

//Private
int Parser::event_type(string str){
    if(Parser::_event_table.contains(str)) return Parser::_event_table[str];
    else if (Parser::_event_table.contains(to_lower(str))) return _event_table[to_lower(str)];
    else return EV_STRING;
}

void Parser::init_map(){
    if(_table.is_made()) return;
    _table.set_made();

    //FSM
    {
    _table.init_table();
    _table.mark_success(ST_SELECT_GET_WHERE);
    _table.mark_success(ST_SELECT_CONDITION);
    _table.mark_success(ST_INSERT_VALUES);
    _table.mark_success(ST_INSERT_VALUES_COMMA);
    _table.mark_success(ST_MAKE_FIELDS);
    _table.mark_success(ST_MAKE_FIELDS_COMMA);

    //Unkown
    for(int i = 1; i < MAX_COLUMNS; ++i){
        _table.mark_cell(ST_GET_COMMAND, i , ST_UNKNOWN_COMMAND);
    }
    //Make case
    _table.mark_cell(ST_GET_COMMAND, EV_MAKE, ST_MAKE);     // "Make" [insert]
    _table.mark_cell(ST_GET_COMMAND, EV_CREATE, ST_MAKE);   // "Create" [insert]
    _table.mark_cell(ST_MAKE, EV_TABLE, ST_MAKE_TABLE_NAME);  // "table" 


    _table.mark_cell(ST_MAKE_TABLE_NAME, EV_STRING, ST_MAKE_GET_FIELD); //input = table_name [insert]
    
    _table.mark_cell(ST_MAKE_GET_FIELD, EV_FIELD, ST_MAKE_PRE_FIELDS); // "field"

    _table.mark_cell(ST_MAKE_PRE_FIELDS, EV_STRING, ST_MAKE_FIELDS);  //field_name1... [insert]
    _table.mark_cell(ST_MAKE_FIELDS, EV_COMMA, ST_MAKE_FIELDS_COMMA); // ','
    _table.mark_cell(ST_MAKE_FIELDS_COMMA, EV_STRING, ST_MAKE_FIELDS); // field_name2 ...
    _table.mark_cell(ST_MAKE_FIELDS, EV_STRING, ST_MAKE_FIELDS); // ','

    //Select case
    _table.mark_cell(ST_GET_COMMAND, EV_SELECT, ST_SELECT);

    _table.mark_cell(ST_SELECT, EV_ASTRISK, ST_SELECT_GET_FROM);    // '*' [insert]

    _table.mark_cell(ST_SELECT, EV_STRING, ST_SELECT_FIELDS_COMMA); // field1 [insert]


    _table.mark_cell(ST_SELECT_FIELDS_COMMA, EV_COMMA, ST_SELECT_FIELDS); //','
    _table.mark_cell(ST_SELECT_FIELDS_COMMA, EV_STRING, ST_SELECT_FIELDS_COMMA); //fields2 [insert]
    

    _table.mark_cell(ST_SELECT_FIELDS, EV_STRING, ST_SELECT_FIELDS_COMMA); // field2 [insert]
    

    _table.mark_cell(ST_SELECT_FIELDS_COMMA, EV_FROM, ST_SELECT_TABLE_NAME);// "from"
    _table.mark_cell(ST_SELECT_GET_FROM, EV_FROM, ST_SELECT_TABLE_NAME);// "from"
    _table.mark_cell(ST_SELECT_TABLE_NAME, EV_STRING, ST_SELECT_GET_WHERE); //table_name [insert]

    _table.mark_cell(ST_SELECT_GET_WHERE, EV_WHERE, ST_SELECT_PRE_CONDITION); //"where" [insert]
    _table.mark_cell(ST_SELECT_PRE_CONDITION, EV_STRING, ST_SELECT_CONDITION); // condition1... [insert]
    _table.mark_cell(ST_SELECT_CONDITION, EV_STRING, ST_SELECT_CONDITION); // condition2... [insert]

    //Insert case
    _table.mark_cell(ST_GET_COMMAND, EV_INSERT, ST_INSERT);
    _table.mark_cell(ST_INSERT, EV_INTO, ST_INSERT_TABLE_NAME);
    _table.mark_cell(ST_INSERT_TABLE_NAME, EV_STRING, ST_INSERT_GET_VALUES); // table_name [Insert]
    _table.mark_cell(ST_INSERT_GET_VALUES, EV_VALUES, ST_INSERT_PRE_VALUES); // "values"
    _table.mark_cell(ST_INSERT_PRE_VALUES, EV_STRING, ST_INSERT_VALUES); // value1...[Insert]
    _table.mark_cell(ST_INSERT_VALUES, EV_COMMA, ST_INSERT_VALUES_COMMA); // ','
    _table.mark_cell(ST_INSERT_VALUES_COMMA, EV_STRING, ST_INSERT_VALUES); // value2...[Insert]
    _table.mark_cell(ST_INSERT_VALUES, EV_STRING, ST_INSERT_VALUES); // value2...[Insert]

    }

    {
        _event_table["make"] = EV_MAKE;
        _event_table["create"] = EV_CREATE;
        _event_table["select"] = EV_SELECT;
        _event_table["insert"] = EV_INSERT;
        _event_table["into"] = EV_INTO;
        _event_table["table"] = EV_TABLE;
        _event_table["from"] = EV_FROM;
        _event_table["fields"] = EV_FIELD;
        _event_table["values"] = EV_VALUES;
        _event_table[","] = EV_COMMA;
        _event_table["where"] = EV_WHERE;
        _event_table["*"] = EV_ASTRISK;

    }

    {
        _type_table[EV_MAKE] = "make";
        _type_table[EV_CREATE]  = "create";
        _type_table[EV_SELECT]  = "select";
        _type_table[EV_INSERT]  = "insert";
        _type_table[EV_INTO]    = "into";
        _type_table[EV_TABLE]   = "table";
        _type_table[EV_FROM]    = "from";
        _type_table[EV_FIELD]   = "fields";
        _type_table[EV_VALUES]  = "values";
        _type_table[EV_COMMA]   = ",";
        _type_table[EV_WHERE]   = "where";
        _type_table[EV_ASTRISK] = "*";
        _type_table[EV_STRING]  = "string";

    }
}

void Parser::state_function(int state, string str, mmap_ss& parse_tree){
    vectorstr::iterator w;
    switch (state)
    {
    case ST_UNKNOWN_COMMAND:
    case ST_MAKE:
    case ST_SELECT:
    case ST_INSERT:
        parse_tree["command"] += str;
        break;
    //MAKE
    case ST_MAKE_FIELDS:        //In MakeTable: get the fields_name 
        parse_tree["col"] += str;
        break;
    
    //SELECT
    case ST_SELECT_FIELDS_COMMA:
    case ST_SELECT_GET_FROM:
        parse_tree["fields"] += str;
        break;
    //SELECT / MAKE / INSERT
    case ST_MAKE_GET_FIELD:
    case ST_SELECT_GET_WHERE:
    case ST_INSERT_GET_VALUES:
        parse_tree["table_name"] += str;
        break;

    //SELECT
    case ST_SELECT_PRE_CONDITION:
        parse_tree["where"].push_back("yes");
        break;
    case ST_SELECT_CONDITION:
        
        parse_tree["condition"] += str;
        break;
    //INSERT
    case ST_INSERT_VALUES:
        parse_tree["values"] += str;
        break;
    // case ST_SELECT_FIELDS_COMMA_STRING:
        // w = parse_tree["fields"].begin();
        // for(auto temp = ++parse_tree["fields"].begin(); temp != parse_tree["fields"].end(); ++temp, ++w){
        //     cout << "\t[Debug] [state_func] : w :" << *w << endl;
        //     cout << "\t\t[Debug] [state_func] : temp :" << *temp << endl;
        // }
        // *w += str;
    default:
        break;
    }
}

#endif