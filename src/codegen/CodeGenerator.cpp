#include "codegen/CodeGenerator.h"
#include "ast/Instructions.h"
#include "ast/Operands.h"

using namespace std;
using namespace llvm;
using namespace AST;

//Static LLVM Components Declaration
std::unique_ptr<llvm::LLVMContext> CodeGenerator::TheContext;
std::unique_ptr<llvm::IRBuilder<>> CodeGenerator::Builder;
std::unique_ptr<llvm::Module> CodeGenerator::TheModule;
std::vector<llvm::Value*> CodeGenerator::IR;
std::unordered_map<std::string, llvm::Value *> CodeGenerator::NamedValues;
std::unordered_map<std::string, llvm::AllocaInst *> CodeGenerator::NamedAllocas;


CodeGenerator::CodeGenerator() {
    // Initialize the LLVM context
    TheContext = make_unique<llvm::LLVMContext>();
    // Initialize the IRBuilder with the context
    Builder = make_unique<llvm::IRBuilder<>>(*TheContext);
    // Initialize the module with a name and the context
    TheModule = make_unique<llvm::Module>("StartASM_module", *TheContext);
    if (!TheContext || !Builder || !TheModule) {
        cerr << "Initialization failed" << std::endl;
        exit(0);
    }
}

bool CodeGenerator::generateCode(AST::ASTNode *AST, int numLines, string& errorString) {
    //Initialize registers into NamedValues and NamedAllocas
    NamedValues["r0"] = nullptr;
    NamedValues["r1"] = nullptr;
    NamedValues["r2"] = nullptr;
    NamedValues["r3"] = nullptr;
    NamedValues["r4"] = nullptr;
    NamedValues["r5"] = nullptr;
    NamedValues["r6"] = nullptr;
    NamedValues["r7"] = nullptr;
    NamedValues["r8"] = nullptr;
    NamedValues["r9"] = nullptr;
    NamedAllocas["r0"] = nullptr;
    NamedAllocas["r1"] = nullptr;
    NamedAllocas["r2"] = nullptr;
    NamedAllocas["r3"] = nullptr;
    NamedAllocas["r4"] = nullptr;
    NamedAllocas["r5"] = nullptr;
    NamedAllocas["r6"] = nullptr;
    NamedAllocas["r7"] = nullptr;
    NamedAllocas["r8"] = nullptr;
    NamedAllocas["r9"] = nullptr;

    AST::RootNode* ASTRoot = dynamic_cast<AST::RootNode*>(AST);
    //Visit the root and iterate over the AST
    cout << "Iterating over AST" << endl;
    if (ASTRoot!=nullptr) {
        ASTRoot->acceptSingleThreaded(*this);
    }

    if (!m_errorString.empty()) {
        errorString += m_errorString;
        return false;
    }
    return true;
}

//Root Node Visitor (Do nothing)
void CodeGenerator::visit(AST::RootNode& node) {}


//Instruction Generators
//Standard instructions
void CodeGenerator::visit(AST::MoveInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    cout << "Move instruction" << endl;
    //LHS - src register and type
    Value* val = localContext[0].second.first;
    //RHS - dest register
    AllocaInst* alloc = localContext[1].second.second;
    localContext.clear();
    if (!val) {
        //Register is not initialized, throw error
        m_errorString = "Uninitialized Register Access. Attempted to read uninitialized register " + node.getChildren().at(0)->getNodeValue() +" at line " + to_string(node.getLine()) + "\n Make sure the register is set before moving its contents!\n";
        m_continueGeneration = false;
    }
    else {
        //Generate LLVM move instruction
        IR.push_back(Builder->CreateStore(val, alloc));
        //Update register type

    }
}

void CodeGenerator::visit(AST::LoadInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    std::cout << "TODO: LoadInstruction\n";
}

void CodeGenerator::visit(AST::StoreInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    std::cout << "TODO: StoreInstruction\n";
}

