#pragma once
#include <iostream>
class Value {
public:

    std::string name;
    unsigned age;

    Value();

    Value (std::string n, unsigned a = 0);

    Value(const Value & other);

    ~Value();

    Value& operator=(const Value& other);

    bool operator==(const Value& b) const;

};
