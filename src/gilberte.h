#pragma once

#define WINDOW_WIDTH        1024
#define WINDOW_HEIGHT       768

#define TARGET_FRAME_RATE   24  // 24 feels so much better when I listen to this: https://youtu.be/mPYBnlMafS0

#define VIEWPORT_WIDTH      1
#define VIEWPORT_HEIGHT     1
#define VIEWPORT_LENGTH     1

#define COLOR_BLACK         0x00, 0x00, 0x00
#define COLOR_WHITE         0xFF, 0xFF, 0xFF

#define COLOR_BACKGROUND    0x34, 0x11, 0x3F
#define COLOR_FOREGROUND    0xBE, 0xB7, 0xDF

typedef enum {
    kErrorOk,
    kErrorInitialization
} EnumError;
