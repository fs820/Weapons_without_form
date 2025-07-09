//------------------------------------------
//
// 3D描画用の処理[objectbillboard.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "objectbillboard.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

using namespace DirectX3D; // DirectX3D名前空間の使用

//---------------------------------------
//
// オブジェクト2Dクラス
//
//---------------------------------------

size_t CObjectBillboard::textureIdx{};           // テクスチャ番号

//------------------------------
// 生成
//------------------------------
CObjectBillboard* CObjectBillboard::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, int priority)
{
	CObjectBillboard* pObjectBillboard = new CObjectBillboard(priority); // インスタンス生成

	if (pObjectBillboard == nullptr)
	{// 生成失敗
		return nullptr;
	}

	textureIdx = CTextureManager::GetInstance().Register("data\\TEXTURE\\enemy001.png");

	// 初期化
	if (FAILED(pObjectBillboard->Init(pos, rot, scale, type)))
	{
		delete pObjectBillboard;
		pObjectBillboard = nullptr;
		return nullptr;
	}

	return pObjectBillboard;
}

//------------------------------
// 生成
//------------------------------
CObjectBillboard* CObjectBillboard::Create(Transform transform, TYPE type, int priority)
{
	CObjectBillboard* pObjectBillboard = new CObjectBillboard(priority); // インスタンス生成

	if (pObjectBillboard == nullptr)
	{// 生成失敗
		return nullptr;
	}

	textureIdx = CTextureManager::GetInstance().Register("data\\TEXTURE\\enemy001.png");

	// 初期化
	if (FAILED(pObjectBillboard->Init(transform, type)))
	{
		delete pObjectBillboard;
		pObjectBillboard = nullptr;
		return nullptr;
	}

	return pObjectBillboard;
}

//------------------------------
//初期化処理
//------------------------------
HRESULT CObjectBillboard::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type)
{
	CObject::Init(type); // 親クラスの初期化

	CRenderer renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer.GetDevice(); // レンダラーからデバイスを取得

	if (pDevice == nullptr)
	{
		return E_POINTER;
	}

	SetTransform(Transform(pos, rot, scale));
	SetSize(D3DXVECTOR3(CTextureManager::GetInstance().GetSize(textureIdx)));

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

	// 頂点情報の設定
	Vertex* pVtx;
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0))) { return E_FAIL; }

	D3DXVECTOR3 resultPos[VT_DEF] = {};
	D3DXVECTOR3 size = GetSize(); // サイズの取得

	resultPos[0] = D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, 0.0f);
	resultPos[1] = D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, 0.0f);
	resultPos[2] = D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, 0.0f);
	resultPos[3] = D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, 0.0f);

	for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
	{
		pVtx[cntVtx].pos = resultPos[cntVtx];
		pVtx[cntVtx].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[cntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[cntVtx].tex = D3DXVECTOR2((float)(cntVtx % 2) * 1.0f, (float)(cntVtx / 2) * 1.0f);
	}

	if (FAILED(m_pVtxBuff->Unlock())) { return E_FAIL; }

	return S_OK;
}

//------------------------------
//初期化処理
//------------------------------
HRESULT CObjectBillboard::Init(Transform transform, TYPE type)
{
	CObject::Init(type); // 親クラスの初期化

	CRenderer renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer.GetDevice(); // レンダラーからデバイスを取得

	if (pDevice == nullptr)
	{
		return E_POINTER;
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

	// 頂点情報の設定
	Vertex* pVtx;
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0))) { return E_FAIL; }

	D3DXVECTOR3 resultPos[VT_DEF] = {};
	D3DXVECTOR3 size = GetSize(); // サイズの取得

	resultPos[0] = D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, 0.0f);
	resultPos[1] = D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, 0.0f);
	resultPos[2] = D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, 0.0f);
	resultPos[3] = D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, 0.0f);

	for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
	{
		pVtx[cntVtx].pos = resultPos[cntVtx];
		pVtx[cntVtx].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[cntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[cntVtx].tex = D3DXVECTOR2((float)(cntVtx % 2) * 1.0f, (float)(cntVtx / 2) * 1.0f);
	}

	if (FAILED(m_pVtxBuff->Unlock())) { return E_FAIL; }

	return S_OK;
}

//------------------------------
//終了処理
//------------------------------
void CObjectBillboard::Uninit(void)
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
void CObjectBillboard::Update(void)
{

}

//------------------------------
//描画処理
//------------------------------
void CObjectBillboard::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice(); // レンダラーからデバイスを取得

	if (pDevice == nullptr || m_pVtxBuff == nullptr)
	{
		return;
	}

	Transform transform = GetTransform();
	D3DXMATRIX mtxRot{}, mtxTrans{}, mtxScale{};//計算マトリックス

	//マトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックス取得
	D3DXMATRIX mtxView;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//カメラの逆行列を設定
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	//大きさの反映
	D3DXMatrixScaling(&mtxScale, transform.scale.x, transform.scale.y, transform.scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//位置の計算
	D3DXMatrixTranslation(&mtxTrans, transform.pos.x, transform.pos.y, transform.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファ
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(Vertex));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX);

	//テクスチャの設定
	pDevice->SetTexture(0, CTextureManager::GetInstance().GetTexture(textureIdx));

	// ポリゴンの描画
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//タイプ
		0,                  // インデックス
		2                   //ポリゴン数
	);
}