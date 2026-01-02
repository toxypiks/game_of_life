cmake_minimum_required(VERSION 3.16)
project(raycaster)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

find_package(raylib)

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
  )
