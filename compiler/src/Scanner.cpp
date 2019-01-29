#include <fstream>
#include <iostream>

#include "../include/Scanner.h"

using namespace ACC;

Scanner::Scanner(const SourceHandler &src) : column(1), line(1) {
  if (src.type == SourceHandler::Type::FILEPATH) {
    // TEMP: Convert atl::string to std::string so we can use the std::ifstream.
    std::string filename(src.value.c_str());
    std::ifstream fileStream(filename);
    if (!fileStream.good())
      throw std::runtime_error("Scanner: Provided filename \"" + filename +
                               "\" could not be read.");

    file = std::string((std::istreambuf_iterator<char>(fileStream)),
                       std::istreambuf_iterator<char>());
  } else if (src.type == SourceHandler::Type::RAW) {
    // TEMP: Convert atl::string to std::string until atl::string supports
    // iterators.
    const atl::string atlval = src.value;
    file = std::string(atlval.c_str());
  }

  currChar = file.begin();
}

char Scanner::next() {
  char nextChar = *currChar;
  if (currChar == file.end() || nextChar == '\0')
    return '\0';

  if (nextChar == '#') {
    currChar++;
    updateCurrFile();
    return next();
  }

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

SourceHandler Scanner::getFileContents() const {
  const atl::string fileStr(file.c_str());
  return SourceHandler(SourceHandler::Type::RAW, fileStr);
}

std::string Scanner::getFilename() const {
  std::vector<std::string> directories;
  std::string currDir;
  for (const char currChar : abspath) {
    if (currChar == '/') {
      directories.push_back(currDir);
      currDir = "";
    } else {
      currDir += currChar;
    }
  }
  directories.push_back(currDir);

  return directories[directories.size() - 1];
}

std::string Scanner::getFilepath() const {
  std::vector<std::string> directories;
  std::string currDir;
  for (const char currChar : abspath) {
    if (currChar == '/') {
      directories.push_back(currDir);
      currDir = "";
    } else {
      currDir += currChar;
    }
  }
  directories.push_back(currDir);

  std::string filepath;
  for (int i = 0; i < directories.size() - 1; i++)
    filepath += directories[i] + '/';

  return filepath;
}

Position Scanner::getPosition() const {
  return Position(line, column, getFilepath() + getFilename());
}

void Scanner::updateCurrFile() {
  char nextChar = next(); // Skip space.

  nextChar = next();

  std::string lineNumStr(1, nextChar);
  while (!std::isspace(peek())) {
    lineNumStr += next();
  }
  unsigned int lineNum = std::stoul(lineNumStr, nullptr, 10);

  nextChar = next(); // Skip space;
  nextChar = next(); // Skip quote;

  std::string filename;
  if (peek() != '<') {
    while (peek() != '\"') {
      filename += next();
    }
  }
  while (peek() != '\n') {
    nextChar = next();
  }
  abspath = filename;
  line = lineNum;
  column = 1;
}