#include "Value.hpp"
#include <iostream>
Value::Value(){}

Value::Value(std::string n, unsigned a) : name(n), age(a){}

Value::Value(const Value & other) : name(other.name), age(other.age){}

Value::~Value() {}

Value& Value::operator=(const Value& other){
    age = other.age;
    name = other.name;
    return *this;
}

bool Value::operator==(const Value& b) const{
    if (b.age == age && b.name == name) return true;
    return false;
}