cmake_minimum_required(VERSION 3.16.3)
project(Interpreter VERSION 0.1)
 
set(SRC main.cpp Commands.cpp Interpreter.cpp MyStack.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Interpreter ${SRC})

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  InterpreterTest
  Test.cpp
  Commands.cpp
  Interpreter.cpp
  MyStack.cpp
)

target_link_libraries(
  InterpreterTest
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(InterpreterTest)