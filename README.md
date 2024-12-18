# Movsucator Compiler

## Overview
Movsucator is an LLVM-based compiler targeting a MOV-only instruction set architecture, demonstrating that MOV is Turing-complete. This project implements a complete backend for LLVM that translates LLVM IR into a sequence of MOV instructions.

## Features
- MOV-only instruction set implementation
- LLVM 17.0.6 integration
- Custom register allocation
- Memory operation support
- Basic calling convention implementation
- MOV-based arithmetic operations
- Stack frame management

## Prerequisites
- LLVM 17.0.6
- CMake (3.13.4 or higher)
- Ninja build system
- C++17 compatible compiler

## Building from Source

1. Clone the repository:
```bash
git clone https://github.com/yourusername/movsucator.git
cd movsucator
```

2. Build LLVM and Movsucator:
```bash
# Build everything (LLVM + Movsucator)
./build.sh

# Build only Movsucator (if LLVM is already built)
./build.sh --target-only

# Skip running tests
./build.sh --skip-tests

# Clean build
./build.sh --clean
```

## Project Structure
```
movsucator/
├── CMakeLists.txt          # Main CMake configuration
├── build.sh               # Build script
├── lib/
│   └── Target/
│       └── Movsucator/    # Movsucator backend implementation
│           ├── MCTargetDesc/      # Machine code description
│           ├── TargetInfo/        # Target information
│           └── ...               # Other target-specific files
├── include/               # Public headers
├── test/                 # Test files
│   └── CodeGen/
│       └── Movsucator/   # LLVM IR test cases
└── docs/                 # Documentation
```

## Testing
The project includes several test cases in the `test/CodeGen/Movsucator` directory:
- `arithmetic.ll`: Tests for MOV-based arithmetic operations
- `memory.ll`: Tests for memory operations
- `controlflow.ll`: Tests for control flow constructs

Run tests using:
```bash
./build.sh --skip-llvm
cd test/CodeGen/Movsucator
../../../llvm-project/build/bin/llvm-lit .
```

## Current Status
- ✅ Basic target infrastructure setup
- ✅ Register definitions and classes
- ✅ Basic MOV instruction definitions
- ✅ Memory operations support
- ✅ MOV-based arithmetic operations
- ✅ Stack frame management
- ⚠️ ABI implementation (in progress)
- ⚠️ Conditional operations (planned)

## Contributing
Contributions are welcome! Please read the contributing guidelines before submitting pull requests.

## License
This project is licensed under the Apache License 2.0 - see the LICENSE file for details.

## Acknowledgments
- LLVM Team for the excellent compiler infrastructure
- Stephen Dolan for the proof that MOV is Turing-complete
