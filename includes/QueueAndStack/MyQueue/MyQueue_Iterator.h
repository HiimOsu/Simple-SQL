#ifndef LL_NODE_MYQUEUE_ITERATOR_H
#define LL_NODE_MYQUEUE_ITERATOR_H

#include "MyQueue.h"

template<class T>
class Queue<T>::Iterator{
    public:
        //give access to list to access _ptr  
        //default ctor
        friend class Queue<T>;                               
        Iterator(){
            _ptr = NULL;
        }                          
        
        //Point Iterator to where
        //  p is pointing to
        Iterator(node<T>* p){
            _ptr = p;
        }                            
                                                          
        //dereference operator
        T operator *(){
            return _ptr->_item;
        }    
        
        //true if _ptr is NULL
        bool is_null()
        {
            return _ptr == nullptr;
        }            

        //true if left != right
        friend bool operator !=(const Iterator& left,
                                const Iterator& right)  
        {
            return (left._ptr != right._ptr);
        }

        //true if left == right
        friend bool operator ==(const Iterator& left,
                                const Iterator& right)  
        {
            return (left._ptr == right._ptr);
        }

        //member operator:++it
        // or ++it = new_value
        Iterator& operator++(){                         
            assert(_ptr != nullptr && "[ERROR] [prefix++()] Queue<T>::Iterator == nulllptr");
            _ptr = _ptr->_next;
            return *this;
        }
        //friend operator: it++
        friend Iterator operator++(Iterator& it,
                                   int unused){         
            assert(it._ptr != nullptr && "[ERROR] [postfix++()] Queue<T>::Iterator == nullptr");
            Iterator temp(it._ptr);
            it._ptr =it._ptr->_next;
            return temp;
        }

    private:
        node<T> *_ptr;                          //pointer being encapsulated
};


#endif