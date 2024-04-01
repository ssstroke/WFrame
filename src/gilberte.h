#pragma once

#define WINDOW_WIDTH        640
#define WINDOW_HEIGHT       480
#define TARGET_FRAME_RATE   24  // 24 feels so much better when I listen to this: https://youtu.be/mPYBnlMafS0

#define COLOR_BLACK         0, 0, 0
#define COLOR_WHITE         255, 255, 255

#define COLOR_BACKGROUND    191,  49, 0
#define COLOR_FOREGROUND    142, 166, 4

typedef enum {
    kErrorOk,
    kErrorInitialization
} EnumError;
