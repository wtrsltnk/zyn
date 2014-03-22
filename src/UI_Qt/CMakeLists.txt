#
# General
#

cmake_minimum_required(VERSION 2.6)
project(ZynAddSubFX)

SET(MYPROJECT "ZynAddSubFX")
SET(MYDECSRIPTION "a real time synthesizer")
SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "0")
SET(VERSION_PATCH "0")
SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
"${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")

# help
MESSAGE("This is the Makefile for ${MYPROJECT} (version ${VERSION}).")
MESSAGE("Options: (-D...=..., cmake -L for details)")


# Configuration
SET(LMMS_INCLUDE_PATH "" CACHE STRING
	"If embedded, path to LMMS' `include' directory.")
option (EMBEDDED_IN_LMMS "Compile the UI for being embedded in lmms." ON)

if(EMBEDDED_IN_LMMS)
 MESSAGE("EMBEDDED")
ELSE()
 MESSAGE("NOT EMBEDDED")
ENDIF()

# Include abs branch


SET(NIO_LIBRARIES "")
SET(AUDIO_LIBRARIES "")

add_subdirectory(lmms)

#include_directories(abs/include)
#include_directories(abs/src)
#include_directories(abs/rtosc/include)

include_directories(../../)
include_directories(../../src)
include_directories(../../rtosc/include)

#
# Machine
#

INCLUDE(DetectMachine)

#
# special headers
#

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/ui_qt_config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/ui_qt_config.h)

#
# Compile
#

# FLAGS
add_definitions(-std=c++11)
file(GLOB zyn_SOURCES "*.cpp" "../UI/NSM/Client.C")
# TODO: Client.C is actually a bug...
file(GLOB zyn_HEADERS "*.h")
file(GLOB zyn_FORMS "*.ui")
file(GLOB zyn_RESOURCES "../../*.qrc")

#MESSAGE("SRC, HDR:")
#MESSAGE("${zyn_SOURCES}")
#MESSAGE("${zyn_HEADERS}")

#
# modules
#

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

QT4_WRAP_CPP(zyn_HEADERS_MOC ${zyn_HEADERS})
QT4_WRAP_UI(zyn_FORMS_HEADERS ${zyn_FORMS})
QT4_ADD_RESOURCES(zyn_RESOURCES_RCC ${zyn_RESOURCES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_LIBRARY(zynaddsubfx_gui STATIC ${zyn_SOURCES} 
    ${zyn_HEADERS_MOC} 
    ${zyn_FORMS_HEADERS} 
    ${zyn_RESOURCES_RCC})

TARGET_LINK_LIBRARIES(zynaddsubfx_gui
lmms_includes
${QT_LIBRARIES} 
zynaddsubfx_core zynaddsubfx_nio ${NIO_LIBRARIES}
${AUDIO_LIBRARIES})

#
# CPack Stuff
#

INCLUDE(MyCPack)
