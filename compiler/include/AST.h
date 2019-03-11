#pragma once

#ifndef ACC_AST_H
#define ACC_AST_H

#include "ast/AddressOf.h"
#include "ast/Allocation.h"
#include "ast/ArrayAccess.h"
#include "ast/ArrayType.h"
#include "ast/Assign.h"
#include "ast/BaseType.h"
#include "ast/BinOp.h"
#include "ast/Block.h"
#include "ast/CharLiteral.h"
#include "ast/ClassType.h"
#include "ast/ClassTypeDecl.h"
#include "ast/ConstructorDecl.h"
#include "ast/ConstructorDef.h"
#include "ast/Decl.h"
#include "ast/DestructorDecl.h"
#include "ast/DestructorDef.h"
#include "ast/DoWhile.h"
#include "ast/EnumTypeDecl.h"
#include "ast/Expr.h"
#include "ast/For.h"
#include "ast/FunCall.h"
#include "ast/FunDecl.h"
#include "ast/FunDef.h"
#include "ast/If.h"
#include "ast/IntLiteral.h"
#include "ast/MemberAccess.h"
#include "ast/MemberCall.h"
#include "ast/Namespace.h"
#include "ast/Op.h"
#include "ast/ParenthExpr.h"
#include "ast/PointerType.h"
#include "ast/PrefixInc.h"
#include "ast/PrimitiveType.h"
#include "ast/Program.h"
#include "ast/ReferenceType.h"
#include "ast/Return.h"
#include "ast/SizeOf.h"
#include "ast/Stmt.h"
#include "ast/StringLiteral.h"
#include "ast/StructType.h"
#include "ast/StructTypeDecl.h"
#include "ast/Type.h"
#include "ast/TypeCast.h"
#include "ast/TypeDefDecl.h"
#include "ast/ValueAt.h"
#include "ast/VarDecl.h"
#include "ast/VarDef.h"
#include "ast/VarExpr.h"
#include "ast/While.h"

#endif