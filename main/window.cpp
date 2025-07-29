//-------------------------------------
//
// ウィンドウ処理 [window.cpp]
// Author: fuma sato
//
//-------------------------------------
#include <string>
#include "resource.h"
#include "window.h"
#include "application.h"
#include "file.h"

#pragma comment(lib, "winmm.lib")

//-----------------------------------------------------
// 初期化
//-----------------------------------------------------
HRESULT Window::Init(const context::InitContext initContext)
{
    m_hInstance = initContext.instance;        // インスタンスハンドル

    //ウインドウ設定
    WNDCLASSEX wcex =
    {
        sizeof(WNDCLASSEX),                                    // メモリサイズ
        CS_OWNDC,                                              // スタイル
        WindowProc,                                            // プロシージャ
        0,                                                     // クラスの追加メモリ
        0,                                                     // ウィンドウの追加メモリ
        m_hInstance,                                           // インスタンスハンドル
        LoadIcon(m_hInstance,MAKEINTRESOURCE(IDI_MYAPP)),      // タスクバーアイコン
        LoadCursor(m_hInstance,MAKEINTRESOURCE(IDC_MYCURSOR)), // マウスカーソル
        nullptr,                                               // 背景色
        nullptr,                                               // メニューバー												
        m_info.className,                                      // クラスの名前
        LoadIcon(m_hInstance,MAKEINTRESOURCE(IDI_SMALL))       // アイコン
    };

    RECT rect = { 0,0,(LONG)m_info.screenWidth,(LONG)m_info.screenHeight };//ウインドウサイズの設定

    timeBeginPeriod(1);//分解能の設定

    //ウインドウクラスの登録
    RegisterClassEx(&wcex);

    //クライアント領域のサイズ調整
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    // ウインドウのスタイルを設定
    DWORD style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;

    //ウインドウを生成
    m_hWnd = CreateWindowEx
    (
        0,                        //スタイル
        m_info.className,         //ウインドウクラスの名前
        m_info.windowName,        //ウインドウの名前
        style,                    //スタイル
        CW_USEDEFAULT,            //左上座標
        CW_USEDEFAULT,            //右下座標
        (rect.right - rect.left), //幅
        (rect.bottom - rect.top), //高さ
        nullptr,                  //親
        nullptr,                  //メニューID
        m_hInstance,              //インスタンスハンドル
        this                      //作成データ
    );

    //表示
    ShowWindow(m_hWnd, initContext.nCmdShow);
    UpdateWindow(m_hWnd);

    // 初期化処理
    DragAcceptFiles(m_hWnd, TRUE);

    // Input登録
    RegisterAllInputDevices();

    // フルスクリーンではない
    m_isFullScreen = false;
}

//----------------------------
// 終了
//----------------------------
HRESULT Window::Uninit(void)
{
    //ウインドウクラスの登録解除
    UnregisterClass(m_info.className, m_hInstance);

    timeEndPeriod(1);//分解能を戻す
}

