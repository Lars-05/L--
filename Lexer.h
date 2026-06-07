
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
    LPAREN,
    RPAREN,
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
    STRING,
    STRINGLITERAL,
    QUOTEMARK,
    ARRAY,
    COMMA
};

struct Token {
    TokenType type;
    std::string text;
};

class Lexer {
public:

    explicit Lexer(std::string src);
    std::vector<Token> tokenize();

private:
    std::string src;
    size_t i = 0;

    char Peek();
    char Advance();
    void skip();
};