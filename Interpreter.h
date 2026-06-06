#pragma once
#include "AST.h"
#include <unordered_map>
#include <variant>
#include <vector>





class Interpreter {
public:
    void Run(const std::vector<std::unique_ptr<Stmt>>& stmts);

    using Data = std::variant<int, std::string>;

private:
    std::unordered_map<std::string, Data> vars;

    Data Evaluate(Expr* e);
};