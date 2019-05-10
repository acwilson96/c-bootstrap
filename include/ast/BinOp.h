#pragma once

#include "Expr.h"
#include "Op.h"
#include "Type.h"

namespace ACC {

class BinOp : public Expr, public atl::enable_shared_from_this<BinOp> {

public:
  atl::shared_ptr<Expr> lhs;
  Op operation;
  atl::shared_ptr<Expr> rhs;

  BinOp(const atl::shared_ptr<Expr> &p_lhs, const Op p_operation,
        const atl::shared_ptr<Expr> &p_rhs);

  atl::string getSignature() const override;

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const BinOp &rhs) const;
  bool operator!=(const BinOp &rhs) const;

  atl::shared_ptr<BinOp> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "BinOp"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
