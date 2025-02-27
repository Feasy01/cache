#include "utils/Lz4Utils.hpp"

namespace Lz4Utils {

std::pair<char*, size_t> compress(const char* input, size_t size) {
    if (size == 0) {
        return std::make_pair(nullptr, 0);
    }

    const int maxCompressedSize = LZ4_compressBound(size);
    
    char* compressed = new char[maxCompressedSize];

    int compressedSize = LZ4_compress_default(
        input,                   
        compressed,              
        static_cast<int>(size),  
        maxCompressedSize             
    );
    
    if (compressedSize <= 0) {
        return std::make_pair(nullptr, 0);
    }
    
    return std::make_pair(compressed, compressedSize);
}

std::string decompress(const std::string& compressed) {
    if (compressed.empty()) {
        return "";
    }
    
    std::vector<char> decompressed(compressed.size() * 4);  
    
    while (true) {
        int decompressedSize = LZ4_decompress_safe(
            compressed.data(),              
            decompressed.data(),           
            static_cast<int>(compressed.size()),  
            static_cast<int>(decompressed.size()) 
        );
        
        if (decompressedSize >= 0) {
            return std::string(decompressed.data(), decompressedSize);
        } else if (decompressedSize == -1) {
            decompressed.resize(decompressed.size() * 2);
            continue;
        } else {
            return "";
        }
    }
}

} // namespace Lz4Utils

