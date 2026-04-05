cmake_minimum_required(VERSION 3.10)
project(CheckerNetwork)

set(CMAKE_CXX_STANDARD 17)

# Find SFML for the client
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Common files
set(COMMON_SOURCES common/board.cpp)

# Server executable (Epoll)
add_executable(server server/server.cpp ${COMMON_SOURCES})

# Client executable
add_executable(client client/graphics.cpp client/main1.cpp ${COMMON_SOURCES})
target_link_libraries(client sfml-graphics sfml-window sfml-system)

