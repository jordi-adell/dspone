cmake_minimum_required(VERSION 3.5.2)
project(dspone VERSION 0.3.0)

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

if (Boost_FOUND)
    add_definitions(-DBOOST_FOUND)
endif()

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
  src/${PROJECT_NAME}/dspHelpers.cpp
  src/${PROJECT_NAME}/dspMath.cpp
  src/${PROJECT_NAME}/DspException.cpp
  src/${PROJECT_NAME}/algorithm/fft.cpp
  src/${PROJECT_NAME}/algorithm/fftImpl.cpp
  src/${PROJECT_NAME}/algorithm/gralCrossCorrelation.cpp
  src/${PROJECT_NAME}/algorithm/gralCrossCorrelationImpl.cpp
  src/${PROJECT_NAME}/filter/BandPassFFTWFilter.cpp
  src/${PROJECT_NAME}/filter/BandPassFFTWFilterImpl.cpp
  src/${PROJECT_NAME}/filter/BandPassFilter.cpp
  src/${PROJECT_NAME}/filter/BandPassFIRFilter.cpp
  src/${PROJECT_NAME}/filter/FFTWeightingFilter.cpp
  src/${PROJECT_NAME}/filter/FilterBank.cpp
  src/${PROJECT_NAME}/filter/FilterBankFFTW.cpp
  src/${PROJECT_NAME}/filter/FilterBankFIR.cpp
  src/${PROJECT_NAME}/filter/FilterBankMelScale.cpp
  src/${PROJECT_NAME}/filter/Filter.cpp
  src/${PROJECT_NAME}/filter/FIRFilter.cpp
  src/${PROJECT_NAME}/filter/IIRFilter.cpp
  src/${PROJECT_NAME}/filter/NotchFilter.cpp
  src/${PROJECT_NAME}/filter/PreEmphasisFilter.cpp
  src/${PROJECT_NAME}/filter/FilterProcess.cpp
  src/${PROJECT_NAME}/pf/IObservationModel.cpp
  src/${PROJECT_NAME}/pf/IPredictionModel.cpp
  src/${PROJECT_NAME}/pf/IResamplingModel.cpp
  src/${PROJECT_NAME}/pf/ParticleFilter.cpp
  src/${PROJECT_NAME}/rt/ShortTimeProcessImpl.cpp
  src/${PROJECT_NAME}/rt/ShortTimeProcess.cpp
  src/${PROJECT_NAME}/rt/ShortTimeAnalysisImpl.cpp
  src/${PROJECT_NAME}/rt/ShortTimeAnalysis.cpp
  src/${PROJECT_NAME}/rt/Filters.cpp
  src/${PROJECT_NAME}/rt/TimeProcess.cpp
  src/${PROJECT_NAME}/rt/DummyShortTimeProcess.cpp
  src/${PROJECT_NAME}/rt/STFTImpl.cpp
  src/${PROJECT_NAME}/rt/ShortTimeFourierTransform.cpp
  src/${PROJECT_NAME}/rt/ShortTimeFourierAnalysis.cpp
  src/${PROJECT_NAME}/rt/ShortTimeFourierSubBand.cpp
  src/${PROJECT_NAME}/rt/ShortTimeFourierSubBandImpl.cpp
  src/${PROJECT_NAME}/rt/DummyShortTimeFourier.cpp
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
  include/${PROJECT_NAME}/dsp.h
  include/${PROJECT_NAME}/dsplogger.h
  include/${PROJECT_NAME}/dspMath.h
  include/${PROJECT_NAME}/DspException.h
  include/${PROJECT_NAME}/complex.h
  DESTINATION ${INCLUDE_DESTINATION}
  COMPONENT dev
  )

install(FILES
  include/${PROJECT_NAME}/algorithm/fft.h
  include/${PROJECT_NAME}/algorithm/gralCrossCorrelation.h
  DESTINATION ${INCLUDE_DESTINATION}/algorithm
  COMPONENT dev
  )

install(FILES
  include/${PROJECT_NAME}/pf/ParticleFilter.hpp
  include/${PROJECT_NAME}/pf/IPredictionModel.h
  include/${PROJECT_NAME}/pf/IResamplingModel.h
  include/${PROJECT_NAME}/pf/IObservationModel.h
  include/${PROJECT_NAME}/pf/IParticleSet.h
  include/${PROJECT_NAME}/pf/IParticleFilterCallback.h
  include/${PROJECT_NAME}/pf/PredictionModel.hpp
  include/${PROJECT_NAME}/pf/ResamplingModel.hpp
  include/${PROJECT_NAME}/pf/ObservationModel.hpp
  include/${PROJECT_NAME}/pf/ParticleSet.hpp
  DESTINATION ${INCLUDE_DESTINATION}/pf/
  COMPONENT dev
  )

install(FILES
  include/${PROJECT_NAME}/filter/Filter.h
  include/${PROJECT_NAME}/filter/FIRFilter.h
  include/${PROJECT_NAME}/filter/IIRFilter.h
  include/${PROJECT_NAME}/filter/FFTWeightingFilter.h
  include/${PROJECT_NAME}/filter/BandPassFilter.h
  include/${PROJECT_NAME}/filter/BandPassFIRFilter.h
  include/${PROJECT_NAME}/filter/BandPassFFTWFilter.h
  include/${PROJECT_NAME}/filter/FilterBank.h
  include/${PROJECT_NAME}/filter/FilterBankFFTW.h
  include/${PROJECT_NAME}/filter/FilterBankFIR.h
  include/${PROJECT_NAME}/filter/FilterBankMelScale.h
  include/${PROJECT_NAME}/filter/PreEmphasisFilter.h
  DESTINATION ${INCLUDE_DESTINATION}/filter
  COMPONENT dev
)

install(FILES
    include/${PROJECT_NAME}/rt/Filters.h
    include/${PROJECT_NAME}/rt/FilterProcess.hpp
    include/${PROJECT_NAME}/rt/ProcessDispatcher.h
    include/${PROJECT_NAME}/rt/ShortTimeFourierTransform.h
    include/${PROJECT_NAME}/rt/ShortTimeFourierAnalysis.h
    include/${PROJECT_NAME}/rt/ShortTimeFourierSubBand.h
    include/${PROJECT_NAME}/rt/ShortTimeProcess.h
    include/${PROJECT_NAME}/rt/ShortTimeAnalysis.h
    include/${PROJECT_NAME}/rt/TimeProcess.h
    include/${PROJECT_NAME}/rt/SignalProcessor.h
    include/${PROJECT_NAME}/rt/SignalAnalyser.h
    DESTINATION ${INCLUDE_DESTINATION}/rt
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


