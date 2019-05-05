#include "gtest/gtest.h"

#include "Error.h"
#include "Parser.h"
#include "SourceHandler.h"
#include "SourceToken.h"

using namespace ACC;

// atl::string test_prefix =
// "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/test/tests/";
atl::string test_prefix = "../../test/tests/";

TEST(ParserTest, AllTokens) {
  const atl::string filepath = test_prefix + "lexer/alltokens.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  try {
    parser.getAST();
  } catch (ACC::Error const &err) {
    ASSERT_TRUE(true);
    return;
  } catch (...) {
    printf("Expected an ACC::Error\n");
    ASSERT_TRUE(false);
    return;
  }
  printf("Expected an ACC::Error, but no exception was thrown.\n");
  ASSERT_TRUE(false);
}

TEST(ParserTest, InvalidIdentifier) {
  const atl::string filepath = test_prefix + "lexer/errors.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  try {
    ACC::Lexer lexer(scanner);
    ACC::Parser parser(lexer);
    parser.getAST();
  } catch (ACC::Error const &err) {
    ASSERT_TRUE(true);
    return;
  } catch (...) {
    printf("Expected an ACC::Error\n");
    ASSERT_TRUE(false);
    return;
  }
  printf("Expected an ACC::Error, but no exception was thrown.\n");
  ASSERT_TRUE(false);
}

TEST(ParserTest, NestedComments) {
  const atl::string filepath = test_prefix + "lexer/nested_comments.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> p = parser.getAST();

  ASSERT_TRUE(true);
}

TEST(ParserTest, StructDecl) {
  const atl::string filepath = test_prefix + "parser/structdecl.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> actual = parser.getAST();

  atl::vector<atl::shared_ptr<Decl>> expectedDecls = {
      atl::make_shared<StructTypeDecl>(StructTypeDecl(
          atl::make_shared<StructType>(StructType(
              atl::make_shared<Identifier>(Identifier("FooStruct")))),
          {atl::make_shared<VarDecl>(
              VarDecl(atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                      atl::make_shared<Identifier>(Identifier("fooInt"))))})),
      atl::make_shared<VarDecl>(
          VarDecl(atl::make_shared<StructType>(StructType(
                      atl::make_shared<Identifier>(Identifier("FooStruct")))),
                  atl::make_shared<Identifier>(Identifier("myFooStruct"))))};

  const int actualSize = actual->decls.size();
  const int expectSize = expectedDecls.size();
  ASSERT_EQ(actualSize, expectSize);

  for (unsigned int i = 0; i < expectedDecls.size(); ++i)
    ASSERT_TRUE(*actual->decls[i] == *expectedDecls[i]);
}

