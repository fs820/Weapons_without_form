//------------------------------------------
//
//メインの処理[main.cpp]
//Author: fuma sato
//
//------------------------------------------
#include "main.h"
#include "resource.h"
#include "manager.h"
#include "gui.h"
#include "debug.h"
#include "input.h"

#include <ShellScalingApi.h>  // SetProcessDpiAwareness用
#include <Shlwapi.h> // パス操作用 (PathRelativePathToなど)

#pragma comment(lib, "winmm.lib")

// C++の標準ライブラリ
#pragma comment(lib, "Shcore.lib")  // SetProcessDpiAwareness用
#pragma comment(lib, "Shlwapi.lib") // Shlwapiライブラリをリンク

// CMainクラスの静的メンバ変数の定義
HWND CMain::m_hWnd = nullptr;        // ウインドウハンドル
bool CMain::m_bFullScreen = false;   // フルスクリーンフラグ
float CMain::m_elapsedTime = 0.0f;   // 実行時間
float CMain::m_deltaTime = 0.0f;     // フレーム時間
bool CMain::m_bStop = false;         // ストップフラグ
float CMain::m_gameSpeed = 1.0f;     // ゲームスピード

//------------------------
//メイン関数
//------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//ウインドウ設定
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),                                  // メモリサイズ
		CS_OWNDC,                                            // スタイル
		CMain::WindowProc,                                   // プロシージャ
		0,                                                   // クラスの追加メモリ
		0,                                                   // ウィンドウの追加メモリ
		hInstance,                                           // インスタンスハンドル
		LoadIcon(hInstance,MAKEINTRESOURCE(IDI_MYAPP)),      // タスクバーアイコン
		LoadCursor(hInstance,MAKEINTRESOURCE(IDC_MYCURSOR)), // マウスカーソル
		nullptr,                                             // 背景色
		nullptr,                                             // メニューバー												
		CMain::CLASS_NAME,                                   // クラスの名前
		LoadIcon(hInstance,MAKEINTRESOURCE(IDI_SMALL))       // アイコン
	};

	MSG msg = { 0 };//メッセージ
	RECT rect = { 0,0,(LONG)CMain::SCREEN_WIDTH,(LONG)CMain::SCREEN_HEIGHT};//ウインドウサイズの設定

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域のサイズ調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウインドウのスタイルを設定
	DWORD style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;

	//ウインドウを生成
	HWND hWnd = nullptr;
	hWnd = CreateWindowEx
	(
		0,                        //スタイル
		CMain::CLASS_NAME,        //ウインドウクラスの名前
		CMain::WINDOW_NAME,       //ウインドウの名前
		style,                    //スタイル
		CW_USEDEFAULT,            //左上座標
		CW_USEDEFAULT,            //右下座標
		(rect.right - rect.left), //幅
		(rect.bottom - rect.top), //高さ
		nullptr,                  //親
		nullptr,                  //メニューID
		hInstance,                //インスタンスハンドル
		nullptr                   //作成データ
	);

	//表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	CMain::SethWnd(hWnd);

	// 初期化処理
	DragAcceptFiles(hWnd, TRUE);

	// Input登録
	CMain::RegisterAllInputDevices();

	// マネージャーの生成
	CManager* pManager = nullptr;
	if (pManager == nullptr)
	{
		pManager = new CManager;
	}
	
	// マネージャーの初期化
	if (pManager != nullptr)
	{
		if (FAILED(pManager->Init(hInstance, hWnd, (CMain::ISBORDERLESS || !CMain::IsFullScreen()))))
		{
			return E_FAIL;
		}
	}

	timeBeginPeriod(1);//分解能の設定

	float Frame = (1.0f / (float)CMain::FPS);                    // フレーム(秒)
	steady_clock::time_point startTime = steady_clock::now();    // 開始時刻 (時刻単位)
	steady_clock::time_point exceLastTime = steady_clock::now(); // 前回時刻 (時刻単位)
	steady_clock::time_point currntTime = steady_clock::now();   // 現在時刻 (時刻単位)

	//メッセージループ
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)//終了メッセージ
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		currntTime = steady_clock::now();                                     // 時刻を取得 (時刻単位)

		float elapsedTime = duration<float>(currntTime - startTime).count();  // 経過時間 (秒)
		float deltaTime = duration<float>(currntTime - exceLastTime).count(); // フレーム間隔 (秒)

		CMain::SetElapsedTime(elapsedTime);                                   // 登録
		CMain::SetDeltaTime(deltaTime);                                       //

		if (deltaTime >= Frame)//設定した間隔が空いたら
		{// 更新
			exceLastTime = currntTime; //今回の更新時刻を記録

			// デバック表示
			CDebugProc::Print("FPS:%f", 1.0f / deltaTime);
			CDebugProc::Print("終了:[ESC]");
			CDebugProc::Print("フルスクリーン:[F11]");
			CDebugProc::Print(CDebugProc::MODE::Window, "経過時間:%f", elapsedTime);
			CDebugProc::Print(CDebugProc::MODE::Window, "フレーム時間:%f", deltaTime);

			if (!CMain::IsStop())
			{// 更新停止状態でない
				pManager->Update(); //更新処理
			}
			if (FAILED(pManager->Draw())) { return E_FAIL; }   //描画処理
		}
	}

	SAFE_UNINIT(pManager); // マネージャーの破棄

	//ウインドウクラスの登録解除
	UnregisterClass(CMain::CLASS_NAME, wcex.hInstance);

	timeEndPeriod(1);//分解能を戻す

	//終了
	return (int)msg.wParam;
}

