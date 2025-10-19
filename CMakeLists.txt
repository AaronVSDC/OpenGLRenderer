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
# GLM
# ----------------------------------------------------------------------------------

FetchContent_Declare(glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# ----------------------------------------------------------------------------------
# ASSIMP
# ----------------------------------------------------------------------------------
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v6.0.2
)
FetchContent_MakeAvailable(assimp) 

# ----------------------------------------------------------------------------------
# GLAD
# ----------------------------------------------------------------------------------
add_library(glad STATIC 3rdParty/glad/src/glad.c )
target_include_directories(glad PUBLIC 3rdParty/glad/include)


# ----------------------------------------------------------------------------------
# stb_image
# ----------------------------------------------------------------------------------
add_library(stb_image STATIC 3rdParty/stb_image/stb_image.cpp)
target_include_directories(stb_image PUBLIC 3rdParty/stb_image)


# ----------------------------------------------------------------------------------
# include all files needed over here (.cpp files or .h files if there are no .cpp )
# ----------------------------------------------------------------------------------
file(GLOB SOURCES
    "Source/main.cpp"
    "Source/Application/Application.cpp"
    "Source/Window/Window.cpp"
    "Source/Singleton.h"
    "Source/InputManager.cpp"
    "Source/OpenGLShader/OpenGLShader.cpp"
)
 
add_executable(OpenGLProject_CMake ${SOURCES})

# ----------------------------------------------------------------------------------
# Auto–collect all header directories so you can #include without relative paths
# ----------------------------------------------------------------------------------
file(GLOB_RECURSE HEADER_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h"
)

set(HEADER_DIRS "")
foreach(HDR ${HEADER_FILES})
  get_filename_component(HDR_DIR ${HDR} DIRECTORY)
  list(APPEND HEADER_DIRS ${HDR_DIR})
endforeach()
list(REMOVE_DUPLICATES HEADER_DIRS)

target_include_directories(OpenGLProject_CMake
  PRIVATE
    ${HEADER_DIRS}
    ${stb_SOURCE_DIR}
)

# Link SDL3 and GLAD
target_link_libraries(OpenGLProject_CMake
    PRIVATE
        SDL3::SDL3
        glad 
        stb_image 
        glm::glm-header-only
        assimp
)

#Copy SDL3.dll next to the executable after build
add_custom_command(TARGET OpenGLProject_CMake POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL3::SDL3>
    $<TARGET_FILE_DIR:OpenGLProject_CMake>)


#same with shader folder
set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Shaders")
set(SHADER_BINARY_DIR "$<TARGET_FILE_DIR:OpenGLProject_CMake>/Shaders")

add_custom_command(TARGET OpenGLProject_CMake POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SHADER_BINARY_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${SHADER_SOURCE_DIR}"
            "${SHADER_BINARY_DIR}"
)
#same with resource folder
set(RESOURCE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Resources")
set(RESOURCE_BINARY_DIR "$<TARGET_FILE_DIR:OpenGLProject_CMake>/Resources")

add_custom_command(TARGET OpenGLProject_CMake POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${RESOURCE_BINARY_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${RESOURCE_SOURCE_DIR}"
            "${RESOURCE_BINARY_DIR}"
)