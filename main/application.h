//------------------------------------------
//
// アプリケーションの定義 [application.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include <memory>
#include "context.h"
#include "common.h"

// 前方宣言
class EventDispatcher; // イベントディスパッチャー
class Window;          // ウインドウ
class CInput;          // 入力デバイス
class CRenderer;       // レンダラー
class CDebugProc;      // デバッグプロセス
class CHierarchy;      // 階層構造
class CSoundManager;   // サウンドマネージャー
class CTextureManager; // テクスチャマネージャー
class CModelManager;   // モデルマネージャー
class CMotionManager;  // モーションマネージャー
class CManager;        // マネージャー

//----------------------------
// アプリケーションクラス
//----------------------------
class Application final
{
// 公開
public:
    Application() : m_hInstance{}, m_appContext{}, m_fps(0.0f), m_isRunning(false) {}
    ~Application() = default;

    int Run(HINSTANCE hInstance, int nCmdShow);

    float GetFPS(void) const { return m_fps; } // フレームレートを取得
    void SetRunning(bool isRunning) { m_isRunning = isRunning; } // アプリケーションの実行状態を設定

// 非公開
private:
    bool Init(int nCmdShow);
    bool Update(void);
    bool CustomLoad(void);

    HINSTANCE m_hInstance;                                                    // インスタンスハンドル
    context::AppContext m_appContext;                                         // アプリケーション全体が依存しているデータ
    float m_fps;                                                              // フレームレート
    bool m_isRunning;                                                         // アプリケーション実行フラグ

    // ポインタ
    std::unique_ptr<EventDispatcher> m_pEvent;                                                  // イベント
    std::unique_ptr<Window, common::UninitDeleter<Window>> m_pWindow;                           // ウインドウ
    std::unique_ptr<CInput, common::UninitDeleter<CInput>> m_pInput;                            // 入力デバイス
    std::unique_ptr<CRenderer, common::UninitDeleter<CRenderer>> m_pRenderer;                   // レンダラー
    std::unique_ptr<CDebugProc, common::UninitDeleter<CDebugProc>> m_pDebugProc;                // デバッグプロセス
    std::unique_ptr<CHierarchy, common::UninitDeleter<CHierarchy>> m_pHierarchy;                // 階層構造
    std::unique_ptr<CSoundManager, common::UninitDeleter<CSoundManager>> m_pSound;              // サウンドマネージャー
    std::unique_ptr<CTextureManager, common::UninitDeleter<CTextureManager>> m_pTextureManager; // テクスチャマネージャー
    std::unique_ptr<CModelManager, common::UninitDeleter<CModelManager>> m_pModelManager;       // モデルマネージャー
    std::unique_ptr<CMotionManager, common::UninitDeleter<CMotionManager>> m_pMotionManager;    // モーションマネージャー
    std::unique_ptr<CManager, common::UninitDeleter<CManager>> m_pManager;                      // マネージャー
};
