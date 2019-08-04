set(cpprestsdk_DIR "/usr/lib/x86_64-linux-gnu/cmake")
find_package(cpprestsdk REQUIRED COMPONENTS cpprest cpprestsdk_boost_internal cpprestsdk_zlib_internal cpprestsdk_openssl_internal cpprestsdk_websocketpp_internal)

if(cpprestsdk_FOUND)
    include_directories(${cpprestsdk_INCLUDE_DIRS})
    link_directories(${cpprestsdk_LIBRARY_DIRS})
    link_libraries(${cpprestsdk_LIBRARIES})
endif()