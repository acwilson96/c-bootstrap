#include <iostream>
#include <stdexcept>

#include "../include/Lexer.h"
#include "../include/Token.h"

using namespace ACC;

Lexer::Lexer(Scanner &scanner) : scanner(scanner) {}

Token Lexer::nextToken() {
  // Get the next Char.
  char c = scanner.next();

  // Find EOF.
  if (c == '\0')
    return Token(Token::TokenClass::ENDOFFILE, scanner.getPosition());

  // Skip through Comments.
  if (c == '/' && (scanner.peek() == '*' || scanner.peek() == '/')) {
    passComment();
    return nextToken();
  }

  /* Parse Multi Symbol Tokens */

  // Recognise STRING_LITERAL Token.
  if (c == '"')
    return lexStringLiteral();

  // Recognise CHAR_LITERAL token.
  if (c == '\'') {
    c = scanner.next();
    if (c == '\\') {
      char peek = scanner.peek();
      // Valid Escapes: '\t' | '\b' | '\n' | '\r' | '\f' | '\'' | '\"' | '\\'
      if (peek == 't' || peek == 'b' || peek == 'n' || peek == 'r' ||
          peek == 'f' || peek == '\'' || peek == '"' || peek == '\\') {
        c = scanner.next();
        if (scanner.peek() == '\'') {
          char val = c;
          scanner.next();
          return Token(Token::TokenClass::CHAR_LITERAL, scanner.getPosition(),
                       std::to_string('\\' + val));
        }
      }
    }
    // Otherwise we have a normal char.
    else if (scanner.peek() == '\'') {
      char val = c;
      scanner.next();
      return Token(Token::TokenClass::CHAR_LITERAL, scanner.getPosition(),
                   std::string(1, val));
    }
  }
  // Recognise IDENTIFIERS & Keyword Tokens.
  if (isalpha(c) || c == '_') {
    std::string literal(1, c);
    // Check for CHAR Token.
    if (c == 'c' && scanner.peek() == 'h') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("char");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::CHAR, scanner.getPosition());
    }
    // Check for CONST Token.
    if (c == 'c' && scanner.peek() == 'o') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("const");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::CONST, scanner.getPosition());
    }
    // Check for DO Token.
    if (c == 'd' && scanner.peek() == 'o') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("do");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::DO, scanner.getPosition());
    }
    // Check for ELSE Token.
    else if (c == 'e' && scanner.peek() == 'l') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("else");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::ELSE, scanner.getPosition());
    }
    // Check for ENUM Token.
    else if (c == 'e' && scanner.peek() == 'n') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("enum");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::ENUM, scanner.getPosition());
    }
    // Check for EXTERN Token.
    else if (c == 'e' && scanner.peek() == 'x') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("extern");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::EXTERN, scanner.getPosition());
    }
    // Check for IF Token.
    else if (c == 'i' && scanner.peek() == 'f') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("if");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::IF, scanner.getPosition());
    }
    // Check for INT token.
    else if (c == 'i' && scanner.peek() == 'n') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("int");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::INT, scanner.getPosition());
    }
    // Check for RETURN Token.
    else if (c == 'r' && scanner.peek() == 'e') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("return");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::RETURN, scanner.getPosition());
    }
    // Check for SIZEOF Token.
    else if (c == 's' && scanner.peek() == 'h') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("short");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::SHORT, scanner.getPosition());
    }
    // Check for SIZEOF Token.
    else if (c == 's' && scanner.peek() == 'i') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("sizeof");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::SIZEOF, scanner.getPosition());
    }
    // Check for STRUCT Token.
    else if (c == 's' && scanner.peek() == 't') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("struct");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::STRUCT, scanner.getPosition());
    }
    // Check for TYPEDEF Token.
    else if (c == 't' && scanner.peek() == 'y') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("typedef");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::TYPEDEF, scanner.getPosition());
    }
    // Check for UNSIGNED Token.
    else if (c == 'u' && scanner.peek() == 'n') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("unsigned");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::UNSIGNED, scanner.getPosition());
    }
    // Check for WHILE Token.
    else if (c == 'w' && scanner.peek() == 'h') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("while");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::WHILE, scanner.getPosition());
    }
    // Check for VOID Token.
    else if (c == 'v' && scanner.peek() == 'o') {
      std::pair<bool, std::string> lexResult = tryLexKeyword("void");
      literal = lexResult.second;

      if (lexResult.first)
        return Token(Token::TokenClass::VOID, scanner.getPosition());
    }

    // No keyword Token has been returned.
    // Now Lex an IDENTIFIER.
    char peek;
    while (true) {
      peek = scanner.peek();
      // If the next character is whitespace, the IDENTIFIER has been
      // identified.
      if (std::isspace(peek)) {
        return Token(Token::TokenClass::IDENTIFIER, scanner.getPosition(),
                     literal);
      }
      // If the next character is an illegal characater for an IDENTIFIER, we
      // have finished finding the token.
      if (!isalpha(peek) && !isdigit(peek) && peek != '_' && peek != '$') {
        return Token(Token::TokenClass::IDENTIFIER, scanner.getPosition(),
                     literal);
      }
      // We are still Lexing the token.
      c = scanner.next();
      literal += c;
    }
  }
  // Recognise INT_LITERAL Token.
  if (isdigit(c)) {
    std::string literal(1, c);
    if (scanner.peek() == 'x') {
      literal += scanner.next();
      while (std::isalpha(scanner.peek()) || std::isdigit(scanner.peek()))
        literal += scanner.next();
    } else {
      while (isdigit(scanner.peek()))
        literal += scanner.next();
    }

    return Token(Token::TokenClass::INT_LITERAL, scanner.getPosition(),
                 literal);
  }

  /* Recognise Two Symbol Tokens. */
  if (c == '=' && scanner.peek() == '=') {
    scanner.next();
    return Token(Token::TokenClass::EQ, scanner.getPosition());
  }
  if (c == '!' && scanner.peek() == '=') {
    scanner.next();
    return Token(Token::TokenClass::NE, scanner.getPosition());
  }
  if (c == '<' && scanner.peek() == '=') {
    scanner.next();
    return Token(Token::TokenClass::LE, scanner.getPosition());
  }
  if (c == '>' && scanner.peek() == '=') {
    scanner.next();
    return Token(Token::TokenClass::GE, scanner.getPosition());
  }
  if (c == '&' && scanner.peek() == '&') {
    scanner.next();
    return Token(Token::TokenClass::AND, scanner.getPosition());
  }
  if (c == '|' && scanner.peek() == '|') {
    scanner.next();
    return Token(Token::TokenClass::OR, scanner.getPosition());
  }

  /* Recognise One Symbol Tokens. */
  if (c == '=')
    return Token(Token::TokenClass::ASSIGN, scanner.getPosition());
  if (c == '{')
    return Token(Token::TokenClass::LBRA, scanner.getPosition());
  if (c == '}')
    return Token(Token::TokenClass::RBRA, scanner.getPosition());
  if (c == '(')
    return Token(Token::TokenClass::LPAR, scanner.getPosition());
  if (c == ')')
    return Token(Token::TokenClass::RPAR, scanner.getPosition());
  if (c == '[')
    return Token(Token::TokenClass::LSBR, scanner.getPosition());
  if (c == ']')
    return Token(Token::TokenClass::RSBR, scanner.getPosition());
  if (c == ';')
    return Token(Token::TokenClass::SC, scanner.getPosition());
  if (c == ',')
    return Token(Token::TokenClass::COMMA, scanner.getPosition());
  if (c == '+')
    return Token(Token::TokenClass::PLUS, scanner.getPosition());
  if (c == '-')
    return Token(Token::TokenClass::MINUS, scanner.getPosition());
  if (c == '*')
    return Token(Token::TokenClass::ASTERIX, scanner.getPosition());
  if (c == '%')
    return Token(Token::TokenClass::REM, scanner.getPosition());
  if (c == '.')
    return Token(Token::TokenClass::DOT, scanner.getPosition());
  if (c == '/')
    return Token(Token::TokenClass::DIV, scanner.getPosition());
  if (c == '>')
    return Token(Token::TokenClass::GT, scanner.getPosition());
  if (c == '<')
    return Token(Token::TokenClass::LT, scanner.getPosition());
  if (c == '&')
    return Token(Token::TokenClass::REF, scanner.getPosition());

  // Skip Whitespace.
  if (std::isspace(c))
    return nextToken();

  // if we reach this point, it means we did not recognise a valid token
  throw std::runtime_error("Lexer: Unexpected Token. " +
                           scanner.getPosition().toString());
}

