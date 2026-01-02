cmake_minimum_required(VERSION 3.16)
project(game_of_life)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

find_package(raylib)

set(PIXELBUFFER_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/pixelbuffer.c
  ${CMAKE_CURRENT_SOURCE_DIR}/include/pixelbuffer.h
)

add_library(pixelbuffer)
target_sources(pixelbuffer
 PRIVATE
  ${PIXELBUFFER_SOURCES}
  )

target_include_directories(pixelbuffer
 PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(MAIN_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
  )

add_executable(main)
target_sources(main
 PRIVATE
  ${MAIN_SRC}
)

target_include_directories(main
 PRIVATE
  ${raylib_INCLUDE_DIRS}
  )

target_link_libraries(main
  ${raylib_LIBRARIES}
  pixelbuffer
  )
