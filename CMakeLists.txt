cmake_minimum_required(VERSION 3.2)
project(SimpleFileTransferTutorial)
include( CTest)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories($ENV{CCNX_HOME}/include)

find_package ( Threads REQUIRED )

find_package ( OpenSSL REQUIRED )

link_directories($ENV{CCNX_HOME}/lib)

set(TUTORIAL_LIBRARIES
       ccnx_common
       ccnx_api_portal
       ccnx_api_notify 
       ccnx_transport_rta 
       ccnx_api_control 
       ccnx_common
       parc 
       longbow 
       longbow-ansiterm)

add_executable(simpleFileTransferTutorial_Server
               simpleFileTransferTutorial_Server.c
        simpleFileTransferTutorial_Common.c
        simpleFileTransferTutorial_FileIO.c)
    
add_executable(simpleFileTransferTutorial_Client 
               simpleFileTransferTutorial_Client.c
        simpleFileTransferTutorial_Common.c
        simpleFileTransferTutorial_FileIO.c)

target_link_libraries(simpleFileTransferTutorial_Client ${TUTORIAL_LIBRARIES})
target_link_libraries(simpleFileTransferTutorial_Server ${TUTORIAL_LIBRARIES})

install(TARGETS simpleFileTransferTutorial_Client RUNTIME DESTINATION bin)
install(TARGETS simpleFileTransferTutorial_Server RUNTIME DESTINATION bin)

add_subdirectory(test)

