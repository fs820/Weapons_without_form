//------------------------------------------
//
// Windowの定義 [window.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include <windows.h>
#include <string_view>
#include <filesystem>
#include <source_location>
#include "context.h"

namespace window
{
    struct Info
    {
        const wchar_t* className;    // ウインドウクラス名
        const wchar_t* windowName;   // ウインドウ名
        float screenWidth;           // ウインドウ幅
        float screenHeight;          // ウインドウ高さ
        bool isBorderless;           // ボーダーレスフラグ
        bool isResizable;            // リサイズ可能フラグ

        Info() : className(L"WindowClass"), windowName(L"Telepathy_Wepon"), screenWidth(1920.0f), screenHeight(1080.0f), isBorderless(true), isResizable(false) {}
        Info(const wchar_t* clsName, const wchar_t* wndName, float width, float height, bool borderless, bool resizable)
            : className(clsName), windowName(wndName), screenWidth(width), screenHeight(height), isBorderless(borderless), isResizable(resizable) {}
        ~Info() = default;
    };
}

//----------------------------
// ウインドウクラス
//----------------------------
class Window final
{
    // 公開
public:
    Window() : m_hInstance{},m_hWnd {}, m_isFullScreen{}, m_info{} {};
    ~Window() = default;

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(const Window&&) = default;
    Window& operator=(const Window&&) = default;

    HRESULT Init(const context::InitContext initContext);
    HRESULT Uninit(void);

    std::filesystem::path GetFilePath(void);
    HRESULT GetClientRect(RECT* rect);

    void ToggleFullScreen(void);
    void ToggleBorderless(void);
    bool IsFullScreen(void) { return m_isFullScreen; }

    void RegisterStandardInputDevices(void);
    void RegisterAllInputDevices(void);
    void DebugPrintDeviceInfo(void);

    void Error(HRESULT hr, const std::source_location& location = std::source_location::current());
    void ConfineCursorToWindow(void);
    void ReleaseCursor(void);
    void CursorSwitch(bool Switch);
    POINT GetCursorPoint(void);

    // 非公開
private:
    static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK InstanceWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    bool CustomLoad(void);

    HINSTANCE m_hInstance; // インスタンスハンドル
    HWND m_hWnd;           // ウインドウハンドル
    bool m_isFullScreen;   // フルスクリーンフラグ
    window::Info m_info;   // 設定
};
