#pragma once

#ifndef ACC_NAMEANALYSIS_H
#define ACC_NAMEANALYSIS_H

#include "../AST.h"
#include "../ASTVisitor.h"

namespace ACC {

class NameAnalysis : public ASTVisitor<void> {

public:
  int errorCount = 0;
  atl::vector<atl::string> errors;

  NameAnalysis(atl::shared_ptr<Program> progAST);

  void error(const atl::string &error);

  void printErrors();

  void run();

private:
  int nodeCount = 0;
  atl::shared_ptr<Program> progAST;

  atl::shared_ptr<Block> currScope;

  /* ---- Visit AST ---- */

  void visit(AddressOf &ao) override;
  void visit(Allocation &a) override;
  void visit(ArrayAccess &aa) override;
  void visit(ArrayType &at) override;
  void visit(Assign &as) override;
  void visit(BaseType &bt) override;
  void visit(BinOp &bo) override;
  void visit(Block &b) override;
  void visit(CharLiteral &cl) override;
  void visit(ClassType &ct) override;
  void visit(ClassTypeDecl &ctd) override;
  void visit(ConstructorDecl &cd) override;
  void visit(ConstructorDef &cd) override;
  void visit(DoWhile &dw) override;
  void visit(EnumTypeDecl &etd) override;
  void visit(MemberAccess &fa) override;
  void visit(For &f) override;
  void visit(FunCall &fc) override;
  void visit(FunDecl &fd) override;
  void visit(FunDef &fd) override;
  void visit(If &i) override;
  void visit(IntLiteral &il) override;
  void visit(Namespace &n) override;
  void visit(ParenthExpr &pe) override;
  void visit(PointerType &pt) override;
  void visit(PrefixInc &pi) override;
  void visit(Program &p) override;
  void visit(ReferenceType &rt) override;
  void visit(Return &r) override;
  void visit(SizeOf &so) override;
  void visit(StringLiteral &sl) override;
  void visit(StructType &st) override;
  void visit(StructTypeDecl &std) override;
  void visit(TypeCast &tc) override;
  void visit(TypeDefDecl &td) override;
  void visit(ValueAt &va) override;
  void visit(VarDecl &vd) override;
  void visit(VarDef &vd) override;
  void visit(VarExpr &ve) override;
  void visit(While &w) override;
};

}; // namespace ACC

#endif