//--------------------------
//ウインドウプロシージャ
//--------------------------
LRESULT CALLBACK CMain::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID = 0;//返り値の格納

	switch (uMsg)
	{
	case WM_DESTROY://破棄
		if (m_bFullScreen && !ISBORDERLESS)
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
					nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);
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
			m_bStop = true;
			break;
		default:
			if (FAILED(CManager::RenererReset((CMain::ISBORDERLESS || !m_bFullScreen))))
			{
				return E_FAIL; // レンダラーのリセット
			}
			m_bStop = false;
			break;
		}
		break;
	case WM_ENTERSIZEMOVE:
		m_bStop = true;
		break;
	case WM_EXITSIZEMOVE:
		m_bStop = false;
		break;
	case WM_KILLFOCUS:
		m_bStop = true;
		break;
	case WM_SETFOCUS:
		m_bStop = false;
		break;
	case WM_ACTIVATEAPP:
		if (wParam == TRUE)
		{
			m_bStop = false;
		}
		else
		{
			m_bStop = true;
		}
		break;
	case WM_SYSCOMMAND:
		if ((wParam & 0xFFF0) == SC_MAXIMIZE)//最大化ボタン
		{
			m_bStop = false;
		}
		else if ((wParam & 0xFFF0) == SC_MINIMIZE)//最小化ボタン
		{
			m_bStop = true;
		}
		else if ((wParam & 0xFFF0) == SC_CLOSE)//最小化ボタン
		{
			nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);
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
		CMain::RegisterAllInputDevices();
		break;
	case WM_POWERBROADCAST:
		break;
	case WM_DROPFILES:
	{
		HDROP hDrop = (HDROP)wParam;
		char filePath[MAX_PATH] = { 0 };
		DragQueryFile(hDrop, 0, filePath, MAX_PATH);
		DragFinish(hDrop);
		// ドロップされたファイルのパスを取得
		string_view droppedFilePath = filePath;
		if (!droppedFilePath.empty())
		{
			// ファイルパスを使用する処理をここに追加
			MessageBox(hWnd, droppedFilePath.data(), "Dropped File", MB_OK);
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
void CMain::ToggleFullScreen(void)
{
	m_bFullScreen = !m_bFullScreen;
	static RECT windowRect = {};

	// ウィンドウスタイル
	static LONG style = GetWindowLong(m_hWnd, GWL_STYLE);
	static LONG exStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);

	if (m_bFullScreen)
	{
		style = GetWindowLong(m_hWnd, GWL_STYLE);
		exStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
		GetWindowRect(m_hWnd, &windowRect);

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
		SetWindowLong(m_hWnd, GWL_STYLE, newStyle);

		SetWindowPos
		(
			m_hWnd,
			HWND_TOP,
			0, 0,
			screenWidth, screenHeight,
			SWP_FRAMECHANGED
		);
	}
	else
	{
		ChangeDisplaySettings(nullptr, 0); // ディスプレイ設定を元に戻す

		SetWindowLong(m_hWnd, GWL_STYLE, style);

		SetWindowPos
		(
			m_hWnd,
			nullptr,
			windowRect.left, windowRect.top,
			windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
			SWP_FRAMECHANGED
		);
	}

	//表示
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
}

//----------------------------
// ボーダーレスウィンドウ
//----------------------------
void CMain::ToggleBorderless(void)
{
	m_bFullScreen = !m_bFullScreen;
	static RECT windowRect = {};

	// ウィンドウスタイル
	static LONG style = GetWindowLong(m_hWnd, GWL_STYLE);
	static LONG exStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);

	if (m_bFullScreen)
	{
		// 現在のウィンドウ位置とサイズとスタイルを保存
		style = GetWindowLong(m_hWnd, GWL_STYLE);
		exStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
		GetWindowRect(m_hWnd, &windowRect);

		// フルスクリーンモード用のスタイルに変更
		// ボーダレスウィンドウ方式を採用

		LONG newStyle = style & ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
		LONG newExStyle = exStyle & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);

		// スタイルを適用
		SetWindowLong(m_hWnd, GWL_STYLE, newStyle);
		SetWindowLong(m_hWnd, GWL_EXSTYLE, newExStyle);

		// 画面サイズを取得
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// ウィンドウをスクリーンサイズに合わせる
		SetWindowPos(
			m_hWnd,
			HWND_TOP,
			0, 0,
			screenWidth, screenHeight,
			SWP_FRAMECHANGED
		);
	}
	else
	{
		// 通常ウィンドウモードに戻す
		SetWindowLong(m_hWnd, GWL_STYLE, style);
		SetWindowLong(m_hWnd, GWL_EXSTYLE, exStyle);

		// 元のウィンドウサイズと位置に戻す
		SetWindowPos(
			m_hWnd,
			HWND_TOP,
			windowRect.left, windowRect.top,
			windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
			SWP_FRAMECHANGED
		);
	}

	// 表示と更新
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
}

