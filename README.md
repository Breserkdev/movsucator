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
│           ├── MCTargetDesc/      # Machine code description
│           ├── TargetInfo/        # Target information
│           └── ...               # Other target-specific files
├── include/               # Public headers
├── test/                 # Test files
└── docs/                 # Documentation
```

## Current Status
- ✅ Basic target infrastructure setup
- ✅ Register definitions and classes
- ✅ Basic MOV instruction definitions
- ✅ Memory operations support
- ⚠️ Frame management (in progress)
- ⚠️ ABI implementation (in progress)
- ⚠️ Conditional operations (planned)

## Contributing
Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments
- LLVM Team for the excellent compiler infrastructure
- Stephen Dolan for the proof that MOV is Turing-complete
