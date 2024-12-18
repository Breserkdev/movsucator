#!/bin/bash

# Exit on error
set -e

# Configuration
LLVM_VERSION="release/17.x"
BUILD_TYPE="Debug"
ENABLE_ASSERTIONS="ON"
ENABLE_TESTS="ON"

# Command line arguments
BUILD_LLVM=true
BUILD_TARGET=true
RUN_TESTS=true
CLEAN_BUILD=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --skip-llvm)
            BUILD_LLVM=false
            shift
            ;;
        --skip-tests)
            RUN_TESTS=false
            shift
            ;;
        --target-only)
            BUILD_LLVM=false
            BUILD_TARGET=true
            RUN_TESTS=false
            CLEAN_BUILD=true
            shift
            ;;
        --clean)
            CLEAN_BUILD=true
            shift
            ;;
        --help)
            echo "Usage: $0 [options]"
            echo "Options:"
            echo "  --skip-llvm     Skip LLVM build"
            echo "  --skip-tests    Skip running tests"
            echo "  --target-only   Only build Movsucator target (implies clean build)"
            echo "  --clean         Clean build directories before building"
            echo "  --help          Show this help message"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Check for ninja
if ! command -v ninja &> /dev/null; then
    echo "Ninja build system not found. Installing via brew..."
    brew install ninja
fi

# Set up environment variables
export LLVM_DIR=$(pwd)/llvm-project
export LLVM_BUILD_DIR=${LLVM_DIR}/build

# Clean build directories if requested
if [ "$CLEAN_BUILD" = true ]; then
    echo "Cleaning build directories..."
    rm -rf build
    if [ "$BUILD_LLVM" = true ]; then
        rm -rf ${LLVM_BUILD_DIR}
    fi
fi

# Clone or update LLVM if needed
if [ ! -d "llvm-project" ]; then
    echo "Cloning LLVM..."
    git clone https://github.com/llvm/llvm-project.git
    cd llvm-project
    git checkout ${LLVM_VERSION}
    cd ..
fi

# Build LLVM if requested
if [ "$BUILD_LLVM" = true ]; then
    echo "Building LLVM..."
    mkdir -p ${LLVM_BUILD_DIR}
    cd ${LLVM_BUILD_DIR}
    
    cmake ../llvm \
        -G Ninja \
        -DLLVM_ENABLE_PROJECTS="clang" \
        -DLLVM_TARGETS_TO_BUILD="X86" \
        -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
        -DLLVM_ENABLE_ASSERTIONS=${ENABLE_ASSERTIONS} \
        -DLLVM_ENABLE_RTTI=ON \
        -DLLVM_ENABLE_EH=ON \
        -DLLVM_BUILD_EXAMPLES=OFF \
        -DLLVM_INCLUDE_TESTS=${ENABLE_TESTS} \
        -DLLVM_OPTIMIZED_TABLEGEN=ON \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    
    ninja
    cd ../..
fi

# Build Movsucator target if requested
if [ "$BUILD_TARGET" = true ]; then
    echo "Building Movsucator target..."
    mkdir -p build
    cd build
    
    # Enable building with PIC by default
    cmake .. \
        -G Ninja \
        -DLLVM_DIR="${LLVM_BUILD_DIR}/lib/cmake/llvm" \
        -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
        -DLLVM_ENABLE_ASSERTIONS=${ENABLE_ASSERTIONS} \
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    
    ninja
    cd ..
fi

# Run tests if requested
if [ "$RUN_TESTS" = true ]; then
    echo "Running tests..."
    
    # Simple test
    if [ -d "test/simple" ]; then
        echo "Running simple tests..."
        cd test/simple
        ${LLVM_BUILD_DIR}/bin/clang -O0 -target x86_64-apple-darwin test.c -o test
        ./test
        cd ../..
    fi
    
    # LLVM tests
    if [ -d "test/CodeGen/Movsucator" ]; then
        echo "Running LLVM tests..."
        cd test/CodeGen/Movsucator
        ${LLVM_BUILD_DIR}/bin/llvm-lit .
        cd ../../..
    fi
fi

echo "Build completed successfully!"