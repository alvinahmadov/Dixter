function(check_found libname)
    if(NOT ${libname_FOUND})
        message(FATAL_ERROR "No module " ../CMake ${libname} ../CMake " found")
    else()
        
        message("-- Found module " ${libname} ", version: " ${${libname}_VERSION} "../CMake")
    endif()
endfunction(check_found)

function(add_package package_name required package_targets...)
    if(${required})
        find_package(${package_name} COMPONENTS ${package_targets...} REQUIRED)
    else()
        find_package(${package_name} COMPONENTS ${package_targets...})
    endif()
    check_found(${package_name})
endfunction(add_package)

function(add_package package_name required)
    if(${required})
        find_package(${package_name} REQUIRED)
    else()
        find_package(${package_name})
    endif()
    check_found(${package_name})
endfunction(add_package)

function(add_target_module executable_name module_name entry_file include_dir_list library_list)
    set(${module_name}_TEST_SOURCE_FILES ${entry_file})
    
    set(${module_name}_TEST_TARGET_INCLUDE_DIRS  ${include_dir_list})
    set(${module_name}_TEST_TARGET_LIBRARIES ${library_list})
    
    add_executable(${executable_name} ${${module_name}_TEST_SOURCE_FILES})
    target_include_directories(${executable_name} PUBLIC ${${module_name}_TEST_TARGET_INCLUDE_DIRS})
    target_link_libraries(     ${executable_name} PUBLIC ${${module_name}_TEST_TARGET_LIBRARIES})
endfunction(add_target_module)


function(cuda_add_target_module executable_name module_name entry_file include_dir_list library_list)
    set(${module_name}_TEST_SOURCE_FILES ${entry_file})
    
    set(${module_name}_TEST_TARGET_INCLUDE_DIRS  ${include_dir_list})
    set(${module_name}_TEST_TARGET_LIBRARIES ${library_list})
    
    CUDA_ADD_EXECUTABLE(${executable_name} ${${module_name}_TEST_SOURCE_FILES})
    target_include_directories(${executable_name} PUBLIC ${${module_name}_TEST_TARGET_INCLUDE_DIRS})
    target_link_libraries(     ${executable_name} PUBLIC ${${module_name}_TEST_TARGET_LIBRARIES})
endfunction(cuda_add_target_module)

function(download_3rdparty _download_path _extract_path _url _file)
    if(NOT EXISTS ${_download_path})
        file(MAKE_DIRECTORY ${_download_path})
        message(STATUS "Download path ${_download_path}")
    endif()
    
    if(NOT EXISTS ${_download_path}/${_file})
        message(STATUS "Downloading ${_url}/${_file} to ${_download_path}/${_file}")
        file(DOWNLOAD ${_url}/${_file} ${_download_path}/${_file} SHOW_PROGRESS)

        message(STATUS "Extracting ${_download_path}/${_file} to ${_extract_path}")
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xvfz "${_download_path}/${_file}"
                        WORKING_DIRECTORY "${_extract_path}")
    endif()
    
endfunction(download_3rdparty)