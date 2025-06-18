//------------------------------------------
//
// プレイヤーの処理 [player.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "player.h"
#include "renderer.h"
#include "manager.h"
#include "math.h"
#include "bullet.h"
#include "explosion.h"
#include "input.h"
#include "particle.h"
#include "debug.h"
#include "sound.h"
#include "score.h"
#include "enemy.h"
#include "texture.h"
#include "model.h"
#include "object3D.h"

//---------------------------------------
//
// プレイヤークラス
//
//---------------------------------------

// 静的メンバ変数
Index* CPlayer::m_modelID{};                       // モデルのインデックス
size_t CPlayer::m_modelNum{};                      // モデルの数

//-----------------------------
// ソース読み込み
//-----------------------------
HRESULT CPlayer::Load(const size_t modelNum, const string_view sTexturePass[], const D3DXVECTOR3 offSet[], const int modelParentIdx[])
{
	m_modelNum = modelNum;
	m_modelID = new Index[m_modelNum];

	if (offSet == nullptr)return E_POINTER;

	for (size_t cntModel = 0; cntModel < m_modelNum; cntModel++)
	{
		m_modelID[cntModel] = CModelManager::GetInstance().Register(sTexturePass[cntModel], offSet[cntModel]);
	}

	if (modelParentIdx != nullptr)
	{
		for (size_t cntModel = 0; cntModel < m_modelNum; cntModel++)
		{
			CModelManager::GetInstance().SetParent(m_modelID[cntModel], m_modelID[modelParentIdx[cntModel]]);
		}
	}

	return S_OK;
}

//------------------------------
// 生成
//------------------------------
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float fSpeed, float fRotSpeed, int priority)
{
	CPlayer* pPlayer = new CPlayer(priority); // インスタンス生成

	if (pPlayer == nullptr)
	{// 生成失敗
		return nullptr;
	}

	// 初期化
	if (FAILED(pPlayer->Init(pos, rot, scale, fSpeed, fRotSpeed)))
	{
		delete pPlayer;
		pPlayer = nullptr;
		return nullptr;
	}

	return pPlayer;
}

//------------------------------
//初期化処理
//------------------------------
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float fSpeed, float fRotSpeed)
{
	CObject::Init(PLAYER); // 親の初期化

	IsCollision(true); // 当たり判定をする	

	// スクリーンサイズの取得
	D3DXVECTOR2 screenSize = {};
	if (FAILED(CManager::GetRenderer().GetDxScreenSize(&screenSize)))
	{
		return E_FAIL;
	}

	SetTransform(Transform(pos, rot, scale));

	m_state = APPEAR;
	m_StateTime = CMain::GetElapsedTimeGameSpeed();

	m_nLife = LIFE; // プレイヤーのライフ

	m_fSpeed = fSpeed;             // ポリゴンの移動速度
	m_fRotSpeed = fRotSpeed;       // ポリゴンの回転速度

	return S_OK;
}

//------------------------------
//終了処理
//------------------------------
void CPlayer::Uninit(void)
{
	
}

