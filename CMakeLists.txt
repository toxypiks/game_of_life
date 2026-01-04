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

set(GAME_OF_LIFE_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/game_of_life.c
  ${CMAKE_CURRENT_SOURCE_DIR}/include/game_of_life.h
)

add_library(game_of_life)
target_sources(game_of_life
 PRIVATE
  ${GAME_OF_LIFE_SOURCES}
  )

target_include_directories(game_of_life
 PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(game_of_life
  ${raylib_LIBRARIES}
  pixelbuffer
  m
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

set(GAME_OF_LIFE_FFMPEG_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/game_of_life_ffmpeg.c
  )

add_executable(game_of_life_ffmpeg)
target_sources(game_of_life_ffmpeg
 PRIVATE
  ${GAME_OF_LIFE_FFMPEG_SRC}
)

target_include_directories(game_of_life_ffmpeg
 PRIVATE
  ${raylib_INCLUDE_DIRS}
  )

target_link_libraries(game_of_life_ffmpeg
  ${raylib_LIBRARIES}
  pixelbuffer
  game_of_life
  )