//--------------------------
// ウインドウプロシージャ
//--------------------------
LRESULT CALLBACK Window::StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window* pWindow = nullptr;

    // 最初のメッセージ(WM_NCCREATE)で、thisポインタをウィンドウのユーザーデータに保存する
    if (uMsg == WM_NCCREATE)
    {
        // lParamからCREATESTRUCTを取得
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        // CREATESTRUCTのlpCreateParamsに、CreateWindowExで渡したthisポインタが入っている
        pWindow = static_cast<Window*>(pCreate->lpCreateParams);

        // thisポインタをウィンドウハンドル(hWnd)に関連付けて保存
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
    }
    else
    {
        // 2回目以降のメッセージでは、保存したthisポインタを取得
        pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    // thisポインタが取得できたら、それを使って実際の処理を行うメンバー関数を呼び出す
    if (pWindow != nullptr)
    {
        return pWindow->InstanceWindowProc(hWnd, uMsg, wParam, lParam);
    }

    // ポインタが取得できる前のメッセージは、デフォルト処理に任せる
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//--------------------------
// ウインドウプロシージャ
//--------------------------
LRESULT CALLBACK Window::InstanceWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int nID = 0;//返り値の格納

    switch (uMsg)
    {
    case WM_DESTROY://破棄
        if (m_isFullScreen && !m_info.isBorderless)
        {// フルスクリーンモードの時
            ChangeDisplaySettings(nullptr, 0); // ウインドウモードを戻す
        }
        //WM_QUITメッセージ
        PostQuitMessage(0);
        break;
    case WM_INPUT:
    {
        // サイズの事前取得
        UINT rawInputSize{};
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &rawInputSize, sizeof(RAWINPUTHEADER));

        // メモリの確保
        vector<BYTE> data(rawInputSize);

        if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, data.data(), &rawInputSize, sizeof(RAWINPUTHEADER)) == -1)
        {// データ取得失敗
            CDebugProc::Print(CDebugProc::MODE::System, "Error: GetRawInputData failed!");
            break;
        }

        RAWINPUT* pRawInput = reinterpret_cast<RAWINPUT*>(data.data()); // RAWINPUT型にする

        if (pRawInput->header.dwType == RIM_TYPEKEYBOARD)
        {// キーボードイベント
            CDebugProc::Print("--- Keyboard WM_INPUT Received ---");

            USHORT vkey = pRawInput->data.keyboard.VKey;
            USHORT flags = pRawInput->data.keyboard.Flags;

            if (!(flags & RI_KEY_BREAK))
            {// 押下
                switch (vkey)
                {// どのキーか
                case VK_ESCAPE: //ESCキー
                    nID = MessageBox(hWnd, L"終了しますか?", L"終了メッセージ", MB_YESNO | MB_ICONQUESTION);
                    if (nID == IDYES)
                    {// はい
                        //破棄(WM_DESTROY)
                        DestroyWindow(hWnd);
                    }
                    break;
                case VK_F11: //F11キー
                    if (ISBORDERLESS)
                    {// ボーダーレスウィンドウモードの時
                        ToggleBorderless();
                    }
                    else
                    {// フルスクリーンモードの時
                        ToggleFullScreen();
                    }
                    break;
                }
            }
        }
        else if (pRawInput->header.dwType == RIM_TYPEMOUSE)
        {// マウスイベント
            CDebugProc::Print(CDebugProc::MODE::System, "--- Mouse WM_INPUT Received ---");
        }
        else if (pRawInput->header.dwType == RIM_TYPEHID)
        {// HIDイベント
            CDebugProc::Print(CDebugProc::MODE::System, "--- HID WM_INPUT Received ---");
        }

        CInputRawInput::AnalysisRawData(pRawInput); // ポインタをInput側に任せる
        break;
    }
    case WM_SIZE:
        switch (wParam)
        {
        case SIZE_MINIMIZED:
            g_bStop = true;
            break;
        default:
            if (FAILED(CManager::RenererReset((ISBORDERLESS || !g_bFullScreen))))
            {
                return E_FAIL; // レンダラーのリセット
            }
            g_bStop = false;
            break;
        }
        break;
    case WM_ENTERSIZEMOVE:
        g_bStop = true;
        break;
    case WM_EXITSIZEMOVE:
        g_bStop = false;
        break;
    case WM_KILLFOCUS:
        g_bStop = true;
        break;
    case WM_SETFOCUS:
        g_bStop = false;
        break;
    case WM_ACTIVATEAPP:
        if (wParam == TRUE)
        {
            g_bStop = false;
        }
        else
        {
            g_bStop = true;
        }
        break;
    case WM_SYSCOMMAND:
        if ((wParam & 0xFFF0) == SC_MAXIMIZE)//最大化ボタン
        {
            g_bStop = false;
        }
        else if ((wParam & 0xFFF0) == SC_MINIMIZE)//最小化ボタン
        {
            g_bStop = true;
        }
        else if ((wParam & 0xFFF0) == SC_CLOSE)//最小化ボタン
        {
            nID = MessageBox(hWnd, L"終了しますか?", L"終了メッセージ", MB_YESNO | MB_ICONQUESTION);
            if (nID == IDNO)
            {
                return 0;
            }
        }
        break;
    case WM_CLOSE://ウインドウの×ボタン
        break;
    case WM_DISPLAYCHANGE://ディスプレイの変更
        break;
    case WM_DEVICECHANGE:
        // Input登録
        RegisterAllInputDevices();
        break;
    case WM_POWERBROADCAST:
        break;
    case WM_DROPFILES:
    {
        HDROP hDrop = (HDROP)wParam;
        path filePath{};
        DragQueryFile(hDrop, 0, LPWSTR(filePath.wstring().c_str()), MAX_PATH);
        DragFinish(hDrop);
        // ドロップされたファイルのパスを取得
        if (!filePath.empty())
        {
            // ファイルパスを使用する処理をここに追加
            MessageBox(hWnd, filePath.wstring().c_str(), L"Dropped File", MB_OK);
        }
        break;
    }
    }

    CManager::GetGui().Proc(hWnd, uMsg, wParam, lParam); // Gui

    //必要データを返す
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//----------------------------
// フルスクリーン
//----------------------------
void Window::ToggleFullScreen(void)
{
    g_bFullScreen = !g_bFullScreen;
    static RECT windowRect = {};

    // ウィンドウスタイル
    static LONG style = GetWindowLong(g_hWnd, GWL_STYLE);
    static LONG exStyle = GetWindowLong(g_hWnd, GWL_EXSTYLE);

    if (g_bFullScreen)
    {
        style = GetWindowLong(g_hWnd, GWL_STYLE);
        exStyle = GetWindowLong(g_hWnd, GWL_EXSTYLE);
        GetWindowRect(g_hWnd, &windowRect);

        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        DEVMODE dm = {};
        dm.dmSize = sizeof(dm);
        dm.dmPelsWidth = (DWORD)screenWidth;    // 解像度
        dm.dmPelsHeight = (DWORD)screenHeight;
        dm.dmBitsPerPel = 32; // フルカラー
        dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

        ChangeDisplaySettings(&dm, CDS_FULLSCREEN);

        LONG newStyle = style & ~(WS_OVERLAPPEDWINDOW);
        SetWindowLong(g_hWnd, GWL_STYLE, newStyle);

        SetWindowPos
        (
            g_hWnd,
            HWND_TOP,
            0, 0,
            screenWidth, screenHeight,
            SWP_FRAMECHANGED
        );
    }
    else
    {
        ChangeDisplaySettings(nullptr, 0); // ディスプレイ設定を元に戻す

        SetWindowLong(g_hWnd, GWL_STYLE, style);

        SetWindowPos
        (
            g_hWnd,
            nullptr,
            windowRect.left, windowRect.top,
            windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
            SWP_FRAMECHANGED
        );
    }

    //表示
    ShowWindow(g_hWnd, SW_SHOW);
    UpdateWindow(g_hWnd);
}

