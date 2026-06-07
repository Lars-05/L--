#pragma once

#include <memory>
#include <vector>
#include <array>

#include "Lexer.h"
#include "AST.h"

class Parser
{
public:
    explicit Parser(std::vector<Token> t);

    std::vector<std::unique_ptr<Stmt>> Parse();

    // global parse error flag (shared across parser runs if static is kept)
    static bool hadError;

private:
    std::vector<Token> tokens;
    size_t i = 0;

    Token& Peek();
    Token& Advance();
    bool MatchToken(TokenType t);

    void CheckForSemiColon();

    std::unique_ptr<Expr> parseExpr();
    std::unique_ptr<Stmt> parseStmt();

    void Error(const std::string& reason, const Token& token);

    bool IsOperatorValid(TokenType op);

    template<typename T>
    bool MatchExpr(const std::unique_ptr<Expr>& expr)
    {
        return dynamic_cast<T*>(expr.get()) != nullptr;
    }

    std::array<TokenType, 6> comparisonTokens =
    {
        TokenType::EQUALEQUAL,
        TokenType::GREATER,
        TokenType::LESSER,
        TokenType::EQUALORLESSER,
        TokenType::EQUALORGREATER,
        TokenType::NOTEQUAL,
    };
};