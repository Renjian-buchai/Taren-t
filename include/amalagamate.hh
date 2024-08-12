#if !defined(TARENT_AMALGAM_HH)
#define TARENT_AMALGAM_HH

#include <cstdint>
#include <filesystem>
#include <vector>

enum class amalError : int8_t {
  NOsUCHtARENTS = -1,
  SUCCESS = 0,
  BADhEADER,
};

amalError amalgamate(const std::filesystem::path& filename,
                     const std::vector<bool>& flags,
                     std::vector<size_t>& problematicFiles);

#endif