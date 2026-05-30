#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "Lexer.h"
#include "Parser.h"

int main() {

    std::string filePath = "L--Scripts/script.l--";

    std::cout << "Trying to open: " << filePath << std::endl;
    std::cout << "CWD: " << std::filesystem::current_path() << std::endl;

    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filePath << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string code = buffer.str();

    Lexer lexer(code);
    auto tokens = lexer.Tokenize();

    std::cout << "Lexer done" << std::endl;
    Parser parser(tokens);
    parser.Run();
    std::cout << "Parser done" << std::endl;

    return 0;
}