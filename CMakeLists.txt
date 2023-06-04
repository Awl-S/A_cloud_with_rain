cmake_minimum_required(VERSION 3.25)
project(test12)

set(CMAKE_CXX_STANDARD 17)

add_executable(test12 main.cpp)


find_package(GLUT REQUIRED)
target_link_libraries(test12 PRIVATE GLUT::GLUT)

find_package(OpenGL REQUIRED)
target_link_libraries(test12 PRIVATE ${OPENGL_LIBRARIES})
target_include_directories(test12 PRIVATE ${OPENGL_INCLUDE_DIR})