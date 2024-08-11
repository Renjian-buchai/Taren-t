#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "split.hh"

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
    return 0;
  }

  if (arguments[0] == "v" || arguments[0] == "V") {
    // version
    std::cout << "version";
    return 0;
  }

  if (arguments.size() < 2) {
    std::cout << "Invalid usage.\n"
                 "For more detailed help, please use:\n"
                 "  `tarent h`\n";
    return 1;
  }

  std::filesystem::path path = arguments[1];

  if (arguments[0] == "s" || arguments[0] == "S") {
    switch (split(path, flags)) {
      case splitError::ACCESS:
        std::cerr << "Access denied; Unable to open file.\n"
                     "Please ensure you have read permissions to the file\n";
        break;

      case splitError::DNE:
        std::cerr << "File does not exist.\n"
                     "Please check your spelling\n";
        break;

      case splitError::EXISTS:
        std::cerr << "The last file created already exists.\n"
                     "Please clean up manually\n";
        break;

      case splitError::SUCCESS:
        [[fallthrough]];
      default:
        break;
    }
  }

  if (arguments[0] == "a" || arguments[0] == "A") {
    // amalgamate
    std::cout << "Amalgamate";
  }

  return 0;
}
