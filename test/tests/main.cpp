#include "test_string.h"

int main(int argc, char **argv) {
  const char *raw = "World";
  test::string str(raw);
  printf("Hello %s!\n", raw);
  return 23;
}
