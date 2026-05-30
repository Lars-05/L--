//
// Created by Lars on 30-5-2026.
//

#ifndef L_LEXER_H
#define L_LEXER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>

#include "Token.h"


class Lexer {
public:
    explicit Lexer(std::string src) : src(src), pos(0) {};
    std::vector<Token> Tokenize();
    std::string src;
    size_t pos;
    Token HandleValue();
    Token HandleIdentifier();


};


#endif //L_LEXER_H