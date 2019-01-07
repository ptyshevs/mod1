cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(cpp_playground/inc cpp_playground/libs/libglm cpp_playground/libs/libglew/src/include cpp_playground/libs/liblodepng)
aux_source_directory(cpp_playground/src MOD_SRC)
add_executable(mod1 ${MOD_SRC} cpp_playground/src/key_bindings.cpp cpp_playground/src/GLItem.cpp)

add_subdirectory(cpp_playground/libs/libglm)

add_library(lodepng STATIC IMPORTED)
set_target_properties(lodepng PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/cpp_playground/libs/liblodepng/libLodePNG.a)

add_subdirectory(cpp_playground/libs/libglew/src/build/cmake)
target_link_libraries(mod1 glew glm "-framework OpenGL" "-framework SDL2" "-framework OpenCL" lodepng)

add_compile_options(-std=c++1z)

aux_source_directory(sph SPH_SRC)
add_executable(sph ${SPH_SRC})
include_directories(sph)
target_link_libraries(sph glm)