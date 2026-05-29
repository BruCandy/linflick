#ifndef UTF8_H
#define UTF8_H


#include <string>
#include <cstdint>


uint32_t utf8ToCp(const std::string& s, size_t pos = 0);

#endif
