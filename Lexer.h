
#pragma once
#include <array>
#include <string>
#include <vector>

enum class TokenType {
    INT,
    PRINT,
    IDENT,
    NUMBER,
    EQUAL,
    SEMI,
    END,
    IF,
    LBRACE,
    RBRACE,
    LBRACKET,
    RBRACKET,

    EQUALEQUAL,
    GREATER,
    LESSER,
    EQUALORLESSER,
    EQUALORGREATER,
    NOTEQUAL,
};




struct Token {
    TokenType type;
    std::string text;
};

class Lexer {
public:
    std::array<TokenType, 6> comparisonTokens = {
        TokenType::EQUALEQUAL,
        TokenType::GREATER,
        TokenType::LESSER,
        TokenType::EQUALORLESSER,
        TokenType::EQUALORGREATER,
        TokenType::NOTEQUAL,
    };
    explicit Lexer(std::string src);
    std::vector<Token> tokenize();

private:
    std::string src;
    size_t i = 0;

    char Peek();
    char Advance();
    void skip();
};