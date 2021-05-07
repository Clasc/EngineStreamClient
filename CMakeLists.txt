cmake_minimum_required(VERSION 3.10)
project(EngineStreamClient)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/ffmpeg)

add_executable(
        EngineStreamClient
        main.cpp
        UDP/UdpReceiver.cpp
)

target_link_libraries(EngineStreamClient ffmpeg)