void CodeGenerator::visit(AST::CreateInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    cout << "Create instruction" << endl;
    //LHS - src literal
    Value* val = localContext[0].second.first;
    cout << "(gotten val)" << endl;
    //RHS - dest register
    AllocaInst* alloc = localContext[1].second.second;
    cout << "(gotten alloc)" << endl;
    localContext.clear();
    //Create value to allocInst, assign type to register
    IR.push_back(Builder->CreateStore(val, alloc));
}

void CodeGenerator::visit(AST::CastInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    std::cout << "TODO: CastInstruction\n";
}

void CodeGenerator::visit(AST::AddInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    cout << "Add instruction" << endl;
    //LHS1 - src register 1
    Value* val1 = localContext[0].second.first;
    //LHS2 - src register 2
    Value* val2 = localContext[1].second.first;
    //RHS - dest register
    AllocaInst* alloc = localContext[2].second.second;
    localContext.clear();
    IR.push_back(Builder->CreateStore(Builder->CreateAdd(val1, val2), alloc));
}

void CodeGenerator::visit(AST::SubInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    //LHS1 - src register 1
    Value* val1 = localContext[0].second.first;
    //LHS2 - src register 2
    Value* val2 = localContext[1].second.first;
    //RHS - dest register
    AllocaInst* alloc = localContext[2].second.second;
    localContext.clear();
    IR.push_back(Builder->CreateStore(Builder->CreateSub(val1, val2), alloc));
}

void CodeGenerator::visit(AST::MultiplyInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    //LHS1 - src register 1
    Value* val1 = localContext[0].second.first;
    //LHS2 - src register 2
    Value* val2 = localContext[1].second.first;
    //RHS - dest register
    AllocaInst* alloc = localContext[2].second.second;
    localContext.clear();
    IR.push_back(Builder->CreateStore(Builder->CreateMul(val1, val2), alloc));
}

void CodeGenerator::visit(AST::DivideInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    //LHS1 - src register 1
    Value* val1 = localContext[0].second.first;
    //LHS2 - src register 2
    Value* val2 = localContext[1].second.first;
    //RHS - dest register
    AllocaInst* alloc = localContext[2].second.second;
    localContext.clear();
    IR.push_back(Builder->CreateStore(Builder->CreateSDiv(val1, val2), alloc));
}

void CodeGenerator::visit(AST::OrInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    std::cout << "TODO: OrInstruction\n";
}

void CodeGenerator::visit(AST::AndInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    std::cout << "TODO: AndInstruction\n";
}

void CodeGenerator::visit(AST::NotInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    std::cout << "TODO: AndInstruction\n";
}

void CodeGenerator::visit(AST::ShiftInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    std::cout << "TODO: ShiftInstruction\n";
}

void CodeGenerator::visit(AST::CompareInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    std::cout << "TODO: CompareInstruction\n";
}

void CodeGenerator::visit(AST::JumpInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    std::cout << "TODO: JumpInstruction\n";
}

void CodeGenerator::visit(AST::CallInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    std::cout << "TODO: CallInstruction\n";
}

void CodeGenerator::visit(AST::PushInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    std::cout << "TODO: PushInstruction\n";
}

void CodeGenerator::visit(AST::PopInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    std::cout << "TODO: PopInstruction\n";
}

void CodeGenerator::visit(AST::ReturnInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    std::cout << "TODO: ReturnInstruction\n";
}

void CodeGenerator::visit(AST::StopInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    std::cout << "TODO: StopInstruction\n";
}


//Runtime library instructions
void CodeGenerator::visit(AST::PrintInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    std::cout << "TODO: PrintInstruction\n";
}

void CodeGenerator::visit(AST::LabelInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    std::cout << "TODO: LabelInstruction\n";
}

void CodeGenerator::visit(AST::InputInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    std::cout << "TODO: InputInstruction\n";
}

