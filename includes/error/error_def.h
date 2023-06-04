#ifndef ERROR_DEF_H 
#define ERROR_DEF_H

enum SQL_LOG2{
    MISSING_TABLE_NAME,
    REPEATED_TABLE_NAME,
    UNKNOWN_TABLE_NAME,
    MISSING_FIELDS,
    MISSING_FIELDS_SELECT,
    MISSING_COLS,
    MISSING_VALUES,
    MISSING_CONDITIONS,
    MISSING_WHERE,
    INVALID_COMMAND
};
enum SQL_error_t{
    ERROR_T_DEFAULT,

    TABLE_EMPTY_INSERT,
    TABLE_EMPTY_SELECT,
    TABLE_INSERT_EXCESSV,
    TABLE_INSERT_MISSINGV,
    TABLE_SELECT_PMISMATCH,
};
#endif