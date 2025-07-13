//------------------------------------------
//
// 3D描画用の処理[object3D.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "object3D.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

using namespace DirectX3D; // DirectX3D名前空間の使用

//---------------------------------------
//
// オブジェクト3Dクラス
//
//---------------------------------------

//------------------------------
// 生成
//------------------------------
CObject3D* CObject3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, Index priority)
{
	CObject3D* pObject3D = new CObject3D(priority); // インスタンス生成

	if (pObject3D == nullptr)
	{// 生成失敗
		return nullptr;
	}

	// 初期化
	if (FAILED(pObject3D->Init(pos, rot, scale, type)))
	{
		delete pObject3D;
		pObject3D = nullptr;
		return nullptr;
	}

	return pObject3D;
}

//------------------------------
// 生成
//------------------------------
CObject3D* CObject3D::Create(Transform transform, TYPE type, Index priority)
{
	CObject3D* pObject3D = new CObject3D(priority); // インスタンス生成

	if (pObject3D == nullptr)
	{// 生成失敗
		return nullptr;
	}

	// 初期化
	if (FAILED(pObject3D->Init(transform, type)))
	{
		delete pObject3D;
		pObject3D = nullptr;
		return nullptr;
	}

	return pObject3D;
}

//------------------------------
//初期化処理
//------------------------------
HRESULT CObject3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type)
{
	CObject::Init(type); // 親クラスの初期化

	CRenderer renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer.GetDevice(); // レンダラーからデバイスを取得

	if (pDevice == nullptr)
	{
		return E_POINTER;
	}

	SetTransform(Transform(pos, rot, scale));
	SetSize(D3DXVECTOR3(CTextureManager::GetInstance().GetSize(Index(TEXTAG::Ground))));

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

	for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
	{
		resultPos[cntVtx] = D3DXVECTOR3(-size.x * 0.5f + size.x * (cntVtx % 2), (cntVtx == 1 || cntVtx == 2) ? 200.0f : 0.0f, size.y * 0.5f + -size.y * (cntVtx / 2));
	}

	D3DXVECTOR3 nor[VT_DEF] = {}; // 法線ベクトルの初期化

	D3DXVECTOR3 vec1 = resultPos[1] - resultPos[0], vec2 = resultPos[2] - resultPos[0];
	D3DXVec3Cross(&nor[0], &vec1, &vec2);
	vec1 = resultPos[2] - resultPos[3], vec2 = resultPos[1] - resultPos[3];
	D3DXVec3Cross(&nor[3], &vec1, &vec2);

	nor[1] = (nor[0] + nor[3]) / 2.0f; // 法線ベクトルの設定
	nor[2] = (nor[0] + nor[3]) / 2.0f; // 法線ベクトルの設定

	for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
	{
		D3DXVec3Normalize(&nor[cntVtx], &nor[cntVtx]); // 法線ベクトルの正規化
	}

	for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
	{
		pVtx[cntVtx].pos = resultPos[cntVtx];
		pVtx[cntVtx].nor = nor[cntVtx];
		pVtx[cntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[cntVtx].tex = D3DXVECTOR2((float)(cntVtx % 2) * 1.0f, (float)(cntVtx / 2) * 1.0f);
	}

	if (FAILED(m_pVtxBuff->Unlock())) { return E_FAIL; }

	return S_OK;
}

//------------------------------
//初期化処理
//------------------------------
HRESULT CObject3D::Init(Transform transform, TYPE type)
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

	for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
	{
		resultPos[cntVtx] = D3DXVECTOR3(-size.x * 0.5f + size.x * (cntVtx % 2), (cntVtx == 1 || cntVtx == 2) ? 200.0f : 0.0f, size.y * 0.5f + -size.y * (cntVtx / 2));
	}

	D3DXVECTOR3 nor[VT_DEF] = {}; // 法線ベクトルの初期化

	D3DXVECTOR3 vec1 = resultPos[1] - resultPos[0], vec2 = resultPos[2] - resultPos[0];
	D3DXVec3Cross(&nor[0], &vec1, &vec2);
	vec1 = resultPos[2] - resultPos[3], vec2 = resultPos[1] - resultPos[3];
	D3DXVec3Cross(&nor[3], &vec1, &vec2);

	nor[1] = (nor[0] + nor[3]) / 2.0f; // 法線ベクトルの設定
	nor[2] = (nor[0] + nor[3]) / 2.0f; // 法線ベクトルの設定

	for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
	{
		D3DXVec3Normalize(&nor[cntVtx], &nor[cntVtx]); // 法線ベクトルの正規化
	}

	for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
	{
		pVtx[cntVtx].pos = resultPos[cntVtx];
		pVtx[cntVtx].nor = nor[cntVtx];
		pVtx[cntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[cntVtx].tex = D3DXVECTOR2((float)(cntVtx % 2) * 1.0f, (float)(cntVtx / 2) * 1.0f);
	}

	if (FAILED(m_pVtxBuff->Unlock())) { return E_FAIL; }

	return S_OK;
}

//------------------------------
//終了処理
//------------------------------
void CObject3D::Uninit(void)
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
void CObject3D::Update(void)
{

}

//------------------------------
//描画処理
//------------------------------
void CObject3D::Draw(void)
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

	// 頂点バッファ
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(Vertex));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX);

	//テクスチャの設定
	pDevice->SetTexture(0, CTextureManager::GetInstance().GetTexture(Index(TEXTAG::Ground)));

	// ポリゴンの描画
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//タイプ
		0,                  // インデックス
		2                   //ポリゴン数
	);
}