//----------------------------
// ボーダーレスウィンドウ
//----------------------------
void Window::ToggleBorderless(void)
{
    g_bFullScreen = !g_bFullScreen;
    static RECT windowRect = {};

    // ウィンドウスタイル
    static LONG style = GetWindowLong(g_hWnd, GWL_STYLE);
    static LONG exStyle = GetWindowLong(g_hWnd, GWL_EXSTYLE);

    if (g_bFullScreen)
    {
        // 現在のウィンドウ位置とサイズとスタイルを保存
        style = GetWindowLong(g_hWnd, GWL_STYLE);
        exStyle = GetWindowLong(g_hWnd, GWL_EXSTYLE);
        GetWindowRect(g_hWnd, &windowRect);

        // フルスクリーンモード用のスタイルに変更
        // ボーダレスウィンドウ方式を採用

        LONG newStyle = style & ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
        LONG newExStyle = exStyle & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);

        // スタイルを適用
        SetWindowLong(g_hWnd, GWL_STYLE, newStyle);
        SetWindowLong(g_hWnd, GWL_EXSTYLE, newExStyle);

        // 画面サイズを取得
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        // ウィンドウをスクリーンサイズに合わせる
        SetWindowPos(
            g_hWnd,
            HWND_TOP,
            0, 0,
            screenWidth, screenHeight,
            SWP_FRAMECHANGED
        );
    }
    else
    {
        // 通常ウィンドウモードに戻す
        SetWindowLong(g_hWnd, GWL_STYLE, style);
        SetWindowLong(g_hWnd, GWL_EXSTYLE, exStyle);

        // 元のウィンドウサイズと位置に戻す
        SetWindowPos(
            g_hWnd,
            HWND_TOP,
            windowRect.left, windowRect.top,
            windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
            SWP_FRAMECHANGED
        );
    }

    // 表示と更新
    ShowWindow(g_hWnd, SW_SHOW);
    UpdateWindow(g_hWnd);
}

