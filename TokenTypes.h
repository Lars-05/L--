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

    LBRACE,
    RBRACE,


    END,

    // Operators
    AND,
    OR,
    EQEQUAL,
    LESSER,
    GREATER,
    NOT_EQUAL,
    GREATER_EQUAL,
    LESSER_EQUAL
};
