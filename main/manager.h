//------------------------------------------
//
// マネージャーの定義 [manager.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once

#include "scene.h"   // シーン
#include "context.h" // コンテキスト
#include "common.h"  // 共通ヘッダ

class CCamera;        // カメラ
class CLight;         // ライト
class CGui;           // Gui

// テクスチャタグ
enum class TEXTAG : common::Index8
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
enum class MODELTAG : common::Index
{
	Test,    // テストモデル
	Shadow, // 影モデル
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
enum class MOTIONTAG : common::Index
{
	Player, // プレイヤー
	Max   // 最大数
};

// 階層構造タグ
enum class HIERARCHYTAG : common::Index
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
    CManager() : m_pScene{}, m_gameSpeed{}, m_bPause{}, m_pGui{}, m_pCamera{}, m_pLight{} {};
	~CManager() = default;

    CManager(const CManager&) = delete;
    CManager& operator=(const CManager&) = delete;
    CManager(const CManager&&) = default;
    CManager& operator=(const CManager&&) = default;

	HRESULT Init(const context::InitContext initContext);
	void Uninit(void);
	void Update(const context::UpdateContext updateContext);
	scene::MODE GetMode(void) { return m_pScene->GetMode(); }
    void SetPause(bool bPause) { m_bPause = bPause; }
    bool IsPause(void) const { return m_bPause; }

// 非公開
private:
	HRESULT SetMode(scene::MODE mode);

	CScene* m_pScene;                 // シーン
    float m_gameSpeed;                // ゲームスピード
	bool m_bPause;                    // ポーズ状態かどうか

	// ポインタ
	CGui* m_pGui;              // Gui
	CCamera* m_pCamera;        // カメラ
	CLight* m_pLight;          // ライト
};
