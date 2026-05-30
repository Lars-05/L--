//
// Created by Lars on 30-5-2026.
//

#ifndef L___TOKEN_H
#define L___TOKEN_H
#include <string>

#include "TokenTypes.h"


struct Token {
    TokenType type;
    std::string value;
};


#endif //L___TOKEN_H