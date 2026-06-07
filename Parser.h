
#pragma once
#include <memory>
#include "Lexer.h"
#include <vector>

#include "AST.h"

class Parser {
public:
    explicit Parser(std::vector<Token> t);
    std::vector<std::unique_ptr<Stmt>> Parse();

private:
    std::vector<Token> tokens;
    size_t i = 0;

    Token& Peek();
    Token& Advance();
    bool MatchToken(TokenType t);
    bool IsOperatorValid(TokenType);

    std::unique_ptr<Expr> parseExpr();
    std::unique_ptr<Stmt> parseStmt();
    void CheckForSemiColon();
    void ThrowError(std::string, std::string);

    template<typename T>
    bool MatchExpr(const std::unique_ptr<Expr>& expr)
    {
        return dynamic_cast<T*>(expr.get()) != nullptr;
    }




    std::array<TokenType, 6> comparisonTokens = {
        TokenType::EQUALEQUAL,
        TokenType::GREATER,
        TokenType::LESSER,
        TokenType::EQUALORLESSER,
        TokenType::EQUALORGREATER,
        TokenType::NOTEQUAL,
    };
};
