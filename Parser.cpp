#include "Parser.h"
#include <array>
#include <iostream>
#include <stdexcept>


bool Parser::hadError = false;


void Parser::Error(const std::string& reason, const Token& token)
{
    hadError = true;

    std::cerr
        << "[Parser Error] " << reason << "\n"
        << "  Token: '" << token.text << "'\n"
        << "  Type: " << static_cast<int>(token.type) << "\n"
        << std::endl;

    throw std::runtime_error(reason);
}


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

    // CONSUMES current token
    return tokens[i++];
}

void Parser::CheckForSemiColon()
{
    if (Peek().type == TokenType::SEMI)
        Advance(); // CONSUMES ';'
}

Parser::Parser(std::vector<Token> t)
    : tokens(std::move(t))
{
}

bool Parser::MatchToken(TokenType t)
{
    return Peek().type == t;
}

bool Parser::IsOperatorValid(TokenType pOperator)
{
    for (auto comparisonToken : comparisonTokens)
    {
        if (comparisonToken == pOperator)
            return true;
    }
    return false;
}

std::unique_ptr<Expr> Parser::parseMultiplicative() {

    auto left = parsePrimary();

    while (MatchToken(TokenType::STAR) || MatchToken(TokenType::SLASH))
    {
        TokenType op = Advance().type;
        auto expr = std::make_unique<BinaryExpr>();
        expr->left = std::move(left);
        expr->op = op;
        expr->right = parsePrimary();
        left = std::move(expr);
    }
    return left;
}

std::unique_ptr<Expr> Parser::parseAdditive() {

    auto left = parsePrimary();

    while (MatchToken(TokenType::PLUS) || MatchToken(TokenType::MINUS))
    {
        TokenType op = Advance().type;
        auto expr = std::make_unique<BinaryExpr>();
        expr->left = std::move(left);
        expr->op = op;
        expr->right = parsePrimary();
        left = std::move(expr);
    }
    return left;
}

std::unique_ptr<Expr> Parser::parsePrimary()
{
    if (MatchToken(TokenType::NUMBER))
    {
        int value = std::stoi(Advance().text);
        // CONSUMES NUMBER
        return std::make_unique<IntExpr>(value);
    }

    if (MatchToken(TokenType::IDENT))
    {
        std::string name = Advance().text;
        // CONSUMES IDENT

        if (MatchToken(TokenType::LBRACKET))
        {
            Advance();
            // CONSUMES '['

            auto expr = std::make_unique<ArrayIndexExpr>();

            expr->array = std::make_unique<VarExpr>(name);
            expr->index = parseMultiplicative();

            if (!MatchToken(TokenType::RBRACKET))
                Error("Expected ']'", Peek());

            Advance();
            // CONSUMES ']'

            return expr;
        }

        return std::make_unique<VarExpr>(name);
    }

    if (MatchToken(TokenType::STRINGLITERAL))
    {
        std::string value = Advance().text;
        return std::make_unique<StringExpr>(value);
    }

    Error("Invalid expression", Peek());
    return nullptr;
}


std::unique_ptr<Expr> Parser::parseExpression()
{
    return parseAdditive();
}

