cmake_minimum_required(VERSION 3.10)
project(startasm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Ensure position-independent code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Set the CMAKE_PREFIX_PATH to the directory containing LLVMConfig.cmake if necessary
if(APPLE)
    set(CMAKE_PREFIX_PATH "/usr/local/opt/llvm")
    set(LLVM_DIR "/usr/local/opt/llvm/lib/cmake/llvm")
    # Set the compiler to use Homebrew LLVM
    set(CMAKE_C_COMPILER "/usr/local/opt/llvm/bin/clang")
    set(CMAKE_CXX_COMPILER "/usr/local/opt/llvm/bin/clang++")
    set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp -I/usr/local/opt/llvm/include")
    set(OpenMP_C_LIB_NAMES "omp")
    set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I/usr/local/opt/llvm/include")
    set(OpenMP_CXX_LIB_NAMES "omp")
    set(OpenMP_omp_LIBRARY "/usr/local/opt/llvm/lib/libomp.dylib")
endif()

# Find LLVM package
find_package(LLVM REQUIRED CONFIG)

# Include LLVM headers
include_directories(${LLVM_INCLUDE_DIRS})

# Find OpenMP package
find_package(OpenMP REQUIRED)

# Add LLVM and OpenMP definitions
add_definitions(${LLVM_DEFINITIONS})
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

# List all source files and headers
set(SOURCES
        src/compiler/Compiler.cpp
        src/lexer/Lexer.cpp
        src/parser/Parser.cpp
        src/semantics/SemanticAnalyzer.cpp
        src/compiler/StartASM.cpp
        src/codegen/CodeGenerator.cpp
        src/misc/.Secrets.cpp
        src/scopecheck/ScopeChecker.cpp
        src/symbolres/SymbolResolver.cpp
        src/ast/ASTBuilder.cpp
        src/ast/AbstractSyntaxTree.cpp
        src/pt/ParseTree.cpp
)

set(HEADERS
        include/compiler/Compiler.h
        include/ast/Instructions.h
        include/lexer/Lexer.h
        include/parser/Parser.h
        include/ast/AbstractSyntaxTree.h
        include/semantics/SemanticAnalyzer.h
        include/codegen/CodeGenerator.h
        include/misc/.Secrets.h
        include/symbolres/SymbolResolver.h
        include/ast/ASTBuilder.h
        include/scopecheck/ScopeChecker.h
        include/ast/Visitor.h
        include/ast/Operands.h
        include/lib/json.hpp
)

# Specify the executable target
add_executable(startasm ${SOURCES} ${HEADERS})

# Include project headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Link against LLVM and OpenMP libraries
llvm_map_components_to_libnames(LLVM_LIBS core support irreader)
target_link_libraries(startasm ${LLVM_LIBS} OpenMP::OpenMP_CXX)

# Set the output directory for the executable to the root folder
set_target_properties(startasm PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# Platform-specific settings
if(APPLE)
    message(STATUS "Configuring for macOS")
    # macOS-specific settings here
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    find_library(LIBCXX_LIB c++)
    find_library(LIBCXXABI_LIB c++abi)
    target_link_libraries(startasm ${LIBCXX_LIB} ${LIBCXXABI_LIB})
elseif(UNIX)
    message(STATUS "Configuring for Linux")
    # Linux-specific settings here
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie -lstdc++fs")
endif()

# Ensure CMake RPATH settings for macOS
if(APPLE)
    set(CMAKE_MACOSX_RPATH 1)
    set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
else()
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif()