TEST(ParserTest, VarDecls) {
  const atl::string filepath = test_prefix + "parser/vardecl.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> actual = parser.getAST();

  atl::vector<atl::shared_ptr<Decl>> expectedDecls = {
      atl::make_shared<StructTypeDecl>(StructTypeDecl(
          atl::make_shared<StructType>(StructType(
              atl::make_shared<Identifier>(Identifier("FooStruct")))),
          {atl::make_shared<VarDecl>(
              VarDecl(atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                      atl::make_shared<Identifier>(Identifier("fooInt"))))})),
      atl::make_shared<VarDecl>(
          VarDecl(atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                  atl::make_shared<Identifier>(Identifier("myInt")))),
      atl::make_shared<VarDecl>(
          VarDecl(atl::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)),
                  atl::make_shared<Identifier>(Identifier("myChar")))),
      atl::make_shared<VarDecl>(VarDecl(
          atl::make_shared<PointerType>(PointerType(
              atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)))),
          atl::make_shared<Identifier>(Identifier("myIntPtr")))),
      atl::make_shared<VarDecl>(VarDecl(
          atl::make_shared<PointerType>(PointerType(
              atl::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)))),
          atl::make_shared<Identifier>(Identifier("myCharPtr")))),
      atl::make_shared<VarDecl>(VarDecl(
          atl::make_shared<PointerType>(PointerType(
              atl::make_shared<BaseType>(BaseType(PrimitiveType::VOID)))),
          atl::make_shared<Identifier>(Identifier("myVoidPtr")))),
      atl::make_shared<VarDecl>(VarDecl(
          atl::make_shared<PointerType>(
              PointerType(atl::make_shared<PointerType>(PointerType(
                  atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)))))),
          atl::make_shared<Identifier>(Identifier("myIntPtrPtr")))),
      atl::make_shared<VarDecl>(VarDecl(
          atl::make_shared<PointerType>(
              PointerType(atl::make_shared<PointerType>(PointerType(
                  atl::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)))))),
          atl::make_shared<Identifier>(Identifier("myCharPtrPtr")))),
      atl::make_shared<VarDecl>(VarDecl(
          atl::make_shared<PointerType>(
              PointerType(atl::make_shared<PointerType>(PointerType(
                  atl::make_shared<BaseType>(BaseType(PrimitiveType::VOID)))))),
          atl::make_shared<Identifier>(Identifier("myVoidPtrPtr")))),
      atl::make_shared<VarDecl>(
          VarDecl(atl::make_shared<ArrayType>(ArrayType(
                      atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                      atl::shared_ptr<IntLiteral>(new IntLiteral("5")))),
                  atl::make_shared<Identifier>(Identifier("myIntArr")))),
      atl::make_shared<VarDecl>(
          VarDecl(atl::make_shared<ArrayType>(ArrayType(
                      atl::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)),
                      atl::shared_ptr<IntLiteral>(new IntLiteral("5")))),
                  atl::make_shared<Identifier>(Identifier("myCharArr")))),
      atl::make_shared<VarDecl>(
          VarDecl(atl::make_shared<StructType>(StructType(
                      atl::make_shared<Identifier>(Identifier("FooStruct")))),
                  atl::make_shared<Identifier>(Identifier("myFooStruct")))),
      atl::make_shared<VarDecl>(VarDecl(
          atl::make_shared<PointerType>(
              PointerType(atl::make_shared<StructType>(StructType(
                  atl::make_shared<Identifier>(Identifier("FooStruct")))))),
          atl::make_shared<Identifier>(Identifier("myFooStructPtr")))),
      atl::make_shared<VarDecl>(VarDecl(
          atl::make_shared<ArrayType>(ArrayType(
              atl::make_shared<StructType>(StructType(
                  atl::make_shared<Identifier>(Identifier("FooStruct")))),
              atl::shared_ptr<IntLiteral>(new IntLiteral("5")))),
          atl::make_shared<Identifier>(Identifier("myFooStructArr"))))};

  ASSERT_EQ(actual->decls.size(), expectedDecls.size());

  for (unsigned int i = 0; i < expectedDecls.size(); ++i)
    ASSERT_TRUE(*actual->decls[i] == *expectedDecls[i]);
}

TEST(ParserTest, FunDecl) {
  const atl::string filepath = test_prefix + "parser/fundecl.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> actual = parser.getAST();
  ASSERT_EQ(actual->decls.size(), 2);
  atl::vector<atl::shared_ptr<Decl>> expectedDecls = {
      atl::make_shared<VarDecl>(
          VarDecl(atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                  atl::make_shared<Identifier>(Identifier("myGlobalInt")))),
      atl::shared_ptr<FunDef>(new FunDef(
          atl::set<FunDecl::FunModifiers>(),
          atl::make_shared<Identifier>(Identifier("main")),
          {atl::make_shared<VarDecl>(
               VarDecl(atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                       atl::make_shared<Identifier>(Identifier("argc")))),
           atl::make_shared<VarDecl>(VarDecl(
               atl::make_shared<PointerType>(PointerType(
                   atl::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)))),
               atl::make_shared<Identifier>(Identifier("argv"))))},
          atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
          atl::make_shared<Block>(Block({}))))};

  ASSERT_EQ(actual->decls.size(), expectedDecls.size());

  for (unsigned int i = 0; i < expectedDecls.size(); ++i)
    ASSERT_TRUE(*actual->decls[i] == *expectedDecls[i]);
}