std::unique_ptr<Stmt> Parser::parseStmt()
{
    if (MatchToken(TokenType::IDENT))
    {
        std::string name = Advance().text;
        // CONSUMES IDENT

        // assignment: x = ...
        if (MatchToken(TokenType::EQUAL))
        {
            Advance();
            // CONSUMES '='

            auto stmt = std::make_unique<AssignmentStmt>();

            stmt->left = std::make_unique<VarExpr>(name);
            stmt->right = parseMultiplicative();

            CheckForSemiColon();
            return stmt;
        }
        if (MatchToken(TokenType::PLUS) ||MatchToken(TokenType::MINUS) ||MatchToken(TokenType::STAR) ||MatchToken(TokenType::SLASH))
        {
            TokenType op = Advance().type;
            auto rhs = parseExpression();

            auto bin = std::make_unique<BinaryExpr>();
            bin->left = std::make_unique<VarExpr>(name);
            bin->right = std::move(rhs);
            bin->op = op;

            auto stmt = std::make_unique<AssignmentStmt>();
            stmt->left = std::make_unique<VarExpr>(name);
            stmt->right = std::move(bin);

            CheckForSemiColon();
            return stmt;
        }



        // array assignment: x[0] = ...
        if (MatchToken(TokenType::LBRACKET))
        {
            Advance();
            // CONSUMES '['

            auto stmt = std::make_unique<ArrayAssignStmt>();
            stmt->arrayName = name;

            stmt->index = parseMultiplicative();

            if (!MatchToken(TokenType::RBRACKET))
                Error("Expected ']'", Peek());

            Advance();
            // CONSUMES ']'

            if (!MatchToken(TokenType::EQUAL))
                Error("Expected '='", Peek());

            Advance();
            // CONSUMES '='

            stmt->value = parseMultiplicative();

            CheckForSemiColon();
            return stmt;
        }



        Error("Unexpected identifier usage", Peek());
    }

    if (MatchToken(TokenType::STRING))
    {
        Advance(); // CONSUMED STRING

        if (!MatchToken(TokenType::IDENT))
            Error("Expected identifier", Peek());

        auto stmt = std::make_unique<VarDecl>();
        stmt->name = Advance().text;

        if (!MatchToken(TokenType::EQUAL))
            Error("Expected '='", Peek());

        Advance();

        stmt->value = parseMultiplicative();

        CheckForSemiColon();
        return stmt;
    }

    if (MatchToken(TokenType::INT))
    {
        Advance();
        // CONSUMES 'int'

        auto stmt = std::make_unique<VarDecl>();

        if (!MatchToken(TokenType::IDENT))
            Error("Expected identifier after int", Peek());

        stmt->name = Advance().text;
        // CONSUMES IDENT

        if (MatchToken(TokenType::EQUAL))
        {
            Advance();
            // CONSUMES '='

            stmt->value = parseMultiplicative();
        }

        CheckForSemiColon();
        return stmt;
    }


    if (MatchToken(TokenType::PRINT))
    {
        Advance();
        // CONSUMES PRINT

        auto stmt = std::make_unique<PrintStmt>();
        stmt->value = parseMultiplicative();

        CheckForSemiColon();
        return stmt;
    }

    if (MatchToken(TokenType::ARRAY))
    {
        Advance();
        // CONSUMES ARRAY

        auto stmt = std::make_unique<ArrayDecl>();

        if (!MatchToken(TokenType::IDENT))
            Error("Expected array name", Peek());

        stmt->name = Advance().text;
        // CONSUMES IDENT

        if (!MatchToken(TokenType::EQUAL))
            Error("Expected '='", Peek());

        Advance();
        // CONSUMES '='

        if (!MatchToken(TokenType::LBRACE))
            Error("Expected '{'", Peek());

        Advance();
        // CONSUMES '{'

        while (!MatchToken(TokenType::RBRACE))
        {
            stmt->values.push_back(parseMultiplicative());

            if (MatchToken(TokenType::COMMA))
                Advance(); // CONSUMES ','
        }

        Advance();
        // CONSUMES '}'

        CheckForSemiColon();
        return stmt;
    }

    if (MatchToken(TokenType::IF))
    {
        Advance();
        // CONSUMES IF

        auto stmt = std::make_unique<IfStmt>();
        stmt->condition = std::make_unique<BinaryExpr>();

        if (!MatchToken(TokenType::LPAREN))
            Error("Expected '('", Peek());

        Advance();
        // CONSUMES '('

        stmt->condition->left = parseMultiplicative();

        TokenType op = Peek().type;

        if (!IsOperatorValid(op))
            Error("Invalid operator in condition", Peek());

        stmt->condition->op = op;
        Advance();
        // CONSUMES OP

        stmt->condition->right = parseMultiplicative();

        if (!MatchToken(TokenType::RPAREN))
            Error("Expected ')'", Peek());

        Advance();
        // CONSUMES ')'

        if (!MatchToken(TokenType::LBRACE))
            Error("Expected '{'", Peek());

        Advance();
        // CONSUMES '{'

        while (!MatchToken(TokenType::RBRACE))
        {
            auto bodyStmt = parseStmt();

            if (!bodyStmt)
                Error("Invalid statement in IF body", Peek());

            stmt->body.push_back(std::move(bodyStmt));
        }

        Advance();
        return stmt;
    }

    Error("Unknown statement", Peek());
    return nullptr;
}

std::vector<std::unique_ptr<Stmt>> Parser::Parse()
{
    std::vector<std::unique_ptr<Stmt>> stmts;

    while (Peek().type != TokenType::END || !hadError)
    {
        if (Peek().type == TokenType::END)
            break;

        auto stmt = parseStmt();


        if (!stmt || hadError)
            break;

        stmts.push_back(std::move(stmt));
    }

    return stmts;
}