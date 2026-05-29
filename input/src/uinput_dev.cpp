#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <unordered_map>
#include <vector>

// linux
#include <linux/uinput.h>

// original
#include "uinput_dev.h"


static const std::unordered_map<uint32_t, std::vector<int>>& romajiMap() {
    static const std::unordered_map<uint32_t, std::vector<int>> m = {
        {0x3042, {KEY_A}},
        {0x3044, {KEY_I}},
        {0x3046, {KEY_U}},
        {0x3048, {KEY_E}},
        {0x304A, {KEY_O}},

        {0x304B, {KEY_K, KEY_A}},
        {0x304D, {KEY_K, KEY_I}},
        {0x304F, {KEY_K, KEY_U}},
        {0x3051, {KEY_K, KEY_E}},
        {0x3053, {KEY_K, KEY_O}},

        {0x3055, {KEY_S, KEY_A}},
        {0x3057, {KEY_S, KEY_I}},
        {0x3059, {KEY_S, KEY_U}},
        {0x305B, {KEY_S, KEY_E}},
        {0x305D, {KEY_S, KEY_O}},

        {0x305F, {KEY_T, KEY_A}},
        {0x3061, {KEY_T, KEY_I}},
        {0x3064, {KEY_T, KEY_U}},
        {0x3066, {KEY_T, KEY_E}},
        {0x3068, {KEY_T, KEY_O}},

        {0x306A, {KEY_N, KEY_A}},
        {0x306B, {KEY_N, KEY_I}},
        {0x306C, {KEY_N, KEY_U}},
        {0x306D, {KEY_N, KEY_E}},
        {0x306E, {KEY_N, KEY_O}},

        {0x306F, {KEY_H, KEY_A}},
        {0x3072, {KEY_H, KEY_I}},
        {0x3075, {KEY_H, KEY_U}},
        {0x3078, {KEY_H, KEY_E}},
        {0x307B, {KEY_H, KEY_O}},

        {0x307E, {KEY_M, KEY_A}},
        {0x307F, {KEY_M, KEY_I}},
        {0x3080, {KEY_M, KEY_U}},
        {0x3081, {KEY_M, KEY_E}},
        {0x3082, {KEY_M, KEY_O}},

        {0x3084, {KEY_Y, KEY_A}},
        {0x3086, {KEY_Y, KEY_U}},
        {0x3088, {KEY_Y, KEY_O}},

        {0x3089, {KEY_R, KEY_A}},
        {0x308A, {KEY_R, KEY_I}},
        {0x308B, {KEY_R, KEY_U}},
        {0x308C, {KEY_R, KEY_E}},
        {0x308D, {KEY_R, KEY_O}},

        {0x308F, {KEY_W, KEY_A}},
        {0x3092, {KEY_W, KEY_O}},
        {0x3093, {KEY_N, KEY_N}},
        {0x3094, {KEY_V, KEY_U}},

        {0x3041, {KEY_X, KEY_A}},
        {0x3043, {KEY_X, KEY_I}},
        {0x3045, {KEY_X, KEY_U}},
        {0x3047, {KEY_X, KEY_E}},
        {0x3049, {KEY_X, KEY_O}},
        {0x3083, {KEY_X, KEY_Y, KEY_A}},
        {0x3085, {KEY_X, KEY_Y, KEY_U}},
        {0x3087, {KEY_X, KEY_Y, KEY_O}},
        {0x3063, {KEY_X, KEY_T, KEY_U}},
        {0x308E, {KEY_X, KEY_W, KEY_A}},

        {0x304C, {KEY_G, KEY_A}},
        {0x304E, {KEY_G, KEY_I}},
        {0x3050, {KEY_G, KEY_U}},
        {0x3052, {KEY_G, KEY_E}},
        {0x3054, {KEY_G, KEY_O}},

        {0x3056, {KEY_Z, KEY_A}},
        {0x3058, {KEY_Z, KEY_I}},
        {0x305A, {KEY_Z, KEY_U}},
        {0x305C, {KEY_Z, KEY_E}},
        {0x305E, {KEY_Z, KEY_O}},

        {0x3060, {KEY_D, KEY_A}},
        {0x3062, {KEY_D, KEY_I}},
        {0x3065, {KEY_D, KEY_U}},
        {0x3067, {KEY_D, KEY_E}},
        {0x3069, {KEY_D, KEY_O}},

        {0x3070, {KEY_B, KEY_A}},
        {0x3073, {KEY_B, KEY_I}},
        {0x3076, {KEY_B, KEY_U}},
        {0x3079, {KEY_B, KEY_E}},
        {0x307C, {KEY_B, KEY_O}},

        {0x3071, {KEY_P, KEY_A}},
        {0x3074, {KEY_P, KEY_I}},
        {0x3077, {KEY_P, KEY_U}},
        {0x307A, {KEY_P, KEY_E}},
        {0x307D, {KEY_P, KEY_O}},

        {0x30FC, {KEY_MINUS}},
    };
    return m;
}

