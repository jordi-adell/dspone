cmake_minimum_required(VERSION 2.8.3)
project(dspone)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 2)
set(PROJECT_VERSION_PATCH 0)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

if(DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -g")
    add_definitions("-DWIPP_DEBUG")
else(DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -msse4")
endif(DEBUG)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)
find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(FFTW)

find_library(WIPP_LIBRARY wipp)
find_path(WIPP_INCLUDE_DIRS wipp/wipp.h)

message(STATUS ${WIPP_LIBRARY})
message(STATUS ${WIPP_INCLUDE_DIRS})


if(FFTW_FOUND)
    message(STATUS "FFTW found")
    add_definitions(-DFFTW_FOUND)
endif(FFTW_FOUND)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
# TODO: Check names of system library include directories (ipp, boost)
include_directories(
  include
  ${WIPP_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)

## Declare a cpp library
add_library(dspone
  src/${PROJECT_NAME}/BandPassFFTWFilter.cpp
  src/${PROJECT_NAME}/BandPassFFTWFilterImpl.cpp
  src/${PROJECT_NAME}/BandPassFilter.cpp
  src/${PROJECT_NAME}/BandPassFIRFilter.cpp
  src/${PROJECT_NAME}/BasicFilterModules.cpp
  src/${PROJECT_NAME}/dspHelpers.cpp
  src/${PROJECT_NAME}/fft.cpp
  src/${PROJECT_NAME}/fftImpl.cpp
  src/${PROJECT_NAME}/FFTWeightingFilter.cpp
  src/${PROJECT_NAME}/FilterBank.cpp
  src/${PROJECT_NAME}/FilterBankFFTW.cpp
  src/${PROJECT_NAME}/FilterBankFIR.cpp
  src/${PROJECT_NAME}/FilterBankMelScale.cpp
  src/${PROJECT_NAME}/Filter.cpp
  src/${PROJECT_NAME}/FilterProcess.cpp
  src/${PROJECT_NAME}/FIRFilter.cpp
  src/${PROJECT_NAME}/gralCrossCorrelation.cpp
  src/${PROJECT_NAME}/IIRFilter.cpp
  src/${PROJECT_NAME}/NotchFilter.cpp
  src/${PROJECT_NAME}/ParticleFilter.cpp
  src/${PROJECT_NAME}/PreEmphasisFilter.cpp
  src/${PROJECT_NAME}/ShortTimeProcess.cpp
  src/${PROJECT_NAME}/SignalProcessor.cpp
  src/${PROJECT_NAME}/stft.cpp
  src/${PROJECT_NAME}/TimeProcess.cpp
)

## Specify libraries to link a library or executable target against
target_link_libraries(dspone
  ${Boost_LIBRARIES}
  ${WIPP_LIBRARY}
  ${FFTW_LIBRARIES}
)

#############
## Install ##
#############

set(LIB_DESTINATION "lib")
set(BIN_DESTINATION "bin")
set(INCLUDE_DESTINATION "include/${PROJECT_NAME}")

## Mark executables and/or libraries for installation
install(TARGETS dspone
  ARCHIVE DESTINATION ${LIB_DESTINATION}
  LIBRARY DESTINATION ${LIB_DESTINATION}
  RUNTIME DESTINATION ${BIN_DESTINATION}
)

## Mark cpp header files for installation
install(FILES
  include/${PROJECT_NAME}/dspdefs.h
    DESTINATION ${INCLUDE_DESTINATION}
)

#############
## Testing ##
#############

if (test)
    enable_testing()

    find_package(GTest REQUIRED COMPONENTS system)
    find_package(Threads REQUIRED COMPONENTS system)

    add_executable(${PROJECT_NAME}-test test/test_dspone.cpp)
    target_link_libraries(${PROJECT_NAME}-test dspone)
    target_link_libraries(${PROJECT_NAME}-test ${WIPP_LIBRARY})
    target_link_libraries(${PROJECT_NAME}-test ${FFTW_LIBRARIES})
    target_link_libraries(${PROJECT_NAME}-test ${GTEST_BOTH_LIBRARIES})
    target_link_libraries(${PROJECT_NAME}-test ${Boost_LIBRARIES})
    target_link_libraries(${PROJECT_NAME}-test ${CMAKE_THREAD_LIBS_INIT})

    add_custom_target(tests DEPENDS ${PROJECT_NAME}-test)

    add_test(NAME ${PROJECT_NAME}-test COMMAND ${PROJECT_NAME}-test)
endif()


