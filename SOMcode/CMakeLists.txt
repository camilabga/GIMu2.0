cmake_minimum_required(VERSION 2.6)
project(cppremoteapi)

INCLUDE(FindPkgConfig)

IF(CMAKE_COMPILER_IS_GNUCC)
SET(CMAKE_CXX_FLAGS
"${CMAKE_CXX_FLAGS} -Wall -std=c++0x -DBUILD_SHARED_LIBS=OFF")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

# definicoes da remoteAPI para CPP
add_definitions(-DNON_MATLAB_PARSING)
add_definitions(-DMAX_EXT_API_CONNECTIONS=255)

set(SOURCES
   
    SOM/DataSet.cpp
    SOM/SOM.cpp
    SOM/Node.cpp
    SOM/Sample.cpp
   
)

set(HEADERS
    
    SOM/SOM.h
    SOM/DataSet.h
    SOM/Node.h
    SOM/Sample.h
    SOM/Utils.h
    
)
include_directories("SOM/")

find_package (Threads)


add_executable(cppremoteapi
               main.cpp
               ${SOURCES}
	       ${HEADERS})

target_link_libraries (cppremoteapi ${CMAKE_THREAD_LIBS_INIT})


install(TARGETS cppremoteapi RUNTIME DESTINATION bin)
