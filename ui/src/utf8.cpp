// original
#include "utf8.h"


uint32_t utf8ToCp(const std::string& s, size_t pos) {
    if (pos >= s.size()) return 0;
    uint8_t c = (uint8_t)s[pos];
    if (c < 0x80) return c;
    if (c < 0xE0) return ((c & 0x1F) << 6)  | ((uint8_t)s[pos+1] & 0x3F);
    if (c < 0xF0) return ((c & 0x0F) << 12) | (((uint8_t)s[pos+1] & 0x3F) << 6) | ((uint8_t)s[pos+2] & 0x3F);
    return ((c & 0x07) << 18) | (((uint8_t)s[pos+1] & 0x3F) << 12) | (((uint8_t)s[pos+2] & 0x3F) << 6) | ((uint8_t)s[pos+3] & 0x3F);
}
