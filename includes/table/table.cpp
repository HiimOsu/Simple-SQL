#ifndef TABLE_CPP
#define TABLE_CPP

#include <iostream>
// #include <windows.h>

#include "table.h"


string operator + (string str, const vectorstr& vstr){
    for(auto x : vstr){
        str += " " + x ;
    }
    return str;
}
Error_msg error_msg(SQL_error_t e_t){
    switch (e_t){
        case TABLE_EMPTY_INSERT:
            return Error_msg("[Table::insert] : table is empty", TABLE_EMPTY_INSERT);
        case TABLE_EMPTY_SELECT:
            return Error_msg("[Table::select] : table is empty", TABLE_EMPTY_SELECT);
        case TABLE_INSERT_EXCESSV:
            return Error_msg("[Table::insert] : too much values, following values will be discarded: ", TABLE_INSERT_EXCESSV);
        case TABLE_INSERT_MISSINGV:
            return Error_msg("[Table::insert] : missing values, following fields will be filled as N/A: ", TABLE_INSERT_MISSINGV);
    }   
}
//============================
//Table class 
long Table::_serial_no = 0;
Map<string, int> Table::token_dic;
bool Table::init_already = false;
void (Table::* Table::arr[4])(vectortkptr&, string); //token functions

//Constructor
Table::Table(){
    init_dic();
    _empty = true;
}

    //Creating a new DB (if DB already exist, OverWrite the original file)
Table::Table(const string& table_name, const vector<string>& field_names)
{   
    init_dic(); 
    bool debug = false;
    if(debug) cout << "[Debug] Entering Constructor(create): \n";

    if(debug) cout << "table: " << table_name << ", fields: " << field_names << endl;
    _empty = false; //if open is no longe a empty table

    import_fields(field_names); //init field_names (and indecies)
    _table_name = table_name; //and table_name

    fstream f;
    //Create _filednames_configs.bin

    if(file_exists((table_name + ".bin").c_str()))  //check if table already there
        cout << "[Alert] [Table(Create)]: table_name: [" + table_name + "] already exist, and will be overwritten" << endl;

    open_fileW(f, (_table_name + "_configs.bin").c_str()); //create/overwrite the configurations file

    FileRecord r(field_names);
    r.write(f); //write all the field names into the configs files
    f.close();

    //Create table_name.bin
    open_fileW(f, (_table_name + ".bin").c_str()); //create/overwrite the .bin file
    f.close();
    if(debug) cout << "[Debug] Leaving Constructor(create): \n";
}    

    //importing table from existing DB
Table::Table(const string& table_name) throw(const Error_msg){
    init_dic();
    bool debug = false;
    // if(debug) cout << "[Debug] Entering Constructor(open): \n";
    _empty = false;

    _table_name = 
    // "table\\" +
     table_name;
    fstream f;
    FileRecord r;

    //reading field names: [ ...
    if( !file_exists(   (table_name + ".bin").c_str()  ) ) //if table does not exist (terminate and throw)
        throw (Error_msg("[Table(open)] : Table name : [" + table_name + "] does not exist"));

    open_fileR(f, (_table_name + "_configs.bin").c_str()); //open table in read mode
    

    if(r.read(f,0) == 0 || r.total_row() == 0){//if does not read any in field names in _configs
        _empty = true;//table is empty
        throw(Error_msg("[Table(open)] : it is a empty table (no fields)"));
    }
    
    // if(debug) cout << "\t[Debug] [Table(open)] Importing field names, here is the record:\n" << r << "\n\n\n";
    
    for(int i = 0; i < r.total_row(); ++i){      
        _field_names.push_back(r.get_row(i));                   // each row contain one exact one field_name;
        // if(debug){
        //     cout << "\t [Debug] [Table(open)] Importing field names:" << r.get_row(i) << endl;
        // }
    }   
    import_fields(_field_names);
    f.close();
    // ... ]
    
    


    //reading records: [ ...
    open_fileR(f, (_table_name + ".bin").c_str());
        // if(debug){
        //     cout << "\t[DEBUG] [Table(1arg)]: Opened table, filed_sizes = " << _field_names.size() << endl;
        // }
    
    long byte;
    long recno = 0;
    byte = r.read(f,recno);    //File record object "read" function, use 'record #' to know which record to read and where it is;
    while(byte != 0){
        import_record(r, recno);
        ++recno;
        byte = r.read(f,recno);
    }
    // ... ] 
    f.close();
    // if(debug) cout << "[Debug] Leaving Constructor(open): \n";
}

