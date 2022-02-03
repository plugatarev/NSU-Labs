#pragma once
#include "Value.hpp"
typedef std::string Key;

struct Entry{
    Key key;
    Value value;
    Entry* next = nullptr;

    Entry(const Entry& other);

    Entry(Key& k, Value& v, Entry* next = nullptr);

    ~Entry();

    bool operator==(const Entry& p);
};