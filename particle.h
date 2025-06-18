//---------------------------------------
//
//パーティクル表示処理の定義・宣言[particle.h]
//Author fuma sato
//
//---------------------------------------
#pragma once

#include "main.h"
#include "object.h"

//--------------------------
// Particle (末端)
//--------------------------
class CParticle final : public CObject
{
// 公開
public:
	CParticle() : m_fLife{}, m_fInterval{} {};
	CParticle(int priority) : CObject(priority), m_fLife{}, m_fInterval{} {}
	~CParticle() = default;

	static CParticle* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int priority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale);
	void Uninit(void) override {};
	void Update(void) override;
	void Draw(void) override {};

// 非公開
private:
	void OnCollision(const CObject& other) override {};

	static constexpr float LIFE = 0.5f;       // 持続時間
	static constexpr float INTERVAL = 0.002f; // 発生間隔
	static constexpr int NUMBER = 1;          // 一度に出す数

	float m_fLife;
	float m_fInterval;
};