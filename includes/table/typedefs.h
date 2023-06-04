#ifndef TABLE_TYPEDEFS_H
#define TABLE_TYPEDEFS_H

#include "../bplustree/multimap.h"
#include "../bplustree/map.h"


#include "../tokenizer/token.h"
#include "../QueueAndStack/MyStack/MyStack.h"
#include "../QueueAndStack/MyQueue/MyQueue.h"

#include <string>
#include <vector>

using namespace std;
typedef Map<string, string> map_ss;
typedef MMap<string, string> mmap_ss;

typedef Map<string, long> map_sl;
typedef MMap<string, long> mmap_sl;


typedef Map<long, string>  map_ls;
typedef MMap<long, string>  mmap_ls;

typedef vector<string> vectorstr;
typedef vector<long> vectorlong;
typedef Queue<string> queuestr;


typedef Queue<Token*> queuetkptr;
typedef vector<Token*> vectortkptr;
#endif // TYPEDEFS_H