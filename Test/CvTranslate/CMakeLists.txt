find_package(OpenCV REQUIRED)

if(${USE_CUDA})
    include(${DIXTER_CMAKE_DIR}/cudnn.cmake)
    find_package(CUDA REQUIRED)
    enable_language(CUDA)
endif()

list(APPEND DIXTER_CVTR_INCLUDES    ${DIXTER_INCLUDE_DIR} ${DIXTER_INCLUDE_DIR}/cvtranslate ${OpenCV_INCLUDE_DIRS} ${CUDNN_INCLUDE_DIR})
list(APPEND DIXTER_CVTR_LIBRARIES   ${DIXTER_BASE} ${DIXTER_CVTR} ${OpenCV_LIBS} ${CUDA_cudadevrt_LIBRARY} ${CUDNN_LIBRARIES} ${CUDA_cudart_LIBRARY})

if(${USE_CUDA})
    add_target_module(Dixter DIXTER_CVTR cuda_test.cu "${DIXTER_CVTR_INCLUDES}" "${DIXTER_CVTR_LIBRARIES}")
else()
    add_target_module(Dixter DIXTER_CVTR cvtr_test.cpp "${DIXTER_CVTR_INCLUDES}" "${DIXTER_CVTR_LIBRARIES}")
endif()