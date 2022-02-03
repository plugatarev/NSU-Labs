#pragma once
#include <sstream>
#include "Commands.hpp"
#include <map>
#include "MyStack.hpp"
#include<memory>
#include<iostream> //delete

class Interpreter {
public:
   //Creates Interpreter
   static Interpreter & getInstance() {
      static Interpreter i;
      return i;
   }
   //Runs Interpreter
   std::stringstream interpret(const std::string & cmds);

   //Adds new command
   bool registerCreator(const std::string& c, std::unique_ptr<Command> && command) {
      _creators[c] = std::move(command);
      return true;
   }

   Interpreter(Interpreter& other);
private:
   Interpreter() = default;
   std::map<std::string, std::unique_ptr<Command>> _creators;
   std::unique_ptr<Command>* get_cmd(std::string::const_iterator & it, std::string::const_iterator & end, std::stringstream& str);
   MyStack value;
};
