#include<iostream>
#include <functional>
#include "Interpreter.hpp"
#include "Interpreter_error.hpp"
#include "Command_Set.hpp"

void Add::apply(MyStack & stack, std::stringstream& s){
    int right = stack.pop();
    int left = stack.pop();
    int res = left + right;
    stack.push(res);
}   

void Sub::apply(MyStack & stack, std::stringstream& s){
    int right = stack.pop();
    int left = stack.pop();
    int res = left - right;
    stack.push(res);
}

void Div::apply(MyStack & value, std::stringstream& s){
    if (value.peek() == 0){
        throw interpreter_error("Error: division by zero");
    }
    int right = value.pop();
    int left = value.pop();
    int res = left / right;
    value.push(res);
}

void Mod::apply(MyStack & stack, std::stringstream& s){
    if (stack.peek() == 0){
        throw interpreter_error("Error: division by zero");
    }
    int right = stack.pop();
    int left = stack.pop();
    int res = left % right;
    stack.push(res);
}

void Mul::apply(MyStack & stack, std::stringstream& s){
    int right = stack.pop();
    int left = stack.pop();
    int res = left * right;
    stack.push(res);
}

void More::apply(MyStack & stack, std::stringstream& s){
    int right = stack.pop();
    int left = stack.pop();
    stack.push(left > right);
}

void Less::apply(MyStack & stack, std::stringstream& s){
    int right = stack.pop();
    int left = stack.pop();
    stack.push(left < right);
}

void Write::apply(MyStack & stack, std::stringstream& s){
    s << stack.pop() << "\n";
}

void Dup::apply(MyStack & stack, std::stringstream& s){
    stack.push(stack.peek());
}

void Drop::apply(MyStack & stack, std::stringstream& s){
    stack.pop();
}

void Swap::apply(MyStack & stack, std::stringstream& s){
    int first = stack.pop();
    int second = stack.pop();
    stack.push(first);
    stack.push(second);
}

void Equal::apply(MyStack & stack, std::stringstream& s){
    int first = stack.pop();
    int second = stack.pop();
    stack.push(first == second);
}

void Rot::apply(MyStack & stack, std::stringstream& s){
    int first = stack.pop();
    int second = stack.pop();
    int third = stack.pop();
    stack.push(second);
    stack.push(first);
    stack.push(third);
}

void Over::apply(MyStack & stack, std::stringstream& s){
    int first = stack.pop();
    int second = stack.peek();
    stack.push(first);
    stack.push(second);    
}

void Emit::apply(MyStack & stack, std::stringstream& s){
    int r = stack.pop();
    if (r < 0 || r > 255) throw interpreter_error("Oveflow ascii code");
    s << char(r) << "\n";
}

void Cr::apply(MyStack & stack, std::stringstream& s){
    s << "\n";
}