#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iostream>
#include <regex>
#include <utility>

#include "amalagamate.hh"

amalError amalgamate(const std::filesystem::path& file,
                     const std::vector<bool>& flags,
                     std::vector<size_t>& problems) {
  (void)flags;

  std::vector<std::pair<std::filesystem::path, size_t>> matches;

  std::string result;
  std::string extension;
  size_t index;

  // Find files that match
  for (std::filesystem::directory_entry entry :
       std::filesystem::recursive_directory_iterator(
           std::filesystem::absolute(file).parent_path())) {
    if (!entry.is_regular_file()) {
      continue;
    }

    if (entry.path().stem() != file) {
      continue;
    }

    extension = entry.path().extension().string();

    if (extension.substr(0, 6) == ".taren" && extension.back() == 't') {
      try {
        index = boost::lexical_cast<size_t>(
            extension.substr(6, extension.size() - 7));

        matches.push_back({entry.path(), index});
      } catch (boost::bad_lexical_cast& _) {
        std::cout << _.what() << "\n";
        continue;
      }
    }
  }

  if (matches.empty()) {
    return amalError::NOsUCHtARENTS;
  }

  std::sort(matches.begin(), matches.end(), [](auto thing1, auto thing2) {
    return thing1.second < thing2.second;
  });

  std::ifstream input(matches[0].first);
  std::string buffer;

  size_t version;
  size_t fragments;
  try {
    std::getline(input, buffer, '.');
    version = boost::lexical_cast<uint64_t>(buffer);

    std::getline(input, buffer, '.');
    fragments = boost::lexical_cast<uint64_t>(buffer);
  } catch (boost::bad_lexical_cast& _) {
    std::cerr << _.what() << " in file: " << matches[0].second << "\n";
    return amalError::BADhEADER;
  }

  std::cout << "v" << version << ", " << fragments << " parts\n";

  for (size_t i = 0; i < fragments; ++i) {
    if (matches[i].second != i + 1) {
      problems.push_back(i + 1);
    }
  }

  return amalError::SUCCESS;
}
