//---------------------------------------
//
//�p�[�e�B�N���\�������̒�`�E�錾[particle.h]
//Author fuma sato
//
//---------------------------------------
#pragma once

#include "main.h"
#include "object.h"

//--------------------------
// Particle (���[)
//--------------------------
class CParticle final : public CObject
{
// ���J
public:
	CParticle() : m_fLife{}, m_fInterval{} {};
	CParticle(int priority) : CObject(priority), m_fLife{}, m_fInterval{} {}
	~CParticle() = default;

	static CParticle* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int priority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale);
	void Uninit(void) override {};
	void Update(void) override;
	void Draw(void) override {};

// ����J
private:
	void OnCollision(const CObject& other) override {};

	static constexpr float LIFE = 0.5f;       // ��������
	static constexpr float INTERVAL = 0.002f; // �����Ԋu
	static constexpr int NUMBER = 1;          // ��x�ɏo����

	float m_fLife;
	float m_fInterval;
};