cmake_minimum_required(VERSION 3.0.0)
project(pcmorse  VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(gcc_like_c "$<COMPILE_LANG_AND_ID:C,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_c "$<COMPILE_LANG_AND_ID:C,MSVC>")

include(CTest)
enable_testing()

add_executable(morse morse.c main.c)
target_link_libraries(morse portaudio rt m asound jack pthread)
target_compile_options(morse INTERFACE
  "$<${gcc_like_c}:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-Werror;>"
  "$<${msvc_c}:-W3;-WX>"
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
