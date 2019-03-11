#include "../../include/passes/DotGraph.h"

using namespace ACC;

DotGraph::DotGraph(atl::shared_ptr<Program> progAST) : progAST(progAST) {}
void DotGraph::print() { visit(*progAST); }

void DotGraph::declare(const atl::string &nodeID, const atl::string &label) {
  put(nodeID + " [label=\"" + label + "\"];");
}

void DotGraph::join(const atl::string &lhs, const atl::string &rhs) {
  atl::string output = lhs + " -> " + rhs;
  printf("%s\n", output.c_str());
}

void DotGraph::put(const atl::string &str) { printf("%s\n", str.c_str()); }

/* ---- Visit AST ---- */

atl::string DotGraph::visit(AddressOf &ao) { return ""; }
atl::string DotGraph::visit(Allocation &a) {
  const atl::string allocationID =
      atl::string("Allocation") + atl::to_string(++nodeCount);
  declare(allocationID, "new");
  atl::string allocationExprID;
  if (a.variableConstructorCall) {
    allocationExprID = a.variableConstructorCall->accept(*this);
    join(allocationID, allocationExprID);
    return allocationID;
  } else {
    const atl::string allocationTypeID =
        atl::string("AllocationType") + atl::to_string(++nodeCount);
    join(allocationID, a.variableType->accept(*this));
    return allocationID;
  }
}
atl::string DotGraph::visit(ArrayAccess &aa) {
  atl::string arrayAccessID =
      atl::string("ArrayAccess") + atl::to_string(++nodeCount);
  declare(arrayAccessID, "ArrayAccess");

  atl::string arrayID = aa.array->accept(*this);
  atl::string indexID = aa.index->accept(*this);

  join(arrayAccessID, arrayID);
  join(arrayAccessID, indexID);

  return arrayAccessID;
}
atl::string DotGraph::visit(ArrayType &at) {
  const atl::string arrayTypeID =
      atl::string("ArrayType") + atl::to_string(++nodeCount);
  declare(arrayTypeID, at.arrayType->accept(*this) + "[]");
  const atl::string arraySize = at.arraySize->accept(*this);
  join(arrayTypeID, arraySize);
  return arrayTypeID;
}
atl::string DotGraph::visit(Assign &as) {
  atl::string assignID = atl::string("Assign") + atl::to_string(++nodeCount);
  declare(assignID, "=");

  atl::string lhsID = as.lhs->accept(*this);
  atl::string rhsID = as.rhs->accept(*this);

  join(assignID, lhsID);
  join(assignID, rhsID);

  return assignID;
}
atl::string DotGraph::visit(BaseType &bt) {
  switch (bt.primitiveType) {
  case PrimitiveType::CHAR:
    return "char";
  case PrimitiveType::INT:
    return "int";
  case PrimitiveType::SHORT:
    return "short";
  case PrimitiveType::VOID:
    return "void";
  case PrimitiveType::UINT:
    return "unsigned int";
  }
}
atl::string DotGraph::visit(BinOp &bo) {
  atl::string binOpID = atl::string("BinOp") + atl::to_string(++nodeCount);
  declare(binOpID, "BinOp");

  atl::string lhsId = bo.lhs->accept(*this);

  atl::string OpID = atl::string("Op") + atl::to_string(++nodeCount);
  switch (bo.operation) {
  case Op::ADD:
    declare(OpID, "+");
    break;
  case Op::AND:
    declare(OpID, "&&");
    break;
  case Op::DIV:
    declare(OpID, "/");
    break;
  case Op::EQ:
    declare(OpID, "==");
    break;
  case Op::GE:
    declare(OpID, ">=");
    break;
  case Op::GT:
    declare(OpID, ">");
    break;
  case Op::LE:
    declare(OpID, "<=");
    break;
  case Op::LT:
    declare(OpID, "<");
    break;
  case Op::MOD:
    declare(OpID, "%");
    break;
  case Op::MUL:
    declare(OpID, "*");
    break;
  case Op::NE:
    declare(OpID, "!=");
    break;
  case Op::OR:
    declare(OpID, "||");
    break;
  case Op::SUB:
    declare(OpID, "-");
    break;
  }

  atl::string rhsId = bo.rhs->accept(*this);

  join(binOpID, lhsId);
  join(binOpID, OpID);
  join(binOpID, rhsId);

  return binOpID;
}
atl::string DotGraph::visit(Block &b) {
  const atl::string blockID =
      atl::string("Block") + atl::to_string(++nodeCount);
  declare(blockID, "{}");

  for (int idx = 0; idx < b.blockStmts.size(); ++idx) {
    const atl::string currStmt = b.blockStmts[idx]->accept(*this);
    join(blockID, currStmt);
  }

  return blockID;
}
atl::string DotGraph::visit(CharLiteral &cl) {
  atl::string charID = atl::string("CharLiteral") + atl::to_string(++nodeCount);
  declare(charID, atl::string("'") + cl.getLiteral() + "'");
  return charID;
}
atl::string DotGraph::visit(ClassType &ct) {
  atl::string classTypeID =
      atl::string("ClassType") + atl::to_string(++nodeCount);
  // declare(classTypeID, ct.identifier);
  return ct.identifier;
}
atl::string DotGraph::visit(ClassTypeDecl &ctd) {
  const atl::string classID =
      atl::string("Class") + atl::to_string(++nodeCount);
  declare(classID, ctd.getIdentifier());

  const atl::string publicDecls =
      atl::string("public") + atl::to_string(++nodeCount);
  const atl::string privateDecls =
      atl::string("private") + atl::to_string(++nodeCount);
  const atl::string protectedDecls =
      atl::string("protected") + atl::to_string(++nodeCount);

  declare(publicDecls, "public:");
  declare(privateDecls, "private:");
  declare(protectedDecls, "protected:");
  join(classID, publicDecls);
  join(classID, privateDecls);
  join(classID, protectedDecls);

  for (int i = 0; i < ctd.classDecls.size(); ++i) {
    if (ctd.classDecls[i]->visibility == Decl::Visibility::PUBLIC)
      join(publicDecls, ctd.classDecls[i]->accept(*this));
    if (ctd.classDecls[i]->visibility == Decl::Visibility::PRIVATE)
      join(privateDecls, ctd.classDecls[i]->accept(*this));
    if (ctd.classDecls[i]->visibility == Decl::Visibility::PROTECTED)
      join(protectedDecls, ctd.classDecls[i]->accept(*this));
  }

  classTypeDeclIDs[ctd.classType->identifier.c_str()] = classID;
  return classID;
}
atl::string DotGraph::visit(ConstructorDecl &cd) {
  atl::string constructorID =
      atl::string("ConstructorDecl") + atl::to_string(++nodeCount);
  // declare(funcID, fd.funName);
  // join(funcID, fd.funBlock->accept(*this));
  return constructorID;
}
atl::string DotGraph::visit(ConstructorDef &cd) {
  atl::string constructorID =
      atl::string("ConstructorDef") + atl::to_string(++nodeCount);
  atl::string funParams = "(";
  for (int i = 0; i < cd.constructorParams.size(); ++i) {
    atl::string currParam = cd.constructorParams[i]->type->accept(*this) + " " +
                            cd.constructorParams[i]->getIdentifier();
    if (i != (cd.constructorParams.size() - 1))
      currParam += ", ";
    funParams += currParam;
  }
  funParams += ")";
  declare(constructorID, cd.classType->identifier + funParams);
  join(constructorID, cd.constructorBlock->accept(*this));
  return constructorID;
}
atl::string DotGraph::visit(DestructorDecl &dd) {
  atl::string destructorID =
      atl::string("DestructorDecl") + atl::to_string(++nodeCount);
  // declare(funcID, fd.funName);
  // join(funcID, fd.funBlock->accept(*this));
  return destructorID;
}
atl::string DotGraph::visit(DestructorDef &dd) {
  atl::string destructorID =
      atl::string("ConstructorDef") + atl::to_string(++nodeCount);
  atl::string funParams = "()";
  declare(destructorID, dd.classType->identifier + funParams);
  join(destructorID, dd.destructorBlock->accept(*this));
  return destructorID;
}
atl::string DotGraph::visit(DoWhile &dw) {
  atl::string whileID = atl::string("While") + atl::to_string(++nodeCount);
  declare(whileID, "do {} while()");
  join(whileID, dw.body->accept(*this));
  join(whileID, dw.condition->accept(*this));
  return whileID;
}
atl::string DotGraph::visit(EnumTypeDecl &etd) { return ""; }
atl::string DotGraph::visit(For &f) {
  atl::string forID = atl::string("For") + atl::to_string(++nodeCount);

  return forID;
}
atl::string DotGraph::visit(FunCall &fc) {
  atl::string funCallID = atl::string("FunCall") + atl::to_string(++nodeCount);
  declare(funCallID, atl::string(fc.funName.c_str()) + "()");

  for (int idx = 0; idx < fc.funArgs.size(); ++idx)
    join(funCallID, fc.funArgs[idx]->accept(*this));

  return funCallID;
}
atl::string DotGraph::visit(FunDecl &fd) {
  atl::string funcID = atl::string("FunDecl") + atl::to_string(++nodeCount);
  // declare(funcID, fd.funName);
  // join(funcID, fd.funBlock->accept(*this));
  return funcID;
}
atl::string DotGraph::visit(FunDef &fd) {
  atl::string funcID = atl::string("FunDef") + atl::to_string(++nodeCount);
  atl::string funParams = "(";
  for (int i = 0; i < fd.funParams.size(); ++i) {
    atl::string currParam = fd.funParams[i]->type->accept(*this) + " " +
                            fd.funParams[i]->getIdentifier();
    if (i != (fd.funParams.size() - 1))
      currParam += ", ";
    funParams += currParam;
  }
  funParams += ")";
  declare(funcID, fd.funName + funParams);
  join(funcID, fd.funBlock->accept(*this));
  return funcID;
}
atl::string DotGraph::visit(If &i) {
  atl::string ifID = atl::string("If") + atl::to_string(++nodeCount);
  declare(ifID, "if then else");

  join(ifID, i.ifCondition->accept(*this));
  join(ifID, i.ifBody->accept(*this));
  if (i.elseBody != nullptr) {
    join(ifID, i.elseBody->accept(*this));
  }

  return ifID;
}
atl::string DotGraph::visit(IntLiteral &il) {
  atl::string intID = atl::string("IntLiteral") + atl::to_string(++nodeCount);
  declare(intID, il.getLiteral());
  return intID;
}
atl::string DotGraph::visit(Namespace &n) {
  atl::string namespaceID =
      atl::string("Namespace") + atl::to_string(++nodeCount);
  for (int i = 0; i < n.namespaceDecls.size(); ++i)
    n.namespaceDecls[i]->accept(*this);
  return namespaceID;
}
atl::string DotGraph::visit(MemberAccess &ma) {
  atl::string memberAccessID =
      atl::string("MemberAccess") + atl::to_string(++nodeCount);
  declare(memberAccessID, atl::string("MemberAccess.") + ma.field.c_str());

  atl::string objID = ma.object->accept(*this);

  join(memberAccessID, objID);

  return memberAccessID;
}
atl::string DotGraph::visit(MemberCall &mc) { return ""; }
atl::string DotGraph::visit(ParenthExpr &pe) {
  return pe.innerExpr->accept(*this);
}
atl::string DotGraph::visit(PointerType &pt) {
  return pt.pointedType->accept(*this) + "*";
}
atl::string DotGraph::visit(PrefixInc &pi) {
  atl::string preficIncID =
      atl::string("PrefixInc") + atl::to_string(++nodeCount);
  declare(preficIncID, "\"PrefixInc\"");
  join(preficIncID, pi.incrementVar->accept(*this));
  return preficIncID;
}
atl::string DotGraph::visit(Program &p) {
  printf("digraph prog {\n");
  for (int idx = 0; idx < p.decls.size(); ++idx) {
    p.decls[idx]->accept(*this);
  }
  printf("}\n");
  return "Node0";
}
atl::string DotGraph::visit(ReferenceType &rt) {
  return rt.referencedType->accept(*this) + " &";
}
atl::string DotGraph::visit(Return &r) {
  atl::string returnID = atl::string("Return") + atl::to_string(++nodeCount);
  declare(returnID, "return");

  if (r.returnExpr != nullptr) {
    join(returnID, r.returnExpr->accept(*this));
  }

  return returnID;
}
atl::string DotGraph::visit(SizeOf &so) {
  atl::string returnID = atl::string("SizeOf") + atl::to_string(++nodeCount);
  declare(returnID, atl::string("sizeof(") + so.type->accept(*this) + ")");

  return returnID;
}
atl::string DotGraph::visit(StringLiteral &sl) {
  atl::string strID =
      atl::string("StringLiteral") + atl::to_string(++nodeCount);
  declare(strID, atl::string("\\\"") + sl.getLiteral() + "\\\"");
  return strID;
}
atl::string DotGraph::visit(StructType &st) {
  return atl::string("struct ") + st.identifier;
}
atl::string DotGraph::visit(StructTypeDecl &std) {
  atl::string structTypeDeclID =
      atl::string("StructTypeDecl") + atl::to_string(++nodeCount);
  declare(structTypeDeclID, std.structType->accept(*this) + " = {}");
  for (int idx = 0; idx < std.varDecls.size(); ++idx)
    join(structTypeDeclID, std.varDecls[idx]->accept(*this));
  return structTypeDeclID;
}
atl::string DotGraph::visit(TypeCast &tc) {
  atl::string typecastID =
      atl::string("TypeCast") + atl::to_string(++nodeCount);
  declare(typecastID, atl::string("(") + tc.type->accept(*this) + ")");
  join(typecastID, tc.expr->accept(*this));
  return typecastID;
}
atl::string DotGraph::visit(TypeDefDecl &td) { return ""; }
atl::string DotGraph::visit(ValueAt &va) {
  atl::string derefID = atl::string("Deref") + atl::to_string(++nodeCount);
  declare(derefID, "\"Deref\"");
  join(derefID, va.derefExpr->accept(*this));
  return derefID;
}
atl::string DotGraph::visit(VarDecl &vd) {
  atl::string varDeclID = atl::string("VarDecl") + atl::to_string(++nodeCount);
  declare(varDeclID, vd.type->accept(*this) + " " + vd.identifer + ";");
  // if (vd.type->astClass() == "ClassType") {
  //   atl::shared_ptr<ClassType> classType =
  //       atl::static_pointer_cast<ClassType>(vd.type);
  //   join(varDeclID, classTypeDeclIDs[classType->identifier.c_str()]);
  // }
  return varDeclID;
}
atl::string DotGraph::visit(VarDef &vd) {
  const atl::string varDefID =
      atl::string("VarDef") + atl::to_string(++nodeCount);
  const atl::string valueID = vd.value->accept(*this);
  declare(varDefID, vd.type->accept(*this) + " " + vd.identifer + " = ");
  join(varDefID, valueID);
  return varDefID;
}
atl::string DotGraph::visit(VarExpr &ve) {
  atl::string varID = atl::string("VarExpr") + atl::to_string(++nodeCount);
  declare(varID, ve.identifier.c_str());
  return varID;
}
atl::string DotGraph::visit(While &w) {
  atl::string whileID = atl::string("While") + atl::to_string(++nodeCount);
  declare(whileID, "while()");
  join(whileID, w.condition->accept(*this));
  join(whileID, w.body->accept(*this));
  return whileID;
}