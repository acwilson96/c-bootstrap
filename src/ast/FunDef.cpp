#include "ast/FunDef.h"

using namespace ACC;

FunDef::FunDef(const atl::set<FunModifiers> &p_funModifiers,
               const atl::shared_ptr<Identifier> &p_funIdentifier,
               const atl::vector<atl::shared_ptr<VarDecl>> &p_funParams,
               const atl::shared_ptr<Type> &p_funType,
               const atl::shared_ptr<Block> &p_funBlock)
    : FunDecl(p_funModifiers, p_funIdentifier, p_funParams, p_funType),
      funBlock(p_funBlock) {}

atl::shared_ptr<Identifier> FunDef::getIdentifier() const {
  return funIdentifier;
}

bool FunDef::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<FunDef *>(&rhs);
  return false;
}

bool FunDef::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool FunDef::operator==(const FunDecl &rhs) const {
  if (funModifiers != rhs.funModifiers)
    return false;

  if (*funType != *rhs.funType)
    return false;

  if (*funIdentifier != *rhs.funIdentifier)
    return false;

  if (funParams.size() != rhs.funParams.size())
    return false;

  for (int i = 0; i < funParams.size(); ++i)
    if (*funParams[i]->type != *rhs.funParams[i]->type)
      return false;

  return true;
}

bool FunDef::operator!=(const FunDecl &rhs) const { return !(*this == rhs); }

bool FunDef::operator==(const FunDef &rhs) const {
  if (funModifiers != rhs.funModifiers)
    return false;

  if (*funType != *rhs.funType)
    return false;

  if (*funIdentifier != *rhs.funIdentifier)
    return false;

  if (funParams.size() != rhs.funParams.size())
    return false;

  for (int i = 0; i < funParams.size(); ++i)
    if (*funParams[i]->type != *rhs.funParams[i]->type)
      return false;

  return true;
}

bool FunDef::operator!=(const FunDef &rhs) const { return !(*this == rhs); }