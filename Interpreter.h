#pragma once
#include "AST.h"
#include <unordered_map>
#include <vector>

class Interpreter {
public:
    void Run(const std::vector<std::unique_ptr<Stmt>>& stmts);

private:
    std::unordered_map<std::string, int> vars;
    int Evaluate(Expr* e);
};