cmake_minimum_required(VERSION 3.16)

project(OpenGLProject_CMake)

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
add_library(glad STATIC glad/src/glad.c )
target_include_directories(glad PUBLIC glad/include)

# ----------------------------------------------------------------------------------
# stb (header-only)
# ----------------------------------------------------------------------------------
FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master   
)
FetchContent_MakeAvailable(stb)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})


# ----------------------------------------------------------------------------------
# include all files needed over here (.cpp files or .h files if there are no .cpp )
# ----------------------------------------------------------------------------------
file(GLOB SOURCES
    "source/main.cpp"
    "source/Application/Application.cpp"
    "source/Window/Window.cpp"
)
 
add_executable(OpenGLProject_CMake ${SOURCES})
target_include_directories(OpenGLProject_CMake PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source
)


# Link SDL3 and GLAD
target_link_libraries(OpenGLProject_CMake
    PRIVATE
        SDL3::SDL3
        glad
        stb
)

#Copy SDL3.dll next to the executable after build
add_custom_command(TARGET OpenGLProject_CMake POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL3::SDL3>
    $<TARGET_FILE_DIR:OpenGLProject_CMake>)

