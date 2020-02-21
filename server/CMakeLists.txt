PROJECT(CXX)
 
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall ${CMAKE_CXX_FLAGS}")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(CMAKE_CXX_COMPILER "g++")

set(MYLIBDIR $ENV{HOME}/lib)


if(NOT DEFINED ENV{PATH})
    message(FATAL_ERROR "defined environment variable:$ENV{PATH}")  
endif()

#add the dir that including head file
include_directories("./Util")
include_directories("./inc")
include_directories("./client")
include_directories("./server")

#LINK_DIRECTORIES(${EXECUTABLE_OUTPUT_PATH}/lib/)

link_libraries(pthread)

#add the source file to exe
#AUX_SOURCE_DIRECTORY(./Util DIR_SRCS1)
AUX_SOURCE_DIRECTORY(./client DIR_SRCS)
ADD_EXECUTABLE(client ${DIR_SRCS})

AUX_SOURCE_DIRECTORY(./server DIR_SRCS1)
ADD_EXECUTABLE(server ${DIR_SRCS1})

