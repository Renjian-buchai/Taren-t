#include "amalgamate.hh"

#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iostream>
#include <regex>
#include <utility>

static amalError validate(std::ifstream& input, char* buffer,
                          size_t& fileVersion, size_t& fileFragments,
                          const size_t fragmentNumber,
                          size_t* gcount = nullptr) {
  try {
    input.getline(buffer, 5, '.');
    fileVersion = boost::lexical_cast<uint64_t>(buffer);
    if (gcount) *gcount = static_cast<size_t>(input.gcount());

    // uint64_t is 20 digits long at max, in decimal
    // log_10(2^64) ~= 19.265
    input.getline(buffer, 20, '.');
    fileFragments = boost::lexical_cast<uint64_t>(buffer);
    if (gcount) *gcount += static_cast<size_t>(input.gcount());
  } catch (boost::bad_lexical_cast& _) {
    std::cerr << _.what() << " in file: " << fragmentNumber << "\n";
    return amalError::BADhEADER;
  }

  return amalError::SUCCESS;
}

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

  size_t version;
  size_t fragments;
  {
    std::ifstream input(matches[0].first);
    char buffer[20];

    if ((validate(input, buffer, version, fragments, matches[0].second)) !=
        amalError::SUCCESS) {
      return amalError::BADhEADER;
    }
  }

  for (size_t i = 0; i < fragments && i < matches.size(); ++i) {
    if (matches[i].second != i + 1) {
      problems.push_back(i + 1);
      i--;
      continue;
    }
  }

  for (size_t i = matches.back().second + 1; i < fragments; ++i) {
    problems.push_back(i);
  }

  if (!problems.empty()) {
    return amalError::FILEcOUNT;
  }

  char* buffer = new char[1'000'001];

  std::ifstream input;
  std::ofstream output(file.string() + ".out",
                       std::ios_base::binary | std::ios_base::out);
  for (size_t i = 0, fileVersion = 0, fileFragments = 0; i < matches.size() - 1;
       ++i) {
    auto [path, number] = matches[i];
    input.open(path, std::ios_base::binary | std::ios_base::in);
    char gayBuffer[20] = {0};

    // Checks for header consistency whilst also removing the header
    if (validate(input, gayBuffer, fileVersion, fileFragments,
                 matches[i].second) != amalError::SUCCESS) {
      return amalError::BADhEADER;
    }

    if (fileVersion != version && fileFragments != fragments) {
      std::cout << "Headers inconsistent between files: \n"
                   "  "
                << matches[0].first.filename()
                << ",\n"
                   "  "
                << matches[i].first.filename() << "\n";
      return amalError::INCONSISTENThEADERS;
    }

    // input.read(buffer, 1);

    for (size_t j = 0; j < 25; ++j) {
      input.read(buffer, 1'000'000);
      output.write(buffer, 1'000'000);
    }
    input.close();
  }

  /** Bug
   * For some reason, it seems that the file keeps looping every 1,000,000
   * bytes? Idk
   */
  input.open(matches.back().first);

  char headerBuffer[21] = {0};

  size_t gcount;
  if (validate(input, headerBuffer, version, fragments, matches.back().second,
               &gcount) != amalError::SUCCESS) {
    return amalError::INCONSISTENThEADERS;
  }

  input.seekg(std::ios_base::beg, std::ios_base::end);
  size_t length = static_cast<size_t>(input.tellg());
  input.seekg(std::ios_base::beg);
  input.read(buffer, static_cast<std::streamsize>(gcount));
  for (size_t i = 0; i < (length) / 1'000'000; ++i) {
    input.read(buffer, 1'000'000);
    output.write(buffer, 1'000'000);
  }

  input.read(buffer, static_cast<std::streamsize>((length) % 1'000'000));
  output.write(buffer, static_cast<std::streamsize>((length) % 1'000'000));
  input.close();
  /** End bug */

  delete[] buffer;

  return amalError::SUCCESS;
}
