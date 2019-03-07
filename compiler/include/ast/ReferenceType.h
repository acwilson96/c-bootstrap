#pragma once

#ifndef ACC_REFERENCETYPE_H
#define ACC_REFERENCETYPE_H

#include "PrimitiveType.h"
#include "Type.h"

namespace ACC {

class ReferenceType : public Type,
                    public atl::enable_shared_from_this<ReferenceType> {

public:
  atl::shared_ptr<Type> referencedType;

  ReferenceType(atl::shared_ptr<Type> referencedType);

  atl::shared_ptr<ReferenceType> getptr();

  bool operator==(Type &rhs) const override;

  bool operator!=(Type &rhs) const override;

  bool operator==(const ReferenceType &rhs) const;
  bool operator!=(const ReferenceType &rhs) const;

  int getBytes() const override;

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