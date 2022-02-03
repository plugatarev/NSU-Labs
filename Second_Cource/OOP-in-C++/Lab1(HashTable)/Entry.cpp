#include "Entry.hpp"

Entry::Entry(const Entry& other): key(other.key), value(other.value){}

Entry::Entry(Key& k, Value& v, Entry* next): key(k), value(v), next(next){}

Entry::~Entry(){}

bool Entry::operator==(const Entry& p){
    if (key == p.key && value == p.value) return true;
    return false;
}

