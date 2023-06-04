#ifndef LL_NODE_MYQUEUE_CPP
#define LL_NODE_MYQUEUE_CPP


#include "MyQueue.h"
#include "MyQueue_Iterator.h"
#include <sstream>
#include <cassert>

template <class T>
Queue<T>::Queue(): _front(nullptr), _rear(nullptr), _size(0)
{}

template <class T>
Queue<T>::Queue(const Queue<T>& copyMe)
{   
    
    _size  = copyMe._size;
    if(_size != 0)
        _front = _copy_list(_rear, copyMe._front);
    else{
        _front = nullptr;
        _rear = nullptr;
    }   
}

template <class T>
Queue<T>::~Queue()
{
    _clear_list(_front);
    _size = 0; _rear = nullptr;
}

template <class T>
Queue<T>& Queue<T>::operator=(const Queue<T>& RHS)
{  
    if(&RHS == this){
        return *this;    
    }else _clear_list(_front);
    
    _size = RHS._size;
    _front = _copy_list(_rear, RHS._front);

    return *this;
}

template <class T>
bool Queue<T>::empty()
{
    return _size == 0;
}

template <class T>
T Queue<T>::front(){  
    assert(!empty() && "[Error] [Queue<T>] [front()]: queue is empty");
    return _front->_item;  
}
template <class T>
T Queue<T>::back(){  
    assert(!empty() && "[Error] [Queue<T>] [back()]: queue is empty");
    return _rear->_item;   
}

template <class T>
bool Queue<T>::push(T item){
    bool debug = true;
    // std::cout << "[Debug] [Queue] [Push] : Enter\n";
    _size++;
    _rear = _insert_after(_front, _rear, item);
    
    // std::cout << "[Debug] [Queue] [Push] : Leave\n";

    return true;
}

template <class T>
T Queue<T>::pop(){
    assert(_size != 0 && "[Error] [Queue<T>] [pop()]: Popping an Emty");
    _size--;
    if(_front == _rear){
        _rear = nullptr;
    }
    return _delete_node(_front, _front);
}

template <class T> 
void Queue<T>::print_pointers(){
   
}

template<typename TT>
std::ostream& operator << (std::ostream& outs, const Queue<TT>& printMe){
    outs << to_string(printMe);
    return outs;
}

template<typename TT>
std::string to_string(const Queue<TT>& printMe){
    typename Queue<TT>::Iterator it1 = printMe.begin();
    using namespace std;
    stringstream ss;
    ss << "Head<->";
    while(it1 != nullptr){
        ss << "["<< *it1 <<"]<->";
        it1++;
    }
    ss << "|||";

    return ss.str();
}
#endif