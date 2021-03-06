cmake_minimum_required(VERSION 3.2.2)
project(dspone VERSION 1.0.0)
set(PROJECT_VERSION_STATUS "-alpha")

###########################
## Configure compilation ##
###########################

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
    add_definitions("-D_DEBUG")
    add_definitions("-DDSP_LOGGER=DEBUG")
    message(STATUS "DEBUG mode.")
else(DEBUG)
  set(OPT_FLAGS, "-O3 -msse4")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPT_FLAGS}")
  message(STATUS "RELEASE mode. Optimisation flags: ${OPT_FLAGS}")
endif(DEBUG)

message(STATUS "CXXFLAGS: ${CMAKE_CXX_FLAGS}")

#############################################
## Find modules and configure dependencies ##
#############################################

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(WIPP REQUIRED)

if (DEBUG)
    find_package(Qwt)
    find_package(Qt COMPONENTS QtCore QtGui)
endif(DEBUG)

if (Boost_FOUND)
    add_definitions(-DBOOST_FOUND)
endif()


include_directories(
  include
  ${WIPP_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${QT_INCLUDES}
  ${Qwt_INCLUDE_DIR}
)

###################################
## Declare and configure targets ##
###################################

add_library(dspone SHARED
  src/${PROJECT_NAME}/dsplogger.cpp
  src/${PROJECT_NAME}/dspHelpers.cpp
  src/${PROJECT_NAME}/dspMath.cpp
  src/${PROJECT_NAME}/DspException.cpp
  src/${PROJECT_NAME}/plot/qtDebug.cpp
  src/${PROJECT_NAME}/algorithm/fft.cpp
  src/${PROJECT_NAME}/algorithm/fftImpl.cpp
  src/${PROJECT_NAME}/algorithm/gralCrossCorrelation.cpp
  src/${PROJECT_NAME}/algorithm/gralCrossCorrelationImpl.cpp
  src/${PROJECT_NAME}/algorithm/signalPower.cpp
  src/${PROJECT_NAME}/filter/BandPassFFTWFilter.cpp
  src/${PROJECT_NAME}/filter/BandPassFFTWFilterImpl.cpp
  src/${PROJECT_NAME}/filter/BandPassFilter.cpp
  src/${PROJECT_NAME}/filter/BandPassFIRFilter.cpp
  src/${PROJECT_NAME}/filter/FFTWeightingFilter.cpp
  src/${PROJECT_NAME}/filter/FilterBank.cpp
  src/${PROJECT_NAME}/filter/FilterBankFFTW.cpp
  src/${PROJECT_NAME}/filter/FilterBankFIR.cpp
  src/${PROJECT_NAME}/filter/FilterBankMelScale.cpp
  src/${PROJECT_NAME}/filter/MedianFilterImpl.cpp
  src/${PROJECT_NAME}/filter/MedianFilter.cpp
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

if (Qt4_FOUND)
    add_definitions(-DQT_FOUND)
    set(MOC_HEADERS
	include/${PROJECT_NAME}/plot/dspplotQwt.h
	include/${PROJECT_NAME}/plot/qtDebug.h
	)
    QT4_WRAP_CPP(MOC_SRCS ${MOC_HEADERS})
    target_sources(dspone PRIVATE
	${MOC_SRCS}
	src/${PROJECT_NAME}/plot/dspplotQwt.cpp
	src/${PROJECT_NAME}/plot/dspgui.cpp
	)
endif(Qt4_FOUND)

set_target_properties(dspone
PROPERTIES
VERSION ${PROJECT_VERSION}
SOVERSION ${PROJECT_VERSION_MAJOR}
)

## Specify libraries to link a library or executable target against
target_link_libraries(dspone
  ${Boost_LIBRARIES}
  ${WIPP_LIBRARIES}
  ${Qwt-Qt4_LIBRARY}
  ${QT_QTCORE_LIBRARY}
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
  include/${PROJECT_NAME}/algorithm/signalPower.h
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
  include/${PROJECT_NAME}/filter/MedianFilter.h
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

if (DEBUG)
install(FILES
    include/${PROJECT_NAME}/plot/dspgui.h
    DESTINATION ${INCLUDE_DESTINATION}/plot
    COMPONENT dev
    )
endif(DEBUG)

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
SET(CPACK_PACKAGE_VERSION ${PROJECT_VERSION}${PROJECT_VERSION_STATUS})
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

    add_definitions("-DAUDIO_FILES_PATH=\"${CMAKE_SOURCE_DIR}/test/\"")

    find_package(GTest REQUIRED COMPONENTS system)
    find_package(Threads REQUIRED COMPONENTS system)
    find_package(SNDFILE REQUIRED)

    set(TEST_DEPENDENCIES
	dspone
	${WIPP_LIBRARY}
	${GTEST_BOTH_LIBRARIES}
	${Boost_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
	${Qwt-Qt4_LIBRARY}
	${QT_QTCORE_LIBRARY}
	${QT_QTGUI_LIBRARY}
	${SNDFILE_LIBRARY}
	)

    add_executable(${PROJECT_NAME}-test test/test_dspone.cpp)
    target_link_libraries(${PROJECT_NAME}-test ${TEST_DEPENDENCIES})

    if (DEBUG)
	add_executable(dspgui-test test/test_dspgui.cpp)
	target_link_libraries(dspgui-test ${TEST_DEPENDENCIES})
    endif(DEBUG)

    add_custom_target(tests DEPENDS
	${PROJECT_NAME}-test
	)

    add_test(NAME ${PROJECT_NAME}-test COMMAND ${PROJECT_NAME}-test)

endif()


