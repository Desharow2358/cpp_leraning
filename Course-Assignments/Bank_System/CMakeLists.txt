cmake_minimum_required(VERSION 3.10)
project(BankSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}/EHsc")
endif()

add_executable(bank main.cpp account.cpp date.cpp user.cpp)
