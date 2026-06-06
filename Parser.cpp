#include "Parser.h"

#include <iostream>
#include <stdexcept>

Token& Parser::Peek()
{
    if (i >= tokens.size())
        throw std::runtime_error("Unexpected end of input");

    return tokens[i];
}

Token& Parser::Advance()
{
    if (i >= tokens.size())
        throw std::runtime_error("Unexpected end of input");

    return tokens[i++];
}

void Parser::CheckForSemiColon()
{
    if (Peek().type == TokenType::SEMI)
        Advance();
}

Parser::Parser(std::vector<Token> t): tokens(std::move(t)){}

bool Parser::Match(TokenType t)
{
    return Peek().type == t;
}

std::unique_ptr<Expr> Parser::parseExpr()
{
    if (Match(TokenType::NUMBER))
    {
        int value = std::stoi(Advance().text);
        return std::make_unique<IntExpr>(value);
    }

    if (Match(TokenType::IDENT))
    {
        std::string name = Advance().text;
        return std::make_unique<VarExpr>(name);
    }

    if (Match(TokenType::STRINGLITERAL))
    {
        std::string name = Advance().text;
        return std::make_unique<StringExpr>(name);
    }
}

std::unique_ptr<Stmt> Parser::parseStmt()
{
    if (Match(TokenType::INT))
    {
        Advance(); // consume INT

        auto stmt = std::make_unique<VarDecl>();

        if (!Match(TokenType::IDENT))
            ThrowError("Expected identifier:", Peek().text);

        stmt->name = Advance().text;

        if (Match(TokenType::EQUAL))
        {
            Advance(); // consume '='
            stmt->value = parseExpr();
        }

        CheckForSemiColon();

        return stmt;
    }

    if (Match(TokenType::STRING))
    {
        Advance(); // consumes STRING

        auto stmt = std::make_unique<VarDecl>();

        if (!Match(TokenType::IDENT))
            ThrowError("Expected identifier:", Peek().text);

        stmt->name = Advance().text;

        if (Match(TokenType::EQUAL))
        {
            Advance(); // consume '='
            stmt->value = parseExpr();
        }

        CheckForSemiColon();

        return stmt;
    }

    if (Match(TokenType::PRINT))
    {
        Advance(); // consume PRINT

        auto stmt = std::make_unique<PrintStmt>();
        stmt->value = parseExpr();

        CheckForSemiColon();

        return stmt;
    }


    if (Match(TokenType::IF))
    {
        Advance();

        auto stmt = std::make_unique<IfStmt>();
        stmt->condition = std::make_unique<BinaryExpr>();

        if (!Match(TokenType::LPAREN))
            ThrowError("Expected (", Peek().text);

        Advance();

        stmt->condition->left = parseExpr();

        TokenType op = Peek().type;

        if (!IsOperatorValid(op))
            ThrowError("Expected ==", Peek().text);

        stmt->condition->op = op;
        Advance();

        stmt->condition->right = parseExpr();

        if (!Match(TokenType::RPAREN))
            ThrowError("Expected )", Peek().text);

        Advance();

        if (!Match(TokenType::LBRACKET))
            ThrowError("Expected {", Peek().text);

        Advance();

        while (!Match(TokenType::RBRACKET))
        {
            auto bodyStmt = parseStmt();

            if (!bodyStmt)
                ThrowError("Invalid statement inside IF body", Peek().text);

            stmt->body.push_back(std::move(bodyStmt));
        }

        Advance();
        return stmt;
    }
}

std::vector<std::unique_ptr<Stmt>> Parser::Parse()
{
    std::vector<std::unique_ptr<Stmt>> stmts;

    while (Peek().type != TokenType::END)
    {
        auto stmt = parseStmt();

        if (!stmt)
            ThrowError("Unexpected Token:", Peek().text);

        stmts.push_back(std::move(stmt));
    }

    return stmts;
}

void Parser::ThrowError(std::string reason, std::string text)
{
    std::cout << reason + " " + text<<std::endl ;
    //throw std::runtime_error(reason + " " + text);
}

bool Parser::IsOperatorValid(TokenType pOperator) {
    bool foundMatch = false;
    for (auto comparisonToken : comparisonTokens)
    {
        if (comparisonToken != pOperator) {
            continue;
        }
        foundMatch = true;
    }
    return foundMatch;
}