void UinputDev::emit(int type, int code, int val) {
    struct input_event ev{};
    ev.type  = type;
    ev.code  = code;
    ev.value = val;
    write(fd_, &ev, sizeof(ev));
}

void UinputDev::emitKey(int code) {
    emit(EV_KEY, code, 1);
    emit(EV_SYN, SYN_REPORT, 0);
    usleep(5000);

    emit(EV_KEY, code, 0);
    emit(EV_SYN, SYN_REPORT, 0);
    usleep(5000);
}

bool UinputDev::sendCharInternal(uint32_t cp) {
    static const int letterKeys[] = {
        KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H,
        KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P,
        KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X,
        KEY_Y, KEY_Z
    };
    static const int numberKeys[] = {
        KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9
    };

    if (cp == 0x0009) {
        emitKey(KEY_TAB);
        return true;
    }

    if (cp == 0x0020 || cp == 0x3000) {
        emitKey(KEY_SPACE);
        return true;
    }

    if (cp >= 0x61 && cp <= 0x7A) {
        emitKey(letterKeys[cp - 0x61]);
        return true;
    }

    if (cp >= 0x30 && cp <= 0x39) {
        emitKey(numberKeys[cp - 0x30]);
        return true;
    }

    auto it = romajiMap().find(cp);
    if (it == romajiMap().end()) return false;
    for (int k : it->second) {
        emitKey(k);
    }
    return true;
}

bool UinputDev::init() {
    fd_ = ::open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd_ < 0) {
        perror("uinput: open");
        return false;
    }

    ::ioctl(fd_, UI_SET_EVBIT, EV_KEY);
    ::ioctl(fd_, UI_SET_EVBIT, EV_SYN);

    const int keys[] = {
        KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H,
        KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P,
        KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X,
        KEY_Y, KEY_Z,
        KEY_0, KEY_1, KEY_2, KEY_3, KEY_4,
        KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
        KEY_BACKSPACE, KEY_MINUS, KEY_SPACE, KEY_TAB,
        KEY_LEFTCTRL, KEY_V, KEY_GRAVE,
    };
    for (int k : keys) {
        ::ioctl(fd_, UI_SET_KEYBIT, k);
    }

    struct uinput_user_dev uidev = {};
    std::snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "LinFlick");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor  = 0x1234;
    uidev.id.product = 0x5678;
    uidev.id.version = 1;

    ::write(fd_, &uidev, sizeof(uidev));
    ::ioctl(fd_, UI_DEV_CREATE);

    return true;
}

void UinputDev::close() {
    if (fd_ >= 0) {
        ::ioctl(fd_, UI_DEV_DESTROY);
        ::close(fd_);
        fd_ = -1;
    }
}

bool UinputDev::sendChar(uint32_t cp) {
    if (fd_ < 0) return false;
    if (cp == 0x0008) {
        emitKey(KEY_BACKSPACE);
        return true;
    }
    return sendCharInternal(cp);
}

void UinputDev::sendBackspace() {
    if (fd_ < 0) return;
    emitKey(KEY_BACKSPACE);
}

void UinputDev::toggleIME() {
    if (fd_ < 0) return;
    emit(EV_MSC, MSC_SCAN, 0x00070035);
    emit(EV_KEY, KEY_GRAVE, 1);
    emit(EV_SYN, SYN_REPORT, 0);
    usleep(5000);
    emit(EV_MSC, MSC_SCAN, 0x00070035);
    emit(EV_KEY, KEY_GRAVE, 0);
    emit(EV_SYN, SYN_REPORT, 0);
    usleep(5000);
}

void UinputDev::sendPaste() {
    if (fd_ < 0) return;
    emit(EV_KEY, KEY_LEFTCTRL, 1);
    emit(EV_SYN, SYN_REPORT, 0);
    usleep(5000);
    emit(EV_KEY, KEY_V, 1);
    emit(EV_SYN, SYN_REPORT, 0);
    usleep(5000);
    emit(EV_KEY, KEY_V, 0);
    emit(EV_SYN, SYN_REPORT, 0);
    usleep(5000);
    emit(EV_KEY, KEY_LEFTCTRL, 0);
    emit(EV_SYN, SYN_REPORT, 0);
    usleep(5000);
}
