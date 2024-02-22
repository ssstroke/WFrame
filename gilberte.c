#include "gilberte.h"

#include <stdio.h>

#include <Windows.h>

BOOL gGameIsRunning = TRUE;
HWND gGameWindow    = NULL;

FRAMEBUFFER gFrameBuffer = { 0 };
UINT32 gPixels[GAME_RESOLUTION_WIDTH * GAME_RESOLUTION_HEIGHT] = { 0 };

MONITORINFO gMonitorInfo = { sizeof(gMonitorInfo) };
LONG gMonitorWidth  = 0;
LONG gMonitorHeight = 0;

/* X and Y in terms of window client area. */
INT gRenderDestX, gRenderDestY = 0;

INT WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR pCmdLine, _In_ INT nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(pCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    if (GameIsAlreadyRunning() == TRUE)
    {
        MessageBox(NULL, L"Another instance of this program is already running!", L"Error!", MB_ICONERROR | MB_OK);
        return 1;
    }

    if (CreateGameWindow() != ERROR_SUCCESS)
    {
        MessageBox(NULL, L"Error occured while creating window!", L"Error!", MB_ICONERROR | MB_OK);
        return 2;
    }

    // At this point window has been successfully created so we may initialize frame buffer.
    gFrameBuffer.bitmap_info.bmiHeader.biSize           = sizeof(gFrameBuffer.bitmap_info.bmiHeader);
    gFrameBuffer.bitmap_info.bmiHeader.biWidth          = GAME_RESOLUTION_WIDTH;
    gFrameBuffer.bitmap_info.bmiHeader.biHeight         = GAME_RESOLUTION_HEIGHT;
    gFrameBuffer.bitmap_info.bmiHeader.biBitCount       = GAME_BITS_PER_PIXEL;
    gFrameBuffer.bitmap_info.bmiHeader.biCompression    = BI_RGB;
    gFrameBuffer.bitmap_info.bmiHeader.biPlanes         = 1; // Must be set to 1 according to MSDN.
    gFrameBuffer.buffer                                 = &gPixels;

    memset(gFrameBuffer.buffer, 0x00, FRAME_BUFFER_BYTES);
    memset(gFrameBuffer.buffer, 0xFF, 4);

    while (gGameIsRunning == TRUE)
    {
        MSG message = { 0 };

        while (PeekMessage(&message, gGameWindow, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&message);
        }

        RenderFrame();
    }

    return 0;
}

LRESULT CALLBACK WindowProc(_In_ HWND hWindow, _In_ UINT uMessage, _In_ WPARAM wParameter, _In_ LPARAM lParameter)
{
    switch (uMessage)
    {
        case WM_KEYDOWN:
        {
            if (wParameter == VK_ESCAPE)
            {
                SetFullscreen(FALSE);
            }
            else if (wParameter == 0x46) // See MSDN on Virtual-Key Codes.
            {
                SetFullscreen(TRUE);
            }
            return 0;
        }

        case WM_SIZE:
        {
            if (wParameter == SIZE_MAXIMIZED)
            {
                SetFullscreen(TRUE);
            }
            return 0;
        }

        case WM_CLOSE:
        {
            gGameIsRunning = FALSE;
            return 0;
        }
    }

    return DefWindowProc(hWindow, uMessage, wParameter, lParameter);
}

DWORD CreateGameWindow(VOID)
{
    DWORD result = ERROR_SUCCESS;

    LPCWSTR CLASS_NAME = L"Gilberte Class";

    // TODO: Check for return values
    WNDCLASSEX window_class     = { 0 };
    window_class.cbSize         = sizeof(window_class);
    window_class.lpfnWndProc    = WindowProc;
    window_class.hInstance      = GetModuleHandle(NULL);
    window_class.lpszClassName  = CLASS_NAME;
    window_class.hbrBackground  = CreateSolidBrush(RGB(255, 0, 255));
    window_class.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    window_class.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);
    window_class.hCursor        = LoadCursor(NULL, IDC_ARROW);

    if (RegisterClassEx(&window_class) == 0)
    {
        result = GetLastError();
        goto Exit;
    }

    if (GetMonitorInfo(MonitorFromWindow(gGameWindow, MONITOR_DEFAULTTOPRIMARY), &gMonitorInfo) == 0)
    {
        result = ERROR_MONITOR_NO_DESCRIPTOR;
        goto Exit;
    }

    gGameWindow = CreateWindowEx(
        0,
        CLASS_NAME,
        GAME_NAME,
        (WS_OVERLAPPEDWINDOW | WS_VISIBLE) ^ WS_THICKFRAME,
        gMonitorInfo.rcMonitor.left, gMonitorInfo.rcMonitor.right,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        window_class.hInstance,
        NULL);

    if (gGameWindow == NULL)
    {
        result = GetLastError();
        goto Exit;
    }

    SetFullscreen(FALSE);

