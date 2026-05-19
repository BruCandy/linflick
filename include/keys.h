#ifndef KEYS_H
#define KEYS_H


static const int KEY_W           = 80;
static const int KEY_H           = 55;
static const int COLS            = 5;
static const int ROWS            = 4;
static const int FLICK_THRESHOLD = 20;
static const int TOP_OFFSET      = 45;

inline const char* const ACT_BACKSPACE  = "\x01";
inline const char* const ACT_DAKUTEN    = "\x02";
inline const char* const ACT_HANDAKUTEN = "\x03";
inline const char* const ACT_SMALL      = "\x04";
inline const char* const ACT_SPACE      = "\x05";
inline const char* const ACT_MODE_HIRA  = "\x06";
inline const char* const ACT_MODE_ABC   = "\x07";
inline const char* const ACT_MODE_123   = "\x08";
inline const char* const ACT_ENTER      = "\x09";
inline const char* const ACT_WINDOW     = "\x0a";

enum FlickDir {
    CENTER  = 0,
    LEFT    = 1,
    UP      = 2,
    RIGHT   = 3,
    DOWN    = 4,
};

enum KeyMode {
    MODE_HIRAGANA = 0,
    MODE_ABC      = 1,
    MODE_123      = 2,
};

enum KeyType {
    NORMAL    = 0,
    MODE      = 1,
    ENTER     = 2,
    SPACE     = 3,
    BACKSPACE = 4,
    WINDOW    = 5,
};

struct KeyDef {
    const char*   label;
    const char*   chars[5];
    const int     font_size;
    const KeyType type;
};

inline const KeyDef KEYS_HIRAGANA[ROWS][COLS] = {
    {
        {"☆123", {ACT_MODE_123, nullptr, nullptr, nullptr, nullptr}, 13, MODE},
        {"あ", {"あ", "い", "う", "え", "お"}, 20, NORMAL},
        {"か", {"か", "き", "く", "け", "こ"}, 20, NORMAL},
        {"さ", {"さ", "し", "す", "せ", "そ"}, 20, NORMAL},
        {"⌫", {ACT_BACKSPACE, nullptr, nullptr, nullptr, nullptr}, 13, BACKSPACE},
    },
    {
        {"ABC", {ACT_MODE_ABC, nullptr, nullptr, nullptr, nullptr}, 13, MODE},
        {"た", {"た", "ち", "つ", "て", "と"}, 20, NORMAL},
        {"な", {"な", "に", "ぬ", "ね", "の"}, 20, NORMAL},
        {"は", {"は", "ひ", "ふ", "へ", "ほ"}, 20, NORMAL},
        {"空白", {ACT_SPACE, nullptr, nullptr, nullptr, nullptr}, 13, SPACE},
    },
    {
        {"あいう", {ACT_MODE_HIRA, nullptr, nullptr, nullptr, nullptr}, 13, MODE},
        {"ま", {"ま", "み", "む", "め", "も"}, 20, NORMAL},
        {"や", {"や", "ゆ", nullptr, "よ", nullptr}, 20, NORMAL},
        {"ら", {"ら", "り", "る", "れ", "ろ"}, 20, NORMAL},
        {"→", {ACT_ENTER, nullptr, nullptr, nullptr, nullptr}, 20, ENTER},
    },
    {
        {"◆", {ACT_WINDOW, nullptr, nullptr, nullptr, nullptr}, 13, WINDOW},
        {"^_^", {ACT_DAKUTEN, ACT_HANDAKUTEN, ACT_SMALL, "ー", "っ"}, 13, NORMAL},
        {"わ", {"わ", "を", "ん", "ー", nullptr}, 20, NORMAL},
        {"、。?!", {"、", "。", "？", "！", nullptr}, 13, NORMAL},
        {"→", {ACT_ENTER, nullptr, nullptr, nullptr, nullptr}, 20, ENTER},
    }
};

