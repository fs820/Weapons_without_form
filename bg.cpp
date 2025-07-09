//------------------------------------------
//
// プレイヤーの処理 [player.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "bg.h"
#include "renderer.h"
#include "manager.h"
#include "object2D.h"
#include "texture.h"

using namespace DirectX2D; // DirectX2D空間の使用

//---------------------------------------
//
// プレイヤークラス
//
//---------------------------------------

// 静的メンバ変数の定義

// テクスチャのパス
const string_view CBg::m_sTexturePass[CBg::MAX_TEXTURE] =
{
	"data\\TEXTURE\\bg100.png",
	"data\\TEXTURE\\bg101.png",
	"data\\TEXTURE\\bg102.png"
};
const float CBg::m_afScrollSpeed[CBg::MAX_TEXTURE] = { 0.01f, 0.05f, 0.1f }; // スクロール速度

CObject2D* CBg::m_pBg[CBg::MAX_TEXTURE] = { nullptr };      // 背景のポインタ配列
size_t* CBg::textureIdx{};                                  // テクスチャID

//------------------------------
// ソース読み込み
//------------------------------
HRESULT CBg::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();

	textureIdx = new size_t[MAX_TEXTURE];

	// テクスチャ
	for (size_t cntTex = 0; cntTex < MAX_TEXTURE; cntTex++)
	{
		if (m_sTexturePass[cntTex].empty())
		{// テクスチャパスが空なら終了
			break;
		}

		textureIdx[cntTex] = CTextureManager::GetInstance().Register(m_sTexturePass[cntTex].data());
	}

	return S_OK;
}

//------------------------------
// 生成
//------------------------------
CBg* CBg::Create(int priority)
{
	CBg* pBg = new CBg(priority);

	if (FAILED(pBg->Init()))
	{
		delete pBg;
		pBg = nullptr;
		return nullptr;
	}
	return pBg;
}

//------------------------------
//初期化処理
//------------------------------
HRESULT CBg::Init(void)
{
	CRenderer renderer = CManager::GetRenderer();

	CObject::Init(TYPE::BackGround);

	// スクリーンサイズの取得
	D3DXVECTOR2 screenSize = {};
	if (FAILED(renderer.GetViewportSize(&screenSize)))
	{
		return E_FAIL;
	}

	// 背景サイズの取得
	for (size_t cntTex = 0; cntTex < MAX_TEXTURE; cntTex++)
	{
		m_pBg[cntTex] = CObject2D::Create(CTextureManager::GetInstance().GetTexture(textureIdx[cntTex]), D3DXVECTOR3(screenSize.x * 0.5f, screenSize.y * 0.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), TYPE::BackGround, GetPriority());

		D3DXVECTOR2 screenSizeRatio = D3DXVECTOR2(screenSize.x / CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).x, screenSize.y / CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).y);
		if (screenSizeRatio.x > screenSizeRatio.y)
		{
			SetSize(D3DXVECTOR3(CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).x * screenSizeRatio.x, CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).y * screenSizeRatio.x, 0.0f)); // サイズを設定
		}
		else
		{
			SetSize(D3DXVECTOR3(CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).x * screenSizeRatio.y, CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).y * screenSizeRatio.y, 0.0f)); // サイズを設定
		}

		// 頂点情報の設定
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pBg[cntTex]->GetVtxBuff();
		Vertex* pVtx;
		if (FAILED(pVtxBuff->Lock(0, 0, (void**)&pVtx, 0))) { return E_FAIL; }

		D3DXVECTOR3 size = GetSize(); // サイズを取得
		for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
		{
			pVtx[cntVtx].pos = D3DXVECTOR3(size.x * (float)(cntVtx % 2) + screenSize.x * 0.5f - size.x * 0.5f, size.y * (float)(cntVtx / 2) + screenSize.y * 0.5f - size.y * 0.5f, 0.0f);
			pVtx[cntVtx].rhw = 1.0f;
			pVtx[cntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[cntVtx].tex = D3DXVECTOR2((float)(cntVtx % 2) + m_TextureU[cntTex], (float)(cntVtx / 2));
		}

		if (FAILED(pVtxBuff->Unlock())) { return E_FAIL; }
	}

	return S_OK;
}

//------------------------------
//終了処理
//------------------------------
void CBg::Uninit(void)
{

}

//------------------------------
//更新処理
//------------------------------
void CBg::Update(void)
{
	const CRenderer renderer = CManager::GetRenderer();

	// スクリーンサイズの取得
	D3DXVECTOR2 screenSize = {};
	if (FAILED(renderer.GetViewportSize(&screenSize)))
	{
		return;
	}

	// 背景サイズの取得
	for (size_t cntTex = 0; cntTex < MAX_TEXTURE; cntTex++)
	{
		if (m_pBg[cntTex] == nullptr)
		{
			break;
		}

		if (CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).x == 0.0f || CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).y == 0.0f)
		{
			continue;
		}

		// スクロール処理
		m_TextureU[cntTex] += m_afScrollSpeed[cntTex] * CMain::GetDeltaTimeGameSpeed();

		D3DXVECTOR2 screenSizeRatio = D3DXVECTOR2(screenSize.x / CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).x, screenSize.y / CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).y);
		if (screenSizeRatio.x > screenSizeRatio.y)
		{
			SetSize(D3DXVECTOR3(CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).x * screenSizeRatio.x, CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).y * screenSizeRatio.x, 0.0f)); // サイズを設定
		}
		else
		{
			SetSize(D3DXVECTOR3(CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).x * screenSizeRatio.y, CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).y * screenSizeRatio.y, 0.0f)); // サイズを設定
		}

		// 頂点情報の設定
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pBg[cntTex]->GetVtxBuff();
		Vertex* pVtx;
		if (FAILED(pVtxBuff->Lock(0, 0, (void**)&pVtx, 0))) { return; }

		D3DXVECTOR3 size = GetSize(); // サイズを取得
		for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
		{
			pVtx[cntVtx].pos = D3DXVECTOR3(size.x * (float)(cntVtx % 2) + screenSize.x * 0.5f - size.x * 0.5f, size.y * (float)(cntVtx / 2) + screenSize.y * 0.5f - size.y * 0.5f, 0.0f);
			pVtx[cntVtx].rhw = 1.0f;
			pVtx[cntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[cntVtx].tex = D3DXVECTOR2((float)(cntVtx % 2) + m_TextureU[cntTex], (float)(cntVtx / 2));
		}

		if (FAILED(pVtxBuff->Unlock())) { return; }
	}
}

//------------------------------
//描画処理
//------------------------------
void CBg::Draw(void)
{

}