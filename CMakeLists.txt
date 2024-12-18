cmake_minimum_required(VERSION 3.13.4)
project(MovsucatorCompiler)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find LLVM
if(NOT DEFINED LLVM_DIR)
    if(DEFINED ENV{LLVM_DIR})
        set(LLVM_DIR $ENV{LLVM_DIR})
    else()
        set(LLVM_DIR "${CMAKE_SOURCE_DIR}/llvm-project/build/lib/cmake/llvm")
    endif()
endif()

list(APPEND CMAKE_PREFIX_PATH "${LLVM_DIR}")

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)
include(HandleLLVMOptions)
include(TableGen)

# Make sure LLVM_TABLEGEN_EXE is set
if(NOT LLVM_TABLEGEN_EXE)
    set(LLVM_TABLEGEN_EXE "${CMAKE_SOURCE_DIR}/llvm-project/build/bin/llvm-tblgen")
endif()

message(STATUS "Using TableGen: ${LLVM_TABLEGEN_EXE}")

# Add LLVM source include directory
include_directories(${CMAKE_SOURCE_DIR}/llvm-project/llvm/include)
include_directories(${CMAKE_SOURCE_DIR}/llvm-project/llvm/include/llvm)
include_directories(${CMAKE_SOURCE_DIR}/llvm-project/llvm/include/llvm/MC)
include_directories(${CMAKE_SOURCE_DIR}/llvm-project/llvm/include/llvm/Support)
include_directories(${CMAKE_SOURCE_DIR}/llvm-project/llvm/include/llvm/TargetParser)
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
include_directories(${CMAKE_BINARY_DIR}/include)

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Add C++ compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

# Add Movsucator target
add_subdirectory(lib/Target/Movsucator)
