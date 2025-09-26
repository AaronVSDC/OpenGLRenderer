cmake_minimum_required(VERSION 3.16) # FetchContent is stable from 3.14+

project(OpenGLProject_CMake LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# ----------------------------------------------------------------------------------
# SDL3
# ----------------------------------------------------------------------------------
FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.2.22
) 
FetchContent_MakeAvailable(SDL3)

# ----------------------------------------------------------------------------------
# GLAD
# ----------------------------------------------------------------------------------
add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

# ----------------------------------------------------------------------------------
# Executable: include all files needed over here
# ----------------------------------------------------------------------------------
add_executable(OpenGLProject_CMake
    main.cpp
)

# Link SDL3 and GLAD
target_link_libraries(OpenGLProject_CMake
    PRIVATE
        SDL3::SDL3
        glad
)

#Copy SDL3.dll next to the executable after build
add_custom_command(TARGET OpenGLProject_CMake POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL3::SDL3>
    $<TARGET_FILE_DIR:OpenGLProject_CMake>)

