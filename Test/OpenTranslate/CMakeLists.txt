list(APPEND DIXTER_OTR_INCLUDES     ${DIXTER_INCLUDE_DIR} ${OT_SRC})
list(APPEND DIXTER_OTR_LIBRARIES    ${DIXTER_OT})

add_definitions(-DOTR_TESTING)
#add_definitions(-DDICT_TESTING)

set(OT_TARGET DixterOt)
set(OT_TARGET_SOURCES DixterOt.cpp)

add_target_module(${OT_TARGET} DIXTER_OT "${OT_TARGET_SOURCES}"
                  "${DIXTER_OTR_INCLUDES}" "${DIXTER_OTR_LIBRARIES}")

add_custom_command(
        TARGET ${OT_TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${OT_TARGET}>
        ${EXECUTABLE_OUTPUT_PATH}
)