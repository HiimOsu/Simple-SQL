#ifndef LINKED_LIST_FUNCTIONS_CPP
#define LINKED_LIST_FUNCTIONS_CPP

#include "linked_list_functionsTemplated.h"
#include <iostream>
#include <cassert>

//Linked List General Functions:
template <typename T>
void _print_list(node<T>* head)
{
    std::cout << "Head";
    while(head != nullptr){
        std::cout << "<->[" << to_string(head) << "]<->";
        head = head->_next;
    }
    std::cout << "Tail";
}

//recursive fun! :)
template <typename T>
void _print_list_backwards(node<T> *tail){
   std::cout << "Tail";
    while(tail != nullptr){
        std::cout << " <->[" << to_string(tail) << "]<->";
        tail = tail->_prev;
    } 
    std::cout << "Head";
}



//return ptr to key or NULL
template <typename T>
node<T>* _search_list(node<T>* head,
                            T key)
{
    while(head != nullptr && head->_item != key){
            head = head->_next;
    }

    return head;
}


template <typename T>
node<T>* _insert_head(node<T> *&head,
                            T insert_this)
{
    head = new node<T>(insert_this, head);
    if(head->_next != nullptr)
        head->_next->_prev = head;
 
    return head;
}                            

//insert after ptr: insert head if marker null
template <typename T>
node<T>* _insert_after(node<T>*& head,
                                node<T> *after_this,
                                T insert_this)
{
    if(after_this == nullptr){
        return _insert_head(head, insert_this);
    }
    
    //linking newNode,  *._prev = [after_this] and *._next = [after_this._next]
    after_this = new node<T>(insert_this, after_this->_next, after_this);
    
    //Linking back after_this, *._next = newNode;
    after_this->_prev->_next = after_this;  
    
    //Linking back after_this->_ntex, *._prev = newNode;
    if(after_this->_next != nullptr){   
        after_this->_next->_prev = after_this;
    }

    return after_this;
}
//insert before ptr: insert head if marker null
template <typename T>
node<T>* _insert_before(node<T>*& head,
                                node<T>* before_this,
                                T insert_this)
{
    if(before_this == nullptr){
        return _insert_head(head, insert_this);
    }

    before_this = new node<T>(insert_this, before_this, before_this->_prev);
    
    //Linking back before_this, *._prev = newNode;
    before_this->_next->_prev = before_this;

    //Linking back before_this->prev, *_next = newNode;
    if(before_this->_prev == nullptr){
        head = before_this;
    }else {
        before_this->_prev->_next = before_this;
    }
    // std::cout << "DEBUG FROM _insert_before: head" << to_string(head) << "\nNewNode: " << to_string(before_this) << std::endl;

    return before_this;
}

//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* prev_to_this){
    assert(prev_to_this != nullptr && "[ERROR] _previous_node(): prev_to_this is NULL");
    return prev_to_this->_prev;
}

//delete, return item
template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this){
    
    //link the prev
    if(delete_this->_prev != nullptr){
        delete_this->_prev->_next = delete_this->_next;
    }

    //link the next
    if(delete_this->_next != nullptr){
        delete_this->_next->_prev = delete_this->_prev;
    }

    //update head
    if(delete_this == head){
        head = delete_this->_next;
    }

    T item = delete_this->_item;
    delete delete_this;

    return item;
}

//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head)
{   
    if(head == nullptr) return nullptr;

    ////Get the Tail
    // while(head->_next != nullptr){
    //     head = head->_next;
    // }

    // node<T>* newHead = nullptr;
    // while(head != nullptr){
    //     _insert_head(newHead, head->_item);
    //     head = head->_prev;
    // }

    node<T>* newHead = nullptr, *after_this = newHead;
    while (head != nullptr)
    {
        after_this = _insert_after(newHead,after_this, head->_item);
        head = head->_next;
    }
    

    return newHead;
}
//duplicate the list, return pointer to last node in dest... 
//     use this function in your queue big three 
template <typename T>
node<T> *_copy_list(node<T> *&dest, node<T> *src){
    if(src == nullptr) return nullptr;

    node<T>* newHead = nullptr, *newTail = nullptr;
    while (src != nullptr)
    {
        newTail = _insert_after(newHead, newTail, src->_item);
        src = src->_next;
    }
    dest = newTail;

    return newHead;
}

//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head){
    node<T>* del_me = head;
    while(head){
        head = head->_next;
        delete del_me;
        del_me = head;
    }
}

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos){
    assert(pos >= 0 && "LinkedList functions[Error] _at(): Position cannot be negtaive");
    for(int i = 0;
        (pos != i && head != nullptr);
        i++, head = head->_next
    );

    assert(head != nullptr && "LinkedList functions[Error] _at(): node position at (pos) not found");
    return head->_item;
}

#endif