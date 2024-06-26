cmake_minimum_required (VERSION 2.8)
project (neblina-opencl-bridge)

include(CTest)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR}) 

find_package(OpenCL REQUIRED)
include_directories( ${OPENCL_INCLUDE_DIR} )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
#set(API_LIBRARIES ${OPENCL_LIBRARIES})
#set(API_INCLUDE_DIRS ${OPENCL_INCLUDE_DIRS})

add_compile_options(
  "-Wall" "-Wpedantic" "-Wextra" "-fexceptions"
  "-O0;-g3;-ggdb" "-fcommon"
)
message("OpenCL_LIBRARY ${OpenCL_LIBRARY}")
message("PROJECT ${PROJECT_SOURCE_DIR}")
set(HEADER_PATH ${PROJECT_SOURCE_DIR}/include)
set(NEBLINA_HEADER_PATH /usr/local/include/)
set(SOURCE_PATH ${PROJECT_SOURCE_DIR}/src)
set(DIST_PATH ${PROJECT_SOURCE_DIR}/dist)
set(DIST_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/dist/include)
set(PROJECT_TEST_DIR ${PROJECT_SOURCE_DIR}/test)
#set(LCOV_PATH /usr/bin/lcov)
#set(GENHTML_PATH /usr/bin/genhtml)

message(STATUS "PROJECT ${NEBLINA_HEADER_PATH}")
include_directories(${HEADER_PATH} ${NEBLINA_HEADER_PATH})
LINK_DIRECTORIES(${PROJECT_DIR})

#if(CMAKE_BUILD_TYPE STREQUAL "coverage" OR CODE_COVERAGE)
#    if(CMAKE_COMPILER_IS_GNUCXX)
#        message("Building with lcov Code Coverage Tools")
#        # Warning/Error messages
#        if(NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
#            message(WARNING "Code coverage results with an optimized (non-Debug) build may be misleading")
#        endif()
#        if(NOT LCOV_PATH)
#            message(FATAL_ERROR "lcov not found! Aborting...")
#        endif()
#        if(NOT GENHTML_PATH)
#            message(FATAL_ERROR "genhtml not found! Aborting...")
#        endif()
#        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
#        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
#    else()
#        message(FATAL_ERROR "Code coverage requires Clang or GCC. Aborting.")
#    endif()
#endif()

add_library(neblina-opencl-bridge SHARED 
                                ${SOURCE_PATH}/oclvector.c 
                                ${SOURCE_PATH}/clutils.c 
                                ${SOURCE_PATH}/neblina_list.c 
                                ${SOURCE_PATH}/neblina_vector.c 
                                ${SOURCE_PATH}/neblina_matrix.c 
                                ${SOURCE_PATH}/neblina_smatrix.c 
                                ${SOURCE_PATH}/neblina_complex.c 
                                 )
target_link_libraries(neblina-opencl-bridge PRIVATE Threads::Threads ${OpenCL_LIBRARY})

add_executable(vector_test ${PROJECT_TEST_DIR}/vector_test.cpp)
target_link_libraries(vector_test PRIVATE Threads::Threads ${OpenCL_LIBRARY} ${GTEST_BOTH_LIBRARIES} neblina-core)

add_executable(matrix_test ${PROJECT_TEST_DIR}/matrix_test.cpp)
target_link_libraries(matrix_test PRIVATE Threads::Threads ${OpenCL_LIBRARY} ${GTEST_BOTH_LIBRARIES} neblina-core)

add_executable(sparse_matrix_test ${PROJECT_TEST_DIR}/sparse_matrix_test.cpp)
target_link_libraries(sparse_matrix_test PRIVATE Threads::Threads ${OpenCL_LIBRARY} ${GTEST_BOTH_LIBRARIES} neblina-core)

add_test( vector_test vector_test )
add_test( matrix_test matrix_test )
add_test( sparse_matrix_test sparse_matrix_test )


# Installs the library
include(GNUInstallDirs)

install (TARGETS neblina-opencl-bridge DESTINATION ${CMAKE_INSTALL_LIBDIR})
install (FILES ${HEADER_PATH}/clutils.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES ${HEADER_PATH}/vector.cl DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
