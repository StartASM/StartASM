#ifndef COMPILER_H
#define COMPILER_H

#include "lexer/Lexer.h"
#include "ast/AbstractSyntaxTree.h"

#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

class Parser;
class SymbolResolver;
class ASTBuilder;
class SemanticAnalyzer;
class ScopeChecker;
class CodeGenerator;

class Compiler {
    public:
        //Constructors and Destructors
        Compiler(std::string& pathname, bool cmdSilent, bool cmdTimings, bool cmdIr);
        ~Compiler();

        Compiler(const Compiler&) = delete;
        Compiler& operator=(const Compiler&) = delete;

        //Accessors
        //Get number of lines
        [[nodiscard]] int getNumLines() const {
            return int(m_codeLines.size());
        }
        //Get current status
        [[nodiscard]] std::string getStatus() const {
            return m_statusMessage;
        }

        //Mutators
        //Change pathname
        void changePath(std::string& pathname) {
            m_pathname = pathname;
        }

        //Printers
        void cmdPrint(const std::string& message) const;
        void cmdTimingPrint(const std::string& message) const;

        //Public facing compile method
        //Code Compiling
        bool compileCode();
        //Public facing tree method (for interpreter
        bool outputAST();


    private:
        //Private variables
        //Data structures
        //Vector containing code lines
        std::vector<std::string> m_codeLines;
        //Vector containing code tokens and tags
        std::vector<std::vector<std::pair<std::string, LexerConstants::TokenType>>> m_codeTokens;
        //Parse tree for the code
        PT::ParseTree* m_parseTree;
        //Hash table for symbol resolution, mapping labels to instruction addresses
        std::unordered_map<std::string, std::pair<std::string, int>> m_symbolTable;

        //Variables and data structures
        //Pathname
        std::string m_pathname;
        //String containing current status
        std::string m_statusMessage;
        //Lexer
        Lexer* m_lexer;
        //Parser (PT nested inside parser)
        Parser* m_parser;
        //Symbol Resolver
        SymbolResolver* m_symbolResolver;
        //AST (used directly by the compiler at multiple stages)
        AST::AbstractSyntaxTree* m_AST;
        //AST Builder
        ASTBuilder* m_ASTBuilder;
        //Pointer to semantic analyzer
        SemanticAnalyzer* m_semanticAnalyzer;
        //Pointer to scope checker
        ScopeChecker* m_scopeChecker;
        //Pointer to code generator
        CodeGenerator* m_codeGenerator;

        //Terminal options
        bool cmd_silent;
        bool cmd_timings;
        bool cmd_tree;
        bool cmd_ir;
};

#endif