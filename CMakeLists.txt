cmake_minimum_required(VERSION 4.0)
project(L__)

set(CMAKE_CXX_STANDARD 20)

add_executable(L__
        main.cpp
        Lexer.cpp
        Parser.cpp
        Token.cpp
        TokenTypes.cpp
)
