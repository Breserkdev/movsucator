# Movsucator Compiler

## Overview
Movsucator is an LLVM-based compiler targeting a MOV-only instruction set architecture, demonstrating that MOV is Turing-complete. This project implements a complete backend for LLVM that translates LLVM IR into a sequence of MOV instructions.

## Features
- MOV-only instruction set implementation
- LLVM 17.0.6 integration
- Custom register allocation
- Memory operation support
- Basic calling convention implementation

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
./build.sh --skip-llvm

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
├── include/               # Public headers
├── test/                 # Test files
└── docs/                 # Documentation
```

## Implementation Details
The compiler implements the following key components:
- Register allocation using 4 general-purpose registers (A, B, C, S)
- MOV instruction variants (register-to-register, immediate-to-register, memory operations)
- Basic calling convention
- Frame management
- Instruction selection patterns

## Contributing
Contributions are welcome! Please feel free to submit a Pull Request.

## License
This project is licensed under the Apache License 2.0 - see the LICENSE file for details.

## Acknowledgments
- Based on the paper "Mov Is Turing-Complete" by Stephen Dolan
- Built using the LLVM compiler infrastructure

## Status
See [current_status.md](current_status.md) for detailed project status and upcoming features.
