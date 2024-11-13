#include "main/Compiler.h"
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "symbolres/SymbolResolver.h"
#include "ast/AbstractSyntaxTree.h"
#include "ast/ASTBuilder.h"
#include "semantics/SemanticAnalyzer.h"
#include "scopecheck/ScopeChecker.h"
#include "codegen/CodeGenerator.h"

#include <iostream>
#include <vector>
#include <string>
#include <omp.h>
#include <future>

using namespace std;

Compiler::Compiler(std::string& pathname, bool cmdSilent, bool cmdTimings, bool cmdTree, bool cmdIr) :
    cmd_silent(cmdSilent),
    cmd_timings(cmdTimings),
    cmd_tree(cmdTree),
    cmd_ir(cmdIr),
    m_lexer(new Lexer()),
    m_parser(new Parser()),
    m_parseTree(new PT::ParseTree()),
    m_symbolResolver(new SymbolResolver()),
    m_AST(new AST::AbstractSyntaxTree()),
    m_ASTBuilder(new ASTBuilder()),
    m_semanticAnalyzer(new SemanticAnalyzer(m_codeLines)),
    m_scopeChecker(new ScopeChecker(m_codeLines)),
    m_codeGenerator(new CodeGenerator()),
    m_pathname(pathname),
    m_numLines(0) {}

Compiler::~Compiler() {
    delete m_lexer;
    delete m_parser;
    delete m_symbolResolver;
    delete m_AST;
    delete m_ASTBuilder;
    delete m_semanticAnalyzer;
    delete m_scopeChecker;
    delete m_codeGenerator;
}

void Compiler::cmdPrint(const std::string& message) const {
    if (!cmd_silent) {
        cout << message;
    }
}

void Compiler::cmdTimingPrint(const std::string& message) const {
    if (!cmd_silent && cmd_timings) {
        cout << message;
    }
}

bool Compiler::compileCode() {
    double start = omp_get_wtime();
    //Lex code//
    cmdTimingPrint("Compiler: Lexing code\n");
    if (!m_lexer->lexFile(m_pathname, m_codeLines, m_codeTokens)) {
        m_statusMessage = "Lexing failed! Either the path was invalid or the file could not be found.";
        return false;
    }
    m_numLines = m_codeLines.size();
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");

    //Parse code//
    cmdTimingPrint("Compiler: Parsing code\n");
    start = omp_get_wtime();
    if(!m_parser->parseCode(m_parseTree, m_codeLines, m_codeTokens, m_statusMessage)) {
        return false;
    }
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");

    // Resolve symbols while concurrently deleting lexer tokens //
    cmdTimingPrint("Compiler: Resolving symbols\n");
    start = omp_get_wtime();
    auto codeTokensDeletionFuture = std::async(std::launch::async, [this] {
        m_codeTokens.clear();
        m_codeTokens.shrink_to_fit();
    });
    bool resolveSymbolsResult = m_symbolResolver->resolveSymbols(m_symbolTable, m_parseTree->getRoot(), m_statusMessage, m_codeLines);
    codeTokensDeletionFuture.get();
    if(!resolveSymbolsResult) {
        return false;
    }
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime() - start) + "\n\n");

    //Delete the lexer and build the AST concurrently//
    cmdTimingPrint("Compiler: Building AST\n");
    start = omp_get_wtime();
    //Delete the lexer after PT creation is finished! It's no longer needed
    auto lexerDeletionFuture = std::async(std::launch::async, [this] {
        delete m_lexer;
        m_lexer = nullptr;
    });
    m_ASTBuilder->buildAST(m_parseTree->getRoot(), m_AST);
    lexerDeletionFuture.get();
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");
    if(cmd_tree && !cmd_silent) {
        cout << endl;
        cout << "AST for '" + m_pathname + "':\n";
        m_AST->printTree();
        cout << endl;
    }

    //Check address scopes and analyze semantics while deleting the parser and parseTree//
    cmdTimingPrint("Compiler: Analyzing semantics and checking address scopes\n");
    start = omp_get_wtime();
    auto parserDeletionFuture = std::async(std::launch::async, [this] {
        delete m_parser;
        m_parser = nullptr;
    });
    auto parseTreeDeletionFuture = std::async(std::launch::async, [this] {
        delete m_parseTree;
        m_parseTree = nullptr;
    });
    auto checkAddressScopesFuture = std::async(&ScopeChecker::checkAddressScopes, m_scopeChecker, m_AST->getRoot(), std::ref(m_statusMessage), std::ref(m_codeLines));
    auto analyzeSemanticsFuture = std::async(&SemanticAnalyzer::analyzeSemantics, m_semanticAnalyzer, m_AST->getRoot(), std::ref(m_statusMessage));
    bool checkAddressScopesResult = checkAddressScopesFuture.get();
    bool analyzeSemanticsResult = analyzeSemanticsFuture.get();
    parserDeletionFuture.get();
    parseTreeDeletionFuture.get();
    if(!checkAddressScopesResult || !analyzeSemanticsResult) {
        return false;
    }
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime() - start) + "\n\n");


    cmdTimingPrint("Compiler: Generating LLVM IR\n");
    start = omp_get_wtime();
    auto generateCodeFuture = std::async(std::launch::async, &CodeGenerator::generateCode, m_codeGenerator, m_AST->getRoot(), m_numLines, std::ref(m_statusMessage));
    auto clearCodeLinesFuture = std::async(std::launch::async, [this]() {
        m_codeLines.clear();
        m_codeLines.shrink_to_fit();
    });
    bool generateCodeResult = generateCodeFuture.get();
    clearCodeLinesFuture.get();
    if(!generateCodeResult) {
        return false;
    }
    cmdTimingPrint("Time taken: " + std::to_string(omp_get_wtime() - start) + "\n\n");

    return true;
}