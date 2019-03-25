#include "../../include/passes/NameAnalysis.h"

#include "atl/include/set.h"

using namespace ACC;

NameAnalysis::NameAnalysis(atl::shared_ptr<Program> progAST)
    : progAST(progAST) {}

void NameAnalysis::error(const atl::string &error) {
  errorCount++;
  errors.push_back(error);
}

void NameAnalysis::printErrors() {
  printf("Name Analysis Errors:\n");
  for (int idx = 0; idx < errors.size(); ++idx)
    printf("\t%s\n", errors[idx].c_str());
}

void NameAnalysis::run() { visit(*progAST); }

/* ---- Visit AST ---- */

void NameAnalysis::visit(AddressOf &ao) { ao.addressOfExpr->accept(*this); }
void NameAnalysis::visit(Allocation &a) {}
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
  if (b.outerScope == nullptr) {
    b.outerScope = currScope;
    currScope = b.getptr();
  }
  for (int idx = 0; idx < b.stmts.size(); ++idx)
    b.stmts[idx]->accept(*this);
  currScope = b.outerScope;
}
void NameAnalysis::visit(BoolLiteral &bl) {}
void NameAnalysis::visit(CharLiteral &cl) {}
void NameAnalysis::visit(ClassType &ct) {}
void NameAnalysis::visit(ClassTypeDecl &ctd) {
  if (currScope->findLocal(ctd.getIdentifier()))
    return error("Attempted to declare a Class with an identifier that is "
                 "already in use: " +
                 ctd.getIdentifier()->toString());
  currScope->insertDecl(ctd.getptr());
  ctd.outerScope = currScope;
  currScope = ctd.getptr();
  /* Visit ClassTypeDecl */
  atl::vector<atl::shared_ptr<Decl>> classMethods;
  for (int idx = 0; idx < ctd.classDecls.size(); ++idx) {
    const atl::shared_ptr<Decl> currDecl = ctd.classDecls[idx];
    if (currDecl->astClass() != "FunDecl" && currDecl->astClass() != "FunDef" &&
        currDecl->astClass() != "ConstructorDecl" &&
        currDecl->astClass() != "ConstructorDef" &&
        currDecl->astClass() != "DestructorDecl" &&
        currDecl->astClass() != "DestructorDef") {
      currDecl->accept(*this);
    } else {
      classMethods.push_back(currDecl);
    }
  }
  for (int idx = 0; idx < classMethods.size(); ++idx)
    classMethods[idx]->accept(*this);
  currScope = ctd.outerScope;
}
void NameAnalysis::visit(ConstructorDecl &cd) {}
void NameAnalysis::visit(ConstructorDef &cd) {}

void NameAnalysis::visit(Deletion &d) {}
void NameAnalysis::visit(DestructorDecl &dd) {}
void NameAnalysis::visit(DestructorDef &dd) {
  dd.destructorBlock->accept(*this);
}
void NameAnalysis::visit(DoWhile &dw) {
  dw.condition->accept(*this);
  dw.body->accept(*this);
}
void NameAnalysis::visit(EnumClassTypeDecl &ectd) {}
void NameAnalysis::visit(EnumTypeDecl &etd) {}
void NameAnalysis::visit(For &f) {
  // TODO: Scope the entire for loop.
  f.initialVarDecl->accept(*this);
  f.condition->accept(*this);
  f.endBodyExpr->accept(*this);
  f.body->accept(*this);
}
void NameAnalysis::visit(FunCall &fc) {
  if (currScope->find(fc.funIdentifier) == nullptr)
    return error("Attempted to call undeclared function: " +
                 fc.funIdentifier->toString());
  for (int idx = 0; idx < fc.funArgs.size(); ++idx)
    fc.funArgs[idx]->accept(*this);
}
void NameAnalysis::visit(FunDecl &fd) {
  // if (currScope->findLocal(fd.getIdentifier()))
  //   return error("Attempted to declare a Function with an identifier that is
  //   "
  //                "already in use: " +
  //                fd.getIdentifier());
  // currScope->insertDecl(fd.getptr());

  // fd.funBlock->setOuterBlock(currScope);
  // currScope = fd.funBlock;

  // for (const auto &param : fd.funParams)
  //   param->accept(*this);
  // fd.funBlock->accept(*this);
  // currScope = fd.funBlock->outerBlock;
}
void NameAnalysis::visit(FunDef &fd) {
  if (currScope->findLocal(fd.getIdentifier()))
    return error("Attempted to define a Function with an identifier that is "
                 "already in use: " +
                 fd.getIdentifier()->toString());
  currScope->insertDecl(fd.getptr());

  fd.funBlock->outerScope = currScope;
  currScope = fd.funBlock;

  for (int idx = 0; idx < fd.funParams.size(); ++idx)
    fd.funParams[idx]->accept(*this);
  fd.funBlock->accept(*this);
  currScope = fd.funBlock->outerScope;
}
void NameAnalysis::visit(If &i) {
  i.ifCondition->accept(*this);
  i.ifBody->accept(*this);
  if (i.elseBody)
    i.elseBody->accept(*this);
}
void NameAnalysis::visit(IntLiteral &il) {}
void NameAnalysis::visit(MemberAccess &ma) { ma.object->accept(*this); }
void NameAnalysis::visit(MemberCall &mc) {}

