#checking include/library paths
message(STATUS "Checking Include Path" $ENV{CMAKE_INCLUDE_PATH} ${CMAKE_INCLUDE_PATH})
message(STATUS "Checking Library Path" $ENV{CMAKE_LIBRARY_PATH} ${CMAKE_LIBRARY_PATH})

#Dependency check
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckCXXSourceCompiles)
include(CheckCXXCompilerFlag) 
find_package(PkgConfig REQUIRED)
find_package(zlib REQUIRED)
find_package(X11)
check_include_file(X11/xpm.h HAS_XPM)
pkg_check_modules(FFTW REQUIRED fftw3)
pkg_check_modules(MXML REQUIRED mxml)
find_package(Threads   REQUIRED)
find_package(OSS)
find_package(Alsa)
pkg_check_modules(JACK jack)
pkg_check_modules(PORTAUDIO portaudio-2.0>=19)
set(FLTK_SKIP_OPENGL true)
pkg_check_modules(NTK ntk)
pkg_check_modules(NTK_IMAGES ntk_images)
find_package(FLTK)
find_package(OpenGL) #for FLTK
find_package(CxxTest)
if(CXXTEST_FOUND)
    set(CXXTEST_USE_PYTHON TRUE)
endif()
# lash
pkg_search_module(LASH lash-1.0)
mark_as_advanced(LASH_LIBRARIES)
pkg_search_module(DSSI dssi>=0.9.0)
mark_as_advanced(DSSI_LIBRARIES)
pkg_search_module(LIBLO liblo>=0.26 REQUIRED)
mark_as_advanced(LIBLO_LIBRARIES)

