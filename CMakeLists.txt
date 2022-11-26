cmake_minimum_required(VERSION 3.23)
project(spider)

set(CMAKE_CXX_STANDARD 17)

add_executable(spider main.cpp Tile.cpp Tile.h Menu.h Cards.cpp SpiderLogic.cpp SpiderLogic.h)
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR D:/studysoft/SFML/SFML-2.5.1/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)
include_directories(D:/studysoft/SFML/SFML-2.5.1/include)
target_link_libraries(spider sfml-system sfml-window sfml-graphics sfml-audio)

