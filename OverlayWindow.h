#pragma once
#include <windows.h>

class OverlayWindow {
public:
    static bool Initialize(HINSTANCE hInstance);
    static void Shutdown();
    static HWND GetHWND();
    static void SetClickThrough(bool clickThrough);

    static void UpdateTargetWindowFocus();
    static bool IsTargetWindowActive();

private:
    static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    static HWND s_hwnd;
    static bool s_isClickThrough;
    static bool s_isTargetActive;
};
