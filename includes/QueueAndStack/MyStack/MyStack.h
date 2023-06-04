#ifndef LL_NODE_MYSTACK_H
#define LL_NODE_MYSTACK_H

#include <iostream>
#include <string>
#include "../node/node.h"
#include "../linked_list_functions/linked_list_functionsTemplated.h"


// template <class T> class Iterator;

template <typename T>
class Stack{
public:
    class Iterator;
    // typedef Iterator<T> Iterator;

    Stack();

    Stack(const Stack<T>& copyMe);
    ~Stack();
    Stack<T>& operator=(const Stack<T>& RHS);

    //return top of the stack as value
    T top();

    void push(T item);
    T pop();

    bool empty() const;
    Iterator begin() const;                   //Iterator to the head node
    Iterator end() const;                     //Iterator to NULL
    int size() const;

    template<typename TT>
    friend std::ostream& operator<<(std::ostream& outs, const Stack<TT>& printMe);

    template<typename TT>
    friend std::string to_string(const Stack<TT>& printMe);

    bool operator== (const Stack<T>& RHS){ return _top == RHS._top && _size == RHS._size;} 
    bool operator!= (const Stack<T>& RHS){ return _top != RHS._top && _size != RHS._size;} 

private:
    node<T>* _top;
    int _size;
};


#include "MyStack.cpp"
#include "MyStack_Iterator.h"
#endif