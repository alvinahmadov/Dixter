include(${DIXTER_CMAKE_DIR}/armadillo.cmake)

set(${DIXTER_OT}_TARGET_INCLUDES
    ${${DIXTER_OT}_TARGET_INCLUDES}
    ${${DIXTER_BASE}_TARGET_INCLUDE_DIRS}
    ${armadillo_INCLUDE_DIRS}
    )

set(${DIXTER_OT}_TARGET_LIBRARIES
    ${${DIXTER_OT}_TARGET_LIBRARIES}
    ${armadillo_LIBRARIES}
    ${DIXTER_BASE}
    )

file(GLOB ${DIXTER_OT}_SOURCE_FILES ${OT_SRC}/*.cpp)

add_library(${DIXTER_OT} SHARED ${${DIXTER_OT}_SOURCE_FILES})
target_include_directories(${DIXTER_OT} PUBLIC ${${DIXTER_OT}_TARGET_INCLUDES})
target_link_libraries(${DIXTER_OT} PUBLIC ${${DIXTER_OT}_TARGET_LIBRARIES})