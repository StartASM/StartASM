#include "compiler/Compiler.h"
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "symbolres/SymbolResolver.h"
#include "ast/AbstractSyntaxTree.h"
#include "ast/ASTBuilder.h"
#include "semantics/SemanticAnalyzer.h"
#include "scopecheck/ScopeChecker.h"
#include "codegen/CodeGenerator.h"

#include <iostream>
#include <string>
#include <omp.h>
#include <future>

using namespace std;

Compiler::Compiler(std::string& pathname, bool cmdSilent, bool cmdTimings, bool cmdIr) :
    cmd_silent(cmdSilent),
    cmd_timings(cmdTimings),
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
    m_pathname(pathname) {}

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
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");

    //Parse code//
    cmdTimingPrint("Compiler: Parsing code\n");
    start = omp_get_wtime();
    if(!m_parser->parseCode(m_parseTree, m_codeLines, m_codeTokens, m_statusMessage)) {
        return false;
    }
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");

    //Resolve symbolres//
    cmdTimingPrint("Compiler: Resolving symbolres\n");
    start = omp_get_wtime();
    if(!m_symbolResolver->resolveSymbols(m_symbolTable, m_parseTree->getRoot(), m_statusMessage, m_codeLines)) {
        return false;
    }
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");

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

    //Check address scopes and analyze semantics while deleting the parse tree concurrently//
    cmdTimingPrint("Compiler: Analyzing semantics and checking address scopes\n");
    start = omp_get_wtime();
    auto parserDeletionFuture = std::async(std::launch::async, [this] {
        delete m_parser;
        m_parser = nullptr;
    });
    auto checkAddressScopesFuture = std::async(&ScopeChecker::checkAddressScopes, m_scopeChecker, m_AST->getRoot(), std::ref(m_statusMessage), std::ref(m_codeLines));
    auto analyzeSemanticsFuture = std::async(&SemanticAnalyzer::analyzeSemantics, m_semanticAnalyzer, m_AST->getRoot(), std::ref(m_statusMessage));
    // Wait for all tasks to complete and retrieve function results//
    bool checkAddressScopesResult = checkAddressScopesFuture.get();
    bool analyzeSemanticsResult = analyzeSemanticsFuture.get();
    parserDeletionFuture.get();
    if(!checkAddressScopesResult || !analyzeSemanticsResult) {
        return false;
    }
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");

    //Generate code//
    cmdTimingPrint("Compiler: Generating LLVM IR\n");
    start = omp_get_wtime();

    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");
    return true;
}

bool Compiler::outputAST() {
    double start = omp_get_wtime();
    //Lex code//
    cmdTimingPrint("Compiler: Lexing code\n");
    if (!m_lexer->lexFile(m_pathname, m_codeLines, m_codeTokens)) {
        m_statusMessage = "Lexing failed! Either the path was invalid or the file could not be found.";
        return false;
    }
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");

    //Parse code//
    cmdTimingPrint("Compiler: Parsing code\n");
    start = omp_get_wtime();
    if(!m_parser->parseCode(m_parseTree, m_codeLines, m_codeTokens, m_statusMessage)) {
        return false;
    }
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");

    //Resolve symbolres//
    cmdTimingPrint("Compiler: Resolving symbolres\n");
    start = omp_get_wtime();
    if(!m_symbolResolver->resolveSymbols(m_symbolTable, m_parseTree->getRoot(), m_statusMessage, m_codeLines)) {
        return false;
    }
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");

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

    //Check address scopes and analyze semantics while deleting the parse tree concurrently
    cmdTimingPrint("Compiler: Analyzing semantics and checking address scopes\n");
    start = omp_get_wtime();
    auto parserDeletionFuture = std::async(std::launch::async, [this] {
        delete m_parser;
        m_parser = nullptr;
    });
    auto checkAddressScopesFuture = std::async(&ScopeChecker::checkAddressScopes, m_scopeChecker, m_AST->getRoot(), std::ref(m_statusMessage), std::ref(m_codeLines));
    auto analyzeSemanticsFuture = std::async(&SemanticAnalyzer::analyzeSemantics, m_semanticAnalyzer, m_AST->getRoot(), std::ref(m_statusMessage));
    // Wait for all tasks to complete and retrieve function results
    bool checkAddressScopesResult = checkAddressScopesFuture.get();
    bool analyzeSemanticsResult = analyzeSemanticsFuture.get();
    parserDeletionFuture.get();
    if(!checkAddressScopesResult || !analyzeSemanticsResult) {
        return false;
    }
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");

    //JSONIFY and output AST//

}