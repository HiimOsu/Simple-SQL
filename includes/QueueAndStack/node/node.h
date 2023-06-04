#ifndef node_H
#define node_H

#include <sstream>
#include <iostream>
template <class T>
struct node
{
    T _item;
    node<T> *_next;
    node<T> *_prev;

    node(T item = T(), node<T> *next = nullptr, node<T> *prev = nullptr):
                                                                                _item(item),
                                                                                _next(next),
                                                                                _prev(prev){}
    

    friend std::ostream& operator << (std::ostream& outs, const node<T> printMe){
        outs << to_string(&printMe);
        return outs;
    }
    friend std::string to_string(const node<T>* me){
        if(me == nullptr)
            return "NULL";

        std::stringstream ss;
        ss << me->_item;
        return ss.str();
    }
};

#endif