#include <iostream>
#include <iomanip>

#include "includes\sql\sql.h"

using namespace std;

const int WINDOW_H = 100;
const int BUFFER_SIZE = MAX_BUFFER;
enum cmd_t{
  CMD_START,
  CMD_LEAVE,
  CMD_HELP,  
  CMD_CLEAR,
  CMD_RUN,
  CMD_SHOW,
  CMD_HIDE,
};
map_sl command_table;

void init_table(){
    command_table.insert("leave", CMD_LEAVE);
    command_table.insert("help", CMD_HELP);
    command_table.insert("clear", CMD_CLEAR);
    command_table.insert("start", CMD_START);
    command_table.insert("hide", CMD_HIDE);
    command_table.insert("show", CMD_SHOW);
}

void print_myCMD(ostream& outs);
void sql_app(istream& ins, ostream& outs = cout);
void sql_app_cmd(long cmd, ostream& outs);

class classA{};
class classB : public classA
{

};

class classC{};

int main(int argv, char** argc) {
    classA* arr[10];
    arr[0] = new classA();
    arr[1] = new classB();
    int i = 0;
    vector<classA *> v;
    switch (i)
    {
    case 1:
        {
            classA* temp = new classA();   
        break;
        }
    default:
        break;
    }
    cout << "\n\nEntering Main()\n\n"
         << endl;
    fstream fin;
    fin.open("../../new_bash.txt", ios::in);

    if(fin.fail()) 
        cout << "file open fail\n";
    else
        sql_app(fin);

    cout << "\nLeaving Main()\n" 
    << "\n\n=====================" << endl;
    return 0;
}

void sql_app_cmd(long cmd, ostream& outs){
    switch (cmd)
    {
    case CMD_HELP:
        print_myCMD(outs);
        break;
    case CMD_CLEAR:
        for(int i = 0; i < WINDOW_H; ++i) outs << "\n";
        break;
    case CMD_START: 
        outs << "Welcome to mySQL\n" 
            << "\t\tIn this application, you can create, store, and select your own database by various mySQL command\n"
            << "Table is initially hidden, to show table type \"Show\""
            << "\tHere is the command List:\n";
            print_myCMD(outs);
        command_table.erase("start");
        break;
    case CMD_HIDE:
        outs << "Table is now hidden\n";
        break;
    case CMD_SHOW:
        outs << "Table is no longer hidden\n";
    default:
        break;
    }
}

void print_myCMD(ostream& outs){
    outs << "*\n*\n*\n"
    << "<Help menu>\n"
    << "\t<CLEAR> : type \"show\" to show and print the table.\n"
    << "\t<CLEAR> : type \"hide\" to hide the table.\n"
    << "\t<CLEAR> : type \"clear\" to clear the screen.\n"
    << "\t<HELP> : type \"help\" to show the command menu again.\n"
    << "\t<LEAVE> : type \"leave\" to exit.\n"
    << "\t<CREATE | MAKE> : {  <create | make> table <TABLE_NAME> fields <FIELD_NAME> [, <FIELD_NAME>...]  }\n"
    << "\t<INSERT> : { insert <INTO> <TABLE_NAME> values <VALUE> [, <VALUE>...]      }\n"
    << "\t<SELECT> : {  select <* | FIELD_NAME> [, ,FIELD_NAME>...]\n"
    << "\t\t\tfrom <TABLE_NAME>\n"
    << "\t\t\t    where <FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE>\n"
    << "\t\t\t        [<LOGICAL_OPERATOR> <FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE>...]\n"
    << "\t\t\t}\n"
	<< "\t<VALUE>  : A string of alphanumeric characters, or a string of alphanumeric\n"
    << "\t\tcharacters and spaces enclosed by double quotation marks:\n"
 	<< "\t\t\"Jean Luise\", Finch, 1923\n"
    << "\t<RELATIONAL OPERATOR> : [ = | > | < | >= | <= ]\n"
    << "\t<LOGICAL OPERATOR>    : [and | or]\n";
}

void sql_app(istream& ins, ostream& outs){
    init_table();
    SQL sql;
    Table selected;
    string buffer = "start";
    long cmd_type = CMD_START;
    char buff[BUFFER_SIZE + 1];
    bool print_table = false;
    char a;
    sql_app_cmd(cmd_type, outs);
    outs << "\n>";
    do
    {
        // cin.get(a) ;
        ins.getline(buff, BUFFER_SIZE, '\n');
        buffer = buff;
        outs << "\ncommand > [ " << buffer << " ]\n"; 

        if(command_table.contains(to_lower(buffer))){
            cmd_type = command_table.get(buffer);
        }
        else
        {   
            selected = sql.command(buffer);
            if(print_table){                
                cout << endl << selected << endl;
                cout << "selected #no: " << sql.select_recnos() << "\n\n";
            }
            cmd_type = CMD_RUN;
        }


        if(cmd_type == CMD_HIDE) print_table = false;
        else if(cmd_type == CMD_SHOW) print_table = true;
        else if(cmd_type != CMD_RUN) sql_app_cmd(cmd_type, outs);

        outs << "\n>";
    } while (cmd_type !=  CMD_LEAVE);
    
    outs << "\nGood Bye!\n";
}   