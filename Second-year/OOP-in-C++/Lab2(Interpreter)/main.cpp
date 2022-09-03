#include"Interpreter.hpp"
#include <iostream>
int main(){
    Interpreter interpreter = Interpreter::getInstance();
    std::string cmds;

    while (true){
        std::cout << std::endl;
        std::cout << "> ";
        std::getline(std::cin, cmds);
        std::cout << interpreter.interpret(cmds).str();
    }
    return 0;
}
