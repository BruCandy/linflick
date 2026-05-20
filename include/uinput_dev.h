#ifndef UINPUT_DEV_H
#define UINPUT_DEV_H


#include <cstdint>

bool uinputInit();
void uinputClose();
bool uinputSendChar(uint32_t codepoint);
void uinputSendBackspace();
void uinputSendPaste();
void uinputToggleIME();

#endif
