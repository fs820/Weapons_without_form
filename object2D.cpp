//------------------------------------------
//
// 2D描画用の処理[object2D.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "object2D.h"
#include "renderer.h"
#include "manager.h"

//---------------------------------------
//
// オブジェクト2Dクラス
//
//---------------------------------------

//------------------------------
// 生成
//------------------------------
CObject2D* CObject2D::Create(LPDIRECT3DTEXTURE9 pTexture, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, int priority)
{
	CObject2D* pObject2D = new CObject2D(priority); // インスタンス生成

	if (pObject2D == nullptr)
	{// 生成失敗
		return nullptr;
	}

	// 初期化
	if (FAILED(pObject2D->Init(pos, rot, scale, type)))
	{
		delete pObject2D;
		pObject2D = nullptr;
		return nullptr;
	}

	return pObject2D;
}

//------------------------------
// 生成
//------------------------------
CObject2D* CObject2D::Create(LPDIRECT3DTEXTURE9 pTexture, Transform transform, TYPE type, int priority)
{
	CObject2D* pObject2D = new CObject2D(priority); // インスタンス生成

	if (pObject2D == nullptr)
	{// 生成失敗
		return nullptr;
	}

	// 初期化
	if (FAILED(pObject2D->Init(transform, type)))
	{
		delete pObject2D;
		pObject2D = nullptr;
		return nullptr;
	}

	return pObject2D;
}

//------------------------------
//初期化処理
//------------------------------
HRESULT CObject2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type)
{
	CObject::Init(type); // 親クラスの初期化

	CRenderer renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer.GetDevice(); // レンダラーからデバイスを取得

	if (pDevice == nullptr)
	{
		return E_POINTER;
	}

	// スクリーンサイズの取得
	D3DXVECTOR2 screenSize = {};
	if (FAILED(renderer.GetDxScreenSize(&screenSize)))
	{
		return E_FAIL;
	}

	SetTransform(Transform(pos, rot, scale));

	//バッファーの設定
	if (FAILED(pDevice->CreateVertexBuffer
	(
		sizeof(Vertex) * VT_DEF,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr
	)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//------------------------------
//初期化処理
//------------------------------
HRESULT CObject2D::Init(Transform transform, TYPE type)
{
	CObject::Init(type); // 親クラスの初期化

	CRenderer renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer.GetDevice(); // レンダラーからデバイスを取得

	if (pDevice == nullptr)
	{
		return E_POINTER;
	}

	// スクリーンサイズの取得
	D3DXVECTOR2 screenSize = {};
	if (FAILED(renderer.GetDxScreenSize(&screenSize)))
	{
		return E_FAIL;
	}

	SetTransform(transform);

	//バッファーの設定
	if (FAILED(pDevice->CreateVertexBuffer
	(
		sizeof(Vertex) * VT_DEF,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr
	)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//------------------------------
//終了処理
//------------------------------
void CObject2D::Uninit(void)
{
	// 頂点バッファの解放
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//------------------------------
//更新処理
//------------------------------
void CObject2D::Update(void)
{

}

//------------------------------
//描画処理
//------------------------------
void CObject2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice(); // レンダラーからデバイスを取得

	if (pDevice == nullptr || m_pVtxBuff == nullptr)
	{
		return;
	}

	// 頂点バッファ
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(Vertex));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX);

	// ポリゴンの描画
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//タイプ
		0,                  // インデックス
		2                   //ポリゴン数
	);
}