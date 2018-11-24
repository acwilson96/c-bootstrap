#include <fstream>

#include "../include/Scanner.h"

using namespace ACC;

Scanner::Scanner(const std::string &filename) : column(1), line(1) {
  std::ifstream t(filename);
  file = std::string((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
  currChar = file.begin();
}

char Scanner::next() {
  char nextChar = *currChar;
  if (currChar == file.end() || nextChar == '\0')
    return '\0';

  if (nextChar == '\n' || nextChar == '\r') {
    line++;
    column = 1;
  } else {
    column++;
  }
  currChar++;
  return nextChar;
}

char Scanner::peek() {
  if (currChar == file.end())
    return '\0';
  return *currChar;
}