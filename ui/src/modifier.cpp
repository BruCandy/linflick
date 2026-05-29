#include <map>
#include <cstdint>

// original
#include "modifier.h"


static uint32_t lookupNext(uint32_t cp, const std::map<uint32_t, uint32_t>& tbl) {
    auto it = tbl.find(cp);
    return (it != tbl.end()) ? it->second : cp;
}

uint32_t modifyHIRAGANA(uint32_t cp) {
    static const std::map<uint32_t, uint32_t> tbl = {
        {U'あ', U'ぁ'}, {U'ぁ', U'あ'},
        {U'い', U'ぃ'}, {U'ぃ', U'い'},
        {U'う', U'ぅ'}, {U'ぅ', U'ゔ'}, {U'ゔ', U'う'},
        {U'え', U'ぇ'}, {U'ぇ', U'え'},
        {U'お', U'ぉ'}, {U'ぉ', U'お'},

        {U'か', U'が'}, {U'が', U'か'},
        {U'き', U'ぎ'}, {U'ぎ', U'き'},
        {U'く', U'ぐ'}, {U'ぐ', U'く'},
        {U'け', U'げ'}, {U'げ', U'け'},
        {U'こ', U'ご'}, {U'ご', U'こ'},

        {U'さ', U'ざ'}, {U'ざ', U'さ'},
        {U'し', U'じ'}, {U'じ', U'し'},
        {U'す', U'ず'}, {U'ず', U'す'},
        {U'せ', U'ぜ'}, {U'ぜ', U'せ'},
        {U'そ', U'ぞ'}, {U'ぞ', U'そ'},

        {U'た', U'だ'}, {U'だ', U'た'},
        {U'ち', U'ぢ'}, {U'ぢ', U'ち'},
        {U'つ', U'っ'}, {U'っ', U'づ'}, {U'づ', U'つ'},
        {U'て', U'で'}, {U'で', U'て'},
        {U'と', U'ど'}, {U'ど', U'と'},

        {U'は', U'ば'}, {U'ば', U'ぱ'}, {U'ぱ', U'は'},
        {U'ひ', U'び'}, {U'び', U'ぴ'}, {U'ぴ', U'ひ'},
        {U'ふ', U'ぶ'}, {U'ぶ', U'ぷ'}, {U'ぷ', U'ふ'},
        {U'へ', U'べ'}, {U'べ', U'ぺ'}, {U'ぺ', U'へ'},
        {U'ほ', U'ぼ'}, {U'ぼ', U'ぽ'}, {U'ぽ', U'ほ'},

        {U'や', U'ゃ'}, {U'ゃ', U'や'},
        {U'ゆ', U'ゅ'}, {U'ゅ', U'ゆ'},
        {U'よ', U'ょ'}, {U'ょ', U'よ'},

        {U'わ', U'ゎ'}, {U'ゎ', U'わ'},
    };
    return lookupNext(cp, tbl);
}

uint32_t modifyABC(uint32_t cp) {
    static const std::map<uint32_t, uint32_t> tbl = {
        {U'a',U'A'},{U'A',U'a'}, {U'b',U'B'},{U'B',U'b'},
        {U'c',U'C'},{U'C',U'c'}, {U'd',U'D'},{U'D',U'd'},
        {U'e',U'E'},{U'E',U'e'}, {U'f',U'F'},{U'F',U'f'},
        {U'g',U'G'},{U'G',U'g'}, {U'h',U'H'},{U'H',U'h'},
        {U'i',U'I'},{U'I',U'i'}, {U'j',U'J'},{U'J',U'j'},
        {U'k',U'K'},{U'K',U'k'}, {U'l',U'L'},{U'L',U'l'},
        {U'm',U'M'},{U'M',U'm'}, {U'n',U'N'},{U'N',U'n'},
        {U'o',U'O'},{U'O',U'o'}, {U'p',U'P'},{U'P',U'p'},
        {U'q',U'Q'},{U'Q',U'q'}, {U'r',U'R'},{U'R',U'r'},
        {U's',U'S'},{U'S',U's'}, {U't',U'T'},{U'T',U't'},
        {U'u',U'U'},{U'U',U'u'}, {U'v',U'V'},{U'V',U'v'},
        {U'w',U'W'},{U'W',U'w'}, {U'x',U'X'},{U'X',U'x'},
        {U'y',U'Y'},{U'Y',U'y'}, {U'z',U'Z'},{U'Z',U'z'},
    };
    return lookupNext(cp, tbl);
}
