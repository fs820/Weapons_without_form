//------------------------------------------
//
// スコア処理[score.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "score.h"
#include "renderer.h"
#include "manager.h"
#include "number.h"

//---------------------------------------
//
// スコアクラス
//
//---------------------------------------

//------------------------------
// 生成
//------------------------------
CScore* CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, size_t digits, float space, int priority)
{
	CScore* pScore = new CScore(digits, space, priority); // インスタンス生成

	if (pScore == nullptr)
	{// 生成失敗
		return nullptr;
	}

	// 初期化
	if (FAILED(pScore->Init(pos, rot, scale)))
	{
		delete pScore;
		pScore = nullptr;
		return nullptr;
	}

	return pScore;
}

//------------------------------
// 生成
//------------------------------
CScore* CScore::Create(Transform transform, size_t digits, float space, int priority)
{
	CScore* pScore = new CScore(digits, space, priority); // インスタンス生成

	if (pScore == nullptr)
	{// 生成失敗
		return nullptr;
	}

	// 初期化
	if (FAILED(pScore->Init(transform)))
	{
		delete pScore;
		pScore = nullptr;
		return nullptr;
	}

	return pScore;
}

//------------------------------
// 初期化処理
//------------------------------
HRESULT CScore::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	CObject::Init(TYPE::Score); // 親クラスの初期化

	CRenderer renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer.GetDevice(); // レンダラーからデバイスを取得

	if (pDevice == nullptr)
	{
		return E_POINTER;
	}

	m_nScore = 0;

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
HRESULT CScore::Init(Transform transform)
{
	CObject::Init(TYPE::Score); // 親クラスの初期化

	CRenderer renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer.GetDevice(); // レンダラーからデバイスを取得

	if (pDevice == nullptr)
	{
		return E_POINTER;
	}

	m_nScore = 0;

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
void CScore::Uninit(void)
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
void CScore::Update(void)
{

}

//------------------------------
// 描画処理
//------------------------------
void CScore::Draw(void)
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
void CScore::SetScore(const int score)
{
	m_nScore = score;

	m_nScore = clamp(m_nScore, 0, INT_MAX);

	int aScore[64]{};
	CNumber::Digit(aScore, m_nScore, m_digits);

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
void CScore::AddScore(const int score)
{
	m_nScore += score;

	m_nScore = clamp(m_nScore, 0, INT_MAX);

	int aScore[64]{};
	CNumber::Digit(aScore, m_nScore, m_digits);

	for (size_t cntDigits = 0; cntDigits < m_digits; cntDigits++)
	{// それぞれの桁の数字
		if (m_apNumber[cntDigits] != nullptr)
		{// あれば
			m_apNumber[cntDigits]->SetNumber(aScore[cntDigits]);
		}
	}
}