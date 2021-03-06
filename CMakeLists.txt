cmake_minimum_required(VERSION 3.10)
project(Dixter C CXX)
set(CMAKE_CXX_STANDARD 17)

set(DIXTER_VERSION_MAJOR "0")
set(DIXTER_VERSION_MINOR "1")
set(DIXTER_VERSION_PATCH "0")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_AUTORCC ON)

set(DIXTER_VERSION "${DIXTER_VERSION_MAJOR}.${DIXTER_VERSION_MINOR}.${DIXTER_VERSION_PATCH}")

option(USE_CUDA "Include cuda support" OFF)
option(CMAKE_CUDA_HOST_COMPILER "CUDA Compiler" /usr/local/cuda/bin/nvcc)
option(BUILD_PROJECT_LIBS "Build project libraries" TRUE)
option(CMAKE_BUILD_TYPE CACHE Release)
set(ENABLE_BENCHMARK TRUE)

set(DIXTER_HOME_DIR                     ${PROJECT_SOURCE_DIR})
set(DIXTER_CMAKE_DIR                    ${DIXTER_HOME_DIR}/CMake)
set(DIXTER_SOURCE_DIR                   ${DIXTER_HOME_DIR}/Source)
set(DIXTER_INCLUDE_DIR                  ${DIXTER_SOURCE_DIR})
set(DIXTER_LIBRARY_DIR                  ${CMAKE_BINARY_DIR}/DixterApp)
set(3DPARTY_DIR                         ${DIXTER_HOME_DIR}/3rdParty)
set(DIXTER_TEST_DIR                     ${DIXTER_HOME_DIR}/Test)
set(LIBRARY_OUTPUT_PATH                 ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY     ${DIXTER_LIBRARY_DIR})
set(EXECUTABLE_OUTPUT_PATH              ${DIXTER_LIBRARY_DIR})

include(${DIXTER_CMAKE_DIR}/CMakeUtils.cmake)

if(${USE_CUDA})
    enable_language(CUDA)
    include(FindCUDA)
    set(CMAKE_CXX_FLAGS "-Wno-user-defined-literals")
    add_definitions(-DCUDA_ENABLED)
    message(STATUS "CUDA ${CUDA_VERSION} enabled.")
    include_directories(${CUDA_INCLUDE_DIRS})
    link_libraries(${CUDA_LIBRARIES})
    
    if(${CUDA_VERSION} GREATER_EQUAL "10")
        set(CMAKE_CXX_STANDARD 17)
    else()
        set(CMAKE_CXX_STANDARD 14)
    endif()
    set(CMAKE_CUDA_STANDARD ${CMAKE_CXX_STANDARD})
endif()

if(UNIX)
	list(APPEND CMAKE_PREFIX_PATH /opt/Qt/5.12.4/gcc_64 /usr)
elseif(WIN32)
	list(APPEND CMAKE_PREFIX_PATH "C://Dev")
endif()

set(QtVersion "5.12")
    
find_package(Qt5Core ${QtVersion} REQUIRED)
include(${DIXTER_CMAKE_DIR}/CMakeUtils.cmake)

if(UNIX)
    include(FindPkgConfig)
    pkg_check_modules(Unicode icu-uc)
    pkg_check_modules(UnicodeIO icu-io)
else()

endif(UNIX)

if(UNIX)
    set(CXX_FLAGS               "-fPIC -Wall -Wextra -Wno-return-type -Wno-ignored-qualifiers -Wno-delete-non-virtual-dtor")
    set(CXX_FLAGS_DEBUG         "-g -O0")
    set(CXX_FLAGS_RELEASE       "-O3 -ffp-contract=off -march=native")
elseif(WIN32)
    set(CXX_FLAGS               "-Wall -EHsc")
    set(CXX_FLAGS_DEBUG         "-g")
    set(CXX_FLAGS_RELEASE       "-O3 -ffp-contract=off -fno-expensive-optimizations -march=native")
endif()

include_directories(${3DPARTY_DIR})

# Shared libraries to build
set(DIXTER_BASE         dxbase)
set(DIXTER_GUI          dxgui)
set(DIXTER_OT           dxot)
set(DIXTER_VSYNTH       dxvsynth)
set(DIXTER_CVTR         dxcvtr)

set(GUI_SRC             ${DIXTER_SOURCE_DIR}/Gui)
set(OT_SRC              ${DIXTER_SOURCE_DIR}/OpenTranslate)
set(VSYNTH_SRC          ${DIXTER_SOURCE_DIR}/SpeechSynthesizer)
set(CVTR_SRC            ${DIXTER_SOURCE_DIR}/CvTranslate)

# Tests
set(GUI_TEST            TRUE)
set(OTR_TEST            FALSE)
set(CVTR_TEST           FALSE)
set(VSYNTH_TEST         FALSE)
set(BUILD_ALL           FALSE)

set(SOURCE_FILES        ${DIXTER_TEST_DIR}/main.cpp)

if(${ENABLE_BENCHMARK})
    add_definitions(-DPERFORMANCE_TEST)
endif()

set(CMAKE_CXX_FLAGS             ${CXX_FLAGS})
set(CMAKE_CXX_FLAGS_DEBUG       ${CXX_FLAGS_DEBUG})
set(CMAKE_CXX_FLAGS_RELEASE     ${CXX_FLAGS_RELEASE})

set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH} ${DIXTER_CMAKE_DIR}")

add_subdirectory(Source)
add_subdirectory(Test)