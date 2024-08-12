#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iostream>
#include <regex>
#include <utility>

#include "amalagamate.hh"

amalError amalgamate(const std::filesystem::path& file,
                     const std::vector<bool>& flags) {
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
      } catch (boost::bad_lexical_cast& _ [[maybe_unused]]) {
        std::cout << _.what() << "\n";
        continue;
      }
    }
  }

  if (matches.empty()) {
    return amalError::NOSUCHTARENTS;
  }

  std::ifstream input(matches[0].first);
  std::string buffer;
  std::getline(input, buffer, '.');
  size_t version = std::stoull(buffer);

  std::getline(input, buffer, '.');
  size_t fragments = std::stoull(buffer);

  std::cout << "v" << version << ", " << fragments << " parts\n";

  std::sort(matches.begin(), matches.end(), [](auto thing1, auto thing2) {
    return thing1.second < thing2.second;
  });

  for (auto [it, jt] : matches) {
    std::cout << it << " " << jt << "\n";
  }

  return amalError::SUCCESS;
}