//Modifier
void Table::insert_into(const vector<string>& values)throw (const Error_msg){
    if(_empty){ //cannot insert record into a empty table
        throw(error_msg(TABLE_EMPTY_INSERT));
    }

    auto val = values;
    bool threw = false;
    Error_msg e("");
    if(values.size() < _field_names.size()){//if # values are less than number of fields
        e = error_msg(TABLE_INSERT_MISSINGV);
        threw = true;
        int i = val.size();

        while(val.size() < _field_names.size()){
            e._msg += "[" + _field_names[i++] + "] ";
            val.push_back("N\\A"); //insert a empty values
        }
    }
    else if(values.size() > _field_names.size()){//else if #values are more than number of fields
        e = error_msg(TABLE_INSERT_EXCESSV);
        threw = true;
        while (val.size() > _field_names.size())
        {
            e._msg += "[" + val.back() + "] ";
            val.pop_back(); //discard excess values
        }
        
    }
    
    
    bool debug = false;
    // if(debug) cout << "[Debug] Entering Inser_Into: \n";
    FileRecord rec(val); //Create a FileRecord Object with values
    fstream f;
    

    open_fileRW(f, (_table_name + ".bin").c_str());

    long recno = rec.write(f); //Write it into the bin file
    f.close();


    import_record(rec, recno); //import into the index system
    // if(debug) cout << "[Debug] Leaving Inser_Into: \n";
    if(threw){
        throw(e); //if discard or insert empty occur throw a error msg
    }
}


//Operator
Table Table::select_all()throw (const Error_msg){

   try
   {
        _last_record = total_recno();
   }
   catch(const Error_msg e)
   {
        throw(e);
   }
    
    return *this;
}

Table Table::select_fields(vectorstr fields){//not secure
    if(!fields.empty() && fields[0] == "*") return select_all();
    
    set_field(fields);
    _last_record = total_recno();
    
    return vector_to_table(fields, _last_record);
}

Table Table::select_where(Queue<Token*> condition){//not secure
    _last_record =  rpn(condition);
    return vector_to_table(_field_names, _last_record);
}

Table Table::select(vectorstr fields, string field, string relational, string value){
    // return select(fields, 
    // shunting_yard({new TokenStr(field), new Relational(relational), new TokenStr(value)}));
    
    if(!fields.empty() && fields[0] == "*") fields = _field_names;
    else set_field(fields);

    if(_field_map.contains(field)){
        Token* v = new TokenStr(value);
        Token* f = new TokenStr(field);
        _last_record = Relational(relational).evaluate(f, v, &_second_index[_field_map[field]]);
        delete v;
        delete f;
    }
    else
    {
        _last_record = {};
    }


    return vector_to_table(fields,_last_record);
}

Table Table::select(vectorstr fields, vector<string> infix_string) throw(const Error_msg){
    bool debug = false;
    if(debug) cout << "[Debug] Entering : \n";
    vector<Token*> infix;

    for(auto tk : infix_string){ //loop through the infix string and conver vectorstr into vector of token

        //and for each string push a corresponding token
        if(token_dic.contains(tk)){//if token exist in the map
            (this->*arr[token_dic[tk]])(infix, tk);//call one of the token function from the arr
        }
        else if(token_dic.contains(to_lower(tk))){//else if the lower case version exist in the map...
            (this->*arr[token_dic[to_lower(tk)]])(infix, tk); // do the same thing
        }
        else{//Else it is a TokenString
            infix.push_back(new TokenStr(tk));
        }

    }

    if(debug){
        cout << "[Debug] : [select(vstr, vstr)] : infix_token: "; print_vector(infix); cout <<endl;
    }
    
    Table t;
    try
    {   
        auto sy_w = shunting_yard(infix); //sy : turn vectortk into Queue of token (ptr)
                    //^^^^sy will not throw exception, but it will return sy_wrapper
        if(sy_w.is_error){ //if there is a error occur during shunting yard
            cout << "[Alert] : [Table::select(vstr,qstr)] : " + sy_w._error._msg + "\n\t Here is you command: " + infix_string + "\n";
        }// it won't terminate cause the error might not be a big deal(mainly the real error will be caught inside RPN)
        
        t = select(fields, sy_w._qtkptr); //call other select function
            //^^^this select function might also throw a exception

    }
    catch(const Error_msg& e) //if error were thrown
    {
        for(auto x : infix) delete x; //we still have to clean up the token we create

        string m = e._msg;
        m.insert(0, "[Table::select(vstr,qstr)] : "); //make our own error msg

       _last_record = vectorlong(); //since we fail to select, _last_record is [ ] empty
        throw Error_msg(m); //let outside handle the error
    }
    
    
    for(auto x : infix) delete x;
    return t;
}

