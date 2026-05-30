#ifndef UINPUT_DEV_H
#define UINPUT_DEV_H


#include <cstdint>


class UinputDev {
public:
    bool init();
    void close();

    bool sendChar(uint32_t cp);
    void sendBackspace();
    void sendEnter();
    void sendWindow();
    void sendArrowUp();
    void sendArrowDown();
    void sendPaste();
    void toggleIME();

private:
    int fd_ = -1;

    void emit(int type, int code, int val);
    void emitKey(int code);
    bool sendCharInternal(uint32_t cp);
};

inline UinputDev uinputDev;

#endif
