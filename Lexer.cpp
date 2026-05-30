//
// Created by Lars on 30-5-2026.
//

#include "Lexer.h"

std::vector<Token> Lexer::Tokenize() {

    std::vector<Token> tokens;

    while (pos < src.size()) {

        char c = src[pos];
        if (isspace(c)) {
            pos++;
        }
        else if (isdigit(c)) {
            tokens.push_back(HandleNumber());
        }
        else if (isalpha(c)) {
            tokens.push_back(HandleIdentifier());
        }
        else
            {
            switch (c)
            {
                case '+':
                    tokens.push_back({TokenType::PLUS, "+"});
                    break;

                case '-':
                    tokens.push_back({TokenType::MINUS, "-"});
                    break;

                case '*':
                    tokens.push_back({TokenType::STAR, "*"});
                    break;

                case '/':
                    tokens.push_back({TokenType::SLASH, "/"});
                    break;

                case '=':
                    tokens.push_back({TokenType::EQUAL, "="});
                    break;
            }
            pos++;
        }
    }
    tokens.push_back({TokenType::END, ""});
    return tokens;
}

Token Lexer::HandleValue() {
    std::string value;
    while (pos < src.size() && isdigit(src[pos])) {
        val += src[pos++];
    }
    return {TokenType::NUMBER, val};
}

Token Lexer::HandleIdentifier() {
    std::string value;
    while (pos < src.size() && isalnum(src[pos])) {
        val += src[pos++];
    }

    if (val == "print")
        return {TokenType::PRINT, val};
}


}



