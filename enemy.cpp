//------------------------------------------
//
// enemy処理 [enemy.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "enemy.h"
#include "renderer.h"
#include "manager.h"
#include "math.h"
#include "explosion.h"
#include "particle.h"
#include "bullet.h"
#include "debug.h"
#include "sound.h"
#include "score.h"
#include "player.h"

//---------------------------------------
//
// Enemy
//
//---------------------------------------

// 静的メンバ変数
LPDIRECT3DTEXTURE9 CEnemy::m_apTexture[TYPE_MAX] = { nullptr }; // 共有テクスチャのポインタ
D3DXVECTOR2 CEnemy::m_aImageSize[TYPE_MAX] = {};             // テクスチャサイズ
int CEnemy::m_nNumAll = 0;                                   // 出現数

//------------------------------
// ソース読み込み
//------------------------------
HRESULT CEnemy::Load(const string_view sTexturePass[TYPE_MAX])
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();

	// テクスチャ
	for (size_t cntTex = 0; cntTex < TYPE_MAX; cntTex++)
	{
		D3DXIMAGE_INFO imageInfo = {};
		if (FAILED(D3DXGetImageInfoFromFile
		(
			sTexturePass[cntTex].data(),
			&imageInfo
		)))
		{
			return E_FAIL;
		}
		m_aImageSize[cntTex] = D3DXVECTOR2((float)imageInfo.Width, (float)imageInfo.Height);

		if (FAILED(D3DXCreateTextureFromFile
		(
			pDevice,
			sTexturePass[cntTex].data(),
			&m_apTexture[cntTex]
		)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//------------------------------
// ソース破棄
//------------------------------
void CEnemy::Unload(void)
{
	// テクスチャ
	for (size_t cntTex = 0; cntTex < TYPE_MAX; cntTex++)
	{
		if (m_apTexture[cntTex] != nullptr)
		{
			m_apTexture[cntTex]->Release();
			m_apTexture[cntTex] = nullptr;
		}
	}
}

//------------------------------
// 生成
//------------------------------
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float fSpeed, TYPE type, int priority)
{
	CEnemy* pBullet = new CEnemy(priority); // インスタンス生成

	if (pBullet == nullptr)
	{// 生成失敗
		return nullptr;
	}

	pBullet->SetSize(D3DXVECTOR3(m_aImageSize[type].x, m_aImageSize[type].y, 0.0f)); // サイズの設定

	// 初期化
	if (FAILED(pBullet->Init(pos, rot, scale, fSpeed, type)))
	{
		delete pBullet;
		pBullet = nullptr;
		return nullptr;
	}

	return pBullet;
}

//------------------------------
//初期化処理
//------------------------------
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float fSpeed, TYPE type)
{
	CObjectBillboard::Init(pos, rot, scale, ENEMY); // 親の初期化

	IsCollision(true); // 当たり判定をする	

	// スクリーンサイズの取得
	D3DXVECTOR2 screenSize = {};
	if (FAILED(CManager::GetRenderer().GetDxScreenSize(&screenSize)))
	{
		return E_FAIL;
	}

	m_nLife = LIFE;                // ライフの初期化
	m_state = NORMAL;              // 初期化
	m_StateTime = 0.0f;  // 出現時間
	m_AttackTime = 0.0f; // 攻撃時間

	m_fSpeed = fSpeed;             // ポリゴンの移動速度
	m_type = type;                 // 敵の種類

	// 頂点情報の設定
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	Vertex* pVtx;
	if (FAILED(pVtxBuff->Lock(0, 0, (void**)&pVtx, 0))) { return E_FAIL; }

	Transform transform = GetTransform();
	D3DXVECTOR3 screenPos = D3DXVECTOR3(transform.pos.x * screenSize.x, transform.pos.y * screenSize.y, 0.0f);

	D3DXVECTOR3 resultPos[VT_DEF] = {};
	D3DXVECTOR3 size = GetSize(); // サイズの取得
	size.x *= transform.scale.x;
	size.y *= transform.scale.y;
	CMath::Rotation(resultPos, D3DXVECTOR2(size.x, size.y), GetRotation().z);

	for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
	{
		resultPos[cntVtx] += screenPos;

		pVtx[cntVtx].pos = resultPos[cntVtx];
		pVtx[cntVtx].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[cntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[cntVtx].tex = D3DXVECTOR2((float)(cntVtx % 2) * 1.0f, (float)(cntVtx / 2) * 1.0f);
	}

	if (FAILED(pVtxBuff->Unlock())) { return E_FAIL; }

	return S_OK;
}

//------------------------------
//終了処理
//------------------------------
void CEnemy::Uninit(void)
{
	CObjectBillboard::Uninit(); // 親の終了
}

//------------------------------
//更新処理
//------------------------------
void CEnemy::Update(void)
{
	CObjectBillboard::Update(); // 親の更新

	CRenderer renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer.GetDevice(); // レンダラーからデバイスを取得

	float deltaTime = CMain::GetDeltaTimeGameSpeed();

	// スクリーンサイズの取得
	D3DXVECTOR2 screenSize = {};
	if (FAILED(renderer.GetDxScreenSize(&screenSize)))
	{
		return;
	}

	// 頂点情報の設定
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	Vertex* pVtx;
	if (FAILED(pVtxBuff->Lock(0, 0, (void**)&pVtx, 0))) { return; }

	Transform transform = GetTransform(); // 変形情報の取得

	// 回転関係処理
	D3DXVECTOR3 screenPos = D3DXVECTOR3(transform.pos.x * screenSize.x, transform.pos.y * screenSize.y, 0.0f);
	D3DXVECTOR3 resultPos[VT_DEF] = {};

	D3DXVECTOR3 size = GetSize(); // テクスチャサイズの取得
	size.x *= transform.scale.x;
	size.y *= transform.scale.y;
	CMath::Rotation(resultPos, D3DXVECTOR2(size.x, size.y), transform.rot.z);

	// 頂点情報の設定
	for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
	{
		resultPos[cntVtx] += screenPos;

		pVtx[cntVtx].pos = resultPos[cntVtx];
		pVtx[cntVtx].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[cntVtx].col = m_state == DAMAGE ? RED : WHITE; // ダメージ状態は赤
		pVtx[cntVtx].tex = D3DXVECTOR2((float)(cntVtx % 2) * 1.0f, (float)(cntVtx / 2) * 1.0f);
	}

	if (FAILED(pVtxBuff->Unlock())) { return; }

	// 状態管理
	switch (m_state)
	{
	case NONE:
		break;
	case NORMAL:
		if ((m_AttackTime += CMain::GetDeltaTimeGameSpeed()) >= ATTACK_TIME)
		{// 一定時間経過
			D3DXVECTOR3 playerPos{};
			D3DXVECTOR3 space = playerPos - GetPosition();
			float angle = atan2f(space.x, space.y);
			CMath::NormalizeAngle(&angle);
			CBullet::Create(transform.pos, D3DXVECTOR3(0.0f, 0.0f, angle), transform.scale, CMath::Random(0.05f, 0.5f), CBullet::ENEMY, 2);

			m_AttackTime = 0.0f; // 戻す
		}
		break;
	case DAMAGE:
		if ((m_StateTime += CMain::GetDeltaTimeGameSpeed()) >= DAMAGE_TIME)
		{// 一定時間経過
			m_state = NORMAL; // 通常状態
			m_StateTime = 0.0f; // 戻す
		}
		break;
	case DEATH:
		if ((m_StateTime += CMain::GetDeltaTimeGameSpeed()) >= DEATH_TIME)
		{// 一定時間経過
			Release();
			return;
		}
		break;
	}

	CDebugProc::Print(CDebugProc::OBJECT, "Enemy%d:%d", GetID(), m_state);
	CDebugProc::Print(CDebugProc::OBJECT, "Enemy%d 状態カウンター:%f", GetID(), m_StateTime);
	CDebugProc::Print(CDebugProc::OBJECT, "Enemy%d 攻撃カウンター:%f", GetID(), m_AttackTime);
}

//------------------------------
//描画処理
//------------------------------
void CEnemy::Draw(void)
{
	CObjectBillboard::Draw(); // 親の描画
}

//------------------------------
// 衝突処理
//------------------------------
void CEnemy::OnCollision(const CObject& other)
{

}

//--------------------------
// ダメージ処理
//--------------------------
void CEnemy::Hit(int damage)
{
	if (m_state == NORMAL || m_state == DAMAGE)
	{// 無敵時間でない
		m_nLife -= damage; // ダメージ
		if (m_nLife <= 0)
		{// 体力がなくなった
			m_state = DEATH; // 死

			Transform transform = GetTransform();
			CExplosion::Create(transform.pos, transform.rot, transform.scale * 50.0f, CMath::Random(CExplosion::DEFAULT, CExplosion::EXTRA), 4); // 爆発エフェクトを生成

			CManager::GetScore()->AddScore(10);
		}
		else
		{
			m_state = DAMAGE; // ダメージ
			CManager::GetScore()->AddScore(1);
		}
		m_StateTime = 0.0f; // 戻す
	}
}