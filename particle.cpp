//---------------------------------------
//
// Particle処理[particle.cpp]
// Author: fuma sato
//
//---------------------------------------
#include "particle.h"
#include "effect.h"
#include "manager.h"
#include "math.h"

//---------------------------------------
//
// Particle
//
//---------------------------------------

//------------------------------
// 生成
//------------------------------
CParticle* CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int priority)
{
	CParticle* pParticle = new CParticle(priority); // インスタンス生成

	if (pParticle == nullptr)
	{// 生成失敗
		return nullptr;
	}

	// 初期化
	if (FAILED(pParticle->Init(pos, rot, scale)))
	{
		delete pParticle;
		pParticle = nullptr;
		return nullptr;
	}

	return pParticle;
}

//------------------------------
//初期化処理
//------------------------------
HRESULT CParticle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	SetTransform(Transform(pos, rot, scale));
	m_fLife = LIFE;
	m_fInterval = INTERVAL;

	return S_OK;
}

//------------------------------
//更新処理
//------------------------------
void CParticle::Update(void)
{
	if (m_fInterval >= INTERVAL)
	{
		Transform transform = GetTransform();
		transform.scale *= m_fLife / LIFE;

		for (int nCnt = 0; nCnt < NUMBER; nCnt++)
		{
			CEffect::Create(transform.pos, D3DXVECTOR3(0.0f, 0.0f, RandomAngle), transform.scale * CMath::Random(0.5f, 10.0f), GetPriority(), RandomColorTranslucent, CMath::Random(0.01f, 1.5f));
		}

		m_fInterval = 0.0f;
	}

	m_fInterval += CMain::GetDeltaTimeGameSpeed();;
	m_fLife -= CMain::GetDeltaTimeGameSpeed();

	if (m_fLife <= 0.0f)
	{
		SetRelease(true);
	}
}