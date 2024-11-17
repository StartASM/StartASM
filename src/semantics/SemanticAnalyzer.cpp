#include "semantics/SemanticAnalyzer.h"

#include <string>
#include <vector>

using namespace std;
using namespace AST;
using namespace ASTConstants;

SemanticAnalyzer::SemanticAnalyzer(std::vector<std::string>& lines) : m_lines(lines) {
    // Initialization code if needed
}
bool SemanticAnalyzer::analyzeSemantics(AST::ASTNode *AST, std::string &errorMessage) {
    //Prepopulate the local semantic context - an operation will never have >3 operands
    std::vector<ASTConstants::OperandType> localContext(3, ASTConstants::EMPTY); //Set initial operands to empty for easier matching
    //Perallocate the global semantic context based on the number of lines
    m_semanticContext = std::vector<std::vector<ASTConstants::OperandType>>(m_lines.size()+1, localContext);

    //Visit the root and iterate over the AST
    AST->accept(*this);

    //Clear the context
    m_semanticContext.clear();

    //Concatenate status message string with all error messages
    for (const auto& pair : m_invalidLines) {
        errorMessage += pair.second;
    }

    //Return true if no errors, false otherwise
    if (!errorMessage.empty()) {
        return false;
    }
    return true;
}

void SemanticAnalyzer::visit(AST::RootNode& node)  {}

void SemanticAnalyzer::visit(AST::MoveInstruction& node) {
    //CASE 1 - Atomic type instruction
    //Expected semantic structure of instruction
    const vector<ASTConstants::OperandType> semanticTemplate = {REGISTER, REGISTER, EMPTY};
    int line = node.getLine();
    //Check local semantic context
    if (m_semanticContext[line] == semanticTemplate) {
        return; //Return instantly if match found
    }
    //Pass to error handler if a match isn't found
    handleAtomicInstructionError(line, semanticTemplate, node);
}

void SemanticAnalyzer::visit(AST::CastInstruction& node) {
    const std::vector<ASTConstants::OperandType> semanticTemplate = {TYPECONDITION, REGISTER, EMPTY};
    int line = node.getLine();
    if (m_semanticContext[line] == semanticTemplate) {
        return;
    }
    handleAtomicInstructionError(line, semanticTemplate, node);
}

void SemanticAnalyzer::visit(AST::AddInstruction& node) {
    const std::vector<ASTConstants::OperandType> semanticTemplate = {REGISTER, REGISTER, REGISTER};
    int line = node.getLine();
    if (m_semanticContext[line] == semanticTemplate) {
        return;
    }
    handleAtomicInstructionError(line, semanticTemplate, node);
}

void SemanticAnalyzer::visit(AST::SubInstruction& node) {
    const std::vector<ASTConstants::OperandType> semanticTemplate = {REGISTER, REGISTER, REGISTER};
    int line = node.getLine();
    if (m_semanticContext[line] == semanticTemplate) {
        return;
    }
    handleAtomicInstructionError(line, semanticTemplate, node);
}

void SemanticAnalyzer::visit(AST::MultiplyInstruction& node) {
    const std::vector<ASTConstants::OperandType> semanticTemplate = {REGISTER, REGISTER, REGISTER};
    int line = node.getLine();
    if (m_semanticContext[line] == semanticTemplate) {
        return;
    }
    handleAtomicInstructionError(line, semanticTemplate, node);
}

void SemanticAnalyzer::visit(AST::DivideInstruction& node) {
    const std::vector<ASTConstants::OperandType> semanticTemplate = {REGISTER, REGISTER, REGISTER};
    int line = node.getLine();
    if (m_semanticContext[line] == semanticTemplate) {
        return;
    }
    handleAtomicInstructionError(line, semanticTemplate, node);
}

void SemanticAnalyzer::visit(AST::OrInstruction& node) {
    const std::vector<ASTConstants::OperandType> semanticTemplate = {REGISTER, REGISTER, EMPTY};
    int line = node.getLine();
    if (m_semanticContext[line] == semanticTemplate) {
        return;
    }
    handleAtomicInstructionError(line, semanticTemplate, node);
}

void SemanticAnalyzer::visit(AST::AndInstruction& node) {
    const std::vector<ASTConstants::OperandType> semanticTemplate = {REGISTER, REGISTER, EMPTY};
    int line = node.getLine();
    if (m_semanticContext[line] == semanticTemplate) {
        return;
    }
    handleAtomicInstructionError(line, semanticTemplate, node);
}

