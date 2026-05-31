#pragma once
#include <array>
#include <vector>
#include <unordered_map>

#include "Token.h"
#include "TokenTools.h"
#include "TokenTypes.h"






class Parser {



public:
    Parser(const std::vector<Token>& tokens);

    void Run();

    std::array<TokenType, 6> operators{{
        TokenType::EQEQUAL,
        TokenType::EQUAL,

        TokenType::GREATER,
        TokenType::GREATER_EQUAL,

        TokenType::LESSER,
        TokenType::LESSER_EQUAL
    }};



    TokenTools tools;
private:




    std::vector<Token> tokens;
    size_t pos;

    std::unordered_map<std::string, int> vars;

    void Statement();


    bool Equality(int,int, TokenType);
    int Comparison(int,int, TokenType);
    int Expression();
    int Term();
    int Primary();

    Token PeekAhead(int);
    Token Peek();
    Token Advance();
};