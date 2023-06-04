#ifndef LL_NODE_MYQUEUE_H
#define LL_NODE_MYQUEUE_H

#include <iostream>
#include <string>
#include "../node/node.h"
#include "../linked_list_functions/linked_list_functionsTemplated.h"


template <typename T>
class Queue
{
public:
    class Iterator;

    Queue();

    Queue(const Queue<T>& copyMe);
    ~Queue();
    Queue& operator=(const Queue<T>& RHS);

    bool empty();
    //Preconditon: Queue is NOT emtpy
    //Post: return the first item in the queue
    T front();
    //Preconditon: Queue is NOT emtpy
    //Post: return the last item in the queue
    T back();

    bool push(T item);
    //Preconditon: Queue is NOT emtpy
    //Post: pop and return first item in the queue
    T pop();

    void clear(){
        while(!empty()) pop();
    }
    
    Iterator begin() const{return Iterator(_front);}                                     //Iterator to the head node
    Iterator end() const{return Iterator(nullptr);}                                       //Iterator to NULL
    void print_pointers();
    int size() const { return _size; }

    template<typename TT>
    friend std::ostream& operator << (std::ostream& outs, const Queue<TT>& printMe);

    template<typename TT>
    friend std::string to_string(const Queue<TT>& printMe);

    bool operator== (const Queue<T>& RHS){ return _front == RHS._front && _rear == RHS._rear && _size == RHS._size;} 
    bool operator!= (const Queue<T>& RHS){ return _front != RHS._front && _rear != RHS._rear && _size != RHS._size;} 
private:
    node<T>* _front;
    node<T>* _rear;
    int _size;
};


#include "MyQueue.cpp"
#include "MyQueue_Iterator.h"    
#endif