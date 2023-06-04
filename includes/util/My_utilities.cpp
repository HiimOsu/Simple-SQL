#ifndef MY_UTIL_CPP
#define MY_UTIL_CPP

#include "My_utilities.h"
void print_queue(Queue<Token*> q){
    while(!q.empty()){
        cout << q.pop()->token_str() <<" ";
    }
}
void print_stack(Stack<Token*> st){
    while(!st.empty()){
        cout << st.pop()->token_str() <<" ";
    }
}
void print_vector(vector<Token*> v){
    for(auto x : v){
        cout << x->token_str() <<" ";
    }
}
string to_lower(string target){
    
    for(int i = 0; i < target.size(); ++i){
        target[i] = target[i] | 32;
    }  
    
    return target;
}


string to_upper(string target){

    for(int i = 0; i < target.size(); ++i){
        target[i] = target[i] &  95;
    }  
    return target;
}


#endif