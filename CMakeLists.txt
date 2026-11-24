cmake_minimum_required(VERSION 3.28 )
project(obd_pid_decoder LANGUAGES CXX)

# C++17 aktivieren und erzwingen
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Kernlogik als statische Bibliothek bauen
add_library(pid_decoder
    src/pid_decoder.cpp
)

# Public bedeutet: Wer gegen pid_decoder linkt (CLI & Tests) bekomme 'include/' automatisch mit
target_include_directories(pid_decoder PUBLIC
        include
)

# CLI-Programm bauen und mit der Decoder-Bibliothek verknüpfen
add_executable(obd_pid_decoder_cli
    src/main.cpp
)
target_link_libraries(obd_pid_decoder_cli PRIVATE
    pid_decoder
)

# Google-Tests per FetchContent einbinden und Tests registrieren
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)

# Verhindert auf Windows mit MSVC Konflikte bei der Runtime-Library
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

add_executable(test_pid_decoder
    tests/test_pid_decoder.cpp
)
target_link_libraries(test_pid_decoder PRIVATE
    pid_decoder
    GTest::gtest_main
)

gtest_discover_tests(test_pid_decoder)