Token Lexer::lexStringLiteral() {
  std::string literal;
  int currLine = scanner.getPosition().line;

  char c;
  while (true) {
    c = scanner.next();
    if (c == '\0')
      throw std::runtime_error("Lexer: Unexpected EOF in String Literal. " +
                               scanner.getPosition().toString());
    if (c == '\n')
      throw std::runtime_error(
          "Lexer: Unexpected Newline Character in String Literal. " +
          scanner.getPosition().toString());

    // Check if we are about to see an escaped character.
    if (c == '\\') {
      literal += c;
      c = scanner.next();
      literal += c;
    } else if (c == '"') {
      break;
    } else {
      literal += c;
    }
  }
  return Token(Token::TokenClass::STRING_LITERAL, scanner.getPosition(),
               literal);
}

void Lexer::passComment() {
  // Consume the '/' or '*' character.
  char c = scanner.next();
  int currLine = scanner.getPosition().line;
  if (c == '/') {
    while (scanner.getPosition().line == currLine)
      scanner.next();
    return;
  } else if (c == '*') {
    c = scanner.next();
    while (true) {
      c = scanner.next();
      if (c == '*' && scanner.peek() == '/') {
        scanner.next(); // Consume the closing DIV.
        c = scanner.next();
        char temp = scanner.peek();
        return;
      }
      if (c == '\0')
        break;
    }
  }
  throw std::runtime_error(
      "Lexer: Parsing Comment Returned Unexpected Token(s). " +
      scanner.getPosition().toString());
}

std::pair<bool, std::string> Lexer::tryLexKeyword(const std::string &keyword) {
  bool keywordMatch = false;
  std::string literal(1, keyword[0]);

  for (int i = 1; i < keyword.length(); i++) {
    char peek = scanner.peek();
    if (scanner.peek() != keyword[i])
      return std::pair<bool, std::string>(false, literal);

    literal += scanner.next();
  }

  char peek = scanner.peek();
  if ((literal == keyword) && (!isalpha(peek)) && (!isdigit(peek)) &&
      (peek != '_'))
    keywordMatch = true;

  return std::pair<bool, std::string>(keywordMatch, literal);
}