#if !defined(TARENT_AMALGAM_HH)
#define TARENT_AMALGAM_HH

#include <cstdint>
#include <filesystem>
#include <vector>

enum class amalError : int8_t { NOSUCHTARENTS = -1, SUCCESS = 0 };

amalError amalgamate(const std::filesystem::path& filename,
                     const std::vector<bool>& flags);

#endif