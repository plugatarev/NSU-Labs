#include <gtest/gtest.h>
#include "HashTable.hpp"
#include <ctime> 
TEST(HashTableTest, CheckingMethodSize){
    HashTable a(17);
    std::string s1 = "fafasc", s2 = "jkl", s3 = "ghj", s4 = "yuiuy";
    a.insert(s1,Value("git12",1));
    a.insert(s2,Value("erw",23));
    a.insert(s3,Value("erw2",33));
    a.insert(s4,Value("git12",1));
    ASSERT_EQ(a.size(), 4);
}

TEST(HashTableTest,DefaultConstructor){
    HashTable a;
    ASSERT_EQ(a.size(), 0);
    ASSERT_EQ(a.capacity(), 16);
}

TEST(HashTableTest,SizeConstructor){
    HashTable a(12);
    ASSERT_EQ(a.capacity(), 12);
    ASSERT_EQ(a.size(), 0);
}

TEST(HashTableTest, InsertInHashtable){
    HashTable a(100);
    Key k = "1234";
    Key name = "abrac";
    Value v(name,22);
    a.insert(k,v);
    ASSERT_EQ(a.size(),1);
    ASSERT_EQ(a.at(k),v);

    std::string s1 = "fafasc", s2 = "jkl", s3 = "ghj";
    a.insert(s1,Value("erw",23));
    a.insert(s2,Value("erw2",33));
    a.insert(s3,Value("git12",1));
    ASSERT_EQ(a.size(),4);
}

TEST(HashTableTest, InsertInHashtableWithEqualKeys){
    HashTable a(100);
    Key k = "1234";
    Key name = "abrac";
    Value v(name,22);
    Key k2 = "1234";
    Key name2 = "qqeq";
    Value v2(name,22);

    a.insert(k,v);
    a.insert(k2, v2);
    ASSERT_EQ(a.size(),1);
    ASSERT_EQ(a.at(k),v2);
}

TEST(HashTableTest, InsertInHashtableWithOverflowHashTable){
    HashTable a(10);
    std::string s1 = "fafasc", s2 = "jkl", s3 = "ghj", s4 = "ghsdj", s5 = "23423", s6 = "676", s7 = "kll", s8 = "hnn", s9 = "[p[]]";
    a.insert(s1,Value("erw",23));
    a.insert(s2,Value("erw2",33));
    a.insert(s3,Value("git12",1));
    a.insert(s4,Value("git12",1));
    a.insert(s5,Value("git12",1));
    a.insert(s6,Value("git12",1));
    a.insert(s7,Value("git12",1));
    a.insert(s8,Value("git12",1));
    a.insert(s9,Value("git12",1));
    ASSERT_EQ(20,a.capacity());
    ASSERT_EQ(9,a.size());
}

TEST(Test_HashTable, OriginalHashTableIsNotChangedOnOperatorAssign){
    HashTable a(12);
    Key k = "1234";
    Key name = "abrac";

    Value v(name,22);
    a.insert(k,v);
    ASSERT_EQ(a.size(), 1);
    ASSERT_EQ(a.at(k),v);
    HashTable b;
    b = a;
    ASSERT_EQ(b.size(), 1);
    ASSERT_EQ(a.at(k),v);
  
    Key name2 = "hey";

    Value v2(name2,34);
    b.insert(name2, v2);
    ASSERT_EQ(a.size(), 1);
    ASSERT_EQ(a.at(k),v);
    ASSERT_EQ(b.size(), 2);
    ASSERT_EQ(b.at(k),v);
    ASSERT_EQ(b.at(name2),v2);
}

TEST(Test_HashTable, OperatorAssignForEmpryHashTable){
    HashTable a(12);
    HashTable b;
    b = a;
    ASSERT_EQ(a.size(), 0);
    ASSERT_EQ(b.size(), 0);
    ASSERT_EQ(a.capacity(),12);
    ASSERT_EQ(b.capacity(), 16);
}

TEST(Test_HashTable, NewHashTableDoesNotChangeWhenTheOlIsChanged){
    HashTable a(12);
    std::string s1 = "fafasc", s2 = "jkl", s3 = "ghj";
    a.insert(s1,Value("erw",23));
    a.insert(s2,Value("erw2",33));
    a.insert(s3,Value("git12",1));
    HashTable b;
    b = a;
    ASSERT_EQ(a.size(), 3);
    ASSERT_EQ(b.size(), 3);
    Key k = "1234";
    Key name = "abrac";
    Value v(name,22);
    a.insert(k,v);
    ASSERT_EQ(a.size(), 4);
    ASSERT_EQ(b.size(), 3);
}

