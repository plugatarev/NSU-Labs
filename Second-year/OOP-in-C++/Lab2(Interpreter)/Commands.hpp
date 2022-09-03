#pragma once
#include "MyStack.hpp"
//Base class of command
class Command {
 public:
    virtual void apply(MyStack & it, std::stringstream& s) = 0;
};


class Add: public Command {
    //Adding two values from the stack. 
    //The peek of the stack is used as the right operand, and the second number on the stack is used as the left operand.
    //The result of the operation is put on the stack.
    //Exception of interpreter_error if there are not enough elements on the stack
    void apply(MyStack & stack, std::stringstream& s) override;
};

class Sub: public Command {
    //Subtraction two values from the stack. 
    //The peek of the stack is used as the right operand, and the second number on the stack is used as the left operand.
    //The result of the operation is put on the stack.
    //Exception of interpreter_error if there are not enough elements on the stack
    void apply(MyStack & stack, std::stringstream& s) override;
};

class Div: public Command {
    //Division two values from the stack. 
    //The peek of the stack is used as the right operand, and the second number on the stack is used as the left operand.
    //The result of the operation is put on the stack.
    //Exception of interpreter_error if there are not enough elements on the stack. Exception when dividing by zero
    void apply(MyStack & value, std::stringstream& s) override;
};

class Mod: public Command {
    //Taking the remainder of the division.
    //The peek of the stack is used as the right operand, and the second number on the stack is used as the left operand.
    //The result of the operation is put on the stack.
    //Exception of interpreter_error if there are not enough elements on the stack. Exception when dividing by zero
    void apply(MyStack & stack, std::stringstream& s) override;
};

class Mul: public Command {
    //Multiplication two values from the stack. 
    //The peek of the stack is used as the right operand, and the second number on the stack is used as the left operand.
    //The result of the operation is put on the stack.
    //Exception of interpreter_error if there are not enough elements on the stack
    void apply(MyStack & stack, std::stringstream& s) override;
};

class More: public Command {
    //Compares two values from the stack(>).
    //The peek of the stack is used as the right operand, and the second number on the stack is used as the left operand.
    //The result of the operation is put on the stack.
    //Exception of interpreter_error if there are not enough elements on the stack
    void apply(MyStack & stack, std::stringstream& s) override;
};

class Less: public Command {
    //Compares two values from the stack(<).
    //The peek of the stack is used as the right operand, and the second number on the stack is used as the left operand.
    //The result of the operation is put on the stack.
    //Exception of interpreter_error if there are not enough elements on the stack
    void apply(MyStack & stack, std::stringstream& s) override;
};

class Equal: public Command {
    //Compares two values from the stack(==).
    //The peek of the stack is used as the right operand, and the second number on the stack is used as the left operand.
    //The result of the operation is put on the stack.
    //Exception of interpreter_error if there are not enough elements on the stack
    void apply(MyStack & stack, std::stringstream& s) override;
};

class Write: public Command {
    //Printing the peek of the stack
    //Exception of interpreter_error if there are not enough elements on the stack
    void apply(MyStack & stack, std::stringstream& s) override;
};

class Dup: public Command {
    //Copy the peek of the stack and put it on the stack
    void apply(MyStack & stack, std::stringstream& s) override;
};

class Drop: public Command {
    //Remove the peek number from the stack
    void apply(MyStack & stack, std::stringstream& s) override;
};

class Swap: public Command {
    //Swap the peek and second numbers on the stack
    void apply(MyStack & stack, std::stringstream& s) override;
};

class Rot: public Command {
    //Cyclically shift the peek three numbers on the stack
    void apply(MyStack & stack, std::stringstream& s) override;
};

class Over: public Command {
    //Copy the second number and put a copy over the peek one
    void apply(MyStack & stack, std::stringstream& s) override;
};

class Emit: public Command {
    //Print the peek number on the stack as an ascii code and remove it from the stack
    void apply(MyStack & stack, std::stringstream& s) override;
};

class Cr: public Command {
    //Print a line break
    void apply(MyStack & stack, std::stringstream& s) override;
};
