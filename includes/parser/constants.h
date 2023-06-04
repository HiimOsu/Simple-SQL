#ifndef PARSER_CONSTANTS
#define PARSER_CONSTANTS


enum state_type{
    ST_GET_COMMAND, 
    ST_UNKNOWN_COMMAND,
    ST_MAKE,
    ST_SELECT,
    ST_INSERT,

    ST_SELECT_FIELDS,
    ST_SELECT_FIELDS_COMMA,
    // ST_SELECT_FIELDS_COMMA_STRING,

    ST_SELECT_GET_FROM,
    ST_SELECT_TABLE_NAME,
    ST_SELECT_GET_WHERE,
    ST_SELECT_PRE_CONDITION,
    ST_SELECT_CONDITION,

    ST_INSERT_TABLE_NAME,
    ST_INSERT_GET_VALUES,
    ST_INSERT_PRE_VALUES,
    ST_INSERT_VALUES,
    ST_INSERT_VALUES_COMMA,

    ST_MAKE_TABLE_NAME,
    ST_MAKE_GET_FIELD,
    ST_MAKE_PRE_FIELDS,
    ST_MAKE_FIELDS,
    ST_MAKE_FIELDS_COMMA,
};

enum event_type{
    EV_MAKE = 1,
    EV_CREATE,
    EV_SELECT,
    EV_INSERT,
    EV_INTO,
    EV_TABLE,
    EV_FROM,
    EV_FIELD,
    EV_VALUES,
    EV_COMMA,
    EV_WHERE,
    EV_STRING,
    EV_ASTRISK
};


#endif