#ifndef MY_UTIL_H
#define MY_UTIL_H

#include <vector>

#include "../tokenizer/token.h"
#include "../QueueAndStack/MyStack/MyStack.h"
#include "../QueueAndStack/MyQueue/MyQueue.h"

using namespace std;

void print_queue(Queue<Token*> q);
void print_stack(Stack<Token*> st);
void print_vector(vector<Token*> v);
string to_lower(string target);
string to_upper(string target);

#endif