CHECK_FUNCTION_EXISTS(sched_setscheduler HAVE_SCHEDULER)
set(CMAKE_REQUIRED_FLAGS "-std=c++11")
CHECK_CXX_SOURCE_COMPILES(
    "#include <future>
    #if ATOMIC_INT_LOCK_FREE<2
    #error unreliable atomics
    #endif
    int main(){return 0;}" HAVE_ASYNC)

check_cxx_compiler_flag("-msse2" SUPPORT_SSE)
check_cxx_compiler_flag("-mfpu=neon -Werror" SUPPORT_NEON)

set(CMAKE_REQUIRED_FLAGS "")


execute_process(COMMAND echo fistpl 0
                COMMAND as -
                ERROR_VARIABLE AVOID_ASM)

######### Settings ###########
# NOTE: These cache variables should normally not be changed in this
# file, but either in in CMakeCache.txt before compile, or by passing
# parameters directly into cmake using the -D flag.
if(NTK_FOUND)
  SET (GuiModule ntk CACHE STRING "GUI module, either fltk, ntk or off")
elseif(FLTK_FOUND)
  SET (GuiModule fltk CACHE STRING "GUI module, either fltk, ntk or off")
else()
  SET (GuiModule off CACHE STRING "GUI module, either fltk, ntk or off")
endif()
SET (CompileTests ${CXXTEST_FOUND} CACHE BOOL "whether tests should be compiled in or not")
SET (AlsaEnable ${ALSA_FOUND} CACHE BOOL
    "Enable support for Advanced Linux Sound Architecture")
SET (JackEnable ${JACK_FOUND} CACHE BOOL
    "Enable support for JACK Audio Connection toolKit")
SET (OssEnable ${OSS_FOUND} CACHE BOOL
    "Enable support for Open Sound System")
SET (PaEnable ${PORTAUDIO_FOUND} CACHE BOOL
    "Enable support for Port Audio System")
SET (LashEnable ${LASH_FOUND} CACHE BOOL
    "Enable LASH Audio Session Handler")
SET (DssiEnable ${DSSI_FOUND} CACHE BOOL
    "Enable DSSI Plugin compilation")
SET (LibloEnable ${LIBLO_FOUND} CACHE BOOL
    "Enable Liblo")

# Now, handle the incoming settings and set define flags/variables based
# on this

# Add version information
add_definitions(-DVERSION="${VERSION}")

if(NOT "Darwin" STREQUAL ${CMAKE_SYSTEM_NAME})
    # Add scheduler function existance info (OSX compatiability)
    add_definitions(-DHAVE_SCHEDULER=${HAVE_SCHEDULER})
endif()

if(HAVE_ASYNC)
    add_definitions(-DHAVE_ASYNC=1)
else()
    add_definitions(-DHAVE_ASYNC=0)
endif()


# Give a good guess on the best Input/Output default backends
if (JackEnable)
    SET (DefaultOutput jack CACHE STRING
        "Default Output module: [null, alsa, oss, jack, portaudio]")
    # Override with perhaps more helpful midi backends
    if (AlsaEnable)
        SET (DefaultInput alsa CACHE STRING
            "Default Input module: [null, alsa, oss, jack]")
    elseif (OssEnable)
        SET (DefaultInput oss CACHE STRING
            "Default Input module: [null, alsa, oss, jack]")
    else ()
        SET (DefaultInput jack CACHE STRING
            "Default Input module: [null, alsa, oss, jack]")
    endif ()
elseif (AlsaEnable)
    SET (DefaultOutput alsa CACHE STRING
        "Default Output module: [null, alsa, oss, jack, portaudio]")
    SET (DefaultInput alsa CACHE STRING
        "Default Input module: [null, alsa, oss, jack]")
elseif (OssEnable)
    SET (DefaultOutput oss CACHE STRING
        "Default Output module: [null, alsa, oss, jack, portaudio]")
    SET (DefaultInput oss CACHE STRING
        "Default Input module: [null, alsa, oss, jack]")
else()
    SET (DefaultOutput null CACHE STRING
        "Default Output module: [null, alsa, oss, jack, portaudio]")
    SET (DefaultInput null CACHE STRING
        "Default Input module: [null, alsa, oss, jack]")
endif()



if (GuiModule STREQUAL qt AND QT_FOUND)
	set (QtGui TRUE)
elseif(GuiModule STREQUAL ntk AND NTK_FOUND)
	set (NtkGui TRUE)
elseif(GuiModule STREQUAL fltk AND FLTK_FOUND)
	set (FltkGui TRUE)
elseif(GuiModule STREQUAL off)
        add_definitions(-DDISABLE_GUI)
else  ()
        set (GuiModule off CACHE STRING "GUI module, either fltk, qt or off")
        add_definitions(-DDISABLE_GUI)
	message(STATUS "GUI module defaulting to off")
endif()


#Build Flags
option (BuildForAMD_X86_64 "Build for AMD x86_64 system" OFF)
option (BuildForCore2_X86_64 "Build for Intel Core2 x86_64 system" OFF)
option (BuildForDebug "Include gdb debugging support" OFF)

set(CMAKE_BUILD_TYPE "Release")


set (BuildOptions_x86_64AMD
    "-march=athlon64 -m64 -Wall -Wno-unused-parameter"
  CACHE STRING "X86_64 compiler options"
)

set (BuildOptions_X86_64Core2
    "-march=core2 -m64 -Wall -Wno-unused-parameter"
  CACHE STRING "X86_64 compiler options"
)

set (BuildOptions_NEON
    "-march=armv7-a -mfloat-abi=hard -mfpu=neon -mcpu=cortex-a9 -mtune=cortex-a9 -pipe -mvectorize-with-neon-quad -funsafe-loop-optimizations"
  CACHE STRING "Cortex_a9 compiler options"
)
set (BuildOptions_SSE
	"-msse -msse2 -mfpmath=sse"
  CACHE STRING "SSE compiler options"
)

set (BuildOptionsBasic
    "-std=c++11 -Wno-unused-parameter -O3 -ffast-math -fomit-frame-pointer"
    CACHE STRING "basic X86 complier options"
)

set (BuildOptionsDebug
    "-std=c++11 -O0 -g3 -ggdb -Wall -Wno-unused-parameter -Wpointer-arith" CACHE STRING "Debug build flags")

########### Settings dependant code ###########
# From here on, the setting variables have  been prepared so concentrate
# on the actual compiling.

# To avoid a conflict if PortAudio v1 is installed, the linker directory
# of portaudio must be specified first, so that xxx/lib/portaudio2 is
# searched before xxx/lib where PortAudio v1 gets installed.
if(PaEnable)
	include_directories(${PORTAUDIO_INCLUDE_DIRS})
	add_definitions(-DPORTAUDIO=1)
	list(APPEND AUDIO_LIBRARIES ${PORTAUDIO_LIBRARIES})
	list(APPEND AUDIO_LIBRARY_DIRS ${PORTAUDIO_LIBRARY_DIRS})
endif()

if(AlsaEnable)
	list(APPEND AUDIO_LIBRARIES ${ASOUND_LIBRARY})
	list(APPEND AUDIO_LIBRARY_DIRS ${ASOUND_LIBRARY_DIRS})
	add_definitions(-DALSA=1)
endif(AlsaEnable)

if(JackEnable)
	list(APPEND AUDIO_LIBRARIES ${JACK_LIBRARIES})
	list(APPEND AUDIO_LIBRARY_DIRS ${JACK_LIBRARY_DIRS})
	add_definitions(-DJACK=1)
endif(JackEnable)

if(OssEnable)
	add_definitions(-DOSS=1)
endif(OssEnable)

if (CompileTests)
	ENABLE_TESTING()
endif()

if(LashEnable)
	include_directories(${LASH_INCLUDE_DIRS})
	add_definitions(-DLASH=1)
	list(APPEND AUDIO_LIBRARIES ${LASH_LIBRARIES})
	list(APPEND AUDIO_LIBRARY_DIRS ${LASH_LIBRARY_DIRS})
	message(STATUS "Compiling with lash")
endif()
if(LibloEnable)
	include_directories(${LIBLO_INCLUDE_DIRS})
	add_definitions(-DUSE_NSM=1)
    list(APPEND AUDIO_LIBRARIES ${LIBLO_LIBRARIES})
	list(APPEND AUDIO_LIBRARY_DIRS ${LIBLO_LIBRARY_DIRS})
	message(STATUS "Compiling with liblo")
endif()

# other include directories
include_directories(${ZLIB_INCLUDE_DIRS} ${MXML_INCLUDE_DIRS})

add_definitions(
	 -Wall
	 -Wextra
	 )
if(NOT AVOID_ASM)
	message(STATUS "Compiling with x86 opcode support")
    add_definitions(-DASM_F2I_YES)
endif()

if (BuildForDebug)
	set (CMAKE_BUILD_TYPE "Debug")
	set (CMAKE_CXX_FLAGS_DEBUG ${BuildOptionsDebug})
	message (STATUS "Building for ${CMAKE_BUILD_TYPE}, flags: ${CMAKE_CXX_FLAGS_DEBUG}")
else (BuildForDebug)
	set (CMAKE_BUILD_TYPE "Release")
	
	set (CMAKE_CXX_FLAGS_RELEASE ${BuildOptionsBasic})
	
	if (BuildForAMD_X86_64)
		set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${BuildOptions_x86_64AMD}")
	endif (BuildForAMD_X86_64)
	
	if (BuildForCore2_X86_64)
			set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${BuildOptions_X86_64Core2}")
	endif (BuildForCore2_X86_64)
				
	if (SUPPORT_SSE)
		set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${BuildOptions_SSE}")
	endif (SUPPORT_SSE)
	
	if (SUPPORT_NEON)
		set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${BuildOptions_NEON}")
	endif (SUPPORT_NEON)
	
	message (STATUS "Building for ${CMAKE_BUILD_TYPE}, flags: ${CMAKE_CXX_FLAGS_RELEASE}")
endif (BuildForDebug)

add_definitions(-fPIC)

if(FLTK_FOUND)
	mark_as_advanced(FORCE FLTK_BASE_LIBRARY)
	mark_as_advanced(FORCE FLTK_CONFIG_SCRIPT)
	mark_as_advanced(FORCE FLTK_DIR)
	mark_as_advanced(FORCE FLTK_FLUID_EXECUTABLE)
	mark_as_advanced(FORCE FLTK_FORMS_LIBRARY)
	mark_as_advanced(FORCE FLTK_GL_LIBRARY)
	mark_as_advanced(FORCE FLTK_IMAGES_LIBRARY)
	mark_as_advanced(FORCE FLTK_INCLUDE_DIR)
	mark_as_advanced(FORCE FLTK_MATH_LIBRARY)
endif(FLTK_FOUND)

if(NTK_FOUND)
	mark_as_advanced(FORCE NTK_BASE_LIBRARY)
	mark_as_advanced(FORCE NTK_CONFIG_SCRIPT)
	mark_as_advanced(FORCE NTK_DIR)
	mark_as_advanced(FORCE FLTK_FLUID_EXECUTABLE)
	mark_as_advanced(FORCE NTK_FORMS_LIBRARY)
	mark_as_advanced(FORCE NTK_GL_LIBRARY)
	mark_as_advanced(FORCE NTK_IMAGES_LIBRARY)
	mark_as_advanced(FORCE NTK_INCLUDE_DIR)
	mark_as_advanced(FORCE NTK_MATH_LIBRARY)
endif(NTK_FOUND)

if(FltkGui)
	#UGLY WORKAROUND
	find_program (FLTK_CONFIG fltk-config)
	if (FLTK_CONFIG)
		execute_process (COMMAND ${FLTK_CONFIG} --use-images --ldflags OUTPUT_VARIABLE FLTK_LDFLAGS)
		string(STRIP ${FLTK_LDFLAGS} FLTK_LIBRARIES)
	endif()

	message(STATUS ${FLTK_LDFLAGS})


	set(GUI_LIBRARIES zynaddsubfx_gui ${FLTK_LIBRARIES} ${FLTK_LIBRARIES} ${OPENGL_LIBRARIES})
    if(X11_FOUND AND HAS_XPM)
        set(GUI_LIBRARIES ${GUI_LIBRARIES} ${X11_LIBRARIES} -lXpm)
    endif()

	add_definitions(-DFLTK_GUI)
	message(STATUS "Will build FLTK gui")

	include_directories(
			${FLTK_INCLUDE_DIR}
			"${CMAKE_CURRENT_SOURCE_DIR}/UI"
			"${CMAKE_CURRENT_BINARY_DIR}/UI"
			)

	add_subdirectory(UI)
endif()

if(NtkGui)

        find_program( FLTK_FLUID_EXECUTABLE ntk-fluid)
        
	message(STATUS ${NTK_LDFLAGS} ${NTK_IMAGES_LDFLAGS})

	set(GUI_LIBRARIES zynaddsubfx_gui ${NTK_LIBRARIES} ${NTK_IMAGES_LIBRARIES} ${OPENGL_LIBRARIES})

    if(X11_FOUND AND HAS_XPM)
        set(GUI_LIBRARIES ${GUI_LIBRARIES} ${X11_LIBRARIES} -lXpm)
    endif()

	add_definitions(-DNTK_GUI)

	message(STATUS "Will build NTK gui")

	include_directories(
			${NTK_INCLUDE_DIRS}
			"${CMAKE_CURRENT_SOURCE_DIR}/UI"
			"${CMAKE_CURRENT_BINARY_DIR}/UI"
			)

	add_subdirectory(UI)
endif()

if(NOT FltkGui AND NOT NtkGui)
    if(LibloEnable)
        set(NSM_WORKAROUND UI/NSM.C UI/NSM/Client.C)
    endif()
    add_library(zynaddsubfx_gui_bridge STATIC UI/ConnectionDummy.cpp ${NSM_WORKAROUND})
    add_definitions(-DNO_UI=1)
endif()

########### General section ##############
# Following this should be only general compilation code, and no mention
# of module-specific variables

link_directories(${AUDIO_LIBRARY_DIRS} ${ZLIB_LIBRARY_DIRS} ${FFTW_LIBRARY_DIRS} ${MXML_LIBRARY_DIRS} ${FLTK_LIBRARY_DIRS} ${NTK_LIBRARY_DIRS})

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)