//----------------------------
// ファイルパス取得
//----------------------------
string_view CMain::GetFilePath(void)
{
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	char currentDir[MAX_PATH] = "";

	// 現在のディレクトリを取得
	GetCurrentDirectory(MAX_PATH, currentDir);
	strcat_s(currentDir, MAX_PATH, "\\data\0");

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFilter = "画像ファイル\0*.jpg;*.jpeg;*.png;*.bmp\0";
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";
	ofn.lpstrInitialDir = currentDir; // 初期ディレクトリを設定

	if (GetOpenFileName(&ofn))
	{
		return string_view(fileName);
	}

	return "";
}

//----------------------------
// クライアント領域取得
//----------------------------
HRESULT CMain::GetClientRect(RECT* rect)
{
	if (rect == nullptr)
	{
		return E_FAIL;
	}
	// ウィンドウのクライアント領域を取得
	if (!::GetClientRect(m_hWnd, rect))
	{
		return E_FAIL;
	}
	return S_OK;
}

//-----------------
// エラー
//-----------------
void CMain::Error(HRESULT hr, const char func[])
{
	char msg[256];
	snprintf(msg, sizeof(msg), "失敗しました。\nHRESULT: 0x%08X", hr);
	MessageBox(m_hWnd, msg, func, MB_OK | MB_ICONERROR);
	PostMessage(m_hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
}

//------------------
//マウス制限
//------------------
void CMain::ConfineCursorToWindow(void)
{
	RECT rect;
	GetClientRect(&rect);

	rect.left += 10;
	rect.top += 10;
	rect.right -= 10;
	rect.bottom -= 10;

	MapWindowPoints(m_hWnd, NULL, (POINT*)&rect, 2);
	ClipCursor(&rect);
}

//-----------------
//マウス解放
//-----------------
void CMain::ReleaseCursor(void)
{
	ClipCursor(nullptr); // 制限を解除
}

//-----------------
// マウス切替
//-----------------
void CMain::CursorSwitch(bool Switch)
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
POINT CMain::GetCursorPoint(void)
{
	POINT point;
	if (!GetCursorPos(&point)) return POINT(~0); // カーソルのスクリーン座標

	// スクリーン座標をクライアント座標に変換する
	return ScreenToClient(m_hWnd, &point) ? point : POINT(~0);
}

//----------------------------------------------------
// インプットの登録 (スタンダード)
//----------------------------------------------------
void CMain::RegisterStandardInputDevices(void)
{
	RAWINPUTDEVICE RawDevice[] =
	{
		{ 0x01, 0x06, RIDEV_INPUTSINK | RIDEV_NOLEGACY, m_hWnd }, // Standard Keyboard
		{ 0x01, 0x02, RIDEV_INPUTSINK | RIDEV_NOLEGACY, m_hWnd }, // Standard Mouse
		{ 0x0C, 0x01, RIDEV_INPUTSINK, m_hWnd },                  // Consumer Control
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
void CMain::RegisterAllInputDevices(void)
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
				rawDevice.hwndTarget = m_hWnd;

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
void CMain::DebugPrintDeviceInfo(void)
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