void SemanticAnalyzer::visit(AST::NotInstruction& node) {
    const std::vector<ASTConstants::OperandType> semanticTemplate = {REGISTER, EMPTY, EMPTY};
    int line = node.getLine();
    if (m_semanticContext[line] == semanticTemplate) {
        return;
    }
    handleAtomicInstructionError(line, semanticTemplate, node);
}

void SemanticAnalyzer::visit(AST::ShiftInstruction& node) {
    const std::vector<ASTConstants::OperandType> semanticTemplate = {SHIFTCONDITION, REGISTER, REGISTER};
    int line = node.getLine();
    if (m_semanticContext[line] == semanticTemplate) {
        return;
    }
    handleAtomicInstructionError(line, semanticTemplate, node);
}

void SemanticAnalyzer::visit(AST::CompareInstruction& node) {
    const std::vector<ASTConstants::OperandType> semanticTemplate = {REGISTER, REGISTER, EMPTY};
    int line = node.getLine();
    if (m_semanticContext[line] == semanticTemplate) {
        return;
    }
    handleAtomicInstructionError(line, semanticTemplate, node);
}

void SemanticAnalyzer::visit(AST::PushInstruction& node) {
    const std::vector<ASTConstants::OperandType> semanticTemplate = {REGISTER, EMPTY, EMPTY};
    int line = node.getLine();
    if (m_semanticContext[line] == semanticTemplate) {
        return;
    }
    handleAtomicInstructionError(line, semanticTemplate, node);
}

void SemanticAnalyzer::visit(AST::PopInstruction& node) {
    const std::vector<ASTConstants::OperandType> semanticTemplate = {REGISTER, EMPTY, EMPTY};
    int line = node.getLine();
    if (m_semanticContext[line] == semanticTemplate) {
        return;
    }
    handleAtomicInstructionError(line, semanticTemplate, node);
}

void SemanticAnalyzer::visit(AST::ReturnInstruction& node) {
    const std::vector<ASTConstants::OperandType> semanticTemplate = {EMPTY, EMPTY, EMPTY};
    int line = node.getLine();
    if (m_semanticContext[line] == semanticTemplate) {
        return;
    }
    handleAtomicInstructionError(line, semanticTemplate, node);
}

void SemanticAnalyzer::visit(AST::StopInstruction& node) {
    const std::vector<ASTConstants::OperandType> semanticTemplate = {EMPTY, EMPTY, EMPTY};
    int line = node.getLine();
    if (m_semanticContext[line] == semanticTemplate) {
        return;
    }
    handleAtomicInstructionError(line, semanticTemplate, node);
}

void SemanticAnalyzer::visit(AST::InputInstruction& node) {
    const std::vector<ASTConstants::OperandType> semanticTemplate = {TYPECONDITION, REGISTER, EMPTY};
    int line = node.getLine();
    if (m_semanticContext[line] == semanticTemplate) {
        return;
    }
    handleAtomicInstructionError(line, semanticTemplate, node);
}

void SemanticAnalyzer::visit(AST::OutputInstruction& node) {
    const std::vector<ASTConstants::OperandType> semanticTemplate = {REGISTER, EMPTY, EMPTY};
    int line = node.getLine();
    if (m_semanticContext[line] == semanticTemplate) {
        return;
    }
    handleAtomicInstructionError(line, semanticTemplate, node);
}

void SemanticAnalyzer::visit(AST::LabelInstruction& node) {
    const std::vector<ASTConstants::OperandType> semanticTemplate = {INSTRUCTIONADDRESS, EMPTY, EMPTY};
    int line = node.getLine();
    if (m_semanticContext[line] == semanticTemplate) {
        return;
    }
    handleAtomicInstructionError(line, semanticTemplate, node);
}

void SemanticAnalyzer::visit(AST::CommentInstruction& node) {
    const std::vector<ASTConstants::OperandType> semanticTemplate = {STRING, EMPTY, EMPTY};
    int line = node.getLine();
    if (m_semanticContext[line] == semanticTemplate) {
        return;
    }
    handleAtomicInstructionError(line, semanticTemplate, node);
}


void SemanticAnalyzer::visit(AST::CreateInstruction& node) {
    //CASE 2 - Multiple type instruction
    //Expected semantic structure of instruction - some operands can be of multiple types
    const vector<unordered_set<ASTConstants::OperandType>> semanticTemplate = {
            {TYPECONDITION},
            {INTEGER, CHARACTER, BOOLEAN, FLOAT, MEMORYADDRESS, INSTRUCTIONADDRESS},
            {REGISTER}
    };
    int line = node.getLine();
    vector<ASTConstants::OperandType> localContext = m_semanticContext[line];
    //Iterate over possible semantics
    for (int i=0; i<localContext.size(); i++) {
        //Check if every operand in the context is a part of valid operands in the template
        if (semanticTemplate[i].find(localContext[i]) == semanticTemplate[i].end()) {
            //Handle error if not
            handleMultipleInstructionError(line, semanticTemplate, node);
            return;
        }
    }
    //Returns successfuly if end of loop is reached
}

