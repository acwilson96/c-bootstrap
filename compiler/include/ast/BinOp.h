#pragma once

#ifndef ACC_BINOP_H
#define ACC_BINOP_H

#include "Expr.h"
#include "Op.h"

namespace ACC {

class BinOp : public Expr, public atl::enable_shared_from_this<BinOp> {

public:
  atl::shared_ptr<Expr> lhs;
  Op operation;
  atl::shared_ptr<Expr> rhs;

  BinOp(const atl::shared_ptr<Expr> &lhs, const Op operation,
        const atl::shared_ptr<Expr> &rhs);

  atl::shared_ptr<BinOp> getptr();

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const BinOp &rhs) const;
  bool operator!=(const BinOp &rhs) const;

  void accept(ASTVisitor<void> &v) override;
  atl::string accept(ASTVisitor<atl::string> &v) override;
  atl::shared_ptr<Type> accept(ASTVisitor<atl::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  atl::shared_ptr<X86::Operand>
  accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) override;
  atl::shared_ptr<ASTNode>
  accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) override;
  atl::string astClass() const override;

  atl::string opToStr(const Op op) const;
};

}; // namespace ACC

#endif
