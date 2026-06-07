#include "Parser.h"
#include <array>
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



bool Parser::MatchToken(TokenType t)
{
    return Peek().type == t;
}



std::unique_ptr<Expr> Parser::parseExpr()
{
    if (MatchToken(TokenType::NUMBER))
    {
        int value = std::stoi(Advance().text);
        return std::make_unique<IntExpr>(value);
    }

    if (MatchToken(TokenType::IDENT))
    {
        std::string name = Advance().text;

        if (MatchToken(TokenType::LBRACKET))
        {
            Advance(); // [

            auto expr = std::make_unique<ArrayIndexExpr>();

            expr->array = std::make_unique<VarExpr>(name);
            expr->index = parseExpr();

            if (!MatchToken(TokenType::RBRACKET))
                ThrowError("Expected ]", Peek().text);

            Advance();

            return expr;
        }

        return std::make_unique<VarExpr>(name);
    }


    if (MatchToken(TokenType::STRINGLITERAL))
    {
        std::string name = Advance().text;
        return std::make_unique<StringExpr>(name);
    }
}

std::unique_ptr<Stmt> Parser::parseStmt()
{
    if (MatchToken(TokenType::IDENT))
    {
        std::string name = Advance().text;

        // assignment: x = ...
        if (MatchToken(TokenType::EQUAL))
        {
            Advance();

            auto stmt = std::make_unique<AssignmentStmt>();

            stmt->left = std::make_unique<VarExpr>(name);
            stmt->right = parseExpr();

            CheckForSemiColon();
            return stmt;
        }

        if (MatchToken(TokenType::LBRACKET))
        {
            Advance(); // [

            auto stmt = std::make_unique<ArrayAssignStmt>();
            stmt->arrayName = name;

            stmt->index = parseExpr();

            if (!MatchToken(TokenType::RBRACKET))
                ThrowError("Expected ]", Peek().text);
            Advance();

            if (!MatchToken(TokenType::EQUAL))
                ThrowError("Expected =", Peek().text);
            Advance();

            stmt->value = parseExpr();

            CheckForSemiColon();
            return stmt;
        }

        // fallback: variable expression? (or error)
        ThrowError("Unexpected identifier usage", name);
    }


    if (MatchToken(TokenType::INT))
    {
        Advance(); // consume INT

        auto stmt = std::make_unique<VarDecl>();

        if (!MatchToken(TokenType::IDENT))
            ThrowError("Expected identifier:", Peek().text);

        stmt->name = Advance().text;

        if (MatchToken(TokenType::EQUAL))
        {
            Advance(); // consume '='
            stmt->value = parseExpr();
        }

        CheckForSemiColon();

        return stmt;
    }


    // for a[0] = 0
    if (MatchToken(TokenType::IDENT))
    {
        Advance(); // consumes IDENT
        MatchToken(TokenType::LBRACKET);
              Advance();
        MatchToken(TokenType::NUMBER);
        MatchToken(TokenType::RBRACKET);
        MatchToken(TokenType::EQUAL);


        auto stmt = std::make_unique<VarDecl>();


        return stmt;
    }

    if (MatchToken(TokenType::STRING))
    {
        Advance(); // consumes STRING

        auto stmt = std::make_unique<VarDecl>();

        if (!MatchToken(TokenType::IDENT))
            ThrowError("Expected identifier:", Peek().text);

        stmt->name = Advance().text;

        if (MatchToken(TokenType::EQUAL))
        {
            Advance(); // consume '='
            stmt->value = parseExpr();
        }

        CheckForSemiColon();

        return stmt;
    }

    if (MatchToken(TokenType::PRINT))
    {
        Advance(); // consume PRINT

        auto stmt = std::make_unique<PrintStmt>();
        stmt->value = parseExpr();

        CheckForSemiColon();

        return stmt;
    }

    if (MatchToken(TokenType::ARRAY))
    {
        Advance(); // array

        auto stmt = std::make_unique<ArrayDecl>();

        if (!MatchToken(TokenType::IDENT))
            ThrowError("Expected array name", Peek().text);

        stmt->name = Advance().text;

        if (!MatchToken(TokenType::EQUAL))
            ThrowError("Expected =", Peek().text);
        Advance();

        if (!MatchToken(TokenType::LBRACE))
            ThrowError("Expected {", Peek().text);
        Advance();

        while (!MatchToken(TokenType::RBRACE))
        {
            stmt->values.push_back(parseExpr());

            if (MatchToken(TokenType::COMMA))
                Advance();
        }

        Advance(); // }

        CheckForSemiColon();
        return stmt;
    }


    if (MatchToken(TokenType::IF))
    {
        Advance();

        auto stmt = std::make_unique<IfStmt>();
        stmt->condition = std::make_unique<BinaryExpr>();

        if (!MatchToken(TokenType::LPAREN))
            ThrowError("Expected (", Peek().text);

        Advance();

        stmt->condition->left = parseExpr();

        TokenType op = Peek().type;

        if (!IsOperatorValid(op))
            ThrowError("Expected ==", Peek().text);

        stmt->condition->op = op;
        Advance();

        stmt->condition->right = parseExpr();

        if (!MatchToken(TokenType::RPAREN))
            ThrowError("Expected )", Peek().text);

        Advance();

        if (!MatchToken(TokenType::LBRACE))
            ThrowError("Expected {", Peek().text);

        Advance();

        while (!MatchToken(TokenType::RBRACE))
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