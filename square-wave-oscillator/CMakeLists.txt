cmake_minimum_required(VERSION 3.13)

project(square-wave-oscillator)

set(CMAKE_CXX_STANDARD 17)

add_executable(square-wave-oscillator
    src/program.cpp
    src/RtAudio.cpp
    src/RtAudio.h)

if (WIN32)
    target_link_libraries(square-wave-oscillator
        ksuser mfplat mfuuid wmcodecdspuuid winmm ole32 dsound)

    target_compile_definitions(square-wave-oscillator
        PRIVATE __WINDOWS_WASAPI__=1
        PRIVATE __WINDOWS_DS__=1)
endif()
