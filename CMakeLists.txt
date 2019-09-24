cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(cpp_playground/inc
        cpp_playground/libs/libglm cpp_playground/libs/libglew/src/include
        cpp_playground/libs/liblodepng)

aux_source_directory(cpp_playground/src MOD_SRC)
aux_source_directory(sph SPH_SRC)

add_executable(mod1 ${MOD_SRC})

add_subdirectory(cpp_playground/libs/libglm)

aux_source_directory(cpp_playground/libs/liblodepng LIBLODEPNG_SRC)
add_library(lodepng STATIC ${LIBLODEPNG_SRC})

add_subdirectory(cpp_playground/libs/libglew/src/build/cmake)
target_link_libraries(mod1 glew glm lodepng SDL2 OpenGL OpenCL)

add_compile_options(-std=c++1z)