TEST(Test_HashTable, OriginalHashTableIsNotChangedOnCopyChangeOperatorAssign){
    HashTable a(12);
    Key k = "1234";
    Key name = "abrac";

    Value v(name,22);
    a.insert(k,v);
    ASSERT_EQ(a.size(), 1);
    ASSERT_EQ(a.at(k),v);
    HashTable b(a);
    ASSERT_EQ(b.size(), 1);
    ASSERT_EQ(a.at(k),v);
    Key name2 = "hey";

    Value v2(name2,34);
    b.insert(name2, v2);
    ASSERT_EQ(a.size(), 1);
    ASSERT_EQ(a.at(k),v);
    ASSERT_EQ(b.size(), 2);
    ASSERT_EQ(b.at(k),v);
}

TEST(Test_HashTable, NewHashTableDoesNotChangeWhenTheOlIsChangedCopyConstructor){
    HashTable a(12);
    std::string s1 = "fafasc", s2 = "jkl", s3 = "ghj";
    a.insert(s1,Value("erw",23));
    a.insert(s2,Value("erw2",33));
    a.insert(s3,Value("git12",1));
    HashTable b(a);
    ASSERT_EQ(a.size(), 3);
    ASSERT_EQ(b.size(), 3);
    Key k = "1234";
    Key name = "abrac";
    Value v(name,22);
    a.insert(k,v);
    ASSERT_EQ(a.size(), 4);
    ASSERT_EQ(b.size(), 3);
}

TEST(Test_HashTable, WorkOfTheOperatorGettingByKeyWhenTheElementExists){
    Key k = "SDFSD";
    Value v;
    v.name = "Obeme";
    v.age = 54;
    HashTable a;
    a.insert(k,v);
    std::string s = "wewq";
    a.insert(s, Value("324",23));
    ASSERT_EQ(a[k],v);   
}

TEST(Test_HashTable, WorkOfTheOperatorGettingByKeyWhenTheElementNotExists){
    Key k = "SDFSD";
    Value v;
    v.name = "Obeme";
    v.age = 54;
    HashTable a;
    a.insert(k,v);
    a.insert("wewq", Value("324",23));
    ASSERT_EQ(a.size(),2);
    Key newk = "!@$%";
    a[newk];
    ASSERT_EQ(a.size(),3);
}

TEST(Test_HashTable, WorkOfTheOperatorEqualForEmptyHashTable){
    HashTable a;
    HashTable b;
    ASSERT_TRUE(a == b);
}

TEST(Test_HashTable, WorkOfTheOperatorEqualForEqualHashTable){
    HashTable a;
    std::string s1 = "fafasc", s2 = "jkl", s3 = "ghj";
    a.insert(s1,Value("erw",23));
    a.insert(s2,Value("erw2",33));
    a.insert(s3,Value("git12",1));
    HashTable b(a);
    ASSERT_TRUE(a == b);
}

TEST(Test_HashTable, WorkOfTheOperatorEqualForNotEqualHashTable){
    HashTable a;
    std::string s1 = "fafasc", s2 = "jkl", s3 = "ghj";
    a.insert(s1,Value("erw",23));
    a.insert(s2,Value("erw2",33));
    a.insert(s3,Value("git12",1));
    HashTable b(a);
    Key newk = "!@$%";
    b.insert(newk,Value());
    ASSERT_FALSE(a == b);
}

TEST(Test_HashTable, WorkOfTheOperatorEqualOneHashTableIsEmpty){
    HashTable a;
    std::string s1 = "fafasc", s2 = "jkl", s3 = "ghj";
    a.insert(s1,Value("erw",23));
    a.insert(s2,Value("erw2",33));
    a.insert(s3,Value("git12",1));
    HashTable b;
    ASSERT_FALSE(a == b);
}
////////////////////////////////////////////////////////////////////////////////////
TEST(Test_HashTable, WorkOfTheOperatorNotEqualForEmptyHashTable){
    HashTable a;
    HashTable b;
    ASSERT_FALSE(a != b);
}

TEST(Test_HashTable, WorkOfTheOperatorNotEqualForEqualHashTable){
    HashTable a;
    std::string s1 = "fafasc", s2 = "jkl", s3 = "ghj";
    a.insert(s1,Value("erw",23));
    a.insert(s2,Value("erw2",33));
    a.insert(s3,Value("git12",1));
    HashTable b(a);
    ASSERT_FALSE(a != b);
}

