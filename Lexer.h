
#pragma once
#include <string>
#include <vector>

enum class TokenType {
    INT, PRINT,
    IDENT, NUMBER,
    EQUAL, SEMI,
    END
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

    char peek();
    char get();
    void skip();
};