//------------------------------
// 高さの取得
//------------------------------
float CObject3D::GetHeight(const Vector3 pos) const
{
	Vector3 vtxPos[VT_DEF] = {};
	Vector3 size = GetSize(); // サイズの取得

	for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
	{
		vtxPos[cntVtx] = Vector3(-size.x * 0.5f + size.x * (cntVtx % 2), (cntVtx == 1 || cntVtx == 2) ? 200.0f : 0.0f, size.y * 0.5f + -size.y * (cntVtx / 2));
	}

	if (pos.x >= vtxPos[0].x && pos.x <= vtxPos[0].x + size.x && pos.z >= vtxPos[3].z && pos.z <= vtxPos[3].z + size.y)
	{
		Vector3 Poly1[3]{ vtxPos[0] ,vtxPos[1] ,vtxPos[2] };
		Vector3 Poly2[3]{ vtxPos[3] ,vtxPos[2] ,vtxPos[1] };
		if (CheakPolygon(Poly1, pos))
		{
			Vector3 nor{}; // 法線ベクトルの初期化
			Vector3 vec1 = vtxPos[1] - vtxPos[0], vec2 = vtxPos[2] - vtxPos[0];
			D3DXVec3Cross(&nor, &vec1, &vec2);
			D3DXVec3Normalize(&nor, &nor); // 法線ベクトルの正規化

			if (nor.y != 0.0f)
			{
				Vector3 vecP = pos - vtxPos[0];
				return (-(vecP.x * nor.x + vecP.z * nor.z) / nor.y) + vtxPos[0].y;
			}
		}
		else if (CheakPolygon(Poly2, pos))
		{
			Vector3 nor{}; // 法線ベクトルの初期化
			Vector3 vec1 = vtxPos[2] - vtxPos[3], vec2 = vtxPos[1] - vtxPos[3];
			D3DXVec3Cross(&nor, &vec1, &vec2);
			D3DXVec3Normalize(&nor, &nor); // 法線ベクトルの正規化

			if (nor.y != 0.0f)
			{
				Vector3 vecP = pos - vtxPos[3];
				return (-(vecP.x * nor.x + vecP.z * nor.z) / nor.y) + vtxPos[3].y;
			}
		}
	}

	return pos.y; // 高さを返す
}

//------------------------------
// ポリゴンの当たり判定
//------------------------------
bool CObject3D::CheakPolygon(Vector3 pPos[3], const Vector3 pos) const
{
	Vector3 vecO[3]{ pPos[1] - pPos[0],pPos[2] - pPos[1],pPos[0] - pPos[2] };
	Vector3 vecP[3]{ pos - pPos[0],pos - pPos[1],pos - pPos[2] };

	Vector3 vecA[3]{};
	for (size_t cnt = 0; cnt < 3; cnt++)
	{
		D3DXVec3Cross(&vecA[cnt], &vecO[cnt], &vecP[cnt]);

		if (vecA[cnt].y < 0.0f)
		{
			return false; // どれかのベクトルが負なら当たり判定なし
		}
	}

	return true; // 全てのベクトルが正なら当たり判定あり
}