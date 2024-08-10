#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

enum class flag {};

int main(int argc, const char** argv, const char** envp [[maybe_unused]]) {
  if (argc == 1) {
    std::cout << "Usage:\n"
                 "  `tarent [h|v|s|a] [options]`\n"
                 "\n"
                 "For more detailed help, please use:\n"
                 "  `tarent h`.\n";
    return 1;
  }

  std::vector<std::string> arguments;
  for (int i = 1; i < argc; ++i) {
    arguments.push_back(std::string(argv[i]));
  }

  std::vector<bool> flags;

  if (arguments[0] == "h" || arguments[0] == "H") {
    // help
    std::cout << "help";
  }

  if (arguments[0] == "v" || arguments[0] == "V") {
    // version
    std::cout << "version";
  }

  if (arguments[0] == "s" || arguments[0] == "S") {
    // split
    std::cout << "Split";
  }

  if (arguments[0] == "a" || arguments[0] == "A") {
    // amalgamate
    std::cout << "Amalgamate";
  }

  return 0;
}
