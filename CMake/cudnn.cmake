find_path(CUDNN_INCLUDE_DIR cudnn.h
          HINTS ${CUDA_INCLUDE_DIRS} ENV CUDNN_INCLUDE_DIR  ENV CUDNN_HOME
          PATHS /usr/local ENV CPATH
          PATH_SUFFIXES include
          )
get_filename_component(cudnn_hint_path "${CUDA_CUBLAS_LIBRARIES}" PATH)
find_library(CUDNN_LIBRARIES cudnn
             HINTS ${cudnn_hint_path} ENV CUDNN_LIBRARY_DIR  ENV CUDNN_HOME
             PATHS /usr/local /usr/local/cuda ENV LD_LIBRARY_PATH
             PATH_SUFFIXES lib64 lib x64
             )
mark_as_advanced(CUDNN_LIBRARIES CUDNN_INCLUDE_DIR)

if (CUDNN_LIBRARIES AND CUDNN_INCLUDE_DIR)
    message(STATUS "Found cuDNN: " ${CUDNN_LIBRARIES})
else()
    message(STATUS "*** cuDNN V5.0 OR GREATER NOT FOUND.                                                       ***")
    message(STATUS "*** Dlib requires cuDNN V5.0 OR GREATER.  Since cuDNN is not found DLIB WILL NOT USE CUDA. ***")
    message(STATUS "*** If you have cuDNN then set CMAKE_PREFIX_PATH to include cuDNN's folder.                ***")
endif()