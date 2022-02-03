#include "MyStack.hpp"
#include "Interpreter_error.hpp"

int MyStack::peek(){
    if (stack.empty()) throw interpreter_error("extracting from an empty stack");
    return stack.top();
}

int MyStack::pop(){
    if (stack.empty()) throw interpreter_error("extracting from an empty stack");
    int r = stack.top();
    stack.pop();
    return r;
}

void MyStack::print(){
    MyStack tmp;
    while (!stack.empty()){
        tmp.push(peek());
        std::cout << pop() << " ";
    }
    while(!tmp.stack.empty()){
        push(tmp.pop());
    }
}

int MyStack::size(){
    return int(stack.size());
}

void MyStack::push(int value){
    stack.push(value);
}