//------------------------------
//更新処理
//------------------------------
void CPlayer::Update(void)
{
	if (m_state != DEATH)
	{// 死んでなければ
		//移動関係
		//CInputKeyboard keyboard = CManager::GetInputKeyboard();
		Transform transform = GetTransform();
		float deltaTime = CMain::GetDeltaTimeGameSpeed();

		D3DXVECTOR3 move = { 0.0f, 0.0f, 0.0f };
		float fAngle = GetRotation().y;

		//if (keyboard.GetPress(DIK_A))//Aキー
		//{
		//	if (keyboard.GetPress(DIK_W))//Wキー
		//	{//左上
		//		move.x += sinf(D3DX_PI * -0.25f) * m_fSpeed * deltaTime;
		//		move.z += cosf(D3DX_PI * -0.25f) * m_fSpeed * deltaTime;
		//		fAngle = D3DX_PI * 0.75f;
		//	}
		//	else if (keyboard.GetPress(DIK_S))//Sキー
		//	{//左下
		//		move.x += sinf(D3DX_PI * -0.75f) * m_fSpeed * deltaTime;
		//		move.z += cosf(D3DX_PI * -0.75f) * m_fSpeed * deltaTime;
		//		fAngle = D3DX_PI * 0.25f;
		//	}
		//	else
		//	{//左
		//		move.x += sinf(D3DX_PI * -0.5f) * m_fSpeed * deltaTime;
		//		move.z += cosf(D3DX_PI * -0.5f) * m_fSpeed * deltaTime;
		//		fAngle = D3DX_PI * 0.5f;
		//	}
		//}
		//else if (keyboard.GetPress(DIK_D))//Dキー
		//{
		//	if (keyboard.GetPress(DIK_W) == true)//Wキー
		//	{//右上
		//		move.x += sinf(D3DX_PI * 0.25f) * m_fSpeed * deltaTime;
		//		move.z += cosf(D3DX_PI * 0.25f) * m_fSpeed * deltaTime;
		//		fAngle = D3DX_PI * -0.75f;
		//	}
		//	else if (keyboard.GetPress(DIK_S) == true)//Sキー
		//	{//右下
		//		move.x += sinf(D3DX_PI * 0.75f) * m_fSpeed * deltaTime;
		//		move.z += cosf(D3DX_PI * 0.75f) * m_fSpeed * deltaTime;
		//		fAngle = D3DX_PI * -0.25f;
		//	}
		//	else
		//	{//右
		//		move.x += sinf(D3DX_PI * 0.5f) * m_fSpeed * deltaTime;
		//		move.z += cosf(D3DX_PI * 0.5f) * m_fSpeed * deltaTime;
		//		fAngle = D3DX_PI * -0.5f;
		//	}
		//}
		//else if (keyboard.GetPress(DIK_W) == true)//Wキー
		//{//上
		//	move.x += sinf(0.0f) * m_fSpeed * deltaTime;
		//	move.z += cosf(0.0f) * m_fSpeed * deltaTime;
		//	fAngle = D3DX_PI;
		//}
		//else if (keyboard.GetPress(DIK_S) == true)//Sキー
		//{//下
		//	move.x += sinf(D3DX_PI) * m_fSpeed * deltaTime;
		//	move.z += cosf(D3DX_PI) * m_fSpeed * deltaTime;
		//	fAngle = 0.0f;
		//}

		move.x += 10.0f * deltaTime; // 重力をかける
		move.z -= 10.0f * deltaTime; // 重力をかける

		move.y += -10.0f * deltaTime; // 重力をかける
		AddPosition(move);

		Vector3 pos = GetPosition();
		SetPosition(D3DXVECTOR3(pos.x, CManager::Get3D().GetHeight(pos), pos.z)); // 移動
		CMath::NormalizeAngle(&fAngle);
		SetRotation(D3DXVECTOR3(0.0f, fAngle, 0.0f));

		//if (keyboard.GetTrigger(DIK_SPACE))
		//{
		//	size_t randomNum = CMath::Random(1, 3);
		//	float angleSet = 2.0f * D3DX_PI / (float)randomNum; // 弾の角度間隔
		//	for (size_t cntBullet = 0; cntBullet < randomNum; cntBullet++)
		//	{
		//		float angle = transform.rot.y - cntBullet * angleSet;
		//		CMath::NormalizeAngle(&angle);
		//		CBullet::Create(transform.pos, D3DXVECTOR3(0.0f, 0.0f, atan2f(sinf(angle), cosf(angle))), transform.scale * 10.0f, CMath::Random(0.05f, 0.5f), CBullet::PLAYER, 2);
		//	}
		//}
	}

	Transform transform = GetTransform();

	// 状態管理
	switch (m_state)
	{
	case NONE:
		break;
	case APPEAR:
		if (CMain::GetElapsedTimeGameSpeed() - m_StateTime >= APPEAR_TIME)
		{// 一定時間経過
			m_state = NORMAL; // 通常状態
			m_StateTime = CMain::GetElapsedTimeGameSpeed(); // 時間記録
		}
		break;
	case NORMAL:
		break;
	case DAMAGE:
		if (CMain::GetElapsedTimeGameSpeed() - m_StateTime >= DAMAGE_TIME)
		{// 一定時間経過
			m_state = NORMAL; // 通常状態
			m_StateTime = CMain::GetElapsedTimeGameSpeed(); // 時間記録
		}
		break;
	case DEATH:
		if (CMain::GetElapsedTimeGameSpeed() - m_StateTime >= DEATH_TIME)
		{// 一定時間経過
			Release();
			return;
		}
		break;
	}

	CDebugProc::Print(CDebugProc::OBJECT, "Player%d:%d", GetID(), m_state);
	CDebugProc::Print(CDebugProc::OBJECT, "Player%d 状態カウンター:%f", GetID(), m_StateTime);
}

//------------------------------
//描画処理
//------------------------------
void CPlayer::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice(); // レンダラーからデバイスを取得

	if (pDevice == nullptr) return;

	Transform transform = GetTransform();
	D3DXMATRIX mtxRot{}, mtxTrans{}, mtxScale{};//計算マトリックス

	//マトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//大きさの反映
	D3DXMatrixScaling(&mtxScale, transform.scale.x, transform.scale.y, transform.scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, transform.rot.y, transform.rot.x, transform.rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置の計算
	D3DXMatrixTranslation(&mtxTrans, transform.pos.x, transform.pos.y, transform.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデルの描画
	for (size_t cntModel = 0; cntModel < m_modelNum; cntModel++)
	{
		CModelManager::GetInstance().Draw(m_modelID[cntModel]);
	}
}

//------------------------------
// 衝突処理
//------------------------------
void CPlayer::OnCollision(const CObject& other)
{

}

//--------------------------
// ダメージ処理
//--------------------------
void CPlayer::Hit(int damage)
{
	m_nLife -= damage; // ダメージ
	if (m_nLife <= 0)
	{// 体力がなくなった
		m_state = DEATH; // 死

		Transform transform = GetTransform();
		CExplosion::Create(transform.pos, transform.rot, transform.scale * 100.0f, CMath::Random(CExplosion::DEFAULT, CExplosion::EXTRA),4); // 爆発エフェクトを生成

		CManager::GetScore()->AddScore(-100);
	}
	else
	{
		m_state = DAMAGE; // ダメージ
		CManager::GetScore()->AddScore(-10);
	}
	m_StateTime = CMain::GetElapsedTimeGameSpeed(); // 時間
}