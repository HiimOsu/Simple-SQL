#ifndef TOKEN_SQLCHILDREN_H
#define TOKEN_SQLCHILDREN_H

#include "token.h"
#include "../util/My_utilities.h"
#include <algorithm>

class Result: public Token{
public:
    Result(vector<long> set): _result_set(set), Token("", SQL_TOKEN_RESULT){}
    virtual vector<long> result_set() override{
        return _result_set;
    }
private:
    vector<long> _result_set;
};

class TokenStr: public Token
{
public:
    TokenStr(std::string str): Token(str, SQL_TOKEN_TKSTR){}
};

class Operator: public Token{
public:
    Operator(std::string str, int type = TOKEN_UNKNOWN): Token(str, type){}
    
    virtual vector<long> evaluate(Token* tk1, Token* tk2, MMap<std::string,long>* index = nullptr)throw(const Error_msg) override{
        return vector<long>();
    }
    virtual bool left_associative(){return false;}
    virtual int priority() const{ return -1;}
};

class Relational: public Operator
{
    
public:
    Relational(std::string str): Operator(str, SQL_TOKEN_OPERATOR){}
    int priority() const{
        return HIGHER;
    }  

    virtual vector<long> evaluate(Token* values, Token* field,  MMap<std::string,long>* _mmap_ptr = nullptr) throw(const Error_msg) override {
        assert(_mmap_ptr);
        vector<long> vlong;
        string value = values->token_str(), relational = token_str();


        // cout << "[Debug] [Relational] [evaluate(Tk* field, Tk* value, MMap* index)] : v = " << value << " relational: "<< relational << endl;

        if(relational == "="){
            vlong = _mmap_ptr->get(value);
        }
        else if(relational == ">"){        
            for(auto it = _mmap_ptr->upper_bound(value); it != _mmap_ptr->end(); ++it){
                // cout << "*it: " << *it << " -->";
                vlong += (*it).value;
            }
        }
        else if(relational == "<"){
            for(auto it = _mmap_ptr->begin(); it != _mmap_ptr->lower_bound(value); ++it){
                vlong += (*it).value;
            }
        }
        else if(relational == ">="){
            for(auto it = _mmap_ptr->lower_bound(value); it != _mmap_ptr->end(); ++it){
                vlong += (*it).value;
            }           
        }
        else if(relational == "<="){
            for(auto it = _mmap_ptr->begin(); it != _mmap_ptr->upper_bound(value); ++it){
                vlong += (*it).value;
            }
        }
        else{
            
            throw(Error_msg("[Relational::evaluate] : Unknown relational operator : " + relational));
            assert(false);    
        }

        // cout << "\t[Debug] [Relational] result: " << vlong << endl;
        return vlong;
    }

};

class Logical: public Operator
{
public:
    Logical(std::string str): Operator(str, SQL_TOKEN_OPERATOR){}
    int priority() const{
        return LOWER;
    }  

    virtual vector<long> evaluate(Token* tk1, Token* tk2, MMap<std::string,long>* _mmap_ptr = nullptr) throw(const Error_msg) override{
        if(tk1->type() != SQL_TOKEN_RESULT || tk2->type() != SQL_TOKEN_RESULT){
            throw(Error_msg("[Logical::evaluate] : Input is not a set"));
            assert(false);
        }
        auto v1 = tk1->result_set(), v2 = tk2->result_set();
        // cout << "[Debug][logical_] v1: " << v1 << endl;
        // cout << "[Debug][logical] v2: " << v2 << endl;

        string tk = to_lower(_token);
        if( tk == "or"){
            for(auto it = v2.begin(); it != v2.end(); ++it){
                v1.push_back(*it);
            }
            std::sort(v1.begin(), v1.end());
            v1.erase(std::unique(v1.begin(), v1.end()), v1.end());

            // cout << "[Debug][logical_or]: Result" << v1 << endl;
            return v1;
        }
        else if(tk == "and"){
            for(auto it = v2.begin(); it != v2.end(); ++it){
                v1.push_back(*it);
            }
            sort(v1.begin(), v1.end());
            vectorlong v3;

            auto it2 = v1.begin();
            if(!v1.empty()){
                ++it2;
                for(auto it1 = v1.begin(); it2 != v1.end(); ++it2, ++it1){
                    if(*it1 == *it2)
                    v3.push_back(*it1);
                }
            }
            
            v1 = v3;
            // cout << "[Debug][logical_and] Result: " << v1 << endl;
            return v1;
        }
        else{
           throw(Error_msg("[Logical::evaluate]: evaluate unknown operation type for token: " + token_str()));
            assert(false);
        }
        return vector<long>();
    }
    virtual bool left_associative(){return true;}

};
#endif