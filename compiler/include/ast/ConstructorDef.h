
#pragma once

#ifndef ACC_CONSTRUCTORDEF_H
#define ACC_CONSTRUCTORDEF_H

#include "Block.h"
#include "ConstructorDecl.h"

namespace ACC {

class ConstructorDef : public ConstructorDecl {

public:
  atl::shared_ptr<Block> constructorBlock;

  ConstructorDef(const atl::shared_ptr<Type> &classType,
                 const atl::vector<atl::shared_ptr<VarDecl>> &constructorParams,
                 const atl::shared_ptr<Block> &constructorBlock);

  atl::shared_ptr<ConstructorDecl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const ConstructorDef &rhs) const;
  bool operator!=(const ConstructorDef &rhs) const;

  atl::string getIdentifier() const override;

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

}; // namespace ACC

#endif