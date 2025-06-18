//------------------------------------------
//
// Bullet処理 [bullet.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "bullet.h"
#include "manager.h"
#include "math.h"
#include "explosion.h"
#include "effect.h"
#include "particle.h"
#include "debug.h"

//---------------------------------------
//
// Bullet
//
//---------------------------------------

// 静的メンバ変数
LPDIRECT3DTEXTURE9 CBullet::m_apTexture[TYPE_MAX] = { nullptr }; // 共有テクスチャのポインタ
D3DXVECTOR2 CBullet::m_aImageSize[TYPE_MAX] = {};             // テクスチャサイズ

//------------------------------
// ソース読み込み
//------------------------------
HRESULT CBullet::Load(const string_view sTexturePass[TYPE_MAX])
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

	m_aImageSize[1] *= 0.2f; // 小さく
	return S_OK;
}

//------------------------------
// ソース破棄
//------------------------------
void CBullet::Unload(void)
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
CBullet* CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float fSpeed, TYPE type, int priority)
{
	CBullet* pBullet = new CBullet(priority); // インスタンス生成

	if (pBullet == nullptr)
	{// 生成失敗
		return nullptr;
	}

	D3DXVECTOR3 imageSize = D3DXVECTOR3(m_aImageSize[type].x, m_aImageSize[type].y, 0.0f); // テクスチャサイズの取得
	pBullet->SetSize(imageSize); // テクスチャサイズの設定

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
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float fSpeed, TYPE type)
{
	CObjectBillboard::Init(pos, rot, scale, BULLET); // 親の初期化

	IsCollision(true); // 当たり判定をする

	// スクリーンサイズの取得
	D3DXVECTOR2 screenSize = {};
	if (FAILED(CManager::GetRenderer().GetDxScreenSize(&screenSize)))
	{
		return E_FAIL;
	}

	m_fSpeed = fSpeed;             // 移動速度
	m_type = type;                 // 種類
	m_fLife = LIFE;                // ライフ
	m_EffectTIme = 0.0f;           // エフェクト時間

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

	resultPos[0] = D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, 0.0f);
	resultPos[1] = D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, 0.0f);
	resultPos[2] = D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, 0.0f);
	resultPos[3] = D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, 0.0f);

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
void CBullet::Uninit(void)
{
	CObjectBillboard::Uninit(); // 親の終了
}

//------------------------------
//更新処理
//------------------------------
void CBullet::Update(void)
{
	if (m_fLife <= 0.0f)
	{// ライフが0以下ならば爆発して削除
		Transform transform = GetTransform();
		CParticle::Create(transform.pos, transform.rot, transform.scale, 5);
		Release();
		return;
	}

	CObjectBillboard::Update(); // 親の更新

	float deltaTime = CMain::GetDeltaTimeGameSpeed();

	m_fLife -= deltaTime; // ライフを減らす

	CRenderer renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer.GetDevice(); // レンダラーからデバイスを取得

	Transform transform = GetTransform(); // 変形情報の取得
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };
	move.x += m_fSpeed * sinf(transform.rot.z) * deltaTime;
	move.y += m_fSpeed * cosf(transform.rot.z) * deltaTime;
	AddPosition(move);

	transform = GetTransform(); // 変形情報の取得
	if (transform.pos.x < 0.0f || transform.pos.x>1.0f || transform.pos.y < 0.0f || transform.pos.y>1.0f)
	{
		Release();
		return;
	}

	if ((m_EffectTIme += CMain::GetDeltaTimeGameSpeed()) >= EFFECT_TIME)
	{// エフェクト時間がたったら
		// エフェクト生成
		D3DXVECTOR3 EffectSize{};
		EffectSize.x = transform.scale.x * GetSize().x;
		EffectSize.y = transform.scale.y * GetSize().y;
		CEffect::Create(transform.pos, transform.rot, EffectSize * 0.03f, 1);

		m_EffectTIme = 0.0f; // 戻す
	}

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

	// 回転関係処理y
	D3DXVECTOR3 screenPos = D3DXVECTOR3(transform.pos.x * screenSize.x, transform.pos.y * screenSize.y, 0.0f);

	D3DXVECTOR3 resultPos[VT_DEF] = {};
	D3DXVECTOR3 size = GetSize(); // サイズの取得
	size.x *= transform.scale.x;
	size.y *= transform.scale.y;

	resultPos[0] = D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, 0.0f);
	resultPos[1] = D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, 0.0f);
	resultPos[2] = D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, 0.0f);
	resultPos[3] = D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, 0.0f);

	// 頂点情報の設定
	for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
	{
		resultPos[cntVtx] += screenPos;

		pVtx[cntVtx].pos = resultPos[cntVtx];
		pVtx[cntVtx].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[cntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[cntVtx].tex = D3DXVECTOR2((float)(cntVtx % 2) * 1.0f, (float)(cntVtx / 2) * 1.0f);
	}

	if (FAILED(pVtxBuff->Unlock())) { return; }
}

//------------------------------
//描画処理
//------------------------------
void CBullet::Draw(void)
{
	CObjectBillboard::Draw(); // 親の描画
}

//------------------------------
// 衝突処理
//------------------------------
void CBullet::OnCollision(const CObject& other)
{
	CObject::TYPE Type = other.GetType();
	if ((m_type == CBullet::PLAYER && Type == CObject::ENEMY) || (m_type == CBullet::ENEMY && Type == CObject::PLAYER))
	{// 当たった場合
		Hit(); // 敵に当たった処理

		CDebugProc::Print(CDebugProc::OBJECT, "BulletHit 撃った人:%d", m_type);
		CDebugProc::Print(CDebugProc::OBJECT, "BulletHit 当たった人:%d", Type);
	}
}