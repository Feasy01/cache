#include "server/Server.hpp"


BaseServer::BaseServer(std::string type, std::string base, std::string quote, std::string exchange) 
    : type(type), base(base), quote(quote), exchange(exchange) {}


constexpr uint32_t BaseServer::hash(uint32_t x) {
    x ^= x >> 16;
    x *= 0x7feb352dU;
    x ^= x >> 15;
    x *= 0x846ca68bU;
    x ^= x >> 16;
    return x;
}

constexpr double BaseServer::compile_time_random(uint32_t seed, double min, double max) {
    return min + (hash(seed) % 1000000) / 1000000.0 * (max - min);
}

OBMessageDTO2 BaseServer::generateRandomMessage(std::string type, std::string base, std::string quote, std::string exchange) {
    OBMessageDTO2 msg;
    msg.version = 1;
    
    Key key_obj{};
    std::strncpy(key_obj.type, type.c_str(), 4);
    std::strncpy(key_obj.base, base.c_str(), 16);
    std::strncpy(key_obj.quote, quote.c_str(), 16);
    std::strncpy(key_obj.exchange, exchange.c_str(), 16);

    memcpy(&msg.key, &key_obj, sizeof(Key));
    
    
    msg.timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
    auto payload_array = BaseServer::generate_compile_time_payloads()[hash(msg.timestamp) % 100];
    auto compressed_payload = Lz4Utils::compress(payload_array.data(), payload_array.size());
    msg.payload_size = compressed_payload.second;
    memcpy(msg.payload, compressed_payload.first, msg.payload_size);

    return msg;
}



constexpr std::array<std::array<char, 1024>, 100> BaseServer::generate_compile_time_payloads() {
    std::array<std::array<char, 1024>, 100> payloads{};
    for (size_t i = 0; i < 100; ++i) {
        std::array<char, 1024> payload{};
        size_t pos = 0;
        
        for (size_t level = 0; level < 20; ++level) {
            if (level > 0) payload[pos++] = ',';
            
            double bid_price = compile_time_random(i * 1000 + level, 100.0, 1000.0);
            pos += BaseServer::sprintf_constexpr(payload.data() + pos, "bid%zu=%.2f", level, bid_price);
            
            payload[pos++] = ',';
            double bid_volume = compile_time_random(i * 2000 + level, 0.1, 10.0);
            pos += BaseServer::sprintf_constexpr(payload.data() + pos, "bidv%zu=%.2f", level, bid_volume);
            
            payload[pos++] = ',';
            double ask_price = bid_price + compile_time_random(i * 3000 + level, 0.1, 5.0);
            pos += BaseServer::sprintf_constexpr(payload.data() + pos, "ask%zu=%.2f", level, ask_price);
            
            payload[pos++] = ',';
            double ask_volume = compile_time_random(i * 4000 + level, 0.1, 10.0);
            pos += BaseServer::sprintf_constexpr(payload.data() + pos, "askv%zu=%.2f", level, ask_volume);
        }
        
        payload[pos] = '\0'; 
        payloads[i] = payload;
    }
    return payloads;
}

constexpr size_t BaseServer::sprintf_constexpr(char* str, const char* format, size_t value, double fvalue) {
    size_t pos = 0;
    
    while (format[pos] && format[pos] != '%') {
        str[pos] = format[pos];
        pos++;
    }
    
    if (format[pos] == '%' && format[pos + 1] == 'z' && format[pos + 2] == 'u') {
        size_t tmp = value;
        char digits[20] = {};
        int digit_pos = 0;
        
        do {
            digits[digit_pos++] = '0' + (tmp % 10);
            tmp /= 10;
        } while (tmp > 0);
        
        while (digit_pos > 0) {
            str[pos++] = digits[--digit_pos];
        }
        pos += sprintf_double(str + pos, fvalue);
    }
    
    return pos;
}

constexpr size_t BaseServer::sprintf_double(char* str, double value) {
    size_t pos = 0;
    
    int integer_part = static_cast<int>(value);
    char digits[20] = {};
    int digit_pos = 0;
    
    do {
        digits[digit_pos++] = '0' + (integer_part % 10);
        integer_part /= 10;
    } while (integer_part > 0);
    
    while (digit_pos > 0) {
        str[pos++] = digits[--digit_pos];
    }
    
    str[pos++] = '.';
    double decimal_part = value - static_cast<int>(value);
    for (int i = 0; i < 2; ++i) {  
        decimal_part *= 10;
        int digit = static_cast<int>(decimal_part);
        str[pos++] = '0' + digit;
        decimal_part -= digit;
    }
    
    return pos;
}