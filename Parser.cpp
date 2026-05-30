#include "Parser.h"
#include <iostream>

void Parser::Run() {
    while (Peek().type != TokenType::END) {
        Statement();
    }
}

void Parser::Statement() {

    if (Peek().type == TokenType::PRINT) {
        Advance();              // print
        Advance();              // (

        int value = Expression();

        Advance();              // )
        std::cout << value << std::endl;
        return;
    }

    if (Peek().type == TokenType::IDENT) {
        std::string name = Advance().value;
        Advance(); // =
        int value = Expression();
        vars[name] = value;
        return;
    }
}

int Parser::Expression() {
    int left = Term();

    while (Peek().type == TokenType::PLUS ||
           Peek().type == TokenType::MINUS) {

        Token op = Advance();
        int right = Term();

        if (op.type == TokenType::PLUS) left += right;
        else left -= right;
           }

    return left;
}

int Parser::Term() {
    int left = Primary();

    while (Peek().type == TokenType::STAR ||
           Peek().type == TokenType::SLASH) {

        Token op = Advance();
        int right = Primary();

        if (op.type == TokenType::STAR) left *= right;
        else left /= right;
           }

    return left;
}

int Parser::Primary() {
    if (Peek().type == TokenType::NUMBER) {
        return std::stoi(Advance().value);
    }

    if (Peek().type == TokenType::IDENT) {
        std::string name = Advance().value;
        return vars[name];
    }

    return 0;
}

Token Parser::Peek() {
    if (pos >= tokens.size())
        return {TokenType::END, ""};

    return tokens[pos];
}

Token Parser::Advance() {
    if (pos < tokens.size())
        return tokens[pos++];

    return {TokenType::END, ""};
}