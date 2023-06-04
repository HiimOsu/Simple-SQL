#include "sql.h"

Table SQL::command(string cmd){

    parser.set_command(cmd);

    mmap_ss ptree = parser.parse_tree();
    if(!ptree.contains("command")){
        return _table;
    }
    string command = ptree["command"][0];
    string log1 = ": [Getting Command] ";
    int command_type = Parser::event_type(command);
    
    switch (command_type)
    {
    case EV_MAKE:
    case EV_CREATE:
        _table = command_make(ptree);
        if(!_table.is_empty())
            _table.select_all();
        break;
    case EV_INSERT:
        _table = command_insert(ptree);
        if(!_table.is_empty())
            _table.select_all();
        break;
    case EV_SELECT:
        _last_selected = command_select(ptree);
        return _last_selected;
        break;
    default:
        debug_log(log1,log2(INVALID_COMMAND, &ptree));//Unknown command retrived
        _table = Table();
        break;
    }
    
    return _table;
}


//Private helper:
Table SQL::command_insert(mmap_ss& ptree){
    string log1 = ": [Command = INSERT] ";
    if(!ptree.contains("table_name")){//if no "table_name"
        debug_log(log1, log2(MISSING_TABLE_NAME));
        return Table();
    }

    if(!ptree.contains("values")){//if no "values"
        debug_log(log1, log2(MISSING_VALUES));
        return Table();
    }

    Table t;
    try{
        t = Table(ptree["table_name"][0]);
        t.insert_into(ptree["values"]);

    }
    catch(Error_msg e){
        debug_log(log1 + ": ", e._msg);
    }

    return t;
}

Table SQL::command_make(mmap_ss& ptree){
    string log1 = ": [Command = MAKE/CREATE] ";
    if(!ptree.contains("table_name")){
        debug_log(log1, log2(MISSING_TABLE_NAME, &ptree));
        return Table();
    }


    if(!ptree.contains("col")){
        debug_log(log1, log2(MISSING_COLS));
        return Table();
    }

    Table t;

    try{
        t = Table(ptree["table_name"][0], ptree["col"]);
    }
    catch(const Error_msg e){
        debug_log(log1, e._msg);
    }


    return t;
}

Table SQL::command_select(mmap_ss& ptree){
    string log1 = ": command = select ";
    if(!ptree.contains("fields") ){
        debug_log(log1, log2(MISSING_FIELDS_SELECT));
        return _last_selected;        
    }

    if(!ptree.contains("table_name")){
        debug_log(log1, log2(MISSING_TABLE_NAME, &ptree));
        return _last_selected;
    }

    if(!ptree.contains("where")){
        debug_log(log1, log2(MISSING_WHERE));
        return Table();
    }


    try{
        _table = Table(ptree["table_name"][0]);

    }
    catch(Error_msg e){
        cout << "[Select] : " << e._msg << endl;
        return _last_selected;
    }

    Table temp = _last_selected;

    if(ptree["where"][0] == "no"){//No conditions...
        temp = _table.select_fields(ptree["fields"]);
        return temp;
    }
    else{//Condition...
        if(!ptree.contains("condition")){//cannot find condition...
            debug_log(log1, log2(MISSING_CONDITIONS));
            _table.select_all();
            return _table;
        }

        try{
            temp = _table.select(ptree["fields"], ptree["condition"]);
        }
        catch(Error_msg e){
            cout << "[Error] [SQL] [Select] : " << e._msg << "\n\tHere is you command : " << ptree["condition"] << endl;
        }

        return temp;
    }
}

void SQL::debug_log(string log1, string log2){
    cout << "[Error] [SQL] " << log1 << log2 << endl;
}

string SQL::log2(int error_, mmap_ss* ptree){
    switch (error_)
    {
    case MISSING_VALUES:
        return ": for get to type in [values] (Missing \"values\" key-word ?)";
    case MISSING_TABLE_NAME:
        return ": for get to type in [table_name] (Missing \"table\" key-word ?)";
    case MISSING_COLS:
        return ": for get to type in [cols] (Missing \"fields\" key-word ?)";
    case MISSING_FIELDS_SELECT:
        return ": for get to select fields";
    case MISSING_CONDITIONS:
        return ": for get to type in [conditions]";
    case MISSING_FIELDS:
        return ": for get to type in [fields] (Missing \"fields\" key-word ?)";
    case MISSING_WHERE:
        return ": missing [where] key word ";
    case UNKNOWN_TABLE_NAME:
        return ": " + (*ptree)["table_name"][0] + " does not exist ";
    case REPEATED_TABLE_NAME:
        return ": table [" + (*ptree)["table_name"][0] + "] already exist ";
    case INVALID_COMMAND:
        return ": command [" + (*ptree)["command"][0] + "] is not a valid command (ie: MAKE, SELECT, INSERT) ";
    default:
        return ": unknown error ";
    }
}