Table Table::select(vectorstr fields, Queue<Token*> postfix) throw(const Error_msg){
    bool debug = false;
    if(_empty) 
            throw(Error_msg("[Error][table::select(fields, postfix)] : this table is empty"));
    // if(debug)
        // cout << "[Debug] : Entering select(fields, postfix)\n";

    if(!fields.empty() && fields[0] == "*"){
        //set fields to all
        fields = _field_names;
    }


    set_field(fields); //take out all the unmatch fields names inside [fields]

    try
    {
        _last_record =  rpn(postfix); //rpn might occur a lot of error
    }
    catch(const Error_msg& e) // if error occur, the error is deadly
    {
       string m = e._msg;
       m.insert(0, "[Table::select(vstr, qtkptr)] :");
       _last_record = vectorlong();
       throw(Error_msg(m));
    }
    
    sort(_last_record.begin(), _last_record.end());
    // if(debug)
        // cout << "[Debug] : [Select(postfix Queue)] : rpn success now returning table\n";

    return vector_to_table(fields, _last_record);
}

bool Table::set_field(vectorstr& fields){
    for(auto it = fields.begin(); it != fields.end(); ++it) 
       if(!_field_map.contains(*it)) {
            auto cur = it - fields.begin();
            std::cout << "\n[Alert] Choosen fields \"" << *it << "\" does not exist in table: " << _table_name << endl;
            fields.erase(it);
            it = fields.begin() + cur;
            if(it == fields.end()) break;
        }
    return true;
}


//Private

    //modifier
void Table::import_record(const FileRecord& r, long recno){
    bool debug = false;
    // if(debug) cout << "\t[Debug][import_record] Entering\n";

    string value;
    
    for(int i = 0; i < r.total_row(); ++i){
        value = r.get_row(i);
        // if(debug) cout << "\t[Debug][import_record] get_row = " << value << endl;
        
        _second_index[i][value].push_back(recno);

        // if(debug) cout << "\t\t[Debug][import_record] after _second_index\n";

    }   

    // if(debug) cout << "\t[Debug][import_record] Leaving\n";
}

void Table::import_fields(const vectorstr& field_names){
    bool debug = false;
    _field_names = field_names;
    _second_index.reserve(_field_names.size());
    // if(debug) cout << "[Debug] [Table::import_fields] fields.size(): " << _field_names.size() << endl;
     for(int i = 0; i < field_names.size();++i){
        _second_index.push_back(mmap_sl());
        _field_map.insert(field_names[i], i);
    }
   
}

    //Initializer
void Table::init_dic(){
    if(init_already) return;

    init_already = true;
    token_dic.insert(">", 0);
    token_dic.insert("<", 0);
    token_dic.insert(">=", 0);
    token_dic.insert("<=", 0);
    token_dic.insert("=", 0);
    token_dic.insert("(", 1);
    token_dic.insert(")", 2);
    token_dic.insert("or", 3);
    token_dic.insert("and", 3);

    arr[0] = &Table::push_Relation;
    arr[1] = &Table::push_LP;
    arr[2] = &Table::push_RP;
    arr[3] = &Table::push_Logic;
    
}


    //Operator
vector<long> Table::rpn(queuetkptr postfix) throw(const Error_msg){
    bool debug = false;
    Stack<Token*> stack_tk;
    // Stack<vectorlong> stack_vlong;

    while(!postfix.empty()){
        Token* tkptr = postfix.pop();
        
        int type = tkptr->type();
        

        try{
            if(type == SQL_TOKEN_OPERATOR){

                if(debug) cout << "operator\n";
                
                
                if(stack_tk.empty()) throw(Error_msg("[Token == OPERATOR] : Set1 Argument Mismatch"));
                auto v1 = stack_tk.pop();   // might be value/result set1
                if(stack_tk.empty()) throw(Error_msg("[Token == OPERATOR] : Set2 Argument Mismatch"));
                auto v2 = stack_tk.pop();   //  might be field/result set2
                stack_tk.push(new Result(tkptr->evaluate(v1,v2, &_second_index[_field_map[v2->token_str()]])));
                if(v1->type() == SQL_TOKEN_RESULT){
                    delete v1;
                    delete v2;
                }
            }
            else if(type == SQL_TOKEN_TKSTR){
                // if(debug) cout << "Tkstr\n";
                stack_tk.push(tkptr);
            }
            else{
                
                throw(Error_msg("Incorrect Token Type: " + to_string(type)));
                assert(false && "[Table::select() required]: [Table::rpn()] : Incorrect Token Type");
            }  

        }
        catch (Error_msg e){
            while (!stack_tk.empty())
            {
                auto v1 = stack_tk.pop();
                if(v1->type() == SQL_TOKEN_RESULT){
                    delete v1;
                }
            }
            e._msg.insert(0, "[Table::rpn()] : ");
            throw(e);
        }
        
    }

    if(stack_tk.size() != 1){
        while (!stack_tk.empty())
        {
            auto v1 = stack_tk.pop();
            if(v1->type() == SQL_TOKEN_RESULT){
                delete v1;
            }
        }
        throw(Error_msg("[Table::rpn()] : multiple tokens in remaining stack : Expecting 1 token = result set"));
    }

    auto result_tk =  stack_tk.pop();
    auto vlong = result_tk->result_set();

    delete result_tk;

    return vlong;
}