TEST(Test_HashTable, WorkOfTheOperatorNotEqualForNotEqualHashTable){
    HashTable a;
    std::string s1 = "fafasc", s2 = "jkl", s3 = "ghj";
    a.insert(s1,Value("erw",23));
    a.insert(s2,Value("erw2",33));
    a.insert(s3,Value("git12",1));
    HashTable b(a);
    Key newk = "!@$%";
    b.insert(newk,Value());
    ASSERT_TRUE(a != b);
}

TEST(Test_HashTable, WorkOfTheOperatorNotEqualOneHashTableIsEmpty){
    HashTable a;
    std::string s1 = "fafasc", s2 = "jkl", s3 = "ghj";
    a.insert(s1,Value("erw",23));
    a.insert(s2,Value("erw2",33));
    a.insert(s3,Value("git12",1));
    HashTable b;
    ASSERT_TRUE(a != b);
}

TEST(Test_HashTable, WorkOfTheSwapForRandomNotEmptyHashTables){
    HashTable a;
    HashTable b;
    std::string s1 = "fafasc", s2 = "jkl", s3 = "ghj";
    a.insert(s1,Value("erw",23));
    a.insert(s2,Value("erw2",33));
    a.insert(s3,Value("git12",1));

    s1 = "yuiuy", s2 = "qqqq", s3 = "rrrr";
    b.insert(s1,Value("sd",23));
    b.insert(s2,Value("jkl",3));
    b.insert(s3,Value("er",14));

    HashTable tmp_a(a);
    HashTable tmp_b(b);
    a.swap(b);
    ASSERT_TRUE(a == tmp_b);
    ASSERT_TRUE(b == tmp_a);
}   

TEST(Test_HashTable, WorkOfTheSwapOneHashTableIsEmpty){
    HashTable a;
    HashTable b;
    std::string s1 = "fafasc", s2 = "jkl", s3 = "ghj";
    a.insert(s1,Value("erw",23));
    a.insert(s2,Value("erw2",33));
    a.insert(s3,Value("git12",1));
    HashTable tmp_a(a);
    HashTable tmp_b(b);
    a.swap(b);
    ASSERT_TRUE(a == tmp_b);
    ASSERT_TRUE(b == tmp_a);
}   

TEST(Test_HashTable, WorkOfTheSwapHashTablesAreEmpty){
    HashTable a;
    HashTable b;
    HashTable tmp_a(a);
    HashTable tmp_b(b);
    a.swap(b);
    ASSERT_TRUE(a == tmp_b);
    ASSERT_TRUE(b == tmp_a);
} 

TEST(Test_HashTable, WorkOfTheSwapHashTablesFor){
    HashTable a;
    HashTable b;
    HashTable tmp_a(a);
    HashTable tmp_b(b);
    a.swap(b);
    ASSERT_TRUE(a == tmp_b);
    ASSERT_TRUE(b == tmp_a);
} 


TEST(Test_HashTable, WorkOfTheSwapHashTablesCheckingForChangesHashTables){
    HashTable a;
    HashTable b;
    std::string s1 = "fafasc", s2 = "jkl", s3 = "ghj";
    a.insert(s1,Value("erw",23));
    a.insert(s2,Value("erw2",33));
    a.insert(s3,Value("git12",1));
    s1 = "qazqa", s2 = "aqtg", s3 = "nm,n";
    b.insert(s1,Value("asd",23));
    b.insert(s2,Value("jhk",33));
    b.insert(s3,Value("zxcds",1));
    HashTable tmp_a(a);
    HashTable tmp_b(b);
    a.swap(b);
    ASSERT_TRUE(a == tmp_b);
    ASSERT_TRUE(b == tmp_a);
    Key k = "1234";
    Key name = "abrac";
    Value v(name,22);
    a.insert(k,v);
    ASSERT_EQ(a.size(),4);
    ASSERT_EQ(b.size(),3);
    ASSERT_TRUE(a.contains(k));
    ASSERT_FALSE(b.contains(k));
} 

TEST(Test_HashTable,CheckingOnTableEmptiness){
    HashTable a;
    ASSERT_TRUE(a.empty());
    std::string k = "wewq";
    a.insert(k, Value("324",23));
    ASSERT_FALSE(a.empty());
}

