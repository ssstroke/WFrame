#pragma once

#define WINDOW_WIDTH        640
#define WINDOW_HEIGHT       480
#define TARGET_FRAME_RATE   24  // 24 feels so much better when I listen to this: https://youtu.be/mPYBnlMafS0

typedef enum {
    kErrorOk,
    kErrorInitialization
} EnumError;
