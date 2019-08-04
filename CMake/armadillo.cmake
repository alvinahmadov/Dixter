set(LIB_NAME "armadillo")
set(${LIB_NAME}_VERSION "8.4")

if(DEFINED UNIX)
    set(${LIB_NAME}_PREFIX "/usr")
    set(${LIB_NAME}_INCLUDEDIR "${${LIB_NAME}_PREFIX}/include ${${LIB_NAME}_PREFIX}/include/armadillo_bits")
    set(${LIB_NAME}_LIBDIR "${${LIB_NAME}_PREFIX}/lib")
endif()

set(${LIB_NAME}_INCLUDE_DIRS ${${LIB_NAME}_INCLUDEDIR}/speech-dispatcher)
set(${LIB_NAME}_LIBRARIES armadillo)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(${LIB_NAME} DEFAULT_MSG
                                  ${LIB_NAME}_LIBRARIES ${LIB_NAME}_INCLUDE_DIRS)

mark_as_advanced(${LIB_NAME}_INCLUDE_DIRS ${LIB_NAME}_LIBRARIES)