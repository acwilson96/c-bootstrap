#include "../../include/ast/FunCall.h"

using namespace ACC;

FunCall::FunCall(const atl::string &funName,
                 atl::vector<std::shared_ptr<Expr>> funArgs)
    : funName(funName), funArgs(funArgs) {}

std::shared_ptr<FunCall> FunCall::getptr() { return shared_from_this(); }

void FunCall::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string FunCall::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type> FunCall::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register FunCall::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
FunCall::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
FunCall::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string FunCall::astClass() const { return "FunCall"; }