cmake_minimum_required(VERSION 3.10)
project(startasm)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Ensure position-independent code (for shared libraries)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Find LLVM package
find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Find OpenMP package
find_package(OpenMP REQUIRED)

# Add definitions for ABI compatibility
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

# Source files and headers
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

# Define executable target
add_executable(startasm ${SOURCES} ${HEADERS})

# Include headers from the include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Link against LLVM and OpenMP libraries
llvm_map_components_to_libnames(LLVM_LIBS core support irreader)
target_link_libraries(startasm ${LLVM_LIBS} OpenMP::OpenMP_CXX)

# Set the output directory for the executable to the root folder
set_target_properties(startasm PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)