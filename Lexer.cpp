// Lexer.cpp
#include "Lexer.h"
#include <cctype>

Lexer::Lexer(std::string s) : src(std::move(s)) {}

char Lexer::peek() { return i < src.size() ? src[i] : '\0'; }

char Lexer::get(){ return src[i++]; }

void Lexer::skip() {
    while (isspace(peek())) get();
}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> out;

    while (i < src.size())
    {
        skip();
        char c = peek();

        if (isalpha(c)) {
            std::string word;

            while (isalnum(peek())) word += get();

            if (word == "int")
                out.push_back({TokenType::INT, word});
            else if (word == "print")
                out.push_back({TokenType::PRINT, word});
            else
                out.push_back({TokenType::IDENT, word});
        }
        else if (isdigit(c))
        {
            std::string num;
            while (isdigit(peek())) num += get();
            out.push_back({TokenType::NUMBER, num});
        }
        else
        {
            switch (c)
            {
                case '=':
                    get(); out.push_back({TokenType::EQUAL, "="});
                    break;
                case ';':
                    get(); out.push_back({TokenType::SEMI, ";"});
                    break;
                default:
                    get();
                    break;
            }
        }
    }

    out.push_back({TokenType::END, ""});
    return out;
}