TEST(ParserTest, BinOp) {
  const atl::string filepath = test_prefix + "parser/binop.cpp";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> actual = parser.getAST();
  ASSERT_EQ(actual->decls.size(), 2);
  atl::set<FunDecl::FunModifiers> isalphaModifiers;
  isalphaModifiers.insert(FunDecl::FunModifiers::STATIC);
  atl::vector<atl::shared_ptr<Decl>> expectedDecls = {
      atl::shared_ptr<FunDef>(new FunDef(
          isalphaModifiers, atl::make_shared<Identifier>(Identifier("isalpha")),
          {atl::make_shared<VarDecl>(
              VarDecl(atl::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)),
                      atl::make_shared<Identifier>(Identifier("c"))))},
          atl::make_shared<BaseType>(BaseType(PrimitiveType::BOOL)),
          atl::make_shared<Block>(Block({atl::make_shared<
              Return>(Return(atl::make_shared<ParenthExpr>(
              ParenthExpr(atl::make_shared<BinOp>(BinOp(
                  atl::make_shared<ParenthExpr>(ParenthExpr(atl::make_shared<
                                                            BinOp>(BinOp(
                      atl::make_shared<ParenthExpr>(
                          ParenthExpr(atl::make_shared<BinOp>(BinOp(
                              atl::make_shared<CharLiteral>(CharLiteral("a")),
                              Op::LE,
                              atl::make_shared<ParenthExpr>(
                                  ParenthExpr(atl::make_shared<VarExpr>(
                                      VarExpr(atl::make_shared<Identifier>(
                                          Identifier("c")))))))))),
                      Op::AND,
                      atl::make_shared<ParenthExpr>(
                          ParenthExpr(atl::make_shared<BinOp>(BinOp(
                              atl::make_shared<ParenthExpr>(
                                  ParenthExpr(atl::make_shared<VarExpr>(
                                      VarExpr(atl::make_shared<Identifier>(
                                          Identifier("c")))))),
                              Op::LE,
                              atl::make_shared<CharLiteral>(
                                  CharLiteral("z")))))))))),
                  Op::OR,
                  atl::make_shared<
                      ParenthExpr>(ParenthExpr(atl::make_shared<BinOp>(BinOp(
                      atl::make_shared<ParenthExpr>(
                          ParenthExpr(atl::make_shared<BinOp>(BinOp(
                              atl::make_shared<CharLiteral>(CharLiteral("A")),
                              Op::LE,
                              atl::make_shared<ParenthExpr>(
                                  ParenthExpr(atl::make_shared<VarExpr>(
                                      VarExpr(atl::make_shared<Identifier>(
                                          Identifier("c")))))))))),
                      Op::AND,
                      atl::make_shared<ParenthExpr>(
                          ParenthExpr(atl::make_shared<BinOp>(BinOp(
                              atl::make_shared<ParenthExpr>(
                                  ParenthExpr(atl::make_shared<VarExpr>(
                                      VarExpr(atl::make_shared<Identifier>(
                                          Identifier("c")))))),
                              Op::LE,
                              atl::make_shared<CharLiteral>(
                                  CharLiteral("Z"))))))))))))))))})))),
      atl::shared_ptr<FunDef>(new FunDef(
          atl::set<FunDecl::FunModifiers>(),
          atl::make_shared<Identifier>(Identifier("main")), {},
          atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
          atl::make_shared<Block>(Block(
              {atl::make_shared<VarDef>(VarDef(
                   atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                   atl::make_shared<Identifier>(Identifier("y")),
                   atl::make_shared<IntLiteral>(IntLiteral("5")))),
               atl::make_shared<VarDecl>(VarDecl(
                   atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                   atl::make_shared<Identifier>(Identifier("x")))),
               atl::make_shared<Assign>(Assign(
                   atl::make_shared<VarExpr>(
                       VarExpr(atl::make_shared<Identifier>(Identifier("x")))),
                   atl::make_shared<BinOp>(BinOp(
                       atl::make_shared<IntLiteral>(IntLiteral("1")), Op::ADD,
                       atl::make_shared<IntLiteral>(IntLiteral("2")))))),
               atl::make_shared<Return>(
                   Return(atl::make_shared<VarExpr>(VarExpr(
                       atl::make_shared<Identifier>(Identifier("x"))))))}))))};

  ASSERT_EQ(actual->decls.size(), expectedDecls.size());

  for (unsigned int i = 0; i < expectedDecls.size(); ++i)
    ASSERT_TRUE(*actual->decls[i] == *expectedDecls[i]);
}

TEST(ParserTest, ComplexBinOp) {
  const atl::string filepath = test_prefix + "parser/fundecls.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> actual = parser.getAST();
  /* @TODO Test AST */
}

TEST(ParserTest, InvalidSignatureFunDef) {
  const atl::string filepath = test_prefix + "parser/invalidfundef.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  try {
    parser.getAST();
  } catch (ACC::Error const &err) {
    ASSERT_TRUE(true);
    return;
  } catch (...) {
    printf("Expected a ACC::Error\n");
    ASSERT_TRUE(false);
    return;
  }
  printf("Expected a ACC::Error, but no exception was thrown.\n");
  ASSERT_TRUE(false);
}

TEST(ParserTest, ClassDefinition) {
  const atl::string filepath = test_prefix + "parser/class.cpp";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> actual = parser.getAST();
  ASSERT_TRUE(true);
}

// The fixture for testing class Project1. From google test primer.
class Test_Parser : public ::testing::Test {
protected:
  Test_Parser() {
    // You can do set-up work for each test here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:
  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }
};