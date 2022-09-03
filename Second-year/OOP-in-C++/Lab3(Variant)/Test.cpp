#include <gtest/gtest.h>
#include "variant.hpp"

MyVariant::variant<double,int> var(int val){
    MyVariant::variant<double,int> a(val);
    return a;
}

class A{
public:
    A(int a):code(a){}
    int code;
};

TEST(VariantTest, CreateEmptyVariant){
    MyVariant::variant<double, int> a();    
}

TEST(VariantTest, CreateVariantWithString){
    std::string s = "sdf1232";
    MyVariant::variant<double, int, float, std::string> a(s);   
    ASSERT_EQ(a.get<std::string>(), s); 
}

TEST(VariantTest, CreateOnBaseLValueWithATypeThatCanBeStored){
    int value = 12321321;
    MyVariant::variant<double, int> a(value);   
    ASSERT_EQ(a.get<int>(), 12321321); 
}

TEST(VariantTest, CreateOnBaseRValueWithATypeThatCanBeStored){
    MyVariant::variant<double, int> a(12);   
    ASSERT_EQ(a.get<int>(), 12); 
}

TEST(VariantTest, CreateOnBaseRValueWithATypeForMoveConstructorThatCanBeStored){
    MyVariant::variant<double, int> a(var(2));   
    ASSERT_EQ(a.get<int>(), 2); 
}

TEST(VariantTest, CreateOnBaseRValueWithATypeForOperatorAssignThatCanBeStored){
    MyVariant::variant<short, class A> a(A(123));  
    MyVariant::variant<short, class A> b = std::move(a);
    ASSERT_EQ(b.get<class A>().code, 123); 
}

TEST(VariantTest, CopyConstructor){
    MyVariant::variant<double, int> a(12.12); 
    MyVariant::variant<double, int> acopy(a);
    ASSERT_EQ(a.get<double>(), 12.12);
    ASSERT_EQ(acopy.get<double>(), 12.12);
}

TEST(VariantTest, GetValueWithOtherType){
    MyVariant::variant<double, int> a(12.12); 
    try{
        a.get<int>();
    }
    catch(variant_access_error& er){
        std::string s = "This type isn't stored in this variant";
        ASSERT_EQ(er.what(), s);
    }
}

TEST(VariantTest, OriginalVariantIsNotChangedOnOperatorAssign){
    int* f = new int(1);
    MyVariant::variant<double, int, int*> a(12.12); 
    MyVariant::variant<double, int, int*> b = a;
    ASSERT_EQ(a.get<double>(), 12.12);
    ASSERT_EQ(b.get<double>(), 12.12);
    b = 12321;
    ASSERT_EQ(a.get<double>(), 12.12);
    ASSERT_EQ(b.get<int>(), 12321);
    a = f;
    ASSERT_EQ(*(a.get<int*>()), 1);
    ASSERT_EQ(b.get<int>(), 12321);
    delete f;
}

TEST(VariantTest, OperatorAssign){
    MyVariant::variant<double, int> a(12.12); 
    a = var(1212);
    try{
        a.get<double>();
    }
    catch(variant_access_error& er){
        std::string s = "This type isn't stored in this variant";
        ASSERT_EQ(er.what(), s);
    }
    ASSERT_EQ(a.get<int>(), 1212);
}

TEST(VariantTest, MethodGetIf){
    MyVariant::variant<double, int, class A> a(A(54)); 
    ASSERT_EQ(a.get_if<double>(), nullptr);
    ASSERT_EQ(a.get_if<A>()->code, 54);
}

TEST(VariantTest, AllMethods){
    MyVariant::variant<double, int> a(std::move(var(12)));
    MyVariant::variant<double, int> ac(1.234);
    ASSERT_EQ(a.get<int>(), 12);
    ASSERT_EQ(ac.get<double>(), 1.234);
    ac = std::move(var(213));
    ASSERT_EQ(ac.get<int>(), 213);
    ASSERT_EQ(a.get_if<double>(), nullptr);
    MyVariant::variant<double, int> b(a);
    ASSERT_EQ(b.get<int>(), 12);
    MyVariant::variant<double, int> e;
    e = 123213;
    e = 32.21;
    ASSERT_EQ(e.get<double>(), 32.21);
    ac = e;
    ASSERT_EQ(ac.get<double>(), 32.21);
}



    