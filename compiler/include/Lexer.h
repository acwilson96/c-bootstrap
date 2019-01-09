#pragma once

#ifndef ACC_TOKENISER_H
#define ACC_TOKENISER_H

#include <string>
#include <utility>

#include "Scanner.h"
#include "Token.h"

namespace ACC {

class Lexer final {

public:
  Lexer(Scanner &src);
  Lexer(Lexer &rhs) = delete;

  /* Parses the next Token out of the Input File. */
  Token nextToken();

private:
  Scanner &scanner;

  Token lexStringLiteral();
  void passComment();
  std::pair<bool, std::string> tryLexKeyword(const std::string &keyword);
};

}; // namespace ACC

#endif