Sy_wrapper Table::shunting_yard(const vector<Token*>& infix){
    bool debug = false;
    Queue<Token*> output;
    Stack<Token*> stack;
    int type;
    // int steps = 0;
    Error_msg m("");
    bool error = false;
    
    for(auto tk : infix){
        type = tk->type();
        
        if(type == SQL_TOKEN_TKSTR){
            output.push(tk);
                //steps +=2;
        }
        else if(type == SQL_TOKEN_OPERATOR){
            while (!stack.empty() && stack.top()->type() != SQL_TOKEN_LP &&
                    (stack.top()->priority() > tk->priority() //if o2 (stack.top) prior > o1 (infix_tk) OR
                    || (stack.top()->priority() == tk->priority()  && stack.top()->left_associative()) // o1 == o2 priority && o2 is left_associative
                    )
             ){//pop stack and push it into output que
                output.push(stack.pop());
                //steps +=3;
            }
            stack.push(tk);
        }
        else if (type == SQL_TOKEN_LP){
            stack.push(tk);
                //steps +=4;
        }else if (type == SQL_TOKEN_RP){
            Token* e;
            do
            {   
                // assert(!stack.empty() && "[Table::shunting_yard] : [vector<Token*> infix] : Parenthesis mismatch");
                if(stack.empty()) break;

                e = stack.pop();
                //steps += 4;
            }while(e->type() != SQL_TOKEN_LP && output.push(e));

            if(e->type() != SQL_TOKEN_LP){
                m = Error_msg("[Table::shunting_yard] : [vector<Token*> infix] : Parenthesis mismatch");
                error = true;
            }
            // assert(tk->type() == SQL_TOKEN_RP);
            //steps +=5;
        }
        else {
            assert(false && "[Table::shunting_yard] : [vector<Token*> infix] : infix containing token, token's type are not SQL_TYPE");
        }
    }


    while(!stack.empty()){
        auto tkptr = stack.pop();
        if(tkptr->type() == SQL_TOKEN_LP){
                m = Error_msg("[Table::shunting_yard] : [vector<Token*> infix] : Parenthesis mismatch");
                error = true;
        }
        else output.push(tkptr);
    }

    if(debug){
        cout << "[Debug][SYard] output_que: "; print_queue(output);cout <<endl;
    }

    // cout << "\nTotal //steps: " << //steps << endl;

    return Sy_wrapper(output, m, error);
}

Table Table::vector_to_table(const vectorstr& new_fields, vector<long> recno){
    Table table(_table_name + "_" + to_string(Table::_serial_no), new_fields);
    ++_serial_no;
    for(auto x : recno)
        table.insert_into(re_index(new_fields,x));
    
    return table;
}

vectorstr Table::re_index(const vectorstr& new_fields, const long& recno){
    long byte = 0;
    FileRecord r;
    fstream f;
    open_fileR(f, (_table_name + ".bin").c_str());
    vectorstr vstr;
    byte = r.read(f, recno);
    if(byte != 0){
        for(auto it = new_fields.begin(); it != new_fields.end(); ++it){
            vstr += string(r.get_row(_field_map[*it]));
        }
    }
    return vstr;
}


void Table::debug_print(){
    cout << "---------------\nfield map:\n\n" << _field_map << endl;

    cout << "---------------\n2nd-index MMap:\n\n";
    for(vectorstr::iterator it = _field_names.begin(); it != _field_names.end(); ++it){
        cout << *it << ":\n" << _second_index[_field_map[*it]] << '\n';
    }

    cout << "---------------\n"
        << "Clustered table:\n\n" << *this << '\n';


}
#endif
