#include "../../include/ast/FunDef.h"

using namespace ACC;

FunDef::FunDef(std::shared_ptr<Block> funBlock, std::string funName,
               std::vector<std::shared_ptr<VarDecl>> funParams,
               std::shared_ptr<Type> funType, bool isExtern)
    : funBlock(funBlock), funName(funName), funParams(funParams),
      funType(funType), isExtern(isExtern) {}

std::shared_ptr<FunDef> FunDef::getptr() { return shared_from_this(); }

bool FunDef::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<FunDef *>(&rhs);
  return false;
}

bool FunDef::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool FunDef::operator==(const FunDef &rhs) const {
  if (*funType != *rhs.funType)
    return false;

  if (funName != rhs.funName)
    return false;

  if (funParams.size() != rhs.funParams.size())
    return false;

  for (int i = 0; i < funParams.size(); i++)
    if (*funParams[i] != *rhs.funParams[i])
      return false;

  /* Check Block*/

  return true;
}

bool FunDef::operator!=(const FunDef &rhs) const { return !(*this == rhs); }

std::string FunDef::getIdentifier() const { return funName; }

void FunDef::accept(ASTVisitor<void> &v) { return v.visit(*this); }

std::string FunDef::accept(ASTVisitor<std::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type> FunDef::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register FunDef::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
FunDef::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
FunDef::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

std::string FunDef::astClass() const { return "FunDef"; }