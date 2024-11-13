#include "ast/Instructions.h"
#include "ast/Operands.h"

namespace AST {
    // ASTNode Implementation
    ASTNode::ASTNode(ASTConstants::NodeType type, const std::string &value)
            : m_nodeType(type), m_nodeValue(value) {}

    ASTNode::~ASTNode() {
        for (auto & child : m_children) {
            delete child;
        }
        m_children.clear();
    }

    ASTNode* ASTNode::insertChild(ASTNode* childNode) {
        if (childNode != nullptr) {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_children.push_back(childNode);
            return childNode;
        } else {
            return nullptr;
        }
    }

    ASTNode* ASTNode::childAt(int index) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (index >= static_cast<int>(m_children.size())) {
            return nullptr;
        } else {
            return m_children[index];
        }
    }

    void ASTNode::reserveChildren(int numChildren) {
        m_children.reserve(numChildren);
    }

    // RootNode Implementation
    RootNode::RootNode() : ASTNode(ASTConstants::NodeType::ROOT, "") {}
    RootNode::~RootNode() = default;

    void RootNode::accept(AST::Visitor &visitor) {
        //Visit for root node first (usually nothing)
        visitor.visit(*this);
        //Visit for all instruction children (multithreaded)
        #pragma omp parallel for schedule(auto) default(none) shared(visitor)
        for (auto* child : m_children) {
            child->accept(visitor);
        }
    }
    void RootNode::acceptSingleThreaded(AST::Visitor &visitor) {
        //Visit for root node first (usually nothing)
        visitor.visit(*this);
        #pragma omp critical
        {
            //Visit for all instruction children (single threaded)
            for (auto* child : m_children) {
                child->accept(visitor);
            }
        };
    }

    // InstructionNode Implementation
    InstructionNode::InstructionNode(const std::string &nodeValue, ASTConstants::InstructionType instructionType, ASTConstants::NumOperands numOperands, int line)
            : ASTNode(ASTConstants::NodeType::INSTRUCTION, nodeValue), m_instructionType(instructionType), m_numOperands(numOperands), m_line(line) {}

    InstructionNode::~InstructionNode() = default;

    // OperandNode Implementation
    OperandNode::OperandNode(const std::string &nodeValue, ASTConstants::OperandType operandType, int line, short int pos)
            : ASTNode(ASTConstants::NodeType::OPERAND, nodeValue), m_operandType(operandType), m_line(line), m_pos(pos) {}

    OperandNode::~OperandNode() = default;

    // AbstractSyntaxTree Implementation
    AbstractSyntaxTree::AbstractSyntaxTree() {
        m_root = new RootNode();
        m_instructionDictionary.reserve(26);
        m_instructionDictionary.emplace("move", ASTConstants::InstructionType::MOVE);
        m_instructionDictionary.emplace("load", ASTConstants::InstructionType::LOAD);
        m_instructionDictionary.emplace("store", ASTConstants::InstructionType::STORE);
        m_instructionDictionary.emplace("create", ASTConstants::InstructionType::CREATE);
        m_instructionDictionary.emplace("cast", ASTConstants::InstructionType::CAST);
        m_instructionDictionary.emplace("add", ASTConstants::InstructionType::ADD);
        m_instructionDictionary.emplace("sub", ASTConstants::InstructionType::SUB);
        m_instructionDictionary.emplace("multiply", ASTConstants::InstructionType::MULTIPLY);
        m_instructionDictionary.emplace("divide", ASTConstants::InstructionType::DIVIDE);
        m_instructionDictionary.emplace("or", ASTConstants::InstructionType::OR);
        m_instructionDictionary.emplace("and", ASTConstants::InstructionType::AND);
        m_instructionDictionary.emplace("not", ASTConstants::InstructionType::NOT);
        m_instructionDictionary.emplace("shift", ASTConstants::InstructionType::SHIFT);
        m_instructionDictionary.emplace("compare", ASTConstants::InstructionType::COMPARE);
        m_instructionDictionary.emplace("jump", ASTConstants::InstructionType::JUMP);
        m_instructionDictionary.emplace("call", ASTConstants::InstructionType::CALL);
        m_instructionDictionary.emplace("push", ASTConstants::InstructionType::PUSH);
        m_instructionDictionary.emplace("pop", ASTConstants::InstructionType::POP);
        m_instructionDictionary.emplace("return", ASTConstants::InstructionType::RETURN);
        m_instructionDictionary.emplace("stop", ASTConstants::InstructionType::STOP);
        m_instructionDictionary.emplace("input", ASTConstants::InstructionType::INPUT);
        m_instructionDictionary.emplace("output", ASTConstants::InstructionType::OUTPUT);
        m_instructionDictionary.emplace("print", ASTConstants::InstructionType::PRINT);
        m_instructionDictionary.emplace("label", ASTConstants::InstructionType::LABEL);
        m_instructionDictionary.emplace("comment", ASTConstants::InstructionType::COMMENT);
    }

    AbstractSyntaxTree::~AbstractSyntaxTree() {
        delete m_root;
    }

    ASTNode* AbstractSyntaxTree::getRoot() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_root;
    }

    ASTConstants::InstructionType AbstractSyntaxTree::getInstructionType(const std::string &instruction) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto itr = m_instructionDictionary.find(instruction);
        if (itr != m_instructionDictionary.end()) {
            return itr->second;
        } else {
            return ASTConstants::InstructionType::NONE;
        }
    }

    ASTConstants::NumOperands AbstractSyntaxTree::getNumOperands(int num) {
        switch (num) {
            case 0:
                return ASTConstants::NumOperands::NULLARY;
            case 1:
                return ASTConstants::NumOperands::UNARY;
            case 2:
                return ASTConstants::NumOperands::BINARY;
            case 3:
                return ASTConstants::NumOperands::TERNARY;
            default:
                return ASTConstants::NumOperands::INVALID;
        }
    }

    ASTConstants::OperandType AbstractSyntaxTree::convertOperandType(PTConstants::OperandType type) {
        switch (type) {
            case PTConstants::OperandType::REGISTER:
                return ASTConstants::OperandType::REGISTER;
            case PTConstants::OperandType::INSTRUCTIONADDRESS:
                return ASTConstants::OperandType::INSTRUCTIONADDRESS;
            case PTConstants::OperandType::MEMORYADDRESS:
                return ASTConstants::OperandType::MEMORYADDRESS;
            case PTConstants::OperandType::FLOAT:
                return ASTConstants::OperandType::FLOAT;
            case PTConstants::OperandType::INTEGER:
                return ASTConstants::OperandType::INTEGER;
            case PTConstants::OperandType::BOOLEAN:
                return ASTConstants::OperandType::BOOLEAN;
            case PTConstants::OperandType::CHARACTER:
                return ASTConstants::OperandType::CHARACTER;
            case PTConstants::OperandType::STRING:
                return ASTConstants::OperandType::STRING;
            case PTConstants::OperandType::NEWLINE:
                return ASTConstants::OperandType::NEWLINE;
            case PTConstants::OperandType::JUMPCONDITION:
                return ASTConstants::OperandType::JUMPCONDITION;
            case PTConstants::OperandType::SHIFTCONDITION:
                return ASTConstants::OperandType::SHIFTCONDITION;
            case PTConstants::OperandType::TYPECONDITION:
                return ASTConstants::OperandType::TYPECONDITION;
            case PTConstants::OperandType::UNKNOWN:
            default:
                return ASTConstants::OperandType::UNKNOWN;
        }
    }

    void AbstractSyntaxTree::printTree() const {
        printNode(m_root, 0);
    }

    void AbstractSyntaxTree::printNode(const ASTNode* node, int level) const {
        if (node == nullptr) return;

        std::string indent(level * 4, ' ');

        const AST::OperandNode* operandNode = dynamic_cast<const AST::OperandNode*>(node);

        if (operandNode != nullptr) {
            std::cout << indent << node->getNodeValue() << " - OperandType: " << operandNode->getOperandType() << std::endl;
        } else {
            std::cout << indent << node->getNodeValue() << " (" << node->getNumChildren() << " children)" << std::endl;
        }

        for (const auto& child : node->getChildren()) {
            printNode(child, level + 1);
        }
    }
}
