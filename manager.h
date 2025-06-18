//------------------------------------------
//
// マネージャーの定義 [manager.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once

#include "main.h"

class CRenderer;      // レンダラー
class CFile;          // ファイル
class CDebugProc;     // デバック表示
class CCamera;        // カメラ
class CLight;         // ライト
class CPlayer;        // プレイヤー
class CObject3D;      // 3Dオブジェクト
class CScore;         // スコア
class CTime;          // タイム
class CGui;           // Gui

//----------------------------
// マネージャークラス
//----------------------------
class CManager
{
// 公開
public:
	// テクスチャタグ
	using TEXTAG = enum
	{
		Number,
		Enemy1, Enemy2,
		Bullet1, Bullet2,
		EX1, EX2,
		Shadow_Effect
	};

	CManager() = default;
	~CManager() = default;

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Update(void);
	void Draw(void);
	void Uninit(void);

	static const CRenderer& GetRenderer(void) { return *m_pRenderer; }
	static HRESULT RenererReset(const BOOL bWindow);

	static HRESULT GameLoad(void);
	static HRESULT GameStart(void);
	static void GameEnd(void);
	static void GameUnload(void);

	static const CObject3D& Get3D(void) { return *m_p3DObject; }

	static CCamera* const GetCamera(void) { return m_pCamera; }
	static CLight* const GetLight(void) { return m_pLight; }
	static CTime* const GetTime(void) { return m_pTime; }
	static CScore* const GetScore(void) { return m_pScore; }
	static const CGui& GetGui(void) { return *m_pGui; }

	static bool IsPause(void) { return m_bPause; }

// 非公開
private:
	static const path m_textureFile; // テクスチャリスト
	static const string_view m_textureTagList[]; // 使いテクスチャを指定 (txtのタグを基にする)

	static CRenderer* m_pRenderer;           // レンダラーのポインタ
	static CFile* m_pFile;                   // ファイルのポインタ
	static CCamera* m_pCamera;               // カメラのポインタ
	static CLight* m_pLight;                 // ライトのポインタ
	static CPlayer* m_pPlayer;               // プライヤーのポインタ
	static CObject3D* m_p3DObject;           // 3Dオブジェクトのポインタ
	static CDebugProc* m_pDebugProc;         // デバック表示のポインタ
	static CTime* m_pTime;                   // タイムのポインタ
	static CScore* m_pScore;                 // スコアのポインタ
	static CGui* m_pGui;                     // Guiのポインタ

	static bool m_bPause;                    // ポーズ状態かどうか
};