cmake_minimum_required(VERSION 2.6)
project(GImu)

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
    I2C/I2C.cpp
    I2C/libi2c/Pi2c.cpp
   
   
)

set(HEADERS
    
    SOM/SOM.h
    SOM/DataSet.h
    SOM/Node.h
    SOM/Sample.h
    SOM/Utils.h
    I2C/I2C.h
    I2C/libi2c/Pi2c.h
)
include_directories("SOM")
include_directories("I2C")
include_directories("I2C/libi2c")

find_package (Threads)


add_executable(GImu
               main.cpp
               ${SOURCES}
	       ${HEADERS})

target_link_libraries (GImu ${CMAKE_THREAD_LIBS_INIT})


install(TARGETS GImu RUNTIME DESTINATION bin)
