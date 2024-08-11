#if !defined(TARENT_SPLIT_HH)
#define TARENT_SPLIT_HH

#include <cstdint>
#include <filesystem>
#include <vector>

enum splitError : int8_t {
  ACCESS = -2,
  DNE = -1,
  SUCCESS = 0,
  EXISTS,
};

splitError split(const std::filesystem::path& activeFile,
                 const std::vector<bool>& flags);

#endif