TEST(Test_HashTable,CheckinGMethodContaintsTheTable){
    HashTable a(12);
    Key k = "1234";
    Key name = "abrac";

    Value v(name,22);
    a.insert(k,v);
    std::string k1 = "wewq";
    a.insert(k1, Value("324",23));
    ASSERT_TRUE(a.contains(k));
    Key n = "!";
    ASSERT_FALSE(a.contains(n));
}

TEST(Test_HashTable,CheckinGMethodContaintsTheTableForSomeKeys){
    HashTable a(12);
    Key k = "1234";
    Key name = "abrac";
    Key kt = k;
    Key namet = name;
    Value v(name,22);
    for (int i = 0; i < 6; i++){
        k+=1;
        name+=1;
        a.insert(k,v);
    }

    for (int i = 0; i < 6; i++){
        kt+=1;
        namet+=1;
        ASSERT_TRUE(a.contains(k));
    } 
}

TEST(Test_HashTable,CheckinGMethodContaintsForEmptyTable){
    HashTable a(12);
    Key k = "1234";
    ASSERT_FALSE(a.contains(k));
}

TEST(Test_HashTable,CheckingMethodClearTheTable){
    HashTable a(12);
    Key k = "1234";
    Key name = "abrac";

    Value v(name,22);
    a.insert(k,v);
    std::string s1 = "fafasc", s2 = "jkl";
    a.insert(s1,Value("erw",23));
    a.insert(s2,Value("erw2",33));
    a.clear();
    ASSERT_EQ(a.capacity(),12);
    ASSERT_EQ(a.size(),0);
    ASSERT_FALSE(a.contains(k));
}

TEST(Test_HashTable,CheckingMethodClearForEmptyTable){
    HashTable a(12);
    a.clear();
    ASSERT_EQ(a.capacity(),12);
    ASSERT_EQ(a.size(),0);
}

TEST(Test_HashTable,CheckingMethodEraseTheTable){
    HashTable a;
    Key k = "!@#";
    Key name = "abrac";

    Value v(name,22);
    a.insert(k,v);
    std::string s1 = "fafasc", s2 = "jkl", s3 = "ghj";
    a.insert(s1,Value("erw",23));
    a.insert(s2,Value("erw2",33));
    a.insert(s3,Value("git12",1));
    ASSERT_TRUE(a.contains(k));
    a.erase(k);
    ASSERT_EQ(a.size(),3);
    ASSERT_FALSE(a.contains(k));
}

TEST(Test_HashTable,CheckingMethodEraseForEmptyTable){
    HashTable a;
    Key k = "erase";
    a.erase(k);
    ASSERT_EQ(a.size(),0);
}

TEST(Test_HashTale,CheckingMethodAtTheTableWhenTheElementIsNotExists){
    HashTable a;
    std::string k1 = "wewq";
    a.insert(k1, Value("324",23));
    Key k = "!@#$#";
    try{
    	a.at(k);
    }
    catch (std::exception  &e) {
        EXPECT_EQ(e.what(), (std::string )"no such element exists");
    }

}

TEST(Test_HashTable,CheckingMethodAtForEmptyTableWhenTheElementIsNotExists){
    HashTable a;
    Key k = "abrac";
    try{
    	a.at(k);
    }
    catch (std::exception  &e) {
        EXPECT_EQ(e.what(), (std::string )"no such element exists");
    }

}

TEST(Test_HashTable,CheckingMethodAtTheTableWhenTheElementExists){
    HashTable a;
    std::string k2 = ";io;6";
    a.insert(k2, Value("324",23));
    Key k = "1234";
    Key name = "abrac";
    Value v(name,22);
    a.insert(k,v);
    ASSERT_EQ(a.at(k),v);
}

TEST(Test_HashTable,OperatorNotEqualForHTWithDifferentCapacity){
    HashTable a(15);
    HashTable b(16);
    std::string s1 = "fafasc", s2 = "jkl", s3 = "ghj";
    a.insert(s1,Value("erw",23));
    a.insert(s2,Value("erw2",33));
    a.insert(s3,Value("git12",1));
    
    b.insert(s1,Value("erw",23));
    b.insert(s2,Value("erw2",33));
    b.insert(s3,Value("git12",1));


    ASSERT_FALSE(a != b);
}

TEST(Test_HashTable,AddingTwoValuesWithTheSameKeys){
    HashTable a(15);
    std::string s1 = "fafasc", s2 = "jkl", s3 = "fafasc";
    a.insert(s1,Value("erw",23));
    a.insert(s2,Value("erw2",33));
    a.insert(s3,Value("git12",1));

    ASSERT_EQ(a.at(s1),Value("git12",1));
}