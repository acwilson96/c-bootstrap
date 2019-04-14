#include "passes/TypeAnalysis.h"

using namespace ACC;
TypeAnalysis::TypeAnalysis(atl::shared_ptr<Program> progAST)
    : progAST(progAST) {}

atl::shared_ptr<Type> TypeAnalysis::error(const atl::string &error) {
  errorCount++;
  errors.push_back(error);
  return nullptr;
}

void TypeAnalysis::printErrors() {
  printf("Type Analysis Errors:\n");
  for (int idx = 0; idx < errors.size(); ++idx)
    printf("\t%s\n", errors[idx].c_str());
}

void TypeAnalysis::run() { visit(*progAST); }

/* ---- Visit AST ---- */

atl::shared_ptr<Type> TypeAnalysis::visit(AddressOf &ao) {
  return atl::make_shared<PointerType>(
      PointerType(ao.addressOfExpr->accept(*this)));
}
atl::shared_ptr<Type> TypeAnalysis::visit(Allocation &a) { return nullptr; }
atl::shared_ptr<Type> TypeAnalysis::visit(ArrayAccess &aa) {
  atl::shared_ptr<Type> arrayExprType = aa.array->accept(*this);
  atl::shared_ptr<Type> arrayIndex = aa.index->accept(*this);
  if (arrayExprType->astClass() != "ArrayType")
    return error("Type Error: Attempted to index an expression which was not "
                 "an array. Was of type: " +
                 arrayExprType->astClass());
  if (arrayIndex->astClass() != "BaseType")
    return error("Type Error: Attempted to index an array using an expression "
                 "which was not of type int. Was of type: " +
                 arrayIndex->astClass());

  return atl::static_pointer_cast<ArrayType>(arrayExprType)->type;
}
atl::shared_ptr<Type> TypeAnalysis::visit(ArrayType &at) { return at.getptr(); }
atl::shared_ptr<Type> TypeAnalysis::visit(Assign &as) {
  atl::shared_ptr<Type> lhs = as.lhs->accept(*this);
  atl::shared_ptr<Type> rhs = as.rhs->accept(*this);
  if (*lhs != *rhs)
    return error("Assignation has mismatched types.");
  return nullptr;
}
atl::shared_ptr<Type> TypeAnalysis::visit(BaseType &bt) { return bt.getptr(); }
atl::shared_ptr<Type> TypeAnalysis::visit(BinOp &bo) {
  bo.lhs->accept(*this);
  bo.rhs->accept(*this);
  return atl::make_shared<BaseType>(BaseType(PrimitiveType::INT));
}
atl::shared_ptr<Type> TypeAnalysis::visit(Block &b) {
  b.outerScope = currScope;
  currScope = b.getptr();

  for (int idx = 0; idx < b.stmts.size(); ++idx)
    b.stmts[idx]->accept(*this);

  currScope = b.outerScope;
  return nullptr;
}
atl::shared_ptr<Type> TypeAnalysis::visit(BoolLiteral &bl) {
  return atl::make_shared<BaseType>(BaseType(PrimitiveType::BOOL));
}
atl::shared_ptr<Type> TypeAnalysis::visit(CharLiteral &cl) {
  return atl::make_shared<BaseType>(BaseType(PrimitiveType::CHAR));
}
atl::shared_ptr<Type> TypeAnalysis::visit(ClassType &ct) {
  // atl::shared_ptr<Decl> findDecl = currScope->find(ct.getptr());
  // if (findDecl->astClass() != "ClassTypeDecl")
  //   return error("Attempted to use a ClassType that was not declared.");

  // ct.typeDefinition = atl::static_pointer_cast<ClassTypeDecl>(findDecl);
  return ct.getptr();
}
atl::shared_ptr<Type> TypeAnalysis::visit(ClassTypeDecl &ctd) {
  return ctd.classType;
}
atl::shared_ptr<Type> TypeAnalysis::visit(ConstructorDecl &cd) {
  return nullptr;
}
atl::shared_ptr<Type> TypeAnalysis::visit(ConstructorDef &cd) {
  return nullptr;
}
atl::shared_ptr<Type> TypeAnalysis::visit(Deletion &d) { return nullptr; }
atl::shared_ptr<Type> TypeAnalysis::visit(DestructorDecl &cd) {
  return nullptr;
}
atl::shared_ptr<Type> TypeAnalysis::visit(DestructorDef &cd) { return nullptr; }
atl::shared_ptr<Type> TypeAnalysis::visit(DoWhile &dw) {
  dw.body->accept(*this);
  atl::shared_ptr<Type> conditionType = dw.condition->accept(*this);
  if (conditionType->astClass() != "BaseType")
    return error("Type Analysis: Type of DoWhile condition is invalid.");
  atl::shared_ptr<BaseType> conditionBaseType =
      atl::static_pointer_cast<BaseType>(conditionType);
  if (conditionBaseType->primitiveType != PrimitiveType::INT &&
      conditionBaseType->primitiveType != PrimitiveType::BOOL)
    return error(
        "Type Analysis: Type of DoWhile condition is not INT or BOOL.");
  return nullptr;
}
atl::shared_ptr<Type> TypeAnalysis::visit(EnumClassTypeDecl &ectd) {
  return nullptr;
}
atl::shared_ptr<Type> TypeAnalysis::visit(For &f) { return nullptr; }
atl::shared_ptr<Type> TypeAnalysis::visit(FunCall &fc) {
  return atl::make_shared<BaseType>(BaseType(PrimitiveType::VOID));
}
atl::shared_ptr<Type> TypeAnalysis::visit(FunDecl &fd) { return fd.funType; }
atl::shared_ptr<Type> TypeAnalysis::visit(FunDef &fd) {
  fd.outerScope = currScope;
  currScope = fd.outerScope;

  for (int idx = 0; idx < fd.funParams.size(); ++idx)
    fd.funParams[idx]->accept(*this);
  fd.funBlock->accept(*this);
  currScope = fd.outerScope;
  return fd.funType;
}
atl::shared_ptr<Type> TypeAnalysis::visit(If &i) {
  atl::shared_ptr<Type> condType = i.ifCondition->accept(*this);

  if (condType->astClass() != "BaseType")
    return error("Type Analysis: Type of If condition is invalid.");

  atl::shared_ptr<BaseType> condBaseType =
      atl::static_pointer_cast<BaseType>(condType);

  if (condBaseType->primitiveType != PrimitiveType::INT &&
      condBaseType->primitiveType != PrimitiveType::BOOL)
    return error("Type Analysis: Type of If condition is not INT or BOOL.");

  i.ifBody->accept(*this);

  if (i.elseBody)
    i.elseBody->accept(*this);
  return nullptr;
}
atl::shared_ptr<Type> TypeAnalysis::visit(IntLiteral &il) {
  return atl::make_shared<BaseType>(BaseType(PrimitiveType::INT));
}
atl::shared_ptr<Type> TypeAnalysis::visit(MemberAccess &ma) {
  atl::shared_ptr<Type> objType = ma.object->accept(*this);
  if (objType->astClass() != "StructType")
    return error("Type Analysis: Attempted to access field on expression "
                 "that is not a struct");

  atl::shared_ptr<StructType> structType =
      atl::static_pointer_cast<StructType>(objType);
  // atl::shared_ptr<Decl> identDecl = currScope->find(structType->identifier);
  // if (identDecl == nullptr)
  //   return error("Type Analysis: Attempted to access field on expression "
  //                "that does not have a type definition.");
  // if (identDecl->astClass() != "StructTypeDecl")
  //   return error("Type Analysis: Attempted to access field on expression that
  //   "
  //                "does not have a StructTypeDecl");

  // atl::shared_ptr<StructTypeDecl> structTypeDecl =
  //     atl::static_pointer_cast<StructTypeDecl>(identDecl);

  // for (int idx = 0; idx < structTypeDecl->varDecls.size(); ++idx)
  //   if (*structTypeDecl->varDecls[idx]->identifier == *ma.fieldIdentifier)
  //     return structTypeDecl->varDecls[idx]->type;
  return nullptr;
  // return error("Type Analysis: Attempted to access field on a struct that "
  //              "does not exist.");
}
atl::shared_ptr<Type> TypeAnalysis::visit(MemberCall &mc) { return nullptr; }
atl::shared_ptr<Type> TypeAnalysis::visit(Namespace &n) {
  for (int i = 0; i < n.namespaceDecls.size(); ++i)
    n.namespaceDecls[i]->accept(*this);
  return nullptr;
}
atl::shared_ptr<Type> TypeAnalysis::visit(ParenthExpr &pe) {
  return pe.innerExpr->accept(*this);
}
atl::shared_ptr<Type> TypeAnalysis::visit(PointerType &pt) {
  return pt.getptr();
}
atl::shared_ptr<Type> TypeAnalysis::visit(PrefixOp &po) {
  // TODO
  return nullptr;
}
atl::shared_ptr<Type> TypeAnalysis::visit(Program &p) {
  currScope = p.globalScope;
  for (int idx = 0; idx < p.decls.size(); ++idx)
    p.decls[idx]->accept(*this);
  return nullptr;
}
atl::shared_ptr<Type> TypeAnalysis::visit(ReferenceType &rt) {
  return rt.getptr();
}
atl::shared_ptr<Type> TypeAnalysis::visit(Return &r) {
  if (r.returnExpr)
    return r.returnExpr->accept(*this);
  return atl::make_shared<BaseType>(BaseType(PrimitiveType::VOID));
  ;
}
atl::shared_ptr<Type> TypeAnalysis::visit(SizeOf &so) {
  return atl::make_shared<BaseType>(BaseType(PrimitiveType::INT));
}
atl::shared_ptr<Type> TypeAnalysis::visit(StringLiteral &sl) { return nullptr; }
atl::shared_ptr<Type> TypeAnalysis::visit(StructType &st) {
  // atl::shared_ptr<Decl> findDecl = currScope->find(st.identifier);
  // if (findDecl->astClass() != "StructTypeDecl")
  // return error("Attempted to use a StructType that was not declared.");

  // st.typeDefinition = atl::static_pointer_cast<StructTypeDecl>(findDecl);
  return st.getptr();
}
atl::shared_ptr<Type> TypeAnalysis::visit(StructTypeDecl &std) {
  return nullptr;
}
atl::shared_ptr<Type> TypeAnalysis::visit(TertiaryExpr &t) { return nullptr; }
atl::shared_ptr<Type> TypeAnalysis::visit(Throw &t) { return nullptr; }
atl::shared_ptr<Type> TypeAnalysis::visit(TypeCast &tc) { return tc.type; }
atl::shared_ptr<Type> TypeAnalysis::visit(TypeDefDecl &td) { return td.type; }
atl::shared_ptr<Type> TypeAnalysis::visit(ValueAt &va) {
  atl::shared_ptr<Type> exprType = va.derefExpr->accept(*this);
  if (exprType->astClass() != "PointerType")
    return error("Attempted to dereference variable that wasn't a pointer. Was "
                 "of type: " +
                 exprType->astClass());
  return atl::static_pointer_cast<PointerType>(exprType)->pointedType;
}
atl::shared_ptr<Type> TypeAnalysis::visit(VarDecl &vd) {
  vd.type->accept(*this);
  return nullptr;
}
atl::shared_ptr<Type> TypeAnalysis::visit(VarDef &vd) { return nullptr; }
atl::shared_ptr<Type> TypeAnalysis::visit(VarExpr &ve) {
  return ve.varDecl->type->accept(*this);
}
atl::shared_ptr<Type> TypeAnalysis::visit(While &w) {
  atl::shared_ptr<Type> conditionType = w.condition->accept(*this);
  if (conditionType->astClass() != "BaseType")
    return error("Type Analysis: Type of While condition is invalid.");
  atl::shared_ptr<BaseType> conditionBaseType =
      atl::static_pointer_cast<BaseType>(conditionType);
  if (conditionBaseType->primitiveType != PrimitiveType::INT &&
      conditionBaseType->primitiveType != PrimitiveType::BOOL)
    return error("Type Analysis: Type of While condition is not INT or BOOl.");
  w.body->accept(*this);
  return nullptr;
}