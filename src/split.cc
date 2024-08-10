#include "split.hh"

#include <bitset>
#include <fstream>
#include <ios>
#include <iostream>

splitError split(const std::filesystem::path& file, std::vector<bool>& flags) {
  (void)flags;

  if (!std::filesystem::exists(file)) {
    return splitError::DNE;
  }

  std::ifstream input(file, std::ios_base::in | std::ios_base::binary);
  if (!input.is_open()) {
    return splitError::ACCESS;
  }

  char buffer[1'000'000];
  input.seekg(0, std::ios_base::end);
  size_t length = input.tellg();
  for (size_t i = 0; i < length / 25'000'000; ++i) {
    // Create the new files and run that logic
    // Loop through the bytes 1'000'000 bytes at once, then write to new file
  }

  // Create the last new file, and run that logic
  // Loop thorough the bytes 1'000'000 bytes at once, until the last 1'000'000
  // bytes, then just return

  return splitError::SUCCESS;
}