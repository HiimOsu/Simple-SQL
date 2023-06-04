#include "Fsm.h"
#include <sstream>
#include <iomanip>
Fsm::Fsm(){
    _made = false;
}
//Fill all cells of the array with -1
void Fsm::init_table(){
    for(int i = 0; i < MAX_ROWS; i++)
        for(int j = 0; j < MAX_COLUMNS; j++){
            if(j == 0)
            _table[i][j] = 0;
            else
            _table[i][j] = -1;
        }
}

//Mark this state (row) with a 1 (success)
void Fsm::mark_success(int state){
    assert(state  < MAX_ROWS && state >= 0);
    _table[state][0] = 1;
}
void Fsm::mark_success(const int state[], int size){
    for(int i = 0; i < size; i++){
        _table[state[i]][0] = 1;
    }}
//Mark this state (row) with a 0 (fail)
void Fsm::mark_fail(int state){
    assert(state  < MAX_ROWS && state >= 0);
    _table[state][0] = 0;
}
void Fsm::mark_fail(const int state[], int size){
    for(int i = 0; i < size; i++){
        _table[state[i]][0] = 0;
    }
}
//true if state is a success state
bool Fsm::is_success(int state){
    assert(state  < MAX_ROWS && state >= 0);
    return _table[state][0];
}

//Mark a range of cells in the array. 
void Fsm::mark_cells(int row, int from, int to, int state){
    assert(state < MAX_ROWS && state >= -1);
    assert(row  < MAX_ROWS && row >= 0);
    assert(from >= 0 && from < MAX_COLUMNS);
    assert(to >= 0 && to < MAX_COLUMNS);

    for(int i = from; i <= to; i++)
        _table[row][i] = state;
}

//Mark columns represented by the string columns[] for this row
void Fsm::mark_cells(int row, const char columns[], int state){
    assert(row  < MAX_ROWS && row >= 0);
    assert(state >= -1 && state < MAX_ROWS);
    int i = 0;
    
    while(columns[i] != '\0'){
        _table[row][static_cast<int>(columns[i++])] = state;
    }
}

//Mark this row and column
void Fsm::mark_cell(int row, int column, int state){
    assert(row < MAX_ROWS && row >= 0);
    assert(state >= -1 && state < MAX_ROWS);
    assert(column < MAX_COLUMNS && column >= 0);

    _table[row][column] = state;
}


std::string Fsm::to_string_row(int row){
    using namespace std;
    stringstream ss;

    for(int i = 0; i < MAX_COLUMNS; i++){
        ss << "[" << _table[row][i] << "]";
    }
    return ss.str();
}


std::string Fsm::to_string_col(int col){
    using namespace std;
    stringstream ss;

    for(int i = 0; i < MAX_ROWS; i++){
        ss << "[" << _table[i][col] << "]";
    }
    return ss.str();
}