#include "../../include/passes/NameAnalysis.h"

using namespace ACC;

NameAnalysis::NameAnalysis(std::shared_ptr<Program> progAST)
    : progAST(progAST) {}

void NameAnalysis::error(std::string error) {
  errorCount++;
  errors.push_back(error);
}

void NameAnalysis::printErrors() {
  std::cerr << "Name Analysis Errors:" << std::endl;
  for (const auto &error : errors)
    std::cerr << "\t" << error << std::endl;
}

void NameAnalysis::run() { visit(*progAST); }

/* ---- Visit AST ---- */

void NameAnalysis::visit(ArrayAccess &aa) { aa.array->accept(*this); }
void NameAnalysis::visit(ArrayType &at) {}
void NameAnalysis::visit(Assign &as) {
  as.lhs->accept(*this);
  as.rhs->accept(*this);
}
void NameAnalysis::visit(BaseType &bt) {}
void NameAnalysis::visit(BinOp &bo) {
  bo.lhs->accept(*this);
  bo.rhs->accept(*this);
}
void NameAnalysis::visit(Block &b) {
  if (b.outerBlock == nullptr) {
    b.setOuterBlock(currScope);
    currScope = b.getptr();
  }
  for (const auto &stmt : b.blockStmts)
    stmt->accept(*this);
  currScope = b.outerBlock;
}
void NameAnalysis::visit(CharLiteral &cl) {}
void NameAnalysis::visit(DoWhile &dw) {
  dw.condition->accept(*this);
  dw.body->accept(*this);
}
void NameAnalysis::visit(FieldAccess &fa) { fa.object->accept(*this); }
void NameAnalysis::visit(FunCall &fc) {
  if (currScope->find(fc.funName) == nullptr)
    return error("Attempted to call undeclared function: " + fc.funName);
  for (const auto &arg : fc.funArgs)
    arg->accept(*this);
}
void NameAnalysis::visit(FunDecl &fd) {
  if (currScope->findLocal(fd.getIdentifier()))
    return error("Attempted to declare a Function with an identifier that is "
                 "already in use: " +
                 fd.getIdentifier());
  currScope->insertDecl(fd.getptr());

  fd.funBlock->setOuterBlock(currScope);
  currScope = fd.funBlock;

  for (const auto &param : fd.funParams)
    param->accept(*this);
  fd.funBlock->accept(*this);
  currScope = fd.funBlock->outerBlock;
}
void NameAnalysis::visit(FunDef &fd) {
  if (currScope->findLocal(fd.getIdentifier()))
    return error("Attempted to declare a Function with an identifier that is "
                 "already in use: " +
                 fd.getIdentifier());
  currScope->insertDecl(fd.getptr());

  fd.funBlock->setOuterBlock(currScope);
  currScope = fd.funBlock;

  for (const auto &param : fd.funParams)
    param->accept(*this);
  fd.funBlock->accept(*this);
  currScope = fd.funBlock->outerBlock;
}
void NameAnalysis::visit(If &i) {
  i.ifCondition->accept(*this);
  i.ifBody->accept(*this);
  if (i.elseBody)
    i.elseBody->accept(*this);
}
void NameAnalysis::visit(IntLiteral &il) {}
void NameAnalysis::visit(ParenthExpr &pe) { pe.innerExpr->accept(*this); }
void NameAnalysis::visit(PointerType &pt) {}
void NameAnalysis::visit(Program &p) {
  currScope = std::make_shared<Block>(Block({}));
  for (const std::shared_ptr<Decl> &decl : p.decls) {
    decl->accept(*this);
  }
  /* Check for main() function */
  std::shared_ptr<Decl> mainDecl = currScope->find("main");
  if (mainDecl == nullptr || mainDecl->astClass() != "FunDecl")
    error("Program did not contain a main() Function.");
  p.globalScope = currScope;
}
void NameAnalysis::visit(Return &r) {
  if (r.returnExpr)
    r.returnExpr->accept(*this);
}
void NameAnalysis::visit(SizeOf &so) {}
void NameAnalysis::visit(StringLiteral &sl) {}
void NameAnalysis::visit(StructType &st) {}
void NameAnalysis::visit(StructTypeDecl &std) {
  if (currScope->findLocal(std.getIdentifier()))
    return error("Attempted to declare a Struct with an identifier that is "
                 "already in use: " +
                 std.getIdentifier());

  currScope->insertDecl(std.getptr());

  /* Check that the fields in this struct are unique */
  std::set<std::string> structTypeFields;
  for (const std::shared_ptr<VarDecl> field : std.varDecls) {
    if (structTypeFields.find(field->getIdentifier()) != structTypeFields.end())
      return error("Struct " + std.getIdentifier() +
                   " contained multiple fields with the same identifier: " +
                   field->getIdentifier());
    structTypeFields.insert(field->identifer);
  }
}
void NameAnalysis::visit(TypeCast &tc) { tc.expr->accept(*this); }
void NameAnalysis::visit(ValueAt &va) { va.derefExpr->accept(*this); }
void NameAnalysis::visit(VarDecl &vd) {
  if (currScope->findLocal(vd.getIdentifier()))
    return error("Attempted to declare a Variable with an identifier that is "
                 "already in use: " +
                 vd.getIdentifier());
  currScope->insertDecl(vd.getptr());
}
void NameAnalysis::visit(VarExpr &ve) {
  if (currScope->find(ve.identifier) == nullptr)
    return error("Attempted to reference undeclared variable: " +
                 ve.identifier);
}
void NameAnalysis::visit(While &w) {
  w.condition->accept(*this);
  w.body->accept(*this);
}