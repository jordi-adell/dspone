cmake_minimum_required(VERSION 3.5.2)
project(dspone VERSION 0.2.0)

###########################
## Configure compilation ##
###########################


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
    add_definitions("-DWIPP_DEBUG")
    message(STATUS "DEBUG mode")
else(DEBUG)
  set(OPT_FLAGS, "-O3 -msse4")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPT_FLAGS}")
  message(STATUS "optimisation flags: ${OPT_FLAGS}")
endif(DEBUG)

#############################################
## Find modules and configure dependencies ##
#############################################

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(WIPP REQUIRED)


include_directories(
  include
  ${WIPP_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)

###################################
## Declare and configure targets ##
###################################

add_library(dspone SHARED
  src/${PROJECT_NAME}/dsplogger.cpp
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

set_target_properties(dspone
PROPERTIES
VERSION ${PROJECT_VERSION}
SOVERSION ${PROJECT_VERSION_MAJOR}
)

## Specify libraries to link a library or executable target against
target_link_libraries(dspone
  ${Boost_LIBRARIES}
  ${WIPP_LIBRARY}
)

#############
## Install ##
#############

set(LIB_DESTINATION "lib")
set(BIN_DESTINATION "bin")
set(INCLUDE_DESTINATION "include/${PROJECT_NAME}")

## Mark executables and/or libraries for installation
install(TARGETS dspone
  COMPONENT bin
  ARCHIVE DESTINATION ${LIB_DESTINATION}
  LIBRARY DESTINATION ${LIB_DESTINATION}
  RUNTIME DESTINATION ${BIN_DESTINATION}
)

## Mark cpp header files for installation
install(FILES
#  include/${PROJECT_NAME}/dspdefs.h
    DESTINATION ${INCLUDE_DESTINATION}
	COMPONENT dev
)

########################
## Package generation ##
########################

SET(CPACK_GENERATOR "DEB")

SET(CPACK_PACKAGE_NAME "lib${PROJECT_NAME}")
SET(CPACK_PACKAGE_CONTACT "Jordi Adell <adellj@gmail.com>") #required
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
SET(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
SET(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Library for mathematical calculation signal processing-oriented.")

SET(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)
SET(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)
SET(CPACK_RESOURCE_FILE_WELCOME ${CMAKE_SOURCE_DIR}/README.md)
SET(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_SOURCE_DIR}/README.md)

SET(CPACK_STRIP_FILES TRUE)

if (${CMAKE_SIZEOF_VOID_P} EQUAL "8")
  SET(CPACK_SYSTEM_NAME "amd64")
else()
  SET(CPACK_SYSTEM_NAME "i386")
endif()

SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS 1)
SET(CPACK_DEB_COMPONENT_INSTALL 1)

SET(CPACK_DEBIAN_DEV_PACKAGE_DEPENDS "${CPACK_PACKAGE_NAME} (=${CPACK_PACKAGE_VERSION})")
SET(CPACK_DEBIAN_DEV_PACKAGE_NAME "lib${PROJECT_NAME}-dev")
SET(CPACK_DEBIAN_BIN_PACKAGE_NAME "lib${PROJECT_NAME}")

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/shlibs "lib${PROJECT_NAME} ${CPACK_PACKAGE_VERSION_MAJOR} lib${PROJECT_NAME} (>= ${CPACK_PACKAGE_VERSION})")
SET(CPACK_DEBIAN_BIN_PACKAGE_CONTROL_EXTRA ${CMAKE_CURRENT_BINARY_DIR}/shlibs)

INCLUDE(CPack)

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
    target_link_libraries(${PROJECT_NAME}-test ${GTEST_BOTH_LIBRARIES})
    target_link_libraries(${PROJECT_NAME}-test ${Boost_LIBRARIES})
    target_link_libraries(${PROJECT_NAME}-test ${CMAKE_THREAD_LIBS_INIT})

    add_custom_target(tests DEPENDS ${PROJECT_NAME}-test)

    add_test(NAME ${PROJECT_NAME}-test COMMAND ${PROJECT_NAME}-test)
endif()


