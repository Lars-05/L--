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

    std::array<TokenType, 3> printFormat = {
        TokenType::LPAREN,
        TokenType::NUMBER,
        TokenType::RPAREN
    };

    std::array<TokenType, 8> operators = {

        TokenType::EQEQUAL,
        TokenType::NOT_EQUAL,

        TokenType::GREATER,
        TokenType::GREATER_EQUAL,

        TokenType::LESSER,
        TokenType::LESSER_EQUAL
    };

    TokenTools tools;
private:




    std::vector<Token> tokens;
    size_t pos;

    std::unordered_map<std::string, int> vars;

    void Statement();


    bool Equality(int,int);
    int Comparison(int,int);
    int Expression();
    int Term();
    int Primary();

    Token PeekForward(int);
    Token Peek();
    Token Advance();
};