set(SPD_LIB "speechd")
set(${SPD_LIB}_VERSION "1.0")

if(DEFINED WIN32)
    set(PREFIX "C:\\Dev\\gtkmm")
    set(LIBDIR "${PREFIX}/lib/x86_64-linux-gnu")
elseif(DEFINED UNIX)
    set(${SPD_LIB}_PREFIX "/usr")
    set(${SPD_LIB}_INCLUDEDIR "${${SPD_LIB}_PREFIX}/include")
    set(${SPD_LIB}_LIBDIR "${${SPD_LIB}_PREFIX}/lib/x86_64-linux-gnu")
endif()

set(${SPD_LIB}_INCLUDE_DIRS ${${SPD_LIB}_INCLUDEDIR}/speech-dispatcher)
set(${SPD_LIB}_LIBRARIES speechd)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(${SPD_LIB} DEFAULT_MSG
                                  ${SPD_LIB}_LIBRARIES ${SPD_LIB}_INCLUDE_DIRS)

mark_as_advanced(${SPD_LIB}_INCLUDE_DIRS ${SPD_LIB}_LIBRARIES)