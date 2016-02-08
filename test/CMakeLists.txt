cmake_minimum_required(VERSION 3.2)
project(SimpleFileTransferTutorialTest)

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

macro(AddTest testFile)
  add_executable(${ARGV0} ${ARGV0}.c)
  target_link_libraries(${ARGV0} ${TUTORIAL_LIBRARIES})
  add_test(${ARGV0} ${ARGV0})
endmacro(AddTest)

AddTest(test_simpleFileTransferTutorial_FileIO)
    



