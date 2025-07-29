//------------------------------------------
//
// Playerの処理の定義・宣言[player.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "object.h"
#include "hierarchy.h"

class CShadow; // 前方宣言
//--------------------------
// プレイヤークラス (末端)
//--------------------------
class CPlayer final : public CObject
{
// 公開
public:
	// 状態管理
	enum class STATE : Index
	{
		None,     // 無
		Appear,   // 出現
		Normal,   // 通常
		Damage,   // ダメージ
		Death,    // 死
		Max       // 状態数
	};

	CPlayer() : m_mtxWorld{}, m_state{}, m_StateTime{}, m_nLife{}, m_fSpeed{}, m_fRotSpeed{}, m_modelID{}, m_motionID{}, m_pShadow{} {}
	CPlayer(int priority) :CObject(priority), m_mtxWorld{}, m_state{}, m_StateTime{}, m_nLife{}, m_fSpeed{}, m_fRotSpeed{}, m_modelID{}, m_motionID{}, m_pShadow{} {}
	~CPlayer() = default;

	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float fSpeed, float fRotSpeed, int priority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float fSpeed, float fRotSpeed);
	void Uninit(void) override;
	void Update(const context::UpdateContext updateContext) override;
	void Draw(const context::DrawContext drawContext) override;

	int GetLife(void) const { return m_nLife; }
	STATE GetState(void) const { return m_state; }

// 非公開
private:
	void OnCollision(const CObject& other) override;
	void Hit(int damage);

	static constexpr int LIFE = 50;            // ライフ
	static constexpr float APPEAR_TIME = 2.0f; // 出現時間
	static constexpr float DAMAGE_TIME = 0.5f; // ダメージ時間
	static constexpr float DEATH_TIME = 2.0f;  // 死時間
	
	static vector<hierarchy::PartsInfo> m_hierarchy; // 階層構造
	vector<Index> m_modelID;                         // モデルのインデックス
	Index m_motionID;                                // モーションのインデックス

	D3DXMATRIX m_mtxWorld;                  // ワールドマトリックス

	STATE m_state;                          // 状態
	float m_StateTime;                      // 状態タイマー
	int m_nLife;                            // ライフ

	float m_fSpeed;                         // 移動速度
	float m_fRotSpeed;                      // 回転速度

	CShadow* m_pShadow; // シャドウ
};
