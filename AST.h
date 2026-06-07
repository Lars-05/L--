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

struct StringExpr : Expr
{
    std::string value;
    StringExpr(std::string v) : value(std::move(v)) {}
};

struct VarExpr : Expr
{
    std::string name;
    VarExpr(std::string n) : name(std::move(n)) {}
};

struct ArrayIndexExpr : Expr
{
    std::unique_ptr<Expr> array;  // VarExpr
    std::unique_ptr<Expr> index;
};

struct BinaryExpr : Expr
{
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    TokenType op;
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

struct AssignmentStmt : Stmt
{
    std::unique_ptr<Expr> left;   // VarExpr or ArrayIndexExpr
    std::unique_ptr<Expr> right;
};

struct ArrayDecl : Stmt
{
    std::string name;
    int size = 0;
    std::vector<std::unique_ptr<Expr>> values;
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