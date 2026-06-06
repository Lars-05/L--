
#include "Interpreter.h"
#include <iostream>

 Interpreter::Data Interpreter::Evaluate(Expr* e)
{
     if (auto i = dynamic_cast<IntExpr*>(e))
         return i->value;


    if (auto v = dynamic_cast<VarExpr*>(e)) {
        return vars[v->name];
    }

    if (auto s = dynamic_cast<StringExpr*>(e)) {
        return s->value;
    }


     if (auto b = dynamic_cast<BinaryExpr*>(e))
     {
         int left = std::get<int>(Evaluate(b->left.get()));
         int right = std::get<int>(Evaluate(b->right.get()));

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
            auto value = Evaluate(p->value.get());

            std::visit([](auto&& v)
            {
                std::cout << v << "\n";
            }, value);
        }

        if (auto ifStmt = dynamic_cast<IfStmt*>(stmt.get()))
        {
            bool result = std::get<int>(Evaluate(ifStmt->condition.get()));
            if (result)
            {
                Run(ifStmt->body);
            }
        }

    }
}