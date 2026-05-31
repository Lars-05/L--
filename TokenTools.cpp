#include "TokenTools.h"

std::string TokenTools::TokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::IDENT:  return "IDENT";
        case TokenType::PRINT:  return "PRINT";

        case TokenType::PLUS:   return "PLUS";
        case TokenType::MINUS:  return "MINUS";
        case TokenType::STAR:   return "STAR";
        case TokenType::SLASH:  return "SLASH";
        case TokenType::EQUAL:  return "EQUAL";

        case TokenType::IF:     return "IF";

        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";

        case TokenType::END:    return "END";

        default: return "UNKNOWN";
    }
}