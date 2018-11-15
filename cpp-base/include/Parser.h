#pragma once

#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <string>
#include <vector>

#include "AST.h"
#include "Lexer.h"
#include "Token.h"

namespace ACC {

class Parser {

public:
  Parser(const Lexer &lexer);

  /* Parses the Tokens into an AST with root node Program. */
  Program parse();

private:
  Token currToken;
  std::vector<Token> tokenBuffer;
  Lexer lexer;

  /* Check if we can expect the provided Token to be next. */
  bool accept(Token::TokenClass expected);

  /* Check if we can expect one of the provided Tokens to be next. */
  bool accept(std::vector<Token::TokenClass> expected);

  /* Check if we can expect a Type. */
  bool acceptType();

  /* Expect the next Token to be of a certain type. Throws if not. */
  Token expect(Token::TokenClass expected);

  /* Expect the next Token to be one of a list of types. Throws if not. */
  Token expect(std::vector<Token::TokenClass> expected);

  /* Looks ahead 'i' number of Tokens. */
  Token lookAhead(int i);

  /* Populates currToken with the next unconsumed Token. */
  void nextToken();

  /* Parsing */

  std::shared_ptr<Type> expectType();
  std::vector<std::shared_ptr<VarDecl>> expectParams();
  std::shared_ptr<VarDecl> expectVarDecl();

  void parseIncludes();
  Program parseProgram();
  std::vector<std::shared_ptr<FunDecl>> parseFunDecls();
  std::vector<std::shared_ptr<StructTypeDecl>> parseStructDecls();
  std::vector<std::shared_ptr<VarDecl>> parseVarDecls();

  /* @TODO Parse the rest of the AST. */

  /* Helpers */

  /* Converts an Token to an Type */
  std::shared_ptr<BaseType> tokenToType(const Token::TokenClass &tc);
};

}; // namespace ACC

#endif