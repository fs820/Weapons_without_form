//-------------------------------
//
// アプリケーション [application.cpp]
// Author: fuma sato
//
//-------------------------------
#include <chrono>
#include "application.h"
#include "window.h"
#include "manager.h"
#include "file.h"
#include "debug.h"
#include "input.h"
#include "renderer.h"
#include "event.h"
#include "sound.h"
#include "texture.h"
#include "model.h"
#include "motion.h"
#include "hierarchy.h"

using namespace std::chrono;

//-----------------------------
//
// アプリケーションクラス
//
//-----------------------------

//----------------------
// アプリケーション実行
//----------------------
int Application::Run(HINSTANCE hInstance, int nCmdShow)
{
    // インスタンスハンドルを保存
    m_hInstance = hInstance;

    // カスタム設定のロードと設定
    CustomLoad();

    // 初期化処理
    Init(nCmdShow);

    // 更新処理
    Update();
}

//----------------------
// 初期化処理
//----------------------
bool Application::Init(int nCmdShow)
{
    context::InitContext initContext;        // 初期化コンテキスト

    // InitContext設定
    initContext.pAppContext = &m_appContext; // アプリケーションコンテキストを設定
    initContext.instance = m_hInstance;      // インスタンス(exe)を設定
    initContext.nCmdShow = nCmdShow;         // ウインドウの表示状態を設定

    // イベントディスパッチャー
    m_pEvent = std::make_unique<EventDispatcher>();
    m_appContext.pEvent = m_pEvent.get(); // アプリケーションコンテキストにイベントディスパッチャーを設定

    // ウインドウ
    m_pWindow.reset(new Window);
    if (FAILED(m_pWindow->Init(initContext)))
    {
        return false; // 初期化失敗
    }
    m_appContext.pWindow = m_pWindow.get(); // アプリケーションコンテキストにウインドウを設定

    // 入力デバイス
    m_pInput.reset(new CInput);
    if (FAILED(m_pInput->Init(initContext)))
    {
        return false; // 初期化失敗
    }
    m_appContext.pInput = m_pInput.get(); // アプリケーションコンテキストに入力デバイスを設定

    // レンダラー
    m_pRenderer.reset(new CRenderer);
    if (FAILED(m_pRenderer->Init(initContext)))
    {
        return false; // 初期化失敗
    }
    m_appContext.pRenderer = m_pRenderer.get(); // アプリケーションコンテキストにレンダラーを設定

    // デバック
    m_pDebugProc.reset(new CDebugProc);
    if (FAILED(m_pDebugProc->Init()))
    {
        return false; // 初期化失敗
    }
    m_appContext.pDebug = m_pDebugProc.get(); // アプリケーションコンテキストにデバッグプロセスを設定

    // 階層構造
    m_pHierarchy.reset(new CHierarchy);
    if (FAILED(m_pHierarchy->Init(initContext)))
    {
        return false; // 初期化失敗
    }
    m_appContext.pHierarchy = m_pHierarchy.get(); // アプリケーションコンテキストに階層構造を設定

    // サウンドマネージャー
    m_pSound.reset(new CSoundManager);
    if (FAILED(m_pSound->Init(initContext)))
    {
        return false; // 初期化失敗
    }
    m_appContext.pSound = m_pSound.get(); // アプリケーションコンテキストにサウンドマネージャーを設定

    // テクスチャマネージャー
    m_pTextureManager.reset(new CTextureManager);
    if (FAILED(m_pTextureManager->Init(initContext)))
    {
        return false; // 初期化失敗
    }
    m_appContext.pTextureManager = m_pTextureManager.get(); // アプリケーションコンテキストにテクスチャマネージャーを設定

    // モデルマネージャー
    m_pModelManager.reset(new CModelManager);
    if (FAILED(m_pModelManager->Init(initContext)))
    {
        return false; // 初期化失敗
    }
    m_appContext.pModelManager = m_pModelManager.get(); // アプリケーションコンテキストにモデルマネージャーを設定

    // モーションマネージャー
    m_pMotionManager.reset(new CMotionManager);
    if (FAILED(m_pMotionManager->Init(initContext)))
    {
        return false; // 初期化失敗
    }
    m_appContext.pMotionManager = m_pMotionManager.get(); // アプリケーションコンテキストにモーションマネージャーを設定

    // マネージャー
    m_pManager.reset(new CManager);
    if (FAILED(m_pManager->Init(initContext)))
    {
        return false; // 初期化失敗
    }

    m_isRunning = true; // アプリケーション実行中フラグを立てる

    return true; // 初期化成功
}

//----------------------
// 更新処理
//----------------------
bool Application::Update(void)
{
    float Frame = (1.0f / m_fps);                                   // フレーム(秒)
    const steady_clock::time_point startTime = steady_clock::now(); // 開始時刻 (時刻単位)
    steady_clock::time_point exceLastTime = steady_clock::now();    // 前回時刻 (時刻単位)
    steady_clock::time_point currntTime = steady_clock::now();      // 現在時刻 (時刻単位)

    MSG msg{};//メッセージ

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

        if (deltaTime >= Frame)//設定した間隔が空いたら
        {// 更新
            exceLastTime = currntTime; //今回の更新時刻を記録

            // デバック表示
            m_pDebugProc->Print("FPS:%f", 1.0f / deltaTime);
            m_pDebugProc->Print("終了:[ESC]");
            m_pDebugProc->Print("フルスクリーン:[F11]");
            m_pDebugProc->Print(CDebugProc::MODE::Window, "経過時間:%f", elapsedTime);
            m_pDebugProc->Print(CDebugProc::MODE::Window, "フレーム時間:%f", deltaTime);

            if (m_isRunning)
            {// アプリケーションが実行中ならば
                // 更新共有情報
                context::UpdateContext updateContext{};
                updateContext.pAppContext = &m_appContext;
                updateContext.elapsedTime = elapsedTime;
                updateContext.deltaTime = deltaTime;

                m_pManager->Update(updateContext); //更新処理
            }

            // 描画共有情報
            context::DrawContext drawContext{};
            drawContext.pAppContext = &m_appContext;

            // 描画処理
            if (FAILED(m_pManager->Draw(drawContext))) { return E_FAIL; }
        }
    }
}

//----------------------
// カスタム設定のロード
//----------------------
bool Application::CustomLoad(void)
{
    // YAMLファイルの読み込み
    CFile* pFile = new CFile("custom.yaml");
    YAML::Node app = pFile->ReadYaml("Application");

    // カスタム設定
    m_fps = app["fps"].as<float>(); // フレームレート

    return true; // YAMLの読み込みに成功した場合
}
