#ifndef TALBE_H
#define TALBE_H
#include <fstream>
#include <algorithm>

#include "typedefs.h" // <--- using namespace std;

#include "../util/My_utilities.h"
#include "../binary_files/file_record.h"       //this DB use file_record object to write/read data(record) 
#include "../tokenizer/children.h"
#include "../error/error.h"


Error_msg error_msg(SQL_error_t e_t);
struct Sy_wrapper{//a wrapper for shunting yard returning type
    Sy_wrapper(queuetkptr qtkptr, Error_msg error = Error_msg(""), bool error_occur = false): _qtkptr(qtkptr), _error(error), is_error(error_occur){}
    
    
    Queue<Token*> _qtkptr; //we need the queue of token pointers
    Error_msg _error;   //a error message(if error ocurred)
    bool is_error; //a bool to tell if error happened inside Shunting Yard

    //We need this wrapper because the error in SY is not deadly, which might just be missing a Parenthesis ?(\_/)? 
    //ie: "age = 1 and ( major = 3" <<<< this command still run even error exist
    //but throwing a exception will instantly terminate the function and not return the queue tokenptr
    //so the wrapper is needed to return all the necessary component to check the error occur or not and get the queue working in RPN
};


string operator + (string str, const vectorstr& vstr); //just for easy debug

class Table{
public:


    //Make
    Table();//A empty table (a empty table cannot select or insert)
    //Preconditon: table_name is the name of the data base
    //              "table_name" and "table_name_fieldname" files exist in the folder.
    //Postcondition: open the data base file(s...) that exist in the file
    //               throw a char* if files does not exist;
    Table(const string& table_name) throw(const Error_msg);
    //Preconditon: table_name is the name of the data base
    //              fileld_names is a vector containing all the name of the filed.
    //Postcondition: create the data base file(s), if files does not exist.
    //               if the Data base already exist in the folder, table will overwrite all the data in the original file.
    Table(const string& table_name, const vectorstr&field_names);


    //Insert
    void insert_into(const vectorstr& values) throw (const Error_msg);//add new record to the table


    //Select: 
        //To get last select record numbers, must call one of the select function below, else Last_recno is empty;
    Table select_all() throw(const Error_msg); //SQL called this to return table
    Table select_fields(vectorstr fields);
    Table select_where(Queue<Token*> condition);

    Table select(vectorstr fields, string field, string relational, string value);  //this never used in SQL class
    Table select(vectorstr fields, vector<string> infix_string) throw(const Error_msg);    //SQL used this to select Table
    Table select(vectorstr fields, Queue<Token*> condition)throw(const Error_msg);        //This function is called by select(vectorstr, vectorstr);

    vector<long> select_recnos()const{//return all the recno in the last selected table
        return _last_record;
    }

    //return true if this table is not initialized
    bool is_empty(){return _empty;}

    long total_record() const throw(const Error_msg){// return the total number of record in this table
        FileRecord r;
        fstream outs;
        // outs.open(_table_name + ".bin", ios::in | ios::binary);
        if(_empty) throw Error_msg("[Table::total_record number] : table is empty");
        
        open_fileR(outs, (_table_name + ".bin").c_str());

        outs.seekg(0, outs.end);
        long pos = outs.tellp();    //retrieves the current position of the

        return pos / sizeof(r._record);
    }
    
    vector<long> total_recno() const throw(const Error_msg){// return a vectorlong hold all record numbers
        vector<long> vlong;
        int total;
        try
        {
            total = total_record();
        }
        catch (const Error_msg e)
        {
            throw(e);
        }

        for(int i =0 ; i < total; ++i){
            vlong.push_back(i);
        }
        return vlong;
    }


    
    void print_table(ostream& outs) const{ //Print table
        if(_empty){
            outs << "\n\tTable is empty\n";
            return;
        }
        
        outs << "\n"
            << "Table Name: " << _table_name << ", records: " << total_record() << "\n";
        outs << setw(30) << right << "Record" <<  FileRecord(_field_names) << endl;
        fstream f;
        f.open(_table_name + ".bin", ios::in | ios::binary);
        FileRecord r;
        for(int i = 0; i < total_record(); ++i){
            r.read(f, i);
            outs << setw(30) << right << i << r << endl;
        }
        
    }

    friend ostream& operator<<(ostream& outs, const Table& t){//it calls print_table(outs)
        t.print_table(outs);
        return outs;
    }


private:
    //print cluster table + 2nd index table + field name
    void debug_print();

    string      _table_name;
    vectorstr   _field_names;
    map_sl      _field_map;
    vector<mmap_sl>  _second_index; //how to use: _second_index[_field_map["field_name"]]["value"] = vector<record_no>
                                                //eg: _second_index[_field_map["age"]]["20"] += 15;
    //Pre: r contains a record which order is corresponding to the order of_field_names
    // Post: import the record into indecise mmap, and the clustered table



    //put the data in FileRecord r into talbe's indeies mmap;
    void import_record(const FileRecord& r, long recno);  

    //Pre: Table is a blank object (initialized with default constructor or not constructed yet)
    //Post: set vector _field_names, mmap _fieldmap, and vector of mmap _second_index
    void import_fields(const vectorstr& field_names);
    

    //Give a set of recno and a set of field name, return a table based on the new field and those recnos
    Table vector_to_table(const vectorstr& new_fields, vector<long> recno);
    
    //Gives a vector of token pointer(interpretation of the infix commnad), return a post fix queue
    Sy_wrapper shunting_yard(const vectortkptr& infix);
    
    //Gives a postfix token ptr, return a set of recno matching the conditions
    vector<long> rpn(queuetkptr postfix) throw(const Error_msg);

    //Pre: names in new_fields must exist in the original table, and recno must be exist;
    //Post: return the record in vectorstr, which is a re-arranged data of the "#recno" record 
    //ie: re_index({"age", "name"}, 0)  :
    //    #0: {"John", "32"}   => return       {"32", "John"} 
    vectorstr re_index(const vectorstr& new_fields, const long& recno);

    //set the vectorstr "fields" arugment to match this table's fields
    bool set_field(vectorstr& fields);

    //Post: push a token* into infix vector
    //Used in converting vectorstr into vector of token ptr
        void push_Relation(vector<Token*>& infix, string tk){
            // cout << "Pushing Relational: " << tk << endl;
            infix.push_back(new Relational(tk));
        }
        void push_LP(vector<Token*>& infix, string tk){
            infix.push_back(new Token(tk, SQL_TOKEN_LP));
        }
        void push_RP(vector<Token*>& infix, string tk){
            infix.push_back(new Token(tk, SQL_TOKEN_RP));
        }
        void push_Logic(vector<Token*>& infix, string tk){
            infix.push_back(new Logical(tk));
        }

        static void init_dic();
        static void (Table::* arr[4])(vectortkptr&, string);
        
    static Map<string, int> token_dic;
    static bool init_already;
    static long _serial_no;
    vectorlong  _last_record;
    bool        _empty = true;
};


#endif