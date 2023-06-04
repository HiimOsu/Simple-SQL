#ifndef TABLE_TOKEN_H
#define TABLE_TOKEN_H
#include <iostream>
#include <string>
#include <vector>

#include "../bplustree/multimap.h"
#include "../error/error.h"

#include "constants.h"

using namespace std;
class Token
{
public:
     Token(std::string str = "", int type = TOKEN_UNKNOWN): _token(str), _type(type){}

    friend std::ostream& operator <<(std::ostream& outs, const Token& t);

    //accessor
    int type() const;
    virtual int priority() const;

    //Operator/Result/Relational/Logical
    virtual vector<long> evaluate(Token* tk1, Token* tk2, MMap<std::string,long>* index = nullptr) throw(const Error_msg);
    virtual vector<long> result_set(){
        cout << "calling result_set from TokenBase\n";
        return vector<long>();
    }
    virtual bool left_associative(){return false;}
    std::string token_str() const;
protected:
    std::string _token;
    int _type;
};
#endif