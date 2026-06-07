#pragma once

#include "AST.h"
#include <unordered_map>
#include <variant>
#include <vector>
#include <memory>
#include <string>

class Interpreter {
public:
    struct Value;

    using Data = std::variant<
        int,
        bool,
        std::string,
        std::vector<Value>
    >;

    struct Value {
        Data data;
    };

    void Run(const std::vector<std::unique_ptr<Stmt>>& stmts);

private:
    std::unordered_map<std::string, Value> vars;
    Value Evaluate(Expr* e);
    void RuntimeError(const std::string& reason);
};