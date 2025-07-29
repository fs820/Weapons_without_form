//------------------------------------------
//
// ゲーム [game.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "game.h"
#include "object3D.h"
#include "objectX.h"
#include "player.h"
#include "time.h"
#include "score.h"
#include "sound.h"
#include "math.h"
#include "manager.h"

using namespace game; // シーン空間の使用

//-----------------------------
// 
// ゲームクラス
// 
//-----------------------------

//------------------------------
// 初期化処理
//------------------------------
HRESULT CGame::Init(void)
{
	//--------------------
    // インスタンス生成
    //--------------------

	// プレイヤー
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, RandomAngle, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), CMath::Random(100.0f, 500.0f), CMath::Random(-0.01f, 0.1f));
	if (m_pPlayer == nullptr)
	{
		return E_POINTER;
	}

	// タイム
	m_pTime = CTime::Create(D3DXVECTOR3(0.5f, 0.05f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2, 0.03f, 7);
	if (m_pTime == nullptr)
	{
		return E_POINTER;
	}

	// スコア
	m_pScore = CScore::Create(D3DXVECTOR3(0.9f, 0.05f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 5, 0.03f, 7);
	if (m_pScore == nullptr)
	{
		return E_POINTER;
	}

	m_pTime->SetTime(120); // 時間セット
	m_pTime->CountDown(); // カウントダウン

    m_bgmIdx = CSoundManager::GetInstance().Play(Index8(sound::LABEL::TestBgm)); // BGM再生

	return S_OK; // 成功
}

//------------------------------
// 終了処理
//------------------------------
void CGame::Uninit(void)
{
    CSoundManager::GetInstance().Stop(Index8(sound::LABEL::TestBgm), m_bgmIdx); // BGM停止
}

//------------------------------
// 更新処理
//------------------------------
void CGame::Update(const context::UpdateContext updateContext)
{

}

//-------------------------------
// 描画処理
//-------------------------------
void CGame::Draw(const context::DrawContext drawContext)
{

}
