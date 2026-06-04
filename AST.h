
#pragma once
#include <string>
#include <memory>
#include <vector>

#include "Lexer.h"

struct Expr
{
    virtual ~Expr() = default;
};

struct IntExpr : Expr
{
    int value;
    IntExpr(int v) : value(v) {}
};

struct VarExpr : Expr
{
    std::string name;
    VarExpr(std::string n) : name(std::move(n)) {}
};


struct Stmt
{
    virtual ~Stmt() = default;
};

struct VarDecl : Stmt
{
    std::string name;
    std::unique_ptr<Expr> value;
};

struct BinaryExpr : Expr
{
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    TokenType op;
};

struct PrintStmt : Stmt
{
    std::unique_ptr<Expr> value;
};

struct IfStmt : Stmt
{
    std::unique_ptr<BinaryExpr> condition;
    std::vector<std::unique_ptr<Stmt>> body;
};