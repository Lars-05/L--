#include "Parser.h"
#include <iostream>

#include "TokenTools.h"


Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens), pos(0) {

}


void Parser::Run() {
    while (Peek().type != TokenType::END) {
        Statement();
    }
}

void Parser::Statement() {

    if (Peek().type == TokenType::PRINT) {

        Advance(); // PRINT

        if (Peek().type != TokenType::LPAREN) {
            std::cout << "Error: print statement is missing '('" << std::endl;
            return;
        }
        Advance(); // (

        int value = Expression();

        if (Peek().type != TokenType::RPAREN) {
            std::cout << "Error: print statement is missing ')'" << std::endl;
            return;
        }
        Advance(); // )

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

Token Parser::PeekForward(int steps) {
    size_t index = pos + steps;

    if (index < tokens.size())
        return tokens[index];

    return {TokenType::END, ""};
}
