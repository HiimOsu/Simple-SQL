#include <iomanip>
#include "file_record.h"

//Operator

FileRecord::FileRecord(char str[]):FileRecord(){
    _total_row = 1;
    strncpy(_record[0], str, MAX);
}

FileRecord::FileRecord(std::string s):FileRecord(){
    _total_row = 1;
    strncpy(_record[0], s.c_str(), MAX);
}

FileRecord::FileRecord(std::vector<std::string> v){
    _total_row = v.size();
    assert(_total_row < MAX && "[Constructor] FileRecord(std::vector<std::string> v): Too many fields");
    std::string s;
    int i = 0;
    std::vector<std::string>::iterator it = v.begin();
    while(it != v.end() && i < MAX)
    {
        strncpy(_record[i++], it++->c_str(), MAX);
    }

    if(_total_row < MAX - 1)
        _record[_total_row][0] = '\0';
    else _total_row = MAX;
}


long FileRecord::write(std::fstream &outs){
    //r.write(f); //take the Record r and write it into file f
    //  and return the record number of this Record

    //write to the end of the file.
    outs.seekg(0, outs.end);


    long pos = outs.tellp();    //retrieves the current position of the
                                //      file pointer
    //pay attention to this:
   
    // cout << "[DEBUG] [Write] sizeof(_record) = " << sizeof(_record) << "\n";
    // for(int i = 0; i < MAX; ++i){
    //     outs.write(_record[i], sizeof(_record[i]));
    //     // std::cout << _record[i] << std::endl;
    // }
    outs.write(_record[0], sizeof(_record));
    return pos/sizeof(_record);  //record number
}

long FileRecord::read(std::fstream &ins, long recno){

    long pos= recno * sizeof(_record);
    ins.seekg(pos, std::ios_base::beg);

    //ins.read(&_record[0], sizeof(_record))
    int i = 0;
    do{
        ins.read(_record[i], sizeof(_record[i]));
        
    } while(i < MAX && _record[i++][0] != '\0');

    if(_total_row < MAX - 1)
        _total_row = i - 1;
    else _total_row = MAX;
    
    return ins.gcount();
}

std::ostream& operator<<(std::ostream& outs,
                    const FileRecord& r){
    for(int i = 0; i < r._total_row; ++i){
        outs << std::right <<  std::setw(15) << r._record[i];
    }
    return outs;
}



//Private Member
void FileRecord::init_2d()
{
        for(int i = 0; i < MAX; ++i)
            init_1d(_record[i]);
}

void FileRecord::init_1d(char* arr){
    for(int i = 0; i < MAX + 1; ++i)
        arr[i] = '\0';
}