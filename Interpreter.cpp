
#include "Interpreter.h"
#include <iostream>

int Interpreter::Evaluate(Expr* e)
{
    if (auto i = dynamic_cast<IntExpr*>(e))
        return i->value;

    if (auto v = dynamic_cast<VarExpr*>(e))
        return vars[v->name];

    if (auto b = dynamic_cast<BinaryExpr*>(e))
    {
        int left = Evaluate(b->left.get());
        int right = Evaluate(b->right.get());

        switch (b->op)
        {
            case TokenType::EQUALEQUAL:
                return left == right;

            case TokenType::GREATER:
                return left > right;

            case TokenType::LESSER:
                return left < right;

            case TokenType::EQUALORLESSER:
                return left <= right;

            case TokenType::EQUALORGREATER:
                return left >= right;
            default:
                return 0;
        }
    }

    return 0;
}

void Interpreter::Run(const std::vector<std::unique_ptr<Stmt>>& stmts)
{
    for (auto& stmt : stmts) {

        if (auto v = dynamic_cast<VarDecl*>(stmt.get()))
        {
            vars[v->name] = Evaluate(v->value.get());
        }

        if (auto p = dynamic_cast<PrintStmt*>(stmt.get()))
        {
            std::cout << Evaluate(p->value.get()) << "\n";
        }

        if (auto ifStmt = dynamic_cast<IfStmt*>(stmt.get()))
        {
            if (Evaluate(ifStmt->condition.get()))
            {
                Run(ifStmt->body);
            }
        }

    }
}