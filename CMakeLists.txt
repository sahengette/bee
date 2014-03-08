cmake_minimum_required(VERSION 2.6)

project(bbe)

Find_Package(GLEW REQUIRED)
Find_Package(GLFW3 REQUIRED)

if ( NOT GLEW_FOUND )
    message(FATAL_ERROR "GLEW introuvable")
endif ( NOT GLEW_FOUND )

if ( NOT GLFW3_FOUND )
    message(FATAL_ERROR "GLFW3 introuvable")
endif ( NOT GLFW3_FOUND )

include_directories(${GLEW_INCLUDE_DIR})

include_directories(${GLFW3_INCLUDE_DIR})


file(
GLOB_RECURSE
source_files
src/*
)

# Déclaration de l'exécutable
add_executable(
    bin/bin
    ${source_files}
)
target_link_libraries(
    bin/bin
    -lglfw3
    ${GLEW_LIBRARY}
    -lGL
    -lX11 
    -lXxf86vm 
    -lXrandr 
    -lpthread 
    -lXi
     
)
if(CMAKE_COMPILER_IS_GNUCXX)
     set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -Wshadow -std=c++0x")
endif()
