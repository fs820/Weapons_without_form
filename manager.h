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

// テクスチャタグ
enum class TEXTAG : Index
{
	Number,           // 数字
	Ground,           // 地面
	Enemy1, Enemy2,   // 敵1, 敵2
	Bullet,           // 弾
	EX1, EX2,         // 爆発1, 爆発2
	Shadow_Effect,    // 影エフェクト
	Max               // 最大数
};

// モデルタグ
enum class MODELTAG : Index
{
	Test, // テストモデル
	// プレイヤーのモデル
	PBody,
	PHead,
	PRArm,
	PRHand,
	PLArm,
	PLHand,
	PRLeg,
	PRFoot,
	PLLeg,
	PLFoot,
	PHat,
	// プレイヤーのモデルEnd
	Max   // 最大数
};

// モーションタグ
enum class MOTIONTAG : Index
{
	Player, // プレイヤー
	Max   // 最大数
};

// 階層構造タグ
enum class HIERARCHYTAG : Index
{
	Player, // プレイヤー
	Max   // 最大数
};

//----------------------------
// マネージャークラス
//----------------------------
class CManager
{
// 公開
public:
	CManager() = default;
	~CManager() = default;

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	HRESULT Draw(void);

	static const CRenderer& GetRenderer(void) { return *m_pRenderer; }
	static HRESULT RenererReset(const BOOL bWindow);

	static const CObject3D& Get3D(void) { return *m_p3DObject; }

	static CCamera* const GetCamera(void) { return m_pCamera; }
	static CLight* const GetLight(void) { return m_pLight; }
	static CTime* const GetTime(void) { return m_pTime; }
	static CScore* const GetScore(void) { return m_pScore; }
	static const CGui& GetGui(void) { return *m_pGui; }

	static bool IsPause(void) { return m_bPause; }

// 非公開
private:
	static bool m_bPause;                    // ポーズ状態かどうか

	// ポインタ
	static CRenderer* m_pRenderer;           // レンダラー
	static CFile* m_pFile;                   // ファイル
	static CCamera* m_pCamera;               // カメラ
	static CLight* m_pLight;                 // ライト
	static CPlayer* m_pPlayer;               // プライヤー
	static CObject3D* m_p3DObject;           // 3Dオブジェクト
	static CDebugProc* m_pDebugProc;         // デバック表示
	static CTime* m_pTime;                   // タイム
	static CScore* m_pScore;                 // スコア
	static CGui* m_pGui;                     // Gui

	// テクスチャ
	static const path m_textureFile;                               // ファイルパス
	static const string_view m_textureTagList[Index(TEXTAG::Max)]; // タグリスト

	// モデル
	static const path m_modelFile;                                 // ファイルパス
	static const string_view m_modelTagList[Index(MODELTAG::Max)]; // タグリスト

	// モーション
	static const path m_motionFile;                                  // ファイルパス
	static const string_view m_motionTagList[Index(MOTIONTAG::Max)]; // タグリスト

	// 階層構造
	static const path m_hierarchyFile;                                     // ファイルパス
	static const string_view m_hierarchyTagList[Index(HIERARCHYTAG::Max)]; // タグリスト

	// プレイヤー
	static const path m_playerFile; // ファイルパス

	HRESULT LoadTexture(void);
	HRESULT LoadModel(void);
	HRESULT LoadMotion(void);
	HRESULT LoadHierarchy(void);
	HRESULT LoadGame(void);
	void UnloadGame(void);
	HRESULT StartGame(void);
	void EndGame(void);
};