#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "../include/passes/DotGraph.h"
#include "../include/passes/NameAnalysis.h"
#include "../include/passes/TypeAnalysis.h"

#include "../include/Lexer.h"
#include "../include/Parser.h"
#include "../include/Scanner.h"
#include "../include/targets/GenerateMIPS.h"
#include "../include/targets/GenerateX86.h"

using namespace ACC;

// std::string test_prefix =
    // "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/compiler/test/tests/";
std::string test_prefix = "../../../compiler/test/tests/";

TEST(CodeGenerationTest, MIPS_Fibonacci) {
  // Preprocessor preprocessor(test_prefix + "fibonacci.c");
  // Lexer lexer(scanner);
  // Parser parser(lexer);
  // std::shared_ptr<Program> progAST = parser.getAST();

  // NameAnalysis nameAnalysis(progAST);
  // nameAnalysis.run();
  // ASSERT_EQ(0, nameAnalysis.errorCount);

  // TypeAnalysis typeAnalysis(progAST);
  // typeAnalysis.run();
  // ASSERT_EQ(0, typeAnalysis.errorCount);

  // GenerateMIPS mipsGenerator(progAST, "./fibonacci_mips.asm");
  // mipsGenerator.run();
}

TEST(CodeGenerationTest, X86_fundeclsc) {
  const std::string abspath = (test_prefix + "parser/fundecls.c");
  ACC::Scanner scanner(abspath);
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  std::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);

  TypeAnalysis typeAnalysis(progAST);
  typeAnalysis.run();
  ASSERT_EQ(0, typeAnalysis.errorCount);

  GenerateX86 x86Generator(progAST, "./fibonacci_mips.s");
  x86Generator.run();
}

// The fixture for testing class Project1. From google test primer.
class Test_CodeGeneration : public ::testing::Test {
protected:
  Test_CodeGeneration() {
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