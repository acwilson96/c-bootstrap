#pragma once

#ifndef ACC_EXPR_H
#define ACC_EXPR_H

#include "Stmt.h"

namespace ACC {

class Expr : public Stmt {
public:
  virtual ~Expr() {}
  virtual bool operator==(Expr &rhs) const = 0;
  virtual bool operator!=(Expr &rhs) const = 0;
};

}; // namespace ACC

#endif