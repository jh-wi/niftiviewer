cmake_minimum_required(VERSION 3.20)

project(
	CMakeMRIviewer
	VERSION 0.1
	DESCRIPTION "Cardiac modeling & visualization software"
)

<<<<<<< HEAD
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

#add_library(glad STATIC ${CMAKE_SOURCE_DIR}/include/glad/glad.h ${CMAKE_SOURCE_DIR}/src/glad.c)
#target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/include)

set(libs libglfw3dll.a glad.c)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(MRIviewer src/main.cpp)
target_link_libraries(MRIviewer libs)
=======
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_executable(MRIviewer src/main.cpp)
>>>>>>> parent of 499602d (Blah)
