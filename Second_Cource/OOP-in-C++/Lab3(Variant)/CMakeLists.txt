cmake_minimum_required(VERSION 3.16.3)
project(Variant VERSION 0.1)
 
set(SRC main.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#AddressSanitizer 
#set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -static-libasan -g")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address -static-libasan -g")

add_executable(Variant ${SRC})

#Google Tests
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
  VariantTest
  Test.cpp
)

target_link_libraries(
  VariantTest
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(VariantTest)