set(NONGUI_LIBRARIES
	zynaddsubfx_misc
	zynaddsubfx_synth
	zynaddsubfx_effect
	zynaddsubfx_params
	zynaddsubfx_dsp
	zynaddsubfx_nio
	)

add_subdirectory(Misc)
add_subdirectory(Synth)
add_subdirectory(Effects)
add_subdirectory(Params)
add_subdirectory(DSP)
add_subdirectory(Nio)

add_library(zynaddsubfx_core STATIC
    globals.cpp
    ../tlsf/tlsf.c
    Containers/NotePool.cpp
    Containers/MultiPseudoStack.cpp
	${zynaddsubfx_dsp_SRCS}
	${zynaddsubfx_effect_SRCS}
	${zynaddsubfx_misc_SRCS}
	${zynaddsubfx_params_SRCS}
	${zynaddsubfx_synth_SRCS}
	)

target_link_libraries(zynaddsubfx_core
	${ZLIB_LIBRARIES}
	${FFTW_LIBRARIES}
	${MXML_LIBRARIES}
	${OS_LIBRARIES}
	pthread
    rtosc rtosc-cpp)

if(CompileTests)
	add_subdirectory(Tests)
endif(CompileTests)


add_executable(zynaddsubfx main.cpp)

#Warning: the required ordering of these to get proper linking depends upon the
#         phase of the moon
target_link_libraries(zynaddsubfx
    zynaddsubfx_core
	zynaddsubfx_nio
    zynaddsubfx_gui_bridge
	${GUI_LIBRARIES}
	${NIO_LIBRARIES}
	${AUDIO_LIBRARIES}
	)

