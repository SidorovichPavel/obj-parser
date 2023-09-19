cmake_minimum_required(VERSION 3.5.0)
project(obj-parser VERSION 0.1.0 LANGUAGES C CXX)

#include(CTest)
#enable_testing()

add_executable(obj-parser main.cpp)

set_property(TARGET obj-parser PROPERTY CXX_STANDARD 23)




set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
