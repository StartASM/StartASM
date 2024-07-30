#include "include/Compiler.h"
#include "include/Lexer.h"
#include "include/Parser.h"
#include "include/SymbolResolver.h"
#include "include/AbstractSyntaxTree.h"
#include "include/ASTBuilder.h"
#include "include/SemanticAnalyzer.h"
#include "include/ScopeChecker.h"
#include "include/CodeGenerator.h"

#include <iostream>
#include <vector>
#include <string>
#include <omp.h>
#include <mutex>
#include <future>
#include <regex>

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
    m_semanticAnalyzer(new SemanticAnalyzer()),
    m_scopeChecker(new ScopeChecker()),
    //m_codeGenerator(new CodeGenerator()),
    m_pathname(pathname) {}

Compiler::~Compiler() {
    delete m_lexer;
    delete m_parser;
    delete m_symbolResolver;
    delete m_AST;
    delete m_ASTBuilder;
    delete m_semanticAnalyzer;
    delete m_scopeChecker;
    //delete m_codeGenerator;
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
    //Lex code
    cmdTimingPrint("Compiler: Lexing code\n");
    if (!m_lexer->lexFile(m_pathname, m_codeLines, m_codeTokens)) {
        m_statusMessage = "Lexing failed! Either the path was invalid or the file could not be found.";
        return false;
    }
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");

    //Parse code
    cmdTimingPrint("Compiler: Parsing code\n");
    start = omp_get_wtime();
    if(!m_parser->parseCode(m_parseTree, m_codeLines, m_codeTokens, m_statusMessage)) {
        return false;
    }
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");

    //Resolve symbols
    cmdTimingPrint("Compiler: Resolving symbols\n");
    start = omp_get_wtime();
    if(!m_symbolResolver->resolveSymbols(m_symbolTable, m_parseTree->getRoot(), m_statusMessage, m_codeLines)) {
        return false;
    }
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");

    //Delete the lexer and build the AST concurrently
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

    //Check address scopes and analyze semantics while deleting the parse tree concurrently
    cmdTimingPrint("Compiler: Analyzing semantics and checking address scopes\n");
    start = omp_get_wtime();
    auto parserDeletionFuture = std::async(std::launch::async, [this] {
        delete m_parser;
        m_parser = nullptr;
    });
    auto checkAddressScopesFuture = std::async(&ScopeChecker::checkAddressScopes, m_scopeChecker, m_AST->getRoot(), std::ref(m_statusMessage), std::ref(m_codeLines));
    auto analyzeSemanticsFuture = std::async(&SemanticAnalyzer::analyzeSemantics, m_semanticAnalyzer, m_AST->getRoot(), std::ref(m_codeLines), std::ref(m_statusMessage));
    // Wait for all tasks to complete and retrieve function results
    bool checkAddressScopesResult = checkAddressScopesFuture.get();
    bool analyzeSemanticsResult = analyzeSemanticsFuture.get();
    parserDeletionFuture.get();
    if(!checkAddressScopesResult || !analyzeSemanticsResult) {
        return false;
    }
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");

    //Generate code
    cmdTimingPrint("Compiler: Generating LLVM IR\n");
    start = omp_get_wtime();
    generateCode();
    cmdTimingPrint("Time taken: " + to_string(omp_get_wtime()-start) + "\n\n");
    /*if(cmd_ir && !cmd_silent) {
        cout << endl;
        cout << "LLVM IR for '" + m_pathname + "':\n";
        m_codeGenerator->printIR();
        cout << endl;
    }*/
    return true;
}

void Compiler::generateCode() {
    /*AST::ASTNode* ASTRoot = m_AST->getRoot();
    int numInstructions = ASTRoot->getNumChildren();

    //#pragma omp parallel for schedule(dynamic) default(none) shared(ASTRoot, numInstructions)
    for (int i = 0; i < numInstructions; i++) {
        // Cast the ASTNode to an instruction node
        auto instructionNode = dynamic_cast<AST::InstructionNode*>(ASTRoot->childAt(i));
        // If not nullptr and if the node isn't empty
        if (instructionNode != nullptr && !instructionNode->getNodeValue().empty()) {
            // Call the code generator for the given line
            m_codeGenerator->generateCode(instructionNode);
        }
    }*/
}