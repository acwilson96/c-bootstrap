#include "../../include/ast/EnumTypeDecl.h"

using namespace ACC;

EnumTypeDecl::EnumTypeDecl(std::map<std::string, unsigned int> states)
    : states(states) {}

std::shared_ptr<EnumTypeDecl> EnumTypeDecl::getptr() {
  return shared_from_this();
}

bool EnumTypeDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<EnumTypeDecl *>(&rhs);
  return false;
}

bool EnumTypeDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool EnumTypeDecl::operator==(const EnumTypeDecl &rhs) const {
  return states == rhs.states;
}

bool EnumTypeDecl::operator!=(const EnumTypeDecl &rhs) const {
  return !(*this == rhs);
}

void EnumTypeDecl::accept(ASTVisitor<void> &v) { return v.visit(*this); }

std::string EnumTypeDecl::accept(ASTVisitor<std::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type>
EnumTypeDecl::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register EnumTypeDecl::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
EnumTypeDecl::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
EnumTypeDecl::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

std::string EnumTypeDecl::astClass() const { return "EnumTypeDecl"; }