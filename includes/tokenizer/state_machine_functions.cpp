#include "state_machine_functions.h"
#include <sstream>
#include <iomanip>
#include <cassert>

//Fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS]){
    for(int i = 0; i < MAX_ROWS; i++)
        for(int j = 0; j < MAX_COLUMNS; j++)
            _table[i][j] = -1;
}

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state){
    assert(state  < MAX_ROWS && state >= 0);
    _table[state][0] = 1;
}

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state){
    assert(state  < MAX_ROWS && state >= 0);
    _table[state][0] = 0;
}

//true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state){
    assert(state  < MAX_ROWS && state >= 0);
    return _table[state][0];
}

//Mark a range of cells in the array. 
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state){
    assert(state < MAX_ROWS && state >= -1);
    assert(row  < MAX_ROWS && row >= 0);
    assert(from >= 0 && from < MAX_COLUMNS);
    assert(to >= 0 && to < MAX_COLUMNS);

    for(int i = from; i <= to; i++)
        _table[row][i] = state;
}

//Mark columns represented by the string columns[] for this row
void mark_cells(int row,int _table[][MAX_COLUMNS], const char columns[], int state){
    assert(row  < MAX_ROWS && row >= 0);
    assert(state >= -1 && state < MAX_ROWS);
    int i = 0;
    
    while(columns[i] != '\0'){
        _table[row][static_cast<int>(columns[i++])] = state;
    }
}

//Mark this row and column
void mark_cell(int row, int _table[][MAX_COLUMNS], int column, int state){
    assert(row < MAX_ROWS && row >= 0);
    assert(state >= -1 && state < MAX_ROWS);
    assert(column < MAX_COLUMNS && column >= 0);

    _table[row][column] = state;
}

void print_table(int _table[][MAX_COLUMNS]){

}

void show_string(char s[], int _pos){
    
}