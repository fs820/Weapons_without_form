//------------------------------------------
//
// タイム処理[time.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "time.h"
#include "renderer.h"
#include "manager.h"
#include "number.h"

//---------------------------------------
//
// タイムクラス
//
//---------------------------------------

//------------------------------
// 生成
//------------------------------
CTime* CTime::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, size_t digits, float space, int priority)
{
	CTime* pTime = new CTime(digits, space, priority); // インスタンス生成

	if (pTime == nullptr)
	{// 生成失敗
		return nullptr;
	}

	// 初期化
	if (FAILED(pTime->Init(pos, rot, scale)))
	{
		delete pTime;
		pTime = nullptr;
		return nullptr;
	}

	return pTime;
}

//------------------------------
// 生成
//------------------------------
CTime* CTime::Create(Transform transform, size_t digits, float space, int priority)
{
	CTime* pTime = new CTime(digits, space, priority); // インスタンス生成

	if (pTime == nullptr)
	{// 生成失敗
		return nullptr;
	}

	// 初期化
	if (FAILED(pTime->Init(transform)))
	{
		delete pTime;
		pTime = nullptr;
		return nullptr;
	}

	return pTime;
}

//------------------------------
// 初期化処理
//------------------------------
HRESULT CTime::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	CObject::Init(TYPE::Score); // 親クラスの初期化

	CRenderer renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer.GetDevice(); // レンダラーからデバイスを取得

	if (pDevice == nullptr)
	{
		return E_POINTER;
	}

	m_fTime = 0.0f; // 時間
	m_count = NONE; // カウント状態

	// スクリーンサイズの取得
	D3DXVECTOR2 screenSize = {};
	if (FAILED(renderer.GetDxScreenSize(&screenSize)))
	{
		return E_FAIL;
	}

	SetTransform(Transform(pos, rot, scale));

	for (size_t cntDigits = 0; cntDigits < m_digits; cntDigits++)
	{// それぞれの桁の数字
		if (m_apNumber[cntDigits] == nullptr)
		{// なければ
			m_apNumber[cntDigits] = CNumber::Create(pos + D3DXVECTOR3(m_space * (float(m_digits) / 2.0f) - float(cntDigits) * m_space, 0.0f, 0.0f), rot, scale, 0);
		}
	}

	return S_OK;
}

//------------------------------
// 初期化処理
//------------------------------
HRESULT CTime::Init(Transform transform)
{
	CObject::Init(TYPE::Score); // 親クラスの初期化

	CRenderer renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer.GetDevice(); // レンダラーからデバイスを取得

	if (pDevice == nullptr)
	{
		return E_POINTER;
	}

	m_fTime = 0.0f; // 時間
	m_count = NONE; // カウント状態

	// スクリーンサイズの取得
	D3DXVECTOR2 screenSize = {};
	if (FAILED(renderer.GetDxScreenSize(&screenSize)))
	{
		return E_FAIL;
	}

	SetTransform(transform);

	for (size_t cntDigits = 0; cntDigits < m_digits; cntDigits++)
	{// それぞれの桁の数字
		if (m_apNumber[cntDigits] == nullptr)
		{// なければ
			m_apNumber[cntDigits] = CNumber::Create(transform.pos + D3DXVECTOR3(m_space * (float(m_digits) / 2.0f) - float(cntDigits) * m_space, 0.0f, 0.0f), transform.rot, transform.scale, 0);
		}
	}

	return S_OK;
}

//------------------------------
// 終了処理
//------------------------------
void CTime::Uninit(void)
{
	for (size_t cntDigits = 0; cntDigits < m_digits; cntDigits++)
	{// それぞれの桁の数字
		if (m_apNumber[cntDigits] != nullptr)
		{// あれば
			m_apNumber[cntDigits]->Uninit(); // 終了
			delete[] m_apNumber[cntDigits];  // 破棄
			m_apNumber[cntDigits] = nullptr; // null
		}
	}
}

//------------------------------
// 更新処理
//------------------------------
void CTime::Update(void)
{
	// カウント状態
	switch (m_count)
	{
		// カウントしない
	case NONE:
		break;
		// カウントアップ
	case UP:
	{
		m_fTime += CMain::GetDeltaTimeGameSpeed();

		m_fTime = clamp(m_fTime, 0.0f, FLT_MAX);

		int aScore[64]{};
		CNumber::Digit(aScore, int(floorf(m_fTime)), m_digits);

		for (size_t cntDigits = 0; cntDigits < m_digits; cntDigits++)
		{// それぞれの桁の数字
			if (m_apNumber[cntDigits] != nullptr)
			{// あれば
				m_apNumber[cntDigits]->SetNumber(aScore[cntDigits]);
			}
		}
		break;
	}
	// カウントダウン
	case DOWN:
	{
		m_fTime -= CMain::GetDeltaTimeGameSpeed();

		m_fTime = clamp(m_fTime, 0.0f, FLT_MAX);

		int aScore[64]{};
		CNumber::Digit(aScore, int(floorf(m_fTime)), m_digits);

		for (size_t cntDigits = 0; cntDigits < m_digits; cntDigits++)
		{// それぞれの桁の数字
			if (m_apNumber[cntDigits] != nullptr)
			{// あれば
				m_apNumber[cntDigits]->SetNumber(aScore[cntDigits]);
			}
		}

		if (m_fTime <= 0.0f) m_count = NONE; // 0になったらカウントダウン終了
		break;
	}
	}
}

//------------------------------
// 描画処理
//------------------------------
void CTime::Draw(void)
{
	for (size_t cntDigits = 0; cntDigits < m_digits; cntDigits++)
	{// それぞれの桁の数字
		if (m_apNumber[cntDigits] != nullptr)
		{// あれば
			m_apNumber[cntDigits]->Draw(); // 描画
		}
	}
}

//------------------------------
// セット処理
//------------------------------
void CTime::SetTime(const float time)
{
	m_fTime = time;

	m_fTime = clamp(m_fTime, 0.0f, FLT_MAX);

	int aScore[64]{};
	CNumber::Digit(aScore, int(floorf(m_fTime)), m_digits);

	for (size_t cntDigits = 0; cntDigits < m_digits; cntDigits++)
	{// それぞれの桁の数字
		if (m_apNumber[cntDigits] != nullptr)
		{// あれば
			m_apNumber[cntDigits]->SetNumber(aScore[cntDigits]);
		}
	}
}

//------------------------------
// 加算処理
//------------------------------
void CTime::AddTime(const float time)
{
	m_fTime += time;

	m_fTime = clamp(m_fTime, 0.0f, FLT_MAX);

	int aScore[64]{};
	CNumber::Digit(aScore, int(floorf(m_fTime)), m_digits);

	for (size_t cntDigits = 0; cntDigits < m_digits; cntDigits++)
	{// それぞれの桁の数字
		if (m_apNumber[cntDigits] != nullptr)
		{// あれば
			m_apNumber[cntDigits]->SetNumber(aScore[cntDigits]);
		}
	}
}