cmake_minimum_required(VERSION 3.29)
project(LinkBankFinal)

set(CMAKE_CXX_STANDARD 20)

add_executable(LinkBankFinal main.cpp
        Account.cpp
        Account.h
        Savings.cpp
        Savings.h
        Interest.cpp
        Interest.h
        InterestEarning.h
)
