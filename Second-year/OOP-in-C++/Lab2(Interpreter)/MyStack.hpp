#pragma once
#include <stack>
class MyStack{
public:
  //Returns peek of the stack and remove from the stack
  int pop();

  //Prints the stack 
  void print();

  //Returns number of elements on the stack
  int size();

  int peek();

  void push(int value);
private:
  std::stack<int> stack;
};