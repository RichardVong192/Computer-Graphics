cmake_minimum_required(VERSION 2.8)

project(Assignment_1)

add_executable(Assignment-1.out Assignment-1.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries( Yard.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
