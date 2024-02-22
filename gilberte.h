#pragma once

#define GAME_NAME				L"Gilberte"

#define GAME_RESOLUTION_WIDTH	640
#define GAME_RESOLUTION_HEIGHT	480
#define GAME_BITS_PER_PIXEL		32
#define FRAME_BUFFER_BYTES		(GAME_RESOLUTION_WIDTH * GAME_RESOLUTION_HEIGHT * (GAME_BITS_PER_PIXEL / 8))

#ifndef UNICODE
#define UNICODE
#define _UNICODE
#endif // !UNICODE

#include <Windows.h>

typedef struct FRAMEBUFFER {
	BITMAPINFO bitmap_info;
	VOID* buffer;
} FRAMEBUFFER;

typedef struct PIXEL32 {
	UINT8 blue;
	UINT8 green;
	UINT8 red;
	UINT8 alpha;
} PIXEL32;

LRESULT CALLBACK WindowProc(_In_ HWND, _In_ UINT, _In_ WPARAM, _In_ LPARAM);

DWORD CreateGameWindow(VOID);

/* Set to fullscreen if `set_fullscreen` is true.
   Exit fullscreen otherwise (i.e. make window float) and set window resolution to GAME_RESOLUTION_WIDTH x GAME_RESOLUTION_HEIGHT. */
DWORD SetFullscreen(BOOL set_fullscreen);

/* Simple sanity check using Mutex. */
BOOL GameIsAlreadyRunning(VOID);

VOID RenderFrame(VOID);