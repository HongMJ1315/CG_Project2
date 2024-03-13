cmake_minimum_required(VERSION 3.12)

set(MY_PROJECT "GLSL")
set(MY_EXECUTABLE "GLSLSampleCode")

project("Helicopter"
    VERSION 0.1.0
    LANGUAGES CXX C
    DESCRIPTION "Helicopter"
)
add_executable(${MY_EXECUTABLE})
set_target_properties(${MY_EXECUTABLE}
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Set C standard for C files
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

#[[
Define Compiler Flags

Depending on the compiler you're using, this block of code sets the corresponding compiler flags. You can ignore this
part and look into it later.
]]
# GNU Compiler Collection (GCC)
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    # General
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic-errors")

    # Debug mode
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb -D_GLIBC_DEBUG")

# LLVM Clang
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    # General
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic-errors")

    # Debug mode
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb")

# Microsoft Visual C++
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    # General
    if(CMAKE_C_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
    endif()
endif()

#[[ End of Compiler Flags ]]

# Print out compiler settings
message(STATUS "C Compiler: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "CXX Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C Flags: ${CMAKE_C_FLAGS}")
message(STATUS "C Debug Flags: ${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "C Release Flags: ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "${VCPKG_INSTALLED_DIR}")

# Find external libraries
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)
find_package(OpenCV REQUIRED)


target_include_directories(${MY_EXECUTABLE} PRIVATE "include" ${STB_INCLUDE_DIRS})
file(GLOB MY_SOURCE CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/*.c"  # Add .c files to be compiled
)
target_sources(${MY_EXECUTABLE} PRIVATE ${MY_SOURCE})

target_link_libraries(${MY_EXECUTABLE} PRIVATE
    glfw
    GLEW::GLEW
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    SDL2::SDL2
    SDL2::SDL2main
    glm::glm
    imgui::imgui
    opencv_ml
    opencv_dnn
    opencv_core
    opencv_flann
    opencv_imgcodecs
)
