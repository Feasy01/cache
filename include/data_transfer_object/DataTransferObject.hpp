#include <cstdint>

struct OBMessageDTO{
    uint8_t version;
    char type[4];
    char base[16];
    char quote[16];
    char exchange[16];
    uint64_t timestamp;
    uint64_t payload_size;
    char payload[1024];
};


struct Key{
    char type[4];
    char base[16];
    char quote[16];
    char exchange[16];
};

struct OBMessageDTO2{
    uint8_t version;
    Key key;
    uint64_t timestamp;
    uint64_t payload_size;
    char payload[1024];
};

