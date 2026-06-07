#include "Interpreter.h"
#include <iostream>

using Value = Interpreter::Value;
using Data  = Interpreter::Data;

void PrintValue(const Value& val)
{
    const Data& v = val.data;

    if (std::holds_alternative<int>(v))
    {
        std::cout << std::get<int>(v);
    }
    else if (std::holds_alternative<bool>(v))
    {
        std::cout << (std::get<bool>(v) ? "true" : "false");
    }
    else if (std::holds_alternative<std::string>(v))
    {
        std::cout << std::get<std::string>(v);
    }
    else if (std::holds_alternative<std::vector<Value>>(v))
    {
        const auto& arr = std::get<std::vector<Value>>(v);

        std::cout << "[";
        for (size_t i = 0; i < arr.size(); i++)
        {
            PrintValue(arr[i]);
            if (i + 1 < arr.size()) std::cout << ", ";
        }
        std::cout << "]";
    }
}


Value Interpreter::Evaluate(Expr* e)
{
    if (auto i = dynamic_cast<IntExpr*>(e))
        return Value{ i->value };

    if (auto s = dynamic_cast<StringExpr*>(e))
        return Value{ s->value };

    if (auto v = dynamic_cast<VarExpr*>(e))
        return vars.at(v->name);

    if (auto idx = dynamic_cast<ArrayIndexExpr*>(e))
    {
        Value arrVal = Evaluate(idx->array.get());

        int i = std::get<int>(Evaluate(idx->index.get()).data);

        const auto& arr = std::get<std::vector<Value>>(arrVal.data);

        return arr.at(i);
    }

    if (auto b = dynamic_cast<BinaryExpr*>(e))
    {
        int left  = std::get<int>(Evaluate(b->left.get()).data);
        int right = std::get<int>(Evaluate(b->right.get()).data);

        switch (b->op)
        {
            case TokenType::EQUALEQUAL:     return Value{ left == right };
            case TokenType::GREATER:        return Value{ left > right };
            case TokenType::LESSER:         return Value{ left < right };
            case TokenType::EQUALORLESSER:  return Value{ left <= right };
            case TokenType::EQUALORGREATER: return Value{ left >= right };
            default: return Value{ false };
        }
    }

    return Value{ 0 };
}

void Interpreter::Run(const std::vector<std::unique_ptr<Stmt>>& stmts)
{
    for (auto& stmt : stmts)
    {
        if (auto v = dynamic_cast<VarDecl*>(stmt.get()))
        {
            vars[v->name] = Evaluate(v->value.get());
        }

        if (auto a = dynamic_cast<ArrayDecl*>(stmt.get()))
        {
            std::vector<Value> arr;

            for (auto& expr : a->values)
                arr.push_back(Evaluate(expr.get()));

            vars[a->name] = Value{ arr };
        }

        if (auto p = dynamic_cast<PrintStmt*>(stmt.get()))
        {
            Value val = Evaluate(p->value.get());
            PrintValue(val);
            std::cout << "\n";
        }

        if (auto ifStmt = dynamic_cast<IfStmt*>(stmt.get()))
        {
            bool cond = std::get<bool>(Evaluate(ifStmt->condition.get()).data);
            if (cond)
                Run(ifStmt->body);
        }
    }
}