void NameAnalysis::visit(Namespace &n) {
  n.outerScope = currScope;
  currScope = n.getptr();
  for (int i = 0; i < n.namespaceDecls.size(); ++i)
    n.namespaceDecls[i]->accept(*this);
  currScope = n.outerScope;
}
void NameAnalysis::visit(ParenthExpr &pe) { pe.innerExpr->accept(*this); }
void NameAnalysis::visit(PointerType &pt) {}
void NameAnalysis::visit(PrefixOp &po) { po.variable->accept(*this); }
void NameAnalysis::visit(Program &p) {
  currScope = atl::make_shared<Block>(Block({}));
  for (int idx = 0; idx < p.decls.size(); ++idx)
    p.decls[idx]->accept(*this);
  p.globalScope = currScope;
}
void NameAnalysis::visit(ReferenceType &rt) {
  rt.referencedType->accept(*this);
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
                 std.getIdentifier()->toString());

  currScope->insertDecl(std.getptr());

  /* Check that the fields in this struct are unique */
  atl::set<atl::shared_ptr<Identifier>> structTypeFields;
  for (int idx = 0; idx < std.varDecls.size(); ++idx) {
    const atl::shared_ptr<VarDecl> field = std.varDecls[idx];
    if (structTypeFields.find(field->getIdentifier()))
      return error("Struct " + std.getIdentifier()->toString() +
                   " contained multiple fields with the same identifier: " +
                   field->getIdentifier()->toString());
    structTypeFields.insert(field->identifer);
  }
}
void NameAnalysis::visit(TertiaryExpr &t) {}
void NameAnalysis::visit(Throw &t) {}
void NameAnalysis::visit(TypeCast &tc) { tc.expr->accept(*this); }
void NameAnalysis::visit(TypeDefDecl &td) {}
void NameAnalysis::visit(ValueAt &va) { va.derefExpr->accept(*this); }
void NameAnalysis::visit(VarDecl &vd) {
  if (currScope->findLocal(vd.getIdentifier()))
    return error("Attempted to declare a Variable with an identifier that is "
                 "already in use: " +
                 vd.getIdentifier()->toString());
  currScope->insertDecl(vd.getptr());
}
void NameAnalysis::visit(VarDef &vd) {
  if (currScope->findLocal(vd.getIdentifier()))
    return error("Attempted to define a Variable with an identifier that is "
                 "already in use: " +
                 vd.getIdentifier()->toString());
  currScope->insertDecl(vd.getptr());
}
void NameAnalysis::visit(VarExpr &ve) {
  if (currScope->find(ve.varIdentifier) == nullptr)
    return error("Attempted to reference undeclared variable: " +
                 ve.varIdentifier->toString());
}
void NameAnalysis::visit(While &w) {
  w.condition->accept(*this);
  w.body->accept(*this);
}