void CodeGenerator::visit(AST::OutputInstruction& node) {
    if (!m_continueGeneration) {
        return;
    }
    std::cout << "TODO: OutputInstruction\n";
}

void CodeGenerator::visit(AST::CommentInstruction& node) {}



//Operand Generators
//Standard operands and types
void CodeGenerator::visit(AST::RegisterOperand& node) {
    if (!m_continueGeneration) {
        return;
    }
    cout << "-> Register" << endl;
    //Get the current register value at position
    //Whether to throw an error if the register is null depends on the instruction and register relationship (e.g. src vs dest)
    localContext.push_back(pair(node.getOperandType(), pair(NamedValues[node.getNodeValue()], NamedAllocas[node.getNodeValue()])));
}

void CodeGenerator::visit(AST::InstructionAddressOperand& node) {
    if (!m_continueGeneration) {
        return;
    }
    std::cout << "-> TODO: InstructionAddressOperand\n";
}

void CodeGenerator::visit(AST::MemoryAddressOperand& node) {
    if (!m_continueGeneration) {
        return;
    }
    std::cout << "-> TODO: MemoryAddressOperand\n";
}

void CodeGenerator::visit(AST::IntegerOperand& node) {
    if (!m_continueGeneration) {
        return;
    }
    cout << "-> Integer" << endl;
    //Basic 32bit int
    localContext.push_back(pair(node.getOperandType(), pair(ConstantInt::get(*TheContext, APInt(32, stoi(node.getNodeValue()), true)), nullptr)));
}

void CodeGenerator::visit(AST::FloatOperand& node) {
    if (!m_continueGeneration) {
        return;
    }
    cout << "-> Float" << endl;
    //Basic 32bit float
    localContext.push_back(pair(node.getOperandType(), pair(ConstantFP::get(*TheContext, APFloat(stof(node.getNodeValue()))), nullptr)));
}

void CodeGenerator::visit(AST::BooleanOperand& node) {
    if (!m_continueGeneration) {
        return;
    }
    cout << "-> Boolean" << endl;
    //Bool is treated as 1 bit integer
    localContext.push_back(pair(node.getOperandType(), pair(ConstantInt::get(*TheContext, APInt(1, (node.getNodeValue() == "true"), false)), nullptr)));
}

void CodeGenerator::visit(AST::CharacterOperand& node) {
    if (!m_continueGeneration) {
        return;
    }
    cout << "-> Character" << endl;
    //Char is treated as 8-bit ASCII integer
    localContext.push_back(pair(node.getOperandType(), pair(ConstantInt::get(*TheContext, APInt(8, node.getNodeValue()[0], false)), nullptr)));
}


//String and newline operands are for runtime library print functions
void CodeGenerator::visit(AST::StringOperand& node) {
    if (!m_continueGeneration) {
        return;
    }
    cout << "-> Register" << endl;
    localContext.push_back(pair(node.getOperandType(), pair(nullptr, nullptr)));
}

void CodeGenerator::visit(AST::NewlineOperand& node) {
    if (!m_continueGeneration) {
        return;
    }
    localContext.push_back(pair(node.getOperandType(), pair(nullptr, nullptr)));
}


//Conditions are for instruction type decision-making
void CodeGenerator::visit(AST::TypeConditionOperand& node) {
    if (!m_continueGeneration) {
        return;
    }
    localContext.push_back(pair(node.getOperandType(), pair(nullptr, nullptr)));
}

void CodeGenerator::visit(AST::ShiftConditionOperand& node) {
    if (!m_continueGeneration) {
        return;
    }
    localContext.push_back(pair(node.getOperandType(), pair(nullptr, nullptr)));
}

void CodeGenerator::visit(AST::JumpConditionOperand& node) {
    if (!m_continueGeneration) {
        return;
    }
    localContext.push_back(pair(node.getOperandType(), pair(nullptr, nullptr)));
}

void CodeGenerator::printIR() {

}
