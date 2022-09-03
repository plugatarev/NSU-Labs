#include "variant.hpp"
#include <variant>
class A{};

MyVariant::variant<double,int> var(){
    MyVariant::variant<double,int> a(12);
    return a;
}

int main(){

    std::string a2 = "3234";
    MyVariant::variant<double, int, float, short, char, std::string> a(123);
    std::cout << a.get<int>() << std::endl;
    a = 1;
    std::cout << "Hello\n";
    std::cout << a.get<int>() << std::endl;
    MyVariant::variant<double, int, float, short, char, std::string> b(a);
    std::cout << b.get<int>() << std::endl;
    b = 200;
    std::cout << a.get<int>() << std::endl;
    std::cout << b.get<int>() << std::endl;
    a = 100;
    std::cout << a.get<int>() << std::endl;
    std::cout << b.get<int>() << std::endl;
    // a = 21321.1;
    // std::cout << a.get<double>() << std::endl;
    // MyVariant::variant<double, int> bb(std::move(a));
    // bb.get<int>();
    return 0;
}