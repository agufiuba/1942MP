#include "palette.h"

std::string warning(std::string msg) {
  return "\033[1;31m" + msg + "\033[0m";
}

std::string notice(std::string msg) {
  return "\033[1;32m" + msg + "\033[0m";
}
