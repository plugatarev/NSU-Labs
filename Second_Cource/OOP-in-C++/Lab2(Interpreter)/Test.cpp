#include <gtest/gtest.h>
#include "Interpreter.hpp"
#include "Command_Set.hpp"
Interpreter interpreter = Interpreter::getInstance();

void test(std::string input, std::string ExpectedOut){
    std::string out = interpreter.interpret(input).str();
    ASSERT_EQ(out, ExpectedOut);
}

TEST(InterpreterTest, TestWithSomeStrings){
    std::string cmds = ".\"dfdfd\" \"dsfsd\" sdwe";
    test(cmds,"dfdfd\nno such command: '\"dsfsd\"'\n");  
}

TEST(InterpreterTest, TestWithArithmeticOperationWhenTHereIsNotEnoughtOnElementTheStack){
    
    std::string cmds = "1 +";
    test(cmds,"extracting from an empty stack\n");
    
    cmds = "+";
    test(cmds,"extracting from an empty stack\n"); 

    cmds = "1 -";
    test(cmds,"extracting from an empty stack\n"); 

    cmds = "-";
    test(cmds,"extracting from an empty stack\n"); 

    cmds = "1 /";
    test(cmds,"extracting from an empty stack\n"); 

    cmds = "/";
    test(cmds,"extracting from an empty stack\n"); 

    cmds = "1 mod";
    test(cmds,"extracting from an empty stack\n"); 

    cmds = "mod";
    test(cmds,"extracting from an empty stack\n"); 

    cmds = "1 *";
    test(cmds,"extracting from an empty stack\n"); 

    cmds = "*";
    test(cmds,"extracting from an empty stack\n"); 

    cmds = "1 ==";
    test(cmds,"extracting from an empty stack\n"); 

    cmds = "==";
    test(cmds,"extracting from an empty stack\n"); 

    cmds = "1 <";
    test(cmds,"extracting from an empty stack\n"); 

    cmds = "<";
    test(cmds,"extracting from an empty stack\n"); 

    cmds = "1 >";
    test(cmds,"extracting from an empty stack\n"); 

    cmds = ">";
    test(cmds,"extracting from an empty stack\n"); 

    cmds = ".";
    test(cmds,"extracting from an empty stack\n"); 

    cmds = "-";
    test(cmds,"extracting from an empty stack\n"); 

    cmds = "emit";
    test(cmds,"extracting from an empty stack\n"); 

    cmds = "1 2 rot";
    test(cmds,"extracting from an empty stack\n"); 

    cmds = "swap";
    test(cmds,"extracting from an empty stack\n"); 

    cmds = "over";
    test(cmds,"extracting from an empty stack\n"); 

    cmds = "dup";
    test(cmds,"extracting from an empty stack\n"); 
}

TEST(InterpreterTest, CheckingPutAnValueToTheStack){
    std::string cmds = "134 .";  
    test(cmds, "134\n");
}

TEST(InterpreterTest, CheckingPutSomeValuesToTheStack){
    std::string cmds = "134 12 . .";
    test(cmds,"12\n134\n");
}

TEST(InterpreterTest, CheckingPutNegativValueToTheStack){
    std::string cmds = "-134 . ";
    test(cmds,"-134\n");
}

TEST(InterpreterTest, OperationWithNegativValues){
    std::string cmds = "-134 -2 - .";
    test(cmds,"-132\n");
}

TEST(InterpreterTest, CheckingAddingElements){
    std::string cmds = "21 43 + .";
    test(cmds,"64\n");
}

TEST(InterpreterTest, CheckingSubtractionElements){
    std::string cmds = "134 12 - .";
    test(cmds,"122\n");
}

TEST(InterpreterTest, CheckingMultiplicationElements){
    std::string cmds = "2 35 * .";
    test(cmds,"70\n");
}

TEST(InterpreterTest, CheckingDivisionElements){
    std::string cmds = "24 8 / .";
    test(cmds,"3\n");
}

