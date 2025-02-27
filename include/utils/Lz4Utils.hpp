#include "lz4.h"

#include <vector>
#include <string>

namespace Lz4Utils {
    std::pair<char*, size_t> compress(const char* input, size_t size);
    std::string decompress(const std::string& compressed);
}

