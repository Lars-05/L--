
#pragma once
#include <memory>
#include "Lexer.h"
#include <vector>

#include "AST.h"

class Parser {
public:
    explicit Parser(std::vector<Token> t);

    std::vector<std::unique_ptr<Stmt>> parse();

private:
    std::vector<Token> tokens;
    size_t i = 0;

    Token& Peek();
    Token& Advance();
    bool match(TokenType t);

    std::unique_ptr<Expr> parseExpr();
    std::unique_ptr<Stmt> parseStmt();
    void CheckForSemiColon();
    void ThrowError(std::string, std::string);
};
