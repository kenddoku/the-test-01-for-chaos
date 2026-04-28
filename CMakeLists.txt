cmake_minimum_required(VERSION 3.10)

#project name 
project(chaos01)

#setting cpp standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#include header files
add_library(chaos_lib 
    src/ChaosAnalyzer.cpp
    src/MathHelper.cpp
    src/ResultExporter.cpp
    src/KWithIndex.cpp
)
target_include_directories(chaos_lib PUBLIC include)

#create executable
add_executable(logistic_map examples/logistic_map.cpp)
add_executable(dummy_period_chaos examples/dummy_period_chaos.cpp)

#link library to executable
target_link_libraries(logistic_map chaos_lib)
target_link_libraries(dummy_period_chaos chaos_lib)