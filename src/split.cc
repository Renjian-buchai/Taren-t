#include "split.hh"

#include <bitset>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>

splitError split(const std::filesystem::path& file,
                 const std::vector<bool>& flags) {
  (void)flags;

  if (!std::filesystem::exists(file)) {
    return splitError::DNE;
  }

  std::ifstream input(file, std::ios_base::in | std::ios_base::binary);
  if (!input.is_open()) {
    return splitError::ACCESS;
  }

  std::filesystem::path newName = file.filename();
  char* buffer = new char[1'000'001];
  size_t index = 0;

  size_t length;
  /* Find length of file */ {
    input.seekg(std::ios_base::beg, std::ios_base::end);
    length = static_cast<size_t>(input.tellg());
    input.seekg(std::ios_base::beg);
  }

  size_t max = (length / 25'000'000) + 1;

  for (auto newFile = file.parent_path() /
                      std::filesystem::path(newName.string() + ".taren" +
                                            std::to_string(index + 1) + "t");
       index < max - 1; ++index) {
    if (std::filesystem::exists(newFile)) {
      delete[] buffer;
      return splitError::EXISTS;
    }

    std::ofstream output(newFile, std::ios_base::out | std::ios_base::binary);

    output << "1." << max << ".";

    for (int i = 0; i < 25; ++i) {
      input.read(buffer, 1'000'000);
      output.write(buffer, 1'000'000);
    }
  }

  std::ofstream output(
      file.parent_path() /
          std::filesystem::path(newName.string() + ".taren" +
                                std::to_string(index + 1) + "t"),
      std::ios_base::out | std::ios_base::binary);

  output << "1." << max << ".";

  for (size_t i = 0; i < (length % 25'000'000) / 1'000'000; ++i) {
    input.read(buffer, 1'000'000);
    output.write(buffer, 1'000'000);
  }

  if (length % 1'000'000) {
    input.read(buffer, static_cast<std::streamsize>(length % 1'000'000));
    output.write(buffer, static_cast<std::streamsize>(length % 1'000'000));
  }

  delete[] buffer;
  return splitError::SUCCESS;
}