// Lexer.cpp
#include "Lexer.h"
#include <cctype>
#include <stdexcept>

Lexer::Lexer(std::string s) : src(std::move(s)) {}

char Lexer::Peek()
{
    return (i < src.size()) ? src[i] : '\0';
}

char Lexer::Advance()
{
    return src[i++];
}

void Lexer::skip()
{
    while (std::isspace(Peek()))
        Advance();
}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> out;

    while (i < src.size())
    {
        skip();
        char c = Peek();


        if (std::isalpha(c))
        {
            std::string word;

            while (std::isalnum(Peek()))
                word += Advance();

            if (word == "int")        out.push_back({TokenType::INT, word});
            else if (word == "print") out.push_back({TokenType::PRINT, word});
            else if (word == "if")    out.push_back({TokenType::IF, word});
            else if (word == "string")out.push_back({TokenType::STRING, word});
            else if (word == "array") out.push_back({TokenType::ARRAY, word});
            else                       out.push_back({TokenType::IDENT, word});

            continue;
        }


        if (std::isdigit(c))
        {
            std::string num;

            while (std::isdigit(Peek()))
                num += Advance();

            out.push_back({TokenType::NUMBER, num});
            continue;
        }

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

            case '+':
                Advance();
                out.push_back({TokenType::PLUS, "+"});
                break;

            case '-':
                Advance();
                out.push_back({TokenType::MINUS, "-"});
                break;

            case '*':
                Advance();
                out.push_back({TokenType::STAR, "*"});
                break;

            case '/':
                Advance();
                out.push_back({TokenType::SLASH, "/"});
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
                out.push_back({TokenType::LBRACE, "{"});
                break;

            case '}':
                Advance();
                out.push_back({TokenType::RBRACE, "}"});
                break;

            case '[':
                Advance();
                out.push_back({TokenType::LBRACKET, "["});
                break;

            case ']':
                Advance();
                out.push_back({TokenType::RBRACKET, "]"});
                break;

            case ',':
                Advance();
                out.push_back({TokenType::COMMA, ","});
                break;

            case '"':
            {
                Advance(); // consume opening quote

                std::string stringContent;

                while (Peek() != '"' && Peek() != '\0')
                    stringContent += Advance();

                if (Peek() == '"')
                    Advance(); // consume closing quote

                out.push_back({TokenType::STRINGLITERAL, stringContent});
                break;
            }


            default:

                Advance();
                break;
        }
    }

    out.push_back({TokenType::END, ""});
    return out;
}