if (DssiEnable)
	add_library(zynaddsubfx_dssi SHARED
			UI/ConnectionDummy.cpp
			Output/DSSIaudiooutput.cpp
			globals.cpp
			Output/DSSIControl.cpp
			Output/DSSIControl.h
			Output/DSSIControlDescription.cpp
			Output/DSSIControlDescription.h)

	target_link_libraries(zynaddsubfx_dssi
        zynaddsubfx_core
		${OS_LIBRARIES}
        ${LIBLO_LIBRARIES}
		)
	if (${CMAKE_SIZEOF_VOID_P} EQUAL "8")
		install(TARGETS zynaddsubfx_dssi LIBRARY DESTINATION lib64/dssi/)
	else ()
		install(TARGETS zynaddsubfx_dssi LIBRARY DESTINATION lib/dssi/)
	endif ()
endif()

install(TARGETS zynaddsubfx
	RUNTIME DESTINATION bin
	)
if(NtkGui)
    install(DIRECTORY ../pixmaps DESTINATION share/zynaddsubfx)
endif(NtkGui)

include(CTest)


##Summarize The Full Configuration
message(STATUS)
message(STATUS "${ColorBold}ZynAddSubFX Build Configuration")
message(STATUS             "===============================${ColorReset}")
message(STATUS)
message(STATUS "Building on a '${CMAKE_SYSTEM_NAME}' System")


