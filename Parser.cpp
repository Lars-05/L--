#include "Parser.h"
#include <stdexcept>

Token& Parser::peek() { return tokens[i]; }
Token& Parser::advance() { return tokens[i++]; }
void Parser::CheckForSemiColon() {
    if (peek().type == TokenType::SEMI) {
        advance();
    }
}

Parser::Parser(std::vector<Token> t) : tokens(std::move(t)) {}

bool Parser::match(TokenType t) {

    if (peek().type == t) {
        advance();
        return true;
    }
    return false;
}

std::unique_ptr<Expr> Parser::parseExpr() {

    if (peek().type == TokenType::NUMBER) {

        int value = std::stoi(peek().text);
        advance();
        return std::make_unique<IntExpr>(value);
    }

    if (peek().type == TokenType::IDENT) {

        std::string name = peek().text;
        advance();
        return std::make_unique<VarExpr>(name);
    }

    throw std::runtime_error("Expected expression");
}

std::unique_ptr<Stmt> Parser::parseStmt() {

    if (match(TokenType::INT)) {

        //structure = [INT][IDENT(x)][=][NUMBER(10)][;]

        auto stmt = std::make_unique<VarDecl>();

        if (peek().type != TokenType::IDENT) {
            throw std::runtime_error("Expected identifier");
        }

        stmt->name = advance().text;

        if (peek().type == TokenType::EQUAL) {
            advance();
            stmt->value = parseExpr();
        }

        CheckForSemiColon();

        return stmt;
    }

    if (match(TokenType::PRINT)) {

        //structure =[PRINT][IDENT(x)][;]

        auto stmt = std::make_unique<PrintStmt>();

        stmt->value = parseExpr();

        CheckForSemiColon();

        return stmt;
    }

    return nullptr;
}

std::vector<std::unique_ptr<Stmt>> Parser::parse() {

    std::vector<std::unique_ptr<Stmt>> stmts;

    while (peek().type != TokenType::END) {

        auto stmt = parseStmt();

        if (!stmt) {
            throw std::runtime_error("Unexpected token: " + peek().text);
        }

        stmts.push_back(std::move(stmt));
    }

    return stmts;
}