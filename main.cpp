#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"

int main() {

    std::string filePath = "L--Scripts/script.l--";

    std::cout << "Loading file: " << filePath << "\n";
    std::cout << "Working dir: " << std::filesystem::current_path() << "\n";


    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open file: " << filePath << "\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    std::cout << "File loaded successfully\n";


    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    std::cout << "Lexer complete\n";


    Parser parser(tokens);
    auto ast = parser.parse();

    std::cout << "Parser complete\n";

    Interpreter interp;
    interp.Run(ast);

    std::cout << "Execution complete\n";

    return 0;
}