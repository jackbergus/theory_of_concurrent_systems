cmake_minimum_required(VERSION 3.20)
project(hml)

include_directories(include)

set(CMAKE_CXX_STANDARD 20)

add_library(hmllib SHARED src/hml/hml_formula.cpp include/hml/hml_formula.h src/hml/system.cpp include/hml/system.h src/hml/Rel.cpp include/hml/Rel.h src/hml/LTS.cpp include/hml/LTS.h src/hml/semantics.cpp include/hml/semantics.h include/hml/hml.h)
add_executable(hml main_hml.cpp )
target_link_libraries(hml hmllib)

add_executable(ccs main_ccs.cpp)
add_library(ccslib SHARED src/ccs/act.cpp src/ccs/ccs.cpp src/ccs/generate_lts.cpp)
target_link_libraries(ccs ccslib)
