#include "OverlayWindow.h"
#include <dwmapi.h>
#include <string>

#include <imgui.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND OverlayWindow::s_hwnd = nullptr;
bool OverlayWindow::s_isClickThrough = true;
bool OverlayWindow::s_isTargetActive = true;

LRESULT WINAPI OverlayWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

bool OverlayWindow::Initialize(HINSTANCE hInstance) {
    WNDCLASSEXW wc = { sizeof(WNDCLASSEXW), CS_CLASSDC, WndProc, 0L, 0L, hInstance, nullptr, nullptr, nullptr, nullptr, L"SimpleCrosshair", nullptr };
    ::RegisterClassExW(&wc);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    s_hwnd = ::CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
        L"SimpleCrosshair", L"Simple Crosshair Overlay",
        WS_POPUP,
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        nullptr, nullptr, wc.hInstance, nullptr);

    if (!s_hwnd) return false;

    SetLayeredWindowAttributes(s_hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);

    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(s_hwnd, &margins);

    ::ShowWindow(s_hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(s_hwnd);

    return true;
}

void OverlayWindow::Shutdown() {
    ::DestroyWindow(s_hwnd);
    ::UnregisterClassW(L"SimpleCrosshair", GetModuleHandle(nullptr));
}

HWND OverlayWindow::GetHWND() {
    return s_hwnd;
}

void OverlayWindow::SetClickThrough(bool clickThrough) {
    if (s_isClickThrough == clickThrough) return;
    s_isClickThrough = clickThrough;

    LONG exStyle = GetWindowLong(s_hwnd, GWL_EXSTYLE);
    if (clickThrough) {
        exStyle |= WS_EX_TRANSPARENT;
    } else {
        exStyle &= ~WS_EX_TRANSPARENT;
    }
    SetWindowLong(s_hwnd, GWL_EXSTYLE, exStyle);
    SetWindowPos(s_hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

void OverlayWindow::UpdateTargetWindowFocus() {
    HWND fgWindow = GetForegroundWindow();
    if (fgWindow == s_hwnd) {
        s_isTargetActive = true;
        return;
    }

    char windowTitle[256];
    GetWindowTextA(fgWindow, windowTitle, sizeof(windowTitle));

    std::string title(windowTitle);
    if (title.empty() || title == "Program Manager") {
        s_isTargetActive = false;
    } else {
        s_isTargetActive = true;
    }
}

bool OverlayWindow::IsTargetWindowActive() {
    return s_isTargetActive;
}
