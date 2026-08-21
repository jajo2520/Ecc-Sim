cmake_minimum_required(VERSION 4.0.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Ecc_Sim)

add_executable(${PROJECT_NAME} src/main.cpp)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

add_executable(run_tests tests/main_tests.cpp)
target_include_directories(run_tests PRIVATE src) # so test files can use hamming stuff

enable_testing()
add_test(NAME hamming_tests COMMAND run_tests)
