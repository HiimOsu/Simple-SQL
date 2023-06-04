#ifndef LL_NODE_MYSTACK_ITERATOR_H
#define LL_NODE_MYSTACK_ITERATOR_H

#include "MyStack.h"
#include <cassert>

template <class T>
class Stack<T>::Iterator
{
    public:
        friend class Stack<T>;              

        //give access to list to access _ptr
        //default ctor
        Iterator()
        {
            _ptr = NULL;
        }            
        //Point Iterator to where
        //  p is pointing to
        Iterator(node<T>* p){
            _ptr = p;
        }      
        //dereference operator
        T operator *(){
            assert(_ptr != nullptr);
            return _ptr->_item;
        }    
        //true if _ptr is NULL
        bool is_null(){
            return _ptr == nullptr;
        }        
        //true if left != right    
        friend bool operator !=(const Iterator& left,
                                const Iterator& right)  
        {
            return left._ptr != right._ptr;
        }
        //true if left == right
        friend bool operator ==(const Iterator& left,
                                const Iterator& right)  
        {
            return left._ptr == right._ptr;
        }

        //member operator:++it
        // or ++it = new_value
        Iterator& operator++(){  
            assert(_ptr!= NULL);
            _ptr = _ptr->_next;
            return *this;            
        }

        //friend operator: it++
        friend Iterator operator++(Iterator& it,
                                   int unused){        
            assert(it._ptr != NULL);
            Iterator temp(it);
            it._ptr = it._ptr->_next;
            return temp;
        }

    private:
        node<T>* _ptr;    //pointer being encapsulated
};

#endif