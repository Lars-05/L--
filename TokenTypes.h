#pragma once
#include <string>

enum class TokenType {
    NUMBER,
    IDENT,
    PRINT,

    PLUS,
    MINUS,
    STAR,
    SLASH,
    EQUAL,

    IF,

    LPAREN,
    RPAREN,

    END
};