macro(package_status foundvar pkg state)#optional color
    set (extra_macro_args ${ARGN})
    list(LENGTH extra_macro_args num_extra_args)
    if (${num_extra_args} GREATER 0)
        list(GET extra_macro_args 0 color)
    endif ()
    if(${foundvar})
        message(STATUS "${pkg} -- ${Green}${state}${ColorReset}")
    else()
        message(STATUS "${pkg} -- ${color}NOT ${state}${ColorReset}")
    endif()
endmacro()

package_status(PKG_CONFIG_FOUND "PkgConfig" "found"   ${Red})
package_status(ZLIB_FOUND       "zlib     " "found"   ${Red})
package_status(MXML_FOUND       "mxml     " "found"   ${Red})
package_status(FFTW_FOUND       "fftw3    " "found"   ${Red})
package_status(LIBLO_FOUND      "liblo    " "found"   ${Red})
package_status(X11_FOUND        "x11      " "found"   ${Yellow})
package_status(HAS_XPM          "xpm      " "found"   ${Yellow})
package_status(FLTK_FOUND       "fltk     " "found"   ${Yellow})
package_status(NTK_FOUND        "ntk      " "found"   ${Yellow})
package_status(X11_FOUND        "x11      " "found"   ${Yellow})
package_status(OSS_FOUND        "OSS      " "found"   ${Yellow})
package_status(ALSA_FOUND       "ALSA     " "found"   ${Yellow})
package_status(JACK_FOUND       "JACK     " "found"   ${Yellow})
package_status(PORTAUDIO_FOUND  "PA       " "found"   ${Yellow})
package_status(LASH_FOUND       "Lash     " "found"   ${Yellow})
package_status(DSSI_FOUND       "DSSI     " "found"   ${Yellow})
package_status(FLTK_FOUND       "fltk     " "found"   ${Yellow})
package_status(NTK_FOUND        "ntk      " "found"   ${Yellow})
package_status(X11_FOUND        "x11      " "found"   ${Yellow})
package_status(CXXTEST_FOUND    "CxxTest  " "found"   ${Yellow})
package_status(LashEnable       "Lash     " "enabled" ${Yellow})
package_status(DssiEnable       "DSSI     " "enabled" ${Yellow})
package_status(LibloEnable      "liblo    " "enabled" ${Red})
package_status(CompileTests     "tests    " "enabled" ${Yellow})
package_status(AlsaEnable       "ALSA     " "enabled" ${Yellow})
package_status(JackEnable       "JACK     " "enabled" ${Yellow})
package_status(OssEnable        "OSS      " "enabled" ${Yellow})
package_status(PaEnable         "PA       " "enabled" ${Yellow})
#TODO GUI MODULE
package_status(HAVE_ASYNC       "c++ async" "usable"  ${Yellow})


message(STATUS "Link libraries: ${ZLIB_LIBRARY} ${FFTW_LIBRARY} ${MXML_LIBRARIES} ${AUDIO_LIBRARIES} ${OS_LIBRARIES}")
