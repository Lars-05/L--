#include "Parser.h"
#include <stdexcept>

Token& Parser::Peek() { return tokens[i]; }
Token& Parser::Advance() { return tokens[i++]; } // consumes current token and move to the next
void Parser::CheckForSemiColon() {
    if (Peek().type == TokenType::SEMI) {
        Advance();
    }
}

Parser::Parser(std::vector<Token> t) : tokens(std::move(t)) {}

bool Parser::match(TokenType t) {

    if (Peek().type == t) {
        Advance();
        return true;
    }
    return false;
}

std::unique_ptr<Expr> Parser::parseExpr() {

    if (Peek().type == TokenType::NUMBER) {

        int value = std::stoi(Peek().text);
        Advance();
        return std::make_unique<IntExpr>(value);
    }

    if (Peek().type == TokenType::IDENT) {

        std::string name = Peek().text;
        Advance();
        return std::make_unique<VarExpr>(name);
    }

    throw std::runtime_error("Expected expression");
}

std::unique_ptr<Stmt> Parser::parseStmt() {

    if (match(TokenType::INT)) {

        //structure = [INT][IDENT(x)][=][NUMBER(10)][;]

        auto stmt = std::make_unique<VarDecl>();

        if (Peek().type != TokenType::IDENT) {
            throw std::runtime_error("Expected identifier");
        }

        stmt->name = Advance().text;

        if (Peek().type == TokenType::EQUAL) {
            Advance();
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

    if (match(TokenType::IF)) {

        //structure = [IF][LBRACE][Condition][RBrace]

        auto stmt = std::make_unique<IfStmt>();



        Advance();

        while (Peek().type != TokenType::LBRACE)
        {
            stmt->body.push_back(parseStmt());
        }

        Advance();

        return stmt;
    }

    return nullptr;
}

std::vector<std::unique_ptr<Stmt>> Parser::parse() {

    std::vector<std::unique_ptr<Stmt>> stmts;

    while (Peek().type != TokenType::END) {

        auto stmt = parseStmt();

        if (!stmt) {
            ThrowError("Unexpected Token", Peek().text);
        }

        stmts.push_back(std::move(stmt));
    }

    return stmts;
}

void Parser::ThrowError(std::string reason, std::string text) {
    throw std::runtime_error(reason + " " + text);
}
