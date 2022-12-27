cmake_minimum_required(VERSION 3.12)

set(YCM_USE_CMAKE_3_13 OFF) # Disable modules from CMake 3.13
set(YCM_USE_CMAKE_3_14 OFF) # Disable modules from CMake 3.14
set(YCM_USE_CMAKE_3_15 OFF) # Disable modules from CMake 3.15
set(YCM_USE_CMAKE_NEXT OFF) # Disable modules from CMake git repository

find_package(YCM REQUIRED)

project(pcmorse  VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(gcc_like_c "$<COMPILE_LANG_AND_ID:C,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_c "$<COMPILE_LANG_AND_ID:C,MSVC>")

include(CTest)
enable_testing()

find_package(PortAudio)


add_executable(morse morse.c main.c)
target_link_libraries(morse m)
target_link_libraries(morse portaudio)
target_compile_options(morse INTERFACE
  "$<${gcc_like_c}:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-Werror;>"
  "$<${msvc_c}:-W3;-WX>"
)

add_test(NAME morse_runs COMMAND ${CMAKE_CURRENT_BINARY_DIR}/morse ${CMAKE_CURRENT_SOURCE_DIR}/sample.txt)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
