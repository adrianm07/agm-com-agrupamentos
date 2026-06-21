cmake_minimum_required(VERSION 3.10)
project(grafo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

set(SOURCE_FILES
    src/Vertice.cpp
    src/Aresta.cpp
    src/Grafo.cpp
    src/GrafoTests.cpp
    src/Grupo.cpp
    src/Menu.cpp
    src/main.cpp
)

add_executable(grafo ${SOURCE_FILES})

set_target_properties(grafo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)