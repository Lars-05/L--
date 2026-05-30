#include "Lexer.h"
#include <iostream>
#include <cctype>

std::vector<Token> Lexer::Tokenize() {

    std::vector<Token> tokens;

    while (pos < src.size()) {

        char c = src[pos];


        if (std::isspace(static_cast<unsigned char>(c))) {
            pos++;
            continue;
        }

        if (std::isdigit(static_cast<unsigned char>(c))) {
            tokens.push_back(HandleValue());
            continue;
        }


        if (std::isalpha(static_cast<unsigned char>(c))) {
            tokens.push_back(HandleIdentifier());
            continue;
        }


        switch (c) {

            case '+':
                tokens.push_back({TokenType::PLUS, "+"});
                pos++;
                break;

            case '-':
                tokens.push_back({TokenType::MINUS, "-"});
                pos++;
                break;

            case '*':
                tokens.push_back({TokenType::STAR, "*"});
                pos++;
                break;

            case '/':
                tokens.push_back({TokenType::SLASH, "/"});
                pos++;
                break;

            case '=':
                tokens.push_back({TokenType::EQUAL, "="});
                pos++;
                break;

            default:
                std::cerr << "Unknown character: " << c << std::endl;
                pos++;
                break;
        }
    }

    tokens.push_back({TokenType::END, ""});
    return tokens;
}

Token Lexer::HandleValue() {

    std::string value;

    while (pos < src.size() &&
           std::isdigit(static_cast<unsigned char>(src[pos]))) {

        value += src[pos++];
           }

    return {TokenType::NUMBER, value};
}

Token Lexer::HandleIdentifier() {

    std::string value;

    while (pos < src.size() &&
           std::isalnum(static_cast<unsigned char>(src[pos]))) {

        value += src[pos++];
           }

    if (value == "print")
        return {TokenType::PRINT, value};

    return {TokenType::IDENT, value};
}