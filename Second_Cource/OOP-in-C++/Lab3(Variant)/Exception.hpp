#pragma once
#include <iostream>
class variant_access_error: public std::runtime_error {
 public:
    explicit variant_access_error(const std::string& msg): std::runtime_error(msg) {}
};