void SemanticAnalyzer::visit(AST::LoadInstruction& node) {
    const std::vector<std::unordered_set<ASTConstants::OperandType>> semanticTemplate = {
            {MEMORYADDRESS, REGISTER},
            {REGISTER},
            {EMPTY}
    };
    int line = node.getLine();
    const auto& localContext = m_semanticContext[line];
    for (int i=0; i < localContext.size(); i++) {
        if (semanticTemplate[i].find(localContext[i]) == semanticTemplate[i].end()) {
            handleMultipleInstructionError(line, semanticTemplate, node);
            return;
        }
    }
}

void SemanticAnalyzer::visit(AST::StoreInstruction& node) {
    const std::vector<std::unordered_set<ASTConstants::OperandType>> semanticTemplate = {
            {REGISTER},
            {MEMORYADDRESS, REGISTER},
            {EMPTY}
    };
    int line = node.getLine();
    const auto& localContext = m_semanticContext[line];
    for (int i=0; i < localContext.size(); i++) {
        if (semanticTemplate[i].find(localContext[i]) == semanticTemplate[i].end()) {
            handleMultipleInstructionError(line, semanticTemplate, node);
            return;
        }
    }
}

void SemanticAnalyzer::visit(AST::JumpInstruction& node) {
    const std::vector<std::unordered_set<ASTConstants::OperandType>> semanticTemplate = {
            {JUMPCONDITION},
            {INSTRUCTIONADDRESS},
            {EMPTY}
    };
    int line = node.getLine();
    const auto& localContext = m_semanticContext[line];
    for (int i=0; i < localContext.size(); i++) {
        if (semanticTemplate[i].find(localContext[i]) == semanticTemplate[i].end()) {
            handleMultipleInstructionError(line, semanticTemplate, node);
            return;
        }
    }
}

void SemanticAnalyzer::visit(AST::CallInstruction& node) {
    const std::vector<std::unordered_set<ASTConstants::OperandType>> semanticTemplate = {
            {INSTRUCTIONADDRESS, REGISTER},
            {EMPTY},
            {EMPTY}
    };
    int line = node.getLine();
    const auto& localContext = m_semanticContext[line];
    for (int i=0; i < localContext.size(); i++) {
        if (semanticTemplate[i].find(localContext[i]) == semanticTemplate[i].end()) {
            handleMultipleInstructionError(line, semanticTemplate, node);
            return;
        }
    }
}

void SemanticAnalyzer::visit(AST::PrintInstruction& node) {
    const std::vector<std::unordered_set<ASTConstants::OperandType>> semanticTemplate = {
            {STRING, NEWLINE},
            {EMPTY},
            {EMPTY}
    };
    int line = node.getLine();
    const auto& localContext = m_semanticContext[line];
    for (int i=0; i < localContext.size(); i++) {
        if (semanticTemplate[i].find(localContext[i]) == semanticTemplate[i].end()) {
            handleMultipleInstructionError(line, semanticTemplate, node);
            return;
        }
    }
}


void SemanticAnalyzer::visit(AST::RegisterOperand& node) {
    //Insert its type in the local semantic context
    m_semanticContext[node.getLine()][node.getPos()] = node.getOperandType();
}

void SemanticAnalyzer::visit(AST::InstructionAddressOperand& node) {
    m_semanticContext[node.getLine()][node.getPos()] = node.getOperandType();
}

void SemanticAnalyzer::visit(AST::MemoryAddressOperand& node) {
    m_semanticContext[node.getLine()][node.getPos()] = node.getOperandType();
}

void SemanticAnalyzer::visit(AST::IntegerOperand& node) {
    m_semanticContext[node.getLine()][node.getPos()] = node.getOperandType();
}

void SemanticAnalyzer::visit(AST::FloatOperand& node) {
    m_semanticContext[node.getLine()][node.getPos()] = node.getOperandType();
}

void SemanticAnalyzer::visit(AST::BooleanOperand& node) {
    m_semanticContext[node.getLine()][node.getPos()] = node.getOperandType();
}

