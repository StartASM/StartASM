#ifndef ABSTRACTSYNTAXTREE_H
#define ABSTRACTSYNTAXTREE_H

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <iostream>
#include <mutex>
#include "pt/ParseTree.h"
#include "Visitor.h"

namespace ASTConstants {
    enum NodeType {ROOT, INSTRUCTION, OPERAND};
    enum InstructionType {MOVE, LOAD, STORE, CREATE, CAST, ADD, SUB, MULTIPLY, DIVIDE, OR, AND, NOT, SHIFT, COMPARE, JUMP, CALL, PUSH, POP, RETURN, STOP, INPUT, OUTPUT, PRINT, LABEL, COMMENT, NONE};
    enum OperandType {REGISTER, INSTRUCTIONADDRESS, MEMORYADDRESS, INTEGER, FLOAT, BOOLEAN, CHARACTER, STRING, NEWLINE, TYPECONDITION, SHIFTCONDITION, JUMPCONDITION, UNKNOWN, EMPTY};
    enum NumOperands {NULLARY, UNARY, BINARY, TERNARY, INVALID};
};

namespace AST {
    class Visitor;

    // Broad AST Node
    class ASTNode {
    public:
        ASTNode(ASTConstants::NodeType type, const std::string &value);
        virtual ~ASTNode();
        ASTNode(const ASTNode&) = delete;
        ASTNode& operator=(const ASTNode&) = delete;

        virtual void accept(Visitor& visitor) = 0;

        // Getters
        std::string getNodeValue() const { return m_nodeValue; }
        ASTConstants::NodeType getNodeType() const { return m_nodeType; }
        int getNumChildren() const { return static_cast<int>(m_children.size()); }
        const std::vector<ASTNode*>& getChildren() const { return m_children; }

        // Setters
        void setNodeValue(const std::string &value) { m_nodeValue = value; }
        ASTNode* insertChild(ASTNode* childNode);
        ASTNode* childAt(int index);
        void reserveChildren(int numChildren);

    protected:
        ASTConstants::NodeType m_nodeType;
        std::string m_nodeValue;
        std::vector<ASTNode*> m_children;
        mutable std::mutex m_mutex;
    };

    // Specialized root node class (top level in AST)
    class RootNode: public ASTNode {
        friend class AST;
    public:
        RootNode();
        ~RootNode() override;
        RootNode(const RootNode&) = delete;
        RootNode& operator=(const RootNode&) = delete;

        void accept(Visitor&) override;
        void acceptSingleThreaded(Visitor&);
    };

    // Template instruction node class
    class InstructionNode: public ASTNode {
    public:
        InstructionNode(const std::string &nodeValue, ASTConstants::InstructionType instructionType, ASTConstants::NumOperands numOperands, int line);
        ~InstructionNode() override;
        InstructionNode(const InstructionNode&) = delete;
        InstructionNode& operator=(const InstructionNode&) = delete;

        ASTConstants::InstructionType getInstructionType() const { return m_instructionType; }
        ASTConstants::NumOperands getNumOperands() const { return m_numOperands; }
        int getLine() const {return m_line; }
        void setInstructionType(ASTConstants::InstructionType type) { m_instructionType = type; }
        void setNumOperands(ASTConstants::NumOperands num) { m_numOperands = num; }

    private:
        ASTConstants::InstructionType m_instructionType;
        ASTConstants::NumOperands m_numOperands;
        int m_line;
    };

    // Operand Node Class
    class OperandNode: public ASTNode {
    public:
        OperandNode(const std::string &nodeValue, ASTConstants::OperandType operandType, int line, short int pos);
        ~OperandNode() override;
        OperandNode(const OperandNode&) = delete;
        OperandNode& operator=(const OperandNode&) = delete;

        ASTConstants::OperandType getOperandType() const { return m_operandType; }
        int getLine() const {return m_line; }
        short int getPos() const {return m_pos;}
        void setOperandType(ASTConstants::OperandType type) { m_operandType = type; }

    private:
        ASTConstants::OperandType m_operandType;
        int m_line;
        short int m_pos;
    };

    // AST wrapper class
    class AbstractSyntaxTree {
    public:
        AbstractSyntaxTree();
        ~AbstractSyntaxTree();
        ASTNode* getRoot();
        ASTConstants::InstructionType getInstructionType(const std::string &instruction);
        ASTConstants::NumOperands getNumOperands(int num);
        ASTConstants::OperandType convertOperandType(PTConstants::OperandType type);
        void printTree() const;

    private:
        ASTNode* m_root;
        std::unordered_map<std::string, ASTConstants::InstructionType> m_instructionDictionary;
        mutable std::mutex m_mutex;
        void printNode(const ASTNode* node, int level) const;
    };
}
#endif
