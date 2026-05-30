#pragma once
#include <vector>
#include <unordered_map>

#include "Token.h"
#include "TokenTypes.h"

class Parser {
public:
    Parser(const std::vector<Token>& tokens)
        : tokens(tokens), pos(0) {}

    void Run();

private:
    std::vector<Token> tokens;
    size_t pos;

    std::unordered_map<std::string, int> vars;

    void Statement();

    int Expression();
    int Term();
    int Primary();

    Token Peek();
    Token Advance();
};