set(LIB_NAME "mycppconn")

if(DEFINED MYCPPCONN_STATIC)
    set(LIB_EXT  ".a")
    else()
    set(LIB_EXT  ".so")
endif()

set(${LIB_NAME}_INCLUDE_DIR /usr/include)
set(${LIB_NAME}_INCLUDE_DIRS ${${LIB_NAME}_INCLUDE_DIR})
set(${LIB_NAME}_LIBDIR /usr/lib/x86_64-linux-gnu)
set(${LIB_NAME}_LIBRARIES mysqlcppconn)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(${LIB_NAME} DEFAULT_MSG
                                  ${LIB_NAME}_LIBRARIES ${LIB_NAME}_INCLUDE_DIRS)

mark_as_advanced(${LIB_NAME}_INCLUDE_DIRS ${LIB_NAME}_LIBRARIES)