
#pragma once
#include <string>
#include <memory>

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

struct PrintStmt : Stmt
{
    std::unique_ptr<Expr> value;
};