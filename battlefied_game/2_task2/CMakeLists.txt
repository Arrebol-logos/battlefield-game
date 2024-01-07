cmake_minimum_required(VERSION 3.22)
project(battlefield)

set(CMAKE_CXX_STANDARD 14)
set(INC_DIR "C:/Users/Arrebol/Desktop/cs1604_lib/src/cs1604/include")
set(LINK_DIR "C:/Users/Arrebol/Desktop/cs1604_lib/src/cs1604/lib")

include_directories(${INC_DIR})
link_directories(${LINK_DIR})

add_executable(battlefield
        actions.cpp
        actions.h
        algorithms.cpp
        algorithms.h
        engine.cpp
        engine.h
        field.cpp
        field.h
        main.cpp
        units.cpp
        units.h)

target_link_libraries(battlefield libCS1604.a)
