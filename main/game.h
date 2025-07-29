//------------------------------------------
//
// ゲームの定義 [game.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "scene.h"

class CPlayer;        // プレイヤー
class CObject3D;      // 3Dオブジェクト
class CScore;         // スコア
class CTime;          // タイム

namespace game
{

}

//-----------------------------
// ゲームクラス
//-----------------------------
class CGame : public CScene
{
// 公開
public:
    CGame() : m_pPlayer{}, m_p3DObject{}, m_pTime{}, m_pScore{}, m_bgmIdx{} {}
	~CGame() = default;

	HRESULT Init(void);
	void Uninit(void) override;
	void Update(const context::UpdateContext updateContext) override;
	void Draw(const context::DrawContext drawContext) override;

	const CPlayer& GetPlayer(void) const { return *m_pPlayer; }       // プレイヤーの取得
	const CObject3D& Get3DObject(void) const { return *m_p3DObject; } // 3Dオブジェクトの取得
	const CTime& GetTime(void) const { return *m_pTime; }             // タイムの取得
	CScore& GetScore(void) const { return *m_pScore; }                // スコアの取得
// 非公開
private:
	void OnCollision(const CObject& other) override {}; // 衝突時の処理は無し

	CPlayer* m_pPlayer;               // プライヤー
	CObject3D* m_p3DObject;           // 3Dオブジェクト
	CTime* m_pTime;                   // タイム
	CScore* m_pScore;                 // スコア

    Index m_bgmIdx; // BGMのインデックス
};
