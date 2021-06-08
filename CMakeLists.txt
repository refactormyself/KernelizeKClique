cmake_minimum_required (VERSION 3.10)
PROJECT(KCliqueKernel LANGUAGES CXX)
#set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_STANDARD 17)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra")
#set(CMAKE_CXX_FLAGS_DEBUG "-g -O3")
set(CMAKE_CXX_FLAGS_DEBUG "-g -fopenmp")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fopenmp")

find_package(Threads REQUIRED)
link_libraries(stdc++fs)
enable_testing()

# copy ./data to ./build
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


include_directories(include)
find_package(OpenMP)

# setup for dominate
add_executable(dominate src/main.cpp src/graph.cpp src/serial_dominate.cpp)
target_link_libraries(dominate PRIVATE Threads::Threads)

if(OpenMP_CXX_FOUND)
    target_sources(dominate PRIVATE src/omp_dominate.cpp include/omp_dominate.hpp)
endif()


#find_package(CUDA QUIET REQUIRED)

if(CUDA_FOUND)
    MESSAGE ("CUDA_VERSION_MAJOR: ${CUDA_VERSION_MAJOR}")
    MESSAGE ("CUDA_VERSION_MINOR: ${CUDA_VERSION_MINOR}")
    MESSAGE ("CUDA_VERSION: ${CUDA_VERSION}")
    MESSAGE ("CUDA_VERSION_STRING: ${CUDA_VERSION_STRING}")

    MESSAGE ("CUDA_TOOLKIT_ROOT_DIR: ${CUDA_TOOLKIT_ROOT_DIR}")
    MESSAGE ("CUDA_SDK_ROOT_DIR: ${CUDA_SDK_ROOT_DIR}")
    MESSAGE ("CUDA_INCLUDE_DIRS: ${CUDA_INCLUDE_DIRS}")
    MESSAGE ("CUDA_LIBRARIES: ${CUDA_LIBRARIES}")

    add_definitions(
            -D_CRT_SECURE_NO_DEPRECATE
    )

    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
    endif ()
    set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(CUDA_NVCC_FLAGS "-arch=sm_20") # old fermi :(

#    cuda_add_executable (cuda_dominate "src/edgedom.cu")
#
#    target_link_libraries(
#            cuda_dominate
#            ${CUDA_LIBRARIES}
#    )
endif()