inline const KeyDef KEYS_ABC[ROWS][COLS] = {
    {
        {"☆123", {ACT_MODE_123, nullptr, nullptr, nullptr, nullptr}, 13, MODE},
        {"@ # / & _", {"@", "#", "/", "&", "_"}, 13, NORMAL},
        {"A B C", {"a", "b", "c", nullptr, nullptr}, 13, NORMAL},
        {"D E F", {"d", "e", "f", nullptr, nullptr}, 13, NORMAL},
        {"⌫", {ACT_BACKSPACE, nullptr, nullptr, nullptr, nullptr}, 13, BACKSPACE},
    },
    {
        {"ABC", {ACT_MODE_ABC, nullptr, nullptr, nullptr, nullptr}, 13, MODE},
        {"G H I", {"g", "h", "i", nullptr, nullptr}, 13, NORMAL},
        {"J K L", {"j", "k", "l", nullptr, nullptr}, 13, NORMAL},
        {"M N O", {"m", "n", "o", nullptr, nullptr}, 13, NORMAL},
        {"空白", {ACT_SPACE, nullptr, nullptr, nullptr, nullptr}, 13, SPACE},
    },
    {
        {"あいう", {ACT_MODE_HIRA, nullptr, nullptr, nullptr, nullptr}, 13, MODE},
        {"P Q R S", {"p", "q", "r", "s", nullptr}, 13, NORMAL},
        {"T U V", {"t", "u", "v", nullptr, nullptr}, 13, NORMAL},
        {"W X Y Z", {"w", "x", "y", "z", nullptr}, 13, NORMAL},
        {"→", {ACT_ENTER, nullptr, nullptr, nullptr, nullptr}, 20, ENTER},
    },
    {
        {"◆", {ACT_WINDOW, nullptr, nullptr, nullptr, nullptr}, 13, WINDOW},
        {"a / A", {nullptr, nullptr, nullptr, nullptr, nullptr}, 13, NORMAL},
        {"' \" ( )", {"'", "\"", "(", ")", nullptr}, 13, NORMAL},
        {". , ? !", {".", ",", "?", "!", nullptr}, 13, NORMAL},
        {"→", {ACT_ENTER, nullptr, nullptr, nullptr, nullptr}, 20, ENTER},
    }
};

inline const KeyDef KEYS_123[ROWS][COLS] = {
    {
        {"☆123", {ACT_MODE_123, nullptr, nullptr, nullptr, nullptr}, 13, MODE},
        {"1", {"1", "☆", "♪", "→", nullptr}, 20, NORMAL},
        {"2", {"2", "￥", "$", "€", nullptr}, 20, NORMAL},
        {"3", {"3", "%", "゜", "#", nullptr}, 20, NORMAL},
        {"⌫", {ACT_BACKSPACE, nullptr, nullptr, nullptr, nullptr}, 13, BACKSPACE},
    },
    {
        {"ABC", {ACT_MODE_ABC, nullptr, nullptr, nullptr, nullptr}, 13, MODE},
        {"4", {"4", "○", "*", "・", nullptr}, 20, NORMAL},
        {"5", {"5", "+", "×", "÷", nullptr}, 20, NORMAL},
        {"6", {"6", "<", "=", ">", nullptr}, 20, NORMAL},
        {"空白", {ACT_SPACE, nullptr, nullptr, nullptr, nullptr}, 13, SPACE},
    },
    {
        {"あいう", {ACT_MODE_HIRA, nullptr, nullptr, nullptr, nullptr}, 13, MODE},
        {"7", {"7", "「", "」", ":", nullptr}, 20, NORMAL},
        {"8", {"8", "〒", "々", "〆", nullptr}, 20, NORMAL},
        {"9", {"9", "^", "|", "\\", nullptr}, 20, NORMAL},
        {"→", {ACT_ENTER, nullptr, nullptr, nullptr, nullptr}, 20, ENTER},
    },
    {
        {"◆", {ACT_WINDOW, nullptr, nullptr, nullptr, nullptr}, 13, WINDOW},
        {"()[]", {"(", ")", "[", "]", nullptr}, 13, NORMAL},
        {"0", {"0", "~", "...", nullptr, nullptr}, 20, NORMAL},
        {".,-/", {".", ",", "-", "/", nullptr}, 13, NORMAL},
        {"→", {ACT_ENTER, nullptr, nullptr, nullptr, nullptr}, 20, ENTER},
    }
};

using KeyLayout = KeyDef[ROWS][COLS];

inline const KeyLayout& keysForMode(KeyMode mode) {
    switch (mode) {
        case MODE_ABC:
            return KEYS_ABC;
        case MODE_123:
            return KEYS_123;
        default:
            return KEYS_HIRAGANA;
    }
}

#endif
