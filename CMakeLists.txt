cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

macro(use_cxx11)
  if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
    endif ()
  else ()
    set (CMAKE_CXX_STANDARD 11)
  endif ()
endmacro(use_cxx11)

use_cxx11()


project (jsonParser)
set(project_sources 
	${CMAKE_CURRENT_LIST_DIR}/src/jsonParser.cpp
	)
add_executable(jsonParser ${project_sources})

target_include_directories(jsonParser PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/deps/"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)