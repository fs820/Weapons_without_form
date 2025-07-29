//------------------------------------------
//
// コンテキストの定義 [context.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once

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
struct HINSTANCE__;

// コンテキスト空間
namespace context
{
    // アプリケーションコンテキスト
    struct AppContext
    {
        const EventDispatcher* pEvent;          // イベントディスパッチャーへのポインタ
        const Window* pWindow;                  // ウインドウへのポインタ
        const CInput* pInput;                   // 入力デバイスへのポインタ
        const CRenderer* pRenderer;             // レンダラーへのポインタ
        const CDebugProc* pDebug;               // デバッグプロセスへのポインタ
        const CHierarchy* pHierarchy;           // 階層構造へのポインタ
        const CSoundManager* pSound;            // サウンドマネージャーへのポインタ
        const CTextureManager* pTextureManager; // テクスチャマネージャーへのポインタ
        const CModelManager* pModelManager;     // モデルマネージャーへのポインタ
        const CMotionManager* pMotionManager;   // モーションマネージャーへのポインタ

        AppContext(const EventDispatcher* event = nullptr, const Window* window = nullptr, const CInput* input = nullptr,
                    const CRenderer* renderer = nullptr, const CDebugProc* debug = nullptr, const CHierarchy* hierarchy = nullptr,
                    const CSoundManager* sound = nullptr, const CTextureManager* textureManager = nullptr,
                    const CModelManager* modelManager = nullptr, const CMotionManager* motionManager = nullptr)
            : pEvent(event), pWindow(window), pInput(input), pRenderer(renderer), pDebug(debug),
            pHierarchy(hierarchy), pSound(sound), pTextureManager(textureManager), pModelManager(modelManager), pMotionManager(motionManager) {}
        ~AppContext() = default;
    };

    // 初期化コンテキスト
    struct InitContext
    {
        const AppContext* pAppContext; // アプリケーションコンテキストへのポインタ
        HINSTANCE__* instance;         // インスタンスハンドル
        int nCmdShow;                  // ウインドウの表示状態

        InitContext(const AppContext* appContext = nullptr, HINSTANCE__* instance = nullptr, int cmdShow = 0)
            : pAppContext(appContext), instance(instance), nCmdShow(cmdShow) {
        }
        ~InitContext() = default;
    };

    // 更新コンテキスト
    struct UpdateContext
    {
        const AppContext* pAppContext; // アプリケーションコンテキストへのポインタ
        float elapsedTime;             // 実行時間
        float deltaTime;               // フレーム時間

        UpdateContext(const AppContext* appContext = nullptr, float elapsedTime = 0.0f, float deltaTime = 0.0f) : pAppContext(appContext), elapsedTime(elapsedTime), deltaTime(deltaTime) {}
        ~UpdateContext() = default;
    };

    // 描画コンテキスト
    struct DrawContext
    {
        const AppContext* pAppContext; // アプリケーションコンテキストへのポインタ

        DrawContext(const AppContext* appContext = nullptr) : pAppContext(appContext) {}
        ~DrawContext() = default;
    };
}
