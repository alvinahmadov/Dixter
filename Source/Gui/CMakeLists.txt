    find_package(Qt5Gui ${QtVersion} REQUIRED)
    find_package(Qt5Widgets ${QtVersion} REQUIRED)

    set(${DIXTER_GUI}_TARGET_LIBRARIES
        ${DIXTER_BASE}
        Qt5::Gui
        Qt5::Widgets
        )

    file(GLOB ${DIXTER_GUI}_SOURCE_FILES ${GUI_SRC}/*.cpp
         ${OT_SRC}/Dictionary.cpp)

    
    add_library(${DIXTER_GUI} SHARED ${${DIXTER_GUI}_SOURCE_FILES})
    target_include_directories(${DIXTER_GUI} PUBLIC ${${DIXTER_GUI}_TARGET_INCLUDE_DIRS})
    target_link_libraries(${DIXTER_GUI} PUBLIC ${${DIXTER_GUI}_TARGET_LIBRARIES})
    
    set_target_properties(
            ${DIXTER_GUI}
            PROPERTIES
            AUTOMOC ON
            AUTOUIC ON
            AUTORCC ON
    )