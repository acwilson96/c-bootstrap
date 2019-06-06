#include "ast/ReferenceType.h"

using namespace ACC;

ReferenceType::ReferenceType(const atl::shared_ptr<Type> &p_referencedType)
    : referencedType(p_referencedType) {}

bool ReferenceType::canCastTo(Type &rhs) const { return false; }

unsigned int ReferenceType::getBytes() const { return 4; }
bool ReferenceType::equivalentTo(Type &rhs) const {
  return referencedType->equivalentTo(rhs);
}

bool ReferenceType::operator==(Type &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ReferenceType *>(&rhs);
  return false;
}

bool ReferenceType::operator!=(Type &t) const { return !(*this == t); }

bool ReferenceType::operator==(const ReferenceType &rhs) const {
  return *referencedType == *rhs.referencedType;
}

bool ReferenceType::operator!=(const ReferenceType &rhs) const {
  return !(*this == rhs);
}
