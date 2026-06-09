#include "Interpreter.h"
#include <iostream>
#include <stdexcept>

using Value = Interpreter::Value;
using Data  = Interpreter::Data;


void RuntimeError(const std::string& reason)
{
    std::cerr
        << "[Runtime Error] " << reason << "\n"
        << std::endl;

    throw std::runtime_error(reason);
}


void PrintValue(const Value& val)
{
    const Data& v = val.data;

    if (std::holds_alternative<int>(v))
    {
        std::cout << std::get<int>(v);
    }
    else if (std::holds_alternative<std::string>(v))
    {
        std::cout << std::get<std::string>(v);
    }
    else if (std::holds_alternative<std::vector<Value>>(v))
    {
        // prints the array as [v,v,v] take that C#!!!
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
    if (!e)
        RuntimeError("Null expression");

    if (auto i = dynamic_cast<IntExpr*>(e))
        return Value{ i->value };

    if (auto s = dynamic_cast<StringExpr*>(e))
        return Value{ s->value };

    if (auto v = dynamic_cast<VarExpr*>(e))
    {
        auto it = vars.find(v->name);
        if (it == vars.end())
            RuntimeError("Undefined variable: " + v->name);

        return it->second;
    }

    if (auto idx = dynamic_cast<ArrayIndexExpr*>(e))
    {
        Value arrVal = Evaluate(idx->array.get());

        int i = std::get<int>(Evaluate(idx->index.get()).data);

        const auto& arr = std::get<std::vector<Value>>(arrVal.data);

        if (i < 0 || i >= (int)arr.size())
            RuntimeError("Array index out of bounds");

        return arr[i];
    }

    if (auto b = dynamic_cast<BinaryExpr*>(e))
    {
        Value lval = Evaluate(b->left.get());
        Value rval = Evaluate(b->right.get());

        if (!std::holds_alternative<int>(lval.data) ||
            !std::holds_alternative<int>(rval.data))
            RuntimeError("Binary expression expects integers");

        int left  = std::get<int>(lval.data);
        int right = std::get<int>(rval.data);

        switch (b->op)
        {
            case TokenType::EQUALEQUAL:   return Value{ left == right? 1 : 0 };
            case TokenType::EQUALORLESSER:  return Value{ left <= right? 1 : 0 };
            case TokenType::EQUALORGREATER:  return Value{ left >= right? 1 : 0 };
            case TokenType::NOTEQUAL:  return Value{ left != right? 1 : 0 };
            case TokenType::GREATER:  return Value{ left > right? 1 : 0  };
            case TokenType::LESSER:  return Value{ left < right? 1 : 0  };

            case TokenType::PLUS:   return Value{ left + right };
            case TokenType::MINUS:  return Value{ left - right };
            case TokenType::STAR:   return Value{ left * right };
            case TokenType::SLASH:  return Value{ left / right };
                if (right == 0)
                    RuntimeError("Division by zero");
                return Value{ left / right };

            default:
                RuntimeError("Invalid binary operator");
        }
    }

    return Value{ 0 };
}

void Interpreter::Run(const std::vector<std::unique_ptr<Stmt>>& stmts)
{
    for (auto& stmt : stmts)
    {
        if (auto a = dynamic_cast<AssignmentStmt*>(stmt.get()))
        {
            auto* var = dynamic_cast<VarExpr*>(a->left.get());

            if (!var)
                RuntimeError("Invalid assignment target");

            vars[var->name] = Evaluate(a->right.get());
        }

        if (auto a = dynamic_cast<ArrayAssignStmt*>(stmt.get()))
        {
            auto it = vars.find(a->arrayName);
            if (it == vars.end())
                RuntimeError("Undefined array: " + a->arrayName);

            auto& arr = std::get<std::vector<Value>>(it->second.data);

            int idx = std::get<int>(Evaluate(a->index.get()).data);
            Value v = Evaluate(a->value.get());

            if (idx < 0 || idx >= (int)arr.size())
                RuntimeError("Array index out of bounds");

            arr[idx] = v;
        }

        if (auto v = dynamic_cast<VarDecl*>(stmt.get()))
        {
            if (!v->value)
                RuntimeError("Variable declared without initializer: " + v->name);

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
            Value condVal = Evaluate(ifStmt->condition.get());



            if (std::get<int>(condVal.data) == 1)
                Run(ifStmt->body);
        }
    }
}

void Interpreter::RuntimeError(const std::string& reason)
{
    std::cerr
        << "[Runtime Error] " << reason << "\n"
        << std::endl;
}