//----------------------------
// ファイルパス取得
//----------------------------
path Window::GetFilePath(void)
{
    OPENFILENAME ofn;
    path filePath{};   // ファイルパスを格納する変数
    path currentDir{}; // 現在のディレクトリを格納する変数

    // 現在のディレクトリを取得
    GetCurrentDirectory(MAX_PATH, LPWSTR(currentDir.wstring().c_str()));

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFilter = L"画像ファイル\0*.jpg;*.jpeg;*.png;*.bmp\0";
    ofn.lpstrFile = LPWSTR(filePath.wstring().c_str());
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = L"";
    ofn.lpstrInitialDir = LPWSTR(currentDir.wstring().c_str()); // 初期ディレクトリを設定

    if (GetOpenFileName(&ofn))
    {
        return filePath;
    }

    return {};
}

//----------------------------
// クライアント領域取得
//----------------------------
HRESULT Window::GetClientRect(RECT* rect)
{
    if (rect == nullptr)
    {
        return E_FAIL;
    }
    // ウィンドウのクライアント領域を取得
    if (!::GetClientRect(g_hWnd, rect))
    {
        return E_FAIL;
    }
    return S_OK;
}

//-----------------
// エラー
//-----------------
void Window::Error(HRESULT hr, const std::source_location& location = std::source_location::current())
{
    ostringstream oss;
    oss << "Error Code: " << std::hex << hr << "\nFunction: " << func;

    wstring msg = Utf8ToWstring(oss.str());
    MessageBox(g_hWnd, msg.c_str(), L"Error", MB_OK | MB_ICONERROR);

    PostMessage(g_hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
}

//------------------
//マウス制限
//------------------
void Window::ConfineCursorToWindow(void)
{
    RECT rect;
    GetClientRect(&rect);

    rect.left += 10;
    rect.top += 10;
    rect.right -= 10;
    rect.bottom -= 10;

    MapWindowPoints(g_hWnd, NULL, (POINT*)&rect, 2);
    ClipCursor(&rect);
}

//-----------------
//マウス解放
//-----------------
void Window::ReleaseCursor(void)
{
    ClipCursor(nullptr); // 制限を解除
}

//-----------------
// マウス切替
//-----------------
void Window::CursorSwitch(bool Switch)
{
    if (Switch == true)
    {
        ReleaseCursor();
        while (ShowCursor(TRUE) < 0);
    }
    else
    {
        while (ShowCursor(FALSE) >= 0);
        ConfineCursorToWindow();
    }
}

//----------------------------------------------------
// マウスカーソルの位置を取得する (クライアント座標)
//----------------------------------------------------
POINT Window::GetCursorPoint(void)
{
    POINT point;
    if (!GetCursorPos(&point)) return POINT(~0); // カーソルのスクリーン座標

    // スクリーン座標をクライアント座標に変換する
    return ScreenToClient(g_hWnd, &point) ? point : POINT(~0);
}

//----------------------------------------------------
// インプットの登録 (スタンダード)
//----------------------------------------------------
void Window::RegisterStandardInputDevices(void)
{
    RAWINPUTDEVICE RawDevice[] =
    {
        { 0x01, 0x06, RIDEV_INPUTSINK | RIDEV_NOLEGACY, g_hWnd }, // Standard Keyboard
        { 0x01, 0x02, RIDEV_INPUTSINK | RIDEV_NOLEGACY, g_hWnd }, // Standard Mouse
        { 0x0C, 0x01, RIDEV_INPUTSINK, g_hWnd },                  // Consumer Control
    };

    if (RegisterRawInputDevices(RawDevice, 3, sizeof(RAWINPUTDEVICE)))
    {
        CDebugProc::Print(CDebugProc::MODE::Static, "Standard input devices registered successfully");
    }
    else
    {
        DWORD error = GetLastError();
        CDebugProc::Print(CDebugProc::MODE::Static, "Standard registration failed: %d", error);
    }
}

//----------------------------------------------------
// インプットの登録 (ALL)
//----------------------------------------------------
void Window::RegisterAllInputDevices(void)
{
    try
    {
        UINT nDevices = 0;
        if (GetRawInputDeviceList(nullptr, &nDevices, sizeof(RAWINPUTDEVICELIST)) != 0)
        {
            CDebugProc::Print(CDebugProc::MODE::Static, "Failed to get raw input device count");
            return;
        }

        if (nDevices == 0)
        {
            CDebugProc::Print(CDebugProc::MODE::Static, "No raw input devices found");
            return;
        }

        std::vector<RAWINPUTDEVICELIST> devices(nDevices);
        if (GetRawInputDeviceList(devices.data(), &nDevices, sizeof(RAWINPUTDEVICELIST)) == (UINT)-1)
        {
            CDebugProc::Print(CDebugProc::MODE::Static, "Failed to get raw input device list");
            return;
        }

        std::vector<RAWINPUTDEVICE> inputDevices;
        std::set<std::pair<USHORT, USHORT>> registeredPairs;

        for (const auto& dev : devices)
        {
            RID_DEVICE_INFO info = {};
            UINT size = sizeof(info);
            info.cbSize = sizeof(info);

            if (GetRawInputDeviceInfo(dev.hDevice, RIDI_DEVICEINFO, &info, &size) > 0)
            {
                RAWINPUTDEVICE rawDevice = {};
                rawDevice.dwFlags = RIDEV_INPUTSINK;
                rawDevice.hwndTarget = g_hWnd;

                if (info.dwType == RIM_TYPEKEYBOARD)
                {
                    // キーボードの場合は標準的なUsagePage/Usageを使用
                    USHORT usagePage = 0x01;             // HID_USAGE_PAGE_GENERIC
                    USHORT usage = 0x06;                 // HID_USAGE_GENERIC_KEYBOARD
                    rawDevice.dwFlags |= RIDEV_NOLEGACY | RIDEV_NOHOTKEYS; // NOLEGACY_NOHOTKEYS

                    auto pair = std::make_pair(usagePage, usage);
                    if (registeredPairs.find(pair) == registeredPairs.end())
                    {
                        registeredPairs.insert(pair);
                        rawDevice.usUsagePage = usagePage;
                        rawDevice.usUsage = usage;
                        inputDevices.push_back(rawDevice);

                        CDebugProc::Print(CDebugProc::MODE::Static, "Adding keyboard: %04x:%04x (Type: %d, SubType: %d, KeyboardMode: %d)",
                            usagePage, usage, info.keyboard.dwType, info.keyboard.dwSubType, info.keyboard.dwKeyboardMode);
                    }
                }
                else if (info.dwType == RIM_TYPEMOUSE)
                {
                    // マウスの場合は標準的なUsagePage/Usageを使用
                    USHORT usagePage = 0x01;             // HID_USAGE_PAGE_GENERIC
                    USHORT usage = 0x02;                 // HID_USAGE_GENERIC_MOUSE

                    auto pair = std::make_pair(usagePage, usage);
                    if (registeredPairs.find(pair) == registeredPairs.end())
                    {
                        registeredPairs.insert(pair);
                        rawDevice.usUsagePage = usagePage;
                        rawDevice.usUsage = usage;
                        inputDevices.push_back(rawDevice);

                        CDebugProc::Print(CDebugProc::MODE::Static, "Adding mouse: %04x:%04x (ID: %d, Buttons: %d, SampleRate: %d)",
                            usagePage, usage, info.mouse.dwId, info.mouse.dwNumberOfButtons, info.mouse.dwSampleRate);
                    }
                }
                else if (info.dwType == RIM_TYPEHID)
                {
                    // HIDデバイスの場合のみ info.hid を使用
                    USHORT usagePage = info.hid.usUsagePage;
                    USHORT usage = info.hid.usUsage;

                    // 有効なUsagePage/Usageかチェック
                    if (usagePage != 0)
                    {
                        auto pair = std::make_pair(usagePage, usage);
                        if (registeredPairs.find(pair) == registeredPairs.end())
                        {
                            registeredPairs.insert(pair);
                            rawDevice.usUsagePage = usagePage;
                            rawDevice.usUsage = usage;
                            inputDevices.push_back(rawDevice);

                            CDebugProc::Print(CDebugProc::MODE::Static, "Adding HID device: %04x:%04x (VendorId: %04x, ProductId: %04x)",
                                usagePage, usage, info.hid.dwVendorId, info.hid.dwProductId);
                        }
                    }
                }
            }
        }

        if (!inputDevices.empty())
        {
            if (RegisterRawInputDevices(inputDevices.data(), UINT(inputDevices.size()), sizeof(RAWINPUTDEVICE)))
            {
                DWORD error = GetLastError();
                CDebugProc::Print(CDebugProc::MODE::Static, "Successfully registered %zu devices LastError:%d", inputDevices.size(), error);
            }
            else
            {
                DWORD error = GetLastError();
                CDebugProc::Print(CDebugProc::MODE::Static, "Registration failed: %d", error);
            }
        }
    }
    catch (...)
    {
        CDebugProc::Print(CDebugProc::MODE::Static, "Exception in RegisterAllKeyboardDevices");
    }
}

//----------------------------------------------------
// デバイス情報の詳細表示（デバッグ用）
//----------------------------------------------------
void Window::DebugPrintDeviceInfo(void)
{
    try
    {
        UINT nDevices = 0;
        if (GetRawInputDeviceList(nullptr, &nDevices, sizeof(RAWINPUTDEVICELIST)) != 0)
        {
            return;
        }

        std::vector<RAWINPUTDEVICELIST> devices(nDevices);
        if (GetRawInputDeviceList(devices.data(), &nDevices, sizeof(RAWINPUTDEVICELIST)) == (UINT)-1)
        {
            return;
        }

        CDebugProc::Print(CDebugProc::MODE::Static, "=== Device Information ===");

        for (size_t i = 0; i < devices.size(); ++i)
        {
            const auto& dev = devices[i];
            RID_DEVICE_INFO info = {};
            UINT size = sizeof(info);
            info.cbSize = sizeof(info);

            if (GetRawInputDeviceInfo(dev.hDevice, RIDI_DEVICEINFO, &info, &size) > 0)
            {
                CDebugProc::Print(CDebugProc::MODE::Static, "Device %zu: Type=%d", i, info.dwType);

                switch (info.dwType)
                {
                case RIM_TYPEKEYBOARD:
                    CDebugProc::Print(CDebugProc::MODE::Static, "  Keyboard - Type:%d SubType:%d KeyboardMode:%d",
                        info.keyboard.dwType, info.keyboard.dwSubType, info.keyboard.dwKeyboardMode);
                    break;

                case RIM_TYPEMOUSE:
                    CDebugProc::Print(CDebugProc::MODE::Static, "  Mouse - ID:%d Buttons:%d SampleRate:%d HasHorizontalWheel:%s",
                        info.mouse.dwId, info.mouse.dwNumberOfButtons, info.mouse.dwSampleRate,
                        info.mouse.fHasHorizontalWheel ? "Yes" : "No");
                    break;

                case RIM_TYPEHID:
                    CDebugProc::Print(CDebugProc::MODE::Static, "  HID - VendorId:%04x ProductId:%04x UsagePage:%04x Usage:%04x",
                        info.hid.dwVendorId, info.hid.dwProductId, info.hid.usUsagePage, info.hid.usUsage);
                    break;
                }
            }
        }
        CDebugProc::Print(CDebugProc::MODE::Static, "========================");
    }
    catch (...)
    {
        CDebugProc::Print(CDebugProc::MODE::Static, "Exception in DebugPrintDeviceInfo");
    }
}

//----------------------
// カスタム設定のロード
//----------------------
bool Window::CustomLoad(void)
{
    // YAMLファイルの読み込み
    CFile* pFile = new CFile("custom.yaml");
    YAML::Node win = pFile->ReadYaml("Window");

    // カスタム設定
    m_info.className = win["class_name"].as<std::wstring>().c_str();   // ウインドウクラス名
    m_info.windowName = win["window_name"].as<std::wstring>().c_str(); // ウインドウ名
    m_info.screenWidth = win["width"].as<float>();                     // ウインドウ幅
    m_info.screenHeight = win["height"].as<float>();                   // ウインドウ高さ
    m_info.isBorderless = win["borderless"].as<bool>();                // ボーダーレスウインドウかどうか
    m_info.isResizable = win["resizable"].as<bool>();                  // リサイズ可能かどうか

    return true; // YAMLの読み込みに成功した場合
}