Exit:
    return result;
}

DWORD SetFullscreen(BOOL set_fullscreen)
{
    DWORD result = ERROR_SUCCESS;

    if (set_fullscreen == TRUE)
    {
        if (SetWindowLongPtr(gGameWindow, GWL_STYLE, WS_VISIBLE) == 0)
        {
            result = GetLastError();
            goto Exit;
        }

        if (GetMonitorInfo(MonitorFromWindow(gGameWindow, MONITOR_DEFAULTTOPRIMARY), &gMonitorInfo) == 0)
        {
            result = ERROR_MONITOR_NO_DESCRIPTOR;
            goto Exit;
        }

        gMonitorWidth = gMonitorInfo.rcMonitor.right - gMonitorInfo.rcMonitor.left;
        gMonitorHeight = gMonitorInfo.rcMonitor.bottom - gMonitorInfo.rcMonitor.top;

        if (SetWindowPos(gGameWindow, HWND_TOP,
            gMonitorInfo.rcMonitor.left, gMonitorInfo.rcMonitor.top,
            gMonitorWidth, gMonitorHeight,
            SWP_FRAMECHANGED | SWP_NOCOPYBITS) == 0)
        {
            result = GetLastError();
            goto Exit;
        }

        gRenderDestX = (gMonitorWidth / 2) - (GAME_RESOLUTION_WIDTH / 2);
        gRenderDestY = (gMonitorHeight / 2) - (GAME_RESOLUTION_HEIGHT / 2);
    }
    else
    {
        if (SetWindowLongPtr(gGameWindow, GWL_STYLE, (WS_OVERLAPPEDWINDOW | WS_VISIBLE) ^ WS_THICKFRAME) == 0)
        {
            result = GetLastError();
            goto Exit;
        }

        if (GetMonitorInfo(MonitorFromWindow(gGameWindow, MONITOR_DEFAULTTOPRIMARY), &gMonitorInfo) == 0)
        {
            result = ERROR_MONITOR_NO_DESCRIPTOR;
            goto Exit;
        }

        gMonitorWidth = gMonitorInfo.rcMonitor.right - gMonitorInfo.rcMonitor.left;
        gMonitorHeight = gMonitorInfo.rcMonitor.bottom - gMonitorInfo.rcMonitor.top;

        RECT client_area_rect = { 0 };
        client_area_rect.right = 640;
        client_area_rect.bottom = 480;

        if (AdjustWindowRect(&client_area_rect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE) == 0)
        {
            result = GetLastError();
            goto Exit;
        }

        if (SetWindowPos(gGameWindow, HWND_TOP,
            gMonitorInfo.rcMonitor.left, gMonitorInfo.rcMonitor.top,
            client_area_rect.right - client_area_rect.left, client_area_rect.bottom - client_area_rect.top,
            SWP_FRAMECHANGED | SWP_NOCOPYBITS) == 0)
        {
            result = GetLastError();
            goto Exit;
        }

        gRenderDestX = 0;
        gRenderDestY = 0;
    }

Exit:
    return result;
}

BOOL GameIsAlreadyRunning(VOID)
{
    if (CreateMutex(NULL, FALSE, GAME_NAME L"_Mutex") == NULL)
    {
        DWORD dwError = GetLastError();
        if (dwError == ERROR_ALREADY_EXISTS)
        {
            return TRUE;
        }
        else
        {
            // TODO: Handle errors other than ERROR_ALREADY_EXISTS
        }
    }

    return FALSE;
}

VOID RenderFrame(VOID)
{
    HDC device_context = GetDC(gGameWindow);

    StretchDIBits(
        device_context,
        gRenderDestX, gRenderDestY,
        GAME_RESOLUTION_WIDTH, GAME_RESOLUTION_HEIGHT,
        0, 0,
        GAME_RESOLUTION_WIDTH, GAME_RESOLUTION_HEIGHT,
        gFrameBuffer.buffer,
        &gFrameBuffer.bitmap_info,
        DIB_RGB_COLORS,
        SRCCOPY);

    ReleaseDC(gGameWindow, device_context);
}