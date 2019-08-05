set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(TEST_LINK_LIBRARIES
    ${DIXTER_GUI} ${DIXTER_VSYNTH} ${DIXTER_CVTR} ${DIXTER_OT}
    )

set(TEST_INCLUDE_DIRS
    ${DIXTER_INCLUDE_DIR}
    ${DIXTER_INCLUDE_DIR}/OpenTranslate
    ${DIXTER_INCLUDE_DIR}/CvTranslate
    ${DIXTER_INCLUDE_DIR}/SpeechSynthesizer
    ${DIXTER_INCLUDE_DIR}/Gui
    )

if(${GUI_TEST})
    add_subdirectory(Gui)
elseif(${OTR_TEST})
    if(${ENABLE_BENCHMARK})
        add_definitions(-DBENCHMARK_TEST)
    endif()
    add_definitions(-DDICT_TESTING)
    add_subdirectory(OpenTranslate)
elseif(${CVTR_TEST})
    add_subdirectory(CvTranslate)
elseif(${VSYNTH_TEST})
    add_definitions(USE_SPEECHD)
    add_subdirectory(SpeechSynthesizer)
elseif(${BUILD_ALL})
    set(TEST_SOURCE_FILES Dixter.cpp)
    add_executable(Dixter ../app.qrc ${TEST_SOURCE_FILES})
    target_include_directories(Dixter PUBLIC ${TEST_INCLUDE_DIRS})
    target_link_libraries(Dixter PUBLIC ${TEST_LINK_LIBRARIES})
endif()