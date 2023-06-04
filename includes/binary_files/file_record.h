#ifndef FILE_RECORD_H
#define FILE_RECORD_H

#include <iostream>
#include <iomanip>
#include <string.h>
#include <vector>
#include <cassert>

#include "utilities.h"

// using namespace std;



//when you construct a FileRecord, it's either empty or it
//  contains some words
class FileRecord{
    
public:

    //base constructor, called when reading record from db
    FileRecord(){
        init_2d();
        recno = -1;
        _total_row = 0;
        //['\0'|  | G | a | r | b | a   |  g  |  e |    ]
    }
    FileRecord(char str[]);
    FileRecord(std::string s);
    FileRecord(std::vector<std::string> v);


    //Precondition: outs is opened
    //Postcondition: write _record (a block ) at the end of the file
    //              return record number of regarding record 
    long write(std::fstream& outs);              

    //Postcondition: load each row of data into _record, 
    //              stop loading if one of the row starting with a NULL char;
    long read(std::fstream& ins, long recno);    //returns the number of bytes
                                            //      read = MAX, or zero if
                                            //      read passed the end of file

    char* get_row(int line){
        assert(line < MAX);
        return _record[line];
    }
    const char* get_row(int line) const{
        assert(line < MAX);
        
        return _record[line];
    }

    int total_row() const {return _total_row;}
    friend std::ostream& operator<<(std::ostream& outs, const FileRecord& r);

    static const int MAX = 100;
// private:
    long recno;
    int _total_row;
    char _record[MAX][MAX+1]; //NULL char
    void init_2d();
    void init_1d(char* arr);
};

#endif