void SemanticAnalyzer::visit(AST::CharacterOperand& node) {
    m_semanticContext[node.getLine()][node.getPos()] = node.getOperandType();
}

void SemanticAnalyzer::visit(AST::StringOperand& node) {
    m_semanticContext[node.getLine()][node.getPos()] = node.getOperandType();
}

void SemanticAnalyzer::visit(AST::NewlineOperand& node) {
    m_semanticContext[node.getLine()][node.getPos()] = node.getOperandType();
}

void SemanticAnalyzer::visit(AST::TypeConditionOperand& node) {
    m_semanticContext[node.getLine()][node.getPos()] = node.getOperandType();
}

void SemanticAnalyzer::visit(AST::ShiftConditionOperand& node) {
    m_semanticContext[node.getLine()][node.getPos()] = node.getOperandType();
}

void SemanticAnalyzer::visit(AST::JumpConditionOperand& node) {
    m_semanticContext[node.getLine()][node.getPos()] = node.getOperandType();
}

void SemanticAnalyzer::handleAtomicInstructionError(int line, const std::vector<ASTConstants::OperandType> &expectedTemplate, AST::InstructionNode &node) {
    //Create the invalid line log first
    string errorLine = "Invalid syntax at line " + to_string(line) + ": " + m_lines[line-1] + "\n";
    vector<ASTConstants::OperandType> localContext = m_semanticContext[line];
    //Check every mismatched operand
    for (int i=0; i<localContext.size(); i++) {
        if (localContext[i] != expectedTemplate[i]) {
            if (expectedTemplate[i] != EMPTY) {
                //Unrecognized operand if not expecting an empty space
                 errorLine += "Unrecognized operand '" + node.childAt(i)->getNodeValue() + "'. Expected " + enumToString(expectedTemplate[i]) + "\n\n";
            }
            else {
                //Excess operand if expecting an empty space
                errorLine += "Unexpected extra operand '" + node.childAt(i)->getNodeValue() + "'\n\n";
            }
        }
    }
    //Add to the invalid lines map
    #pragma omp critical
    {
        m_invalidLines[line] = errorLine;
    };
}

void SemanticAnalyzer::handleMultipleInstructionError(int line, const std::vector<std::unordered_set<ASTConstants::OperandType>> &expectedTemplate, AST::InstructionNode &node) {
    //Create the invalid line log first
    const unordered_set<ASTConstants::OperandType> emptyTemplate = {EMPTY};
    string errorLine = "Invalid syntax at line " + to_string(line) + ": " + m_lines[line-1] + "\n";
    vector<ASTConstants::OperandType> localContext = m_semanticContext[line];

    //Iterate over all given operands in the local context
    for (int i=0; i<localContext.size(); i++) {
        //If a local context token doesn't match any in the template for that index
        if (expectedTemplate[i].find(localContext[i]) == expectedTemplate[i].end()) {
            if (expectedTemplate[i] != emptyTemplate) {
                errorLine += "Unrecognized operand '" + node.childAt(i)->getNodeValue() + "'. Expected ";
                //Add all possible expected operands
                for (auto it = expectedTemplate[i].begin(); it != expectedTemplate[i].end(); ++it) {
                    if (it != expectedTemplate[i].begin()) {
                        errorLine += " or ";
                    }
                    errorLine += enumToString(*it);
                }
                errorLine += "\n\n";
            }
            else {
                //Excess operand if expecting an empty space
                errorLine += "Unexpected extra operand '" + node.childAt(i)->getNodeValue() + "'\n\n";
            }
        }
    }
    //Add to the invalid lines map
    #pragma omp critical
    {
        m_invalidLines[line] = errorLine;
    };
}

string SemanticAnalyzer::enumToString(OperandType type) {
    //This method simply switches an OperandType with a string outlining the expected operand for easy error printing
    switch (type) {
        case REGISTER:
            return "register r0-r9";
        case INSTRUCTIONADDRESS:
            return "instruction address i[...]";
        case MEMORYADDRESS:
            return "memory address m<...>";
        case INTEGER:
            return "integer";
        case FLOAT:
            return "float";
        case BOOLEAN:
            return "boolean";
        case CHARACTER:
            return "character";
        case STRING:
            return "comment string";
        case NEWLINE:
            return "newline";
        case TYPECONDITION:
            return "type condition: integer/float/boolean/character/memory/instruction";
        case SHIFTCONDITION:
            return "shift condition: left/right";
        case JUMPCONDITION:
            return "jump condition: unconditional/greater/less/equal/zero/unequal/nonzero";
        default:
            return "undefined";
    }
}
