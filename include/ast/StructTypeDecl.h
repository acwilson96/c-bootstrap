#pragma once

#ifndef STRUCTTYPEDECL_H
#define STRUCTTYPEDECL_H

#include <vector>

#include "Decl.h"
#include "StructType.h"
#include "VarDecl.h"

namespace ACC {

class StructTypeDecl : public Decl {

public:
  std::shared_ptr<StructType> structType;
  std::vector<std::shared_ptr<VarDecl>> varDecls;
  int allocSize;
  int compactSize;

  StructTypeDecl(std::shared_ptr<StructType> structType,
                 const std::vector<std::shared_ptr<VarDecl>> &varDecls)
      : structType(structType), varDecls(varDecls) {}

  bool operator==(Decl &rhs) const override {
    if (rhs.astClass() == astClass())
      return *this == *static_cast<StructTypeDecl *>(&rhs);
    return false;
  }
  bool operator!=(Decl &rhs) const override { return !(*this == rhs); }

  bool operator==(const StructTypeDecl &rhs) const {
    if (*structType != *rhs.structType)
      return false;

    if (varDecls.size() != rhs.varDecls.size())
      return false;

    for (int i = 0; i < varDecls.size(); i++)
      if (*varDecls[i] != *rhs.varDecls[i])
        return false;

    return true;
  }
  bool operator!=(const StructTypeDecl &rhs) const { return !(*this == rhs); }

  std::string getIdentifier() const override {
    return "struct " + structType->identifier;
  }

  void accept(ASTVisitor<void> &v) override { return v.visit(*this); }
  std::string accept(ASTVisitor<std::string> &v) override {
    return v.visit(*this);
  }
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override {
    return v.visit(*this);
  }
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override {
    return v.visit(*this);
  }
  std::string astClass() const override { return "StructTypeDecl"; }
};

int StructType::getBytes() const {
  int aggregateBytes = 0;
  if (typeDefinition == nullptr)
    return aggregateBytes;

  for (const std::shared_ptr<VarDecl> &structField : typeDefinition->varDecls)
    aggregateBytes += structField->type->getBytes();

  return aggregateBytes;
}

}; // namespace ACC

#endif