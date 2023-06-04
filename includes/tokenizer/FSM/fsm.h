#ifndef FSM_H
#define FSM_H

#include <string>
#include <cassert>

#include "../constants.h"

/*          =====Class Fsm(Finite State Machine)=====
    -->>>Fsm is a class for storing the state_table for our string tokenizer, so the fsm's input is always a char
    The table is a transitional function by giving the [Current State] and the [Input],
    and the function gives back the destinational state.

    -->>>When the next_state is -1, the Fsm terminate, and return 
    from the start to the position at last success state
    -->>>The Zero Column were used as indicating current state is a success/fail state.
*/
//Fsm[Current_State][Input];
//Fsm[Rows][Columns];
class Fsm{
public:
    Fsm();

    int* operator [](int i){
        assert(i < MAX_ROWS);
        return _table[i];
    }

    bool is_made(){return _made;}
    void set_made(){_made = true;}
    void unset_made(){_made = false;}

//=============================================
//Table Making functions

    //Fill all cells of the array with -1
    void init_table();
    //Mark this state (row) with a 1 (success)
    void mark_success(int state);
    void mark_success(const int state[], int size);

    //Mark this state (row) with a 0 (fail)
    void mark_fail(int state);
    void mark_fail(const int state[], int size);

    //true if state is a success state
    bool is_success(int state);

    //Mark a range of cells in the array. 
    void mark_cells(int row, int from, int to, int state);

    //Mark columns represented by the string columns[] for this row
    void mark_cells(int row, const char columns[], int state);

    //Mark this row and column
    void mark_cell(int row, int column, int state);

    //Post: return a formtted std::string of the table;
    std::string to_string_col(int column);
    std::string to_string_row(int row);


private:
    bool _made;
    int _table[MAX_ROWS][MAX_COLUMNS];
};

#endif