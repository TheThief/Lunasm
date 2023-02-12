cmake_minimum_required(VERSION 3.24)

project(Lunasm)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/fmtlib/)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/cxxopts/)

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/)

include_directories(
	${PROJECT_SOURCE_DIR}/src/include/Lexer
)

add_executable(${PROJECT_NAME} 
	${SOURCE_DIR}/Lexer.cpp
	${SOURCE_DIR}/main.cpp
)

target_include_directories(${PROJECT_NAME} 
	PUBLIC external/fmtlib/include
	PUBLIC external/cxxopts/include
)

target_link_directories(${PROJECT_NAME}
	PRIVATE external/fmtlib
	PRIVATE external/cxxopts
)

target_link_libraries(${PROJECT_NAME} fmt cxxopts)
