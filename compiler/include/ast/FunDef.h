#pragma once

#include "Block.h"
#include "Decl.h"
#include "FunDecl.h"
#include "Type.h"
#include "VarDecl.h"

namespace ACC {

class FunDef : public FunDecl {

public:
  atl::shared_ptr<Block> funBlock;

  FunDef(const atl::set<FunModifiers> &p_funModifiers,
         const atl::shared_ptr<Identifier> &p_funIdentifier,
         const atl::vector<atl::shared_ptr<VarDecl>> &p_funParams,
         const atl::shared_ptr<Type> &p_funType,
         const atl::shared_ptr<Block> &p_funBlock);

  atl::shared_ptr<FunDecl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const FunDef &rhs) const;
  bool operator!=(const FunDef &rhs) const;

  atl::shared_ptr<Identifier> getIdentifier() const override;

  void accept(ASTVisitor<void> &v) override;
  atl::string accept(ASTVisitor<atl::string> &v) override;
  atl::shared_ptr<Type> accept(ASTVisitor<atl::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  atl::shared_ptr<X86::Operand>
  accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) override;
  atl::shared_ptr<ASTNode>
  accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) override;
  atl::string astClass() const override;
};

} // namespace ACC
