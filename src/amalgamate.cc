#include <fstream>
#include <regex>
#include <utility>

#include "amalagamate.hh"

amalError amalgamate(const std::filesystem::path& file,
                     const std::vector<bool>& flags) {
  (void)flags;

  std::vector<std::pair<std::filesystem::path, size_t>> matches;

  std::regex regex(file.string() + R"regex(\.taren(\d+)t)regex");
  std::string result;
  std::string filename;
  std::smatch index;

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

    // Regex can't match temporary strings so fuck
    filename = entry.path().filename().string();
    if (std::regex_match(filename, index, regex)) {
      matches.push_back({entry.path(), std::stoull(index[1])});
    }
  }

  if (matches.empty()) {
    return amalError::NOSUCHTARENTS;
  }

  std::ifstream input(matches[0].first);
  std::string buffer;
  // std::getline(input, buffer, '.');
  // size_t version = std::stoull(buffer);

  // std::getline(input, buffer, '.');
  // size_t fragments = std::stoull(buffer);

  std::sort(matches.begin(), matches.end(), [](auto thing1, auto thing2) {
    return thing1.second < thing2.second;
  });

  return amalError::SUCCESS;
}
