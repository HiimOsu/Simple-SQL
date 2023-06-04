#include "token.h"

#include <iomanip>

int Token::type() const{
    return _type;
}

 
std::string Token::token_str() const{
    return _token;
}


std::ostream& operator <<(std::ostream& outs, const Token& t){
    outs  << "|" << t.token_str() << "|";
    return outs;
}

int Token::priority() const{
    return -1;
}

vector<long> Token::evaluate(Token* tk1, Token* tk2, MMap<std::string,long>* index) throw(const Error_msg) {
    cout << "calling evaluate from TokenBase\n";
    return vector<long>();
}
