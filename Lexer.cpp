// Lexer.cpp
#include "Lexer.h"
#include <cctype>

Lexer::Lexer(std::string s) : src(std::move(s)) {}

char Lexer::Peek() { return i < src.size() ? src[i] : '\0'; }

char Lexer::Advance(){ return src[i++]; }

void Lexer::skip() {
    while (isspace(Peek())) Advance();
}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> out;

    while (i < src.size())
    {
        skip();
        char c = Peek();

        if (isalpha(c))
        {
            std::string word;

            while (isalnum(Peek()))
                word += Advance();

            if (word == "int")
                out.push_back({TokenType::INT, word});
            else if (word == "print")
                out.push_back({TokenType::PRINT, word});
            else if (word == "if")
                out.push_back({TokenType::IF, word});
            else if (word == "string")
                out.push_back({TokenType::STRING, word});
            else
                out.push_back({TokenType::IDENT, word});
        }
        else if (isdigit(c))
        {
            std::string num;

            while (isdigit(Peek()))
                num += Advance();

            out.push_back({TokenType::NUMBER, num});
        }
        else
        {
            switch (c)
            {
                case '=':
                    Advance();
                    if (Peek() == '=')
                    {
                        Advance();
                        out.push_back({TokenType::EQUALEQUAL, "=="});
                    }
                    else
                        out.push_back({TokenType::EQUAL, "="});
                    break;

                case '<':
                    Advance();
                    if (Peek() == '=')
                    {
                        Advance();
                        out.push_back({TokenType::EQUALORLESSER, "<="});
                    }
                    else
                        out.push_back({TokenType::LESSER, "<"});
                    break;

                case '>':
                    Advance();
                    if (Peek() == '=')
                    {
                        Advance();
                        out.push_back({TokenType::EQUALORGREATER, ">="});
                    }
                    else
                        out.push_back({TokenType::GREATER, ">"});
                    break;
                case ';':
                    Advance();
                    out.push_back({TokenType::SEMI, ";"});
                    break;

                case '(':
                    Advance();
                    out.push_back({TokenType::LPAREN, "("});
                    break;

                case ')':
                    Advance();
                    out.push_back({TokenType::RPAREN, ")"});
                    break;

                case '{':
                    Advance();
                    out.push_back({TokenType::LBRACKET, "{"});
                    break;

                case '}':
                    Advance();
                    out.push_back({TokenType::RBRACKET, "}"});
                    break;

                case '"':
                    std::string stringContent;
                    Advance();
                    while (Peek() != '"' && Peek() != '\0')
                    {
                        stringContent += Advance();
                    }
                    out.push_back({TokenType::STRINGLITERAL, stringContent});
                    Advance();
                break;

                //default:
                    //Advance();
                //break;
            }
        }
    }
    out.push_back({TokenType::END, ""});
    return out;
}
