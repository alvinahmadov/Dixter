set(${DIXTER_BASE}_SOURCE_FILES
    Commons.cpp
    UnicodeStringWrapper.cpp
    Node.cpp
    Configuration.cpp
    SettingsController.cpp
    Utilities.cpp
    )

file(GLOB DB_SOURCE_FILES ${DIXTER_SOURCE_DIR}/database/*.cpp)
set(${DIXTER_BASE}_SOURCE_FILES ${${DIXTER_BASE}_SOURCE_FILES} ${DB_SOURCE_FILES})
unset(DB_SOURCE_FILES)

set(BOOST_COMPONENTS)

include(${DIXTER_CMAKE_DIR}/boost.cmake)

find_package(Qt5Core REQUIRED)

include(${DIXTER_CMAKE_DIR}/cppconnector.cmake)

find_package(Boost 1.65 COMPONENTS filesystem system timer REQUIRED)
if(NOT ${Boost_FOUND})
    message(FATAL_ERROR "Boost libraries not found")
endif()

if(NOT ${mycppconn_FOUND})
    message(FATAL_ERROR "Library mysqlcppconn not found")
endif()


set(${DIXTER_BASE}_TARGET_INCLUDE_DIRS
    ${DIXTER_INCLUDE_DIR}
    ${Unicode_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${mycppconn_INCLUDE_DIRS}
    )


set(${DIXTER_BASE}_TARGET_LIBRARIES
    Boost::filesystem
    Boost::system
    Boost::timer
    Qt5::Core
    ${Unicode_LIBRARIES}
    ${mycppconn_LIBRARIES}
    )

if(${OTR_TEST} OR ${BUILD_ALL})
    if(NOT ${BUILD_SIMPLE})
        file(GLOB NNET_SOURCE_FILES ${DIXTER_SOURCE_DIR}/nnet/*.cpp)
        set(${DIXTER_BASE}_SOURCE_FILES ${${DIXTER_BASE}_SOURCE_FILES} ${NNET_SOURCE_FILES})
    endif()
endif()

add_library(${DIXTER_BASE} SHARED ${${DIXTER_BASE}_SOURCE_FILES})
target_include_directories(${DIXTER_BASE} PUBLIC ${${DIXTER_BASE}_TARGET_INCLUDE_DIRS})
target_link_libraries(${DIXTER_BASE} PUBLIC ${${DIXTER_BASE}_TARGET_LIBRARIES})

if(${GUI_TEST})
    add_subdirectory(Gui)
elseif(${OTR_TEST})
    if(${ENABLE_BENCHMARK})
        add_compile_definitions(-DBENCHMARK_TEST)
    endif()
    add_compile_definitions(-DDICT_TESTING)
    add_subdirectory(OpenTranslate)
elseif(${CVTR_TEST})
    add_subdirectory(CvTranslate)
elseif(${VSYNTH_TEST})
    add_compile_definitions(-DUSE_SPEECHD)
    add_subdirectory(SpeechSynthesizer)
elseif(${BUILD_ALL})
    add_subdirectory(Gui)
    add_subdirectory(OpenTranslate)
    add_subdirectory(CvTranslate)
    add_subdirectory(SpeechSynthesizer)
endif()