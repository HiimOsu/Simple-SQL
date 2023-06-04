#ifndef LL_NODE_MYSTACK_CPP
#define LL_NODE_MYSTACK_CPP

//from cstd
#include <sstream>
#include <cassert>

#include "MyStack.h"
#include "MyStack_Iterator.h"

template<typename T>
Stack<T>::Stack(): _top(nullptr), _size(0)
{}

template<typename T>
Stack<T>::Stack(const Stack<T>& copyMe){
    _size = copyMe._size;
    
    //Get head/bottom
    node<T>* walker = copyMe._top;
    if(walker != nullptr)
        while(_previous_node(walker) != nullptr){ walker = _previous_node(walker);};
    _copy_list(_top, walker);
}
template<typename T>
Stack<T>::~Stack()
{
    node<T>* walker = _top;
    if(walker != nullptr)
        while(_previous_node(walker) != nullptr){ walker = _previous_node(walker);};
    _clear_list(walker);

}
template<typename T>
Stack<T>& Stack<T>::operator=(const Stack<T>& RHS)
{
    if(*this == RHS){
        return *this;
    }

    //getting this_head
    node<T>* _this_walker = _top;
    if(_this_walker != nullptr)
        while(_previous_node(_this_walker) != nullptr){ _this_walker = _previous_node(_this_walker);};
    _clear_list(_this_walker);

    //getting RHS.head
    node<T>* walker = RHS._top;
    if(walker != nullptr)
        while(_previous_node(walker) != nullptr){ walker = _previous_node(walker);};    _size = RHS._size;
    
    _copy_list(_top ,walker);

    return *this;
}

template<typename T>
T Stack<T>::top()
{
    assert(!empty());
    return _top->_item;
}
template<typename T>
bool Stack<T>::empty() const
{
    return _size == 0;
}

template<typename T>
void Stack<T>::push(T item)
{
    _size++;
    _top = _insert_after(_top,_top, item);
}
template<typename T>
T Stack<T>::pop()
{
    assert(_top != nullptr);
    
    _size--;
    node<T>* pop_me = _top;
    _top = _previous_node(pop_me);
    return _delete_node(_top,pop_me);
}


template<typename TT>
typename Stack<TT>::Iterator Stack<TT>::begin() const
{
    node<TT>* head = _top;
    if(head != nullptr)
        while(_previous_node(head) != nullptr){ head = _previous_node(head);};
    return head;   
}    
template<typename TT>
typename Stack<TT>::Iterator Stack<TT>::end() const
{
    return nullptr;
}
template<typename TT>
std::ostream& operator<<(std::ostream& outs, const Stack<TT>& printMe)
{
    outs << to_string(printMe);
    return outs;
}

template<typename TT>
std::string to_string(const Stack<TT>& printMe)
{
    using namespace std;
    stringstream ss;
    typename Stack<TT>::Iterator it = printMe.begin();

    ss << "[BOTTOM] || ";
    while(it != nullptr){
        ss << "["<< *it++ << "] || ";
        
    }
    ss << "[TOP]";
    return ss.str();
}



template<typename T>
int Stack<T>::size() const 
{
    return _size; 
}

#endif