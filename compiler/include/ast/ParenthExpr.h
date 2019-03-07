#pragma once

#ifndef ACC_PARENTHEXPR_H
#define ACC_PARENTHEXPR_H

#include "Expr.h"

namespace ACC {

class ParenthExpr : public Expr,
                    public atl::enable_shared_from_this<ParenthExpr> {

public:
  atl::shared_ptr<Expr> innerExpr;

  ParenthExpr(atl::shared_ptr<Expr> innerExpr);

  atl::shared_ptr<ParenthExpr> getptr();

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const ParenthExpr &rhs) const;
  bool operator!=(const ParenthExpr &rhs) const;

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