TEST(InterpreterTest, CheckingDivisionByZero){
    std::string cmds = "24 0 / .";
    std::string out = interpreter.interpret(cmds).str();
    ASSERT_EQ(out, "Error: division by zero\n");  
}

TEST(InterpreterTest, CheckingModElements){
    std::string cmds = "24 8 mod .";
    test(cmds,"0\n");
}

TEST(InterpreterTest, CheckingOperatorMoreForElements){
    std::string cmds = "24 8 > .";
    test(cmds,"1\n");

    cmds = "5 8 > .";
    test(cmds,"0\n");
}

TEST(InterpreterTest, CheckingOperatorLessForElements){
    std::string cmds = "24 8 < .";
    test(cmds,"0\n");

    cmds = "5 8 < .";
    test(cmds,"1\n");
}

TEST(InterpreterTest, CheckingOperatorEqualForElements){
    std::string cmds = "24 8 == .";
    test(cmds,"0\n");

    cmds = "5 5 == .";
    test(cmds,"1\n");
}

TEST(InterpreterTest, CheckingDupMethod){
    std::string cmds = "11 dup . .";
    interpreter.interpret(cmds);
    test(cmds,"11\n11\n");    
}

TEST(InterpreterTest, CheckingDropMethod){
    std::string cmds = "11 123 drop .";
    test(cmds,"11\n");   
}

TEST(InterpreterTest, CheckingSwapMethod){
    std::string cmds = "11 4 swap . .";
    test(cmds,"11\n4\n");   
}

TEST(InterpreterTest, CheckingRotMethod){
    std::string cmds = "1 2 3 rot . . .";
    test(cmds,"1\n3\n2\n");
}

TEST(InterpreterTest, WritingToTheConsoleTopTheStack){
    std::string cmds = "12 1214 .";
    test(cmds,"1214\n");
}

TEST(InterpreterTest, CheckingWorkOfMethodEmit){
    std::string cmds = "43 emit";
    test(cmds,"+\n");
}

TEST(InterpreterTest, CheckingWorkOfMethodCr){
    std::string cmds = "1 cr";
    test(cmds,"\n"); 
}

TEST(InterpreterTest, TestNoSuchCommand){
    std::string cmds = "GHrot";
    test(cmds,"no such command: 'GHrot'\n");
}

TEST(InterpreterTest, TestNoSuchCommandWithDot){
    std::string cmds = ".\"";
    test(cmds, "no such command: '.\"'\n");  
}

TEST(InterpreterTest, TestPrintingStringWithSpace){
    std::string cmds = ".\"  sdf  *\"";
    test(cmds ,"  sdf  *\n");  
}

TEST(InterpreterTest, TestWithPrintingEmptyString){
    std::string cmds = ".\"\"";
    test(cmds,"\n");  
}

TEST(InterpreterTest, TestWithThreeQuotesInARow){
    std::string cmds = R"(.""")";
    test(cmds,"\nno such command: '\"'\n");  
}

TEST(InterpreterTest, TestWithStringAndTwoQuotesInTheEnd){
    std::string cmds = R"(."wer"")";
    test(cmds,"wer\nno such command: '\"'\n");  
}

TEST(InterpreterTest, TestWithTwoQuotesInTheBeginning){
    std::string cmds = R"(.""werew")";
    test(cmds,"\nno such command: 'werew\"'\n");  
}

TEST(InterpreterTest, TestWithOnlySpaceInString){
    std::string cmds = "    .\"   \"    ";
    test(cmds,"   \n");  
}

TEST(InterpreterTest, TestWithPrintingStringAndIncorrectString){
    std::string cmds = R"(."ds"f")";
    test(cmds,"ds\nno such command: 'f\"'\n");  
}

TEST(InterpreterTest, TestOnOverflowValue){
    std::string cmds = "1111111111111";
    test(cmds,"stoi\n");  
}

TEST(InterpreterTest, TestOnOverflowValue){
    std::string cmds = " 1111111111111 . ";
    test(cmds,"stoi\n");  
}

TEST(InterpreterTest, TestOnOverflowValue){
    std::string cmds = " 1234 emit ";
    test(cmds,"Overflow ascii code\n");  
}