//----------------------------------------------------
//
// モデル処理 [model.cpp]
// Author: fuma sato
//
//----------------------------------------------------
#include "model.h"
#include "manager.h"
#include "texture.h"

using namespace model; // モデル空間の使用

//----------------------------------------------------
//
// モデルクラス
//
//----------------------------------------------------

//----------------------------------------------------
// 初期化処理
//----------------------------------------------------
HRESULT CModel::Init(void)
{
	m_transform.pos = { 0.0f,0.0f,0.0f };    // 初期位置はオフセット
	m_transform.rot = { 0.0f,0.0f,0.0f };    // 回転
	m_transform.scale = { 1.0f,1.0f,1.0f };  // 大きさ
	return S_OK;
}

//----------------------------------------------------
// 初期化処理
//----------------------------------------------------
HRESULT CModel::Init(const D3DXVECTOR3 offSet, const D3DXVECTOR3 rot, const D3DXVECTOR3 scale, const int nIdxParent)
{
	m_offSet.Set(offSet);                // オフセットを設定
	m_transform.pos = m_offSet.Get();    // 初期位置はオフセット
	m_transform.rot = rot;               // 回転
	m_transform.scale = scale;           // 大きさ
	SetParent(nIdxParent);               // 親の設定
	return S_OK;
}

//----------------------------------------------------
// モデル描画
//----------------------------------------------------
void CModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice(); // レンダラーからデバイスを取得

	if (pDevice == nullptr) return;

	D3DXMATRIX mtxRot{}, mtxTrans{}, mtxScale{}; // 計算用マトリックス

	// パーツのローカルマトリックスの初期化
	D3DXMatrixIdentity(&m_transform.mtxWorld);

	// パーツの大きさを反映
	D3DXMatrixScaling(&mtxScale, m_transform.scale.x, m_transform.scale.y, m_transform.scale.z);
	D3DXMatrixMultiply(&m_transform.mtxWorld, &m_transform.mtxWorld, &mtxScale);

	// パーツの向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_transform.rot.y, m_transform.rot.x, m_transform.rot.z);
	D3DXMatrixMultiply(&m_transform.mtxWorld, &m_transform.mtxWorld, &mtxRot);

	// パーツの位置(オフセット)を反映
	D3DXMatrixTranslation(&mtxTrans, m_transform.pos.x, m_transform.pos.y, m_transform.pos.z);
	D3DXMatrixMultiply(&m_transform.mtxWorld, &m_transform.mtxWorld, &mtxTrans);

	// 親のマトリックスを掛けてワールド座標に変換
	D3DXMATRIX mtxParent;
	if (m_pModel != nullptr)
	{// 親モデルがある
		mtxParent = m_pModel->GetMatrix();
	}
	else
	{// 親モデルがいない (Playerが親)
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	D3DXMatrixMultiply(&m_transform.mtxWorld, &m_transform.mtxWorld, &mtxParent);

	// マトリックスの適応
	pDevice->SetTransform(D3DTS_WORLD, &m_transform.mtxWorld);

	// マテリアルを取得
	D3DMATERIAL9 matDef{};
	pDevice->GetMaterial(&matDef);

	// サブセット数
	for (DWORD cntSubSet = 0; cntSubSet < m_pSource->mesh.dwNumSubSet; cntSubSet++)
	{// サブセット繰り返し
		if (!m_pSource->pSubSet[cntSubSet].bUse) continue; // サブセットが使われていないならスキップ

		// マテリアルの設定
		pDevice->SetMaterial(&m_pSource->mesh.pMat[m_pSource->pSubSet[cntSubSet].AttribId].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, CTextureManager::GetInstance().GetTexture(m_pSource->pSubSet[cntSubSet].texIdx));

		// サブセットの描画
		m_pSource->mesh.pMesh->DrawSubset(m_pSource->pSubSet[cntSubSet].AttribId);
	}

	// マテリアルを元に戻す
	pDevice->SetMaterial(&matDef);
}

void CModel::SetParent(const int nIdxParent) { m_pModel = CModelManager::GetInstance().GetModel(nIdxParent); }

//----------------------------------------------------
//
// モデル管理クラス
//
//----------------------------------------------------

// 静的メンバ変数
CModelManager CModelManager::m_instance{};     // 自分のインスタンス

//----------------------------------------------------
// 初期化処理
//----------------------------------------------------
HRESULT CModelManager::Load(const vector<path> pathList)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice(); // レンダラーからデバイスを取得

	if (pDevice == nullptr) return E_FAIL;

	// vectorのサイズを事前に確保
	if (m_aModelSource.size() < pathList.size())
	{
		m_aModelSource.resize(pathList.size());
	}

	// 規定テクスチャの生成
	for (size_t cntModel = 0; cntModel < m_aModelSource.size(); cntModel++)
	{// 規定タイプ回す
		m_aModelSource[cntModel].Path = pathList[cntModel]; // パスをセット
		// Xファイルの読み込み
		if (FAILED(LoadX(pDevice, &m_aModelSource[cntModel])))
		{
			return E_FAIL; // エラー値
		}
	}

	return S_OK;
}

//----------------------------------------------------
// 終了処理
//----------------------------------------------------
void CModelManager::Unload(void)
{
	for (auto& modelSource : m_aModelSource)
	{// 全てのモデルソース
		SAFE_DELETE_ARRAYPTR(modelSource.pSubSet);  // サブセット情報

		SAFE_RELEASE(modelSource.mesh.pBuffMat);    // マテリアル

		SAFE_RELEASE(modelSource.mesh.pMesh);       // 頂点
	}
}

//----------------------------------------------------
// 登録
//----------------------------------------------------
HRESULT CModelManager::Register(const path Path)
{
	// 同一モデル確認
	for (size_t cntModel = 0; cntModel < m_aModelSource.size(); cntModel++)
	{// 今までのモデル
		if (m_aModelSource[cntModel].Path == Path)
		{// 同じモデルを発見
			return E_FAIL;             // エラー値
		}
	}

	// 新モデルの生成
	m_aModelSource.push_back(model::Source{}); // 新しいモデル情報を追加
	m_aModelSource.back().Path = Path; // パスをセット
	if (FAILED(LoadX(CManager::GetRenderer().GetDevice(), &m_aModelSource.back())))
	{// Xファイルの読み込み失敗
		m_aModelSource.pop_back(); // 追加したモデル情報を削除
		return E_FAIL;             // エラー値
	}

	return S_OK; // 成功値
}

//----------------------------------------------------
// モデルを使う
//----------------------------------------------------
Index CModelManager::Create(Index tag)
{
	if (tag < 0 || tag >= m_aModelSource.size())
	{// タグが範囲外
		return INVALID_ID; // エラー値
	}

	// モデルの生成
	CModel* pModel = nullptr;
	pModel = new CModel(&m_aModelSource[tag]);
	if (pModel == nullptr)
	{// 生成失敗
		return INVALID_ID; // エラー値
	}

	if (FAILED(pModel->Init()))
	{// 初期化失敗
		pModel->Uninit();
		delete pModel;
		pModel = nullptr;
		return INVALID_ID;
	}

	m_apModel.push_back(pModel); // モデルを追加
	m_aModelSource[tag].countUse++; // 使われている数を追加

	return m_apModel.size() - 1;
}

//----------------------------------------------------
// モデルを使う
//----------------------------------------------------
Index CModelManager::Create(Index tag, const D3DXVECTOR3 offSet, const D3DXVECTOR3 rot, const D3DXVECTOR3 scale, const int nIdxParent)
{
	if (tag < 0 || tag >= m_aModelSource.size())
	{// タグが範囲外
		return INVALID_ID; // エラー値
	}

	// モデルの生成
	CModel* pModel = nullptr;
	pModel = new CModel(&m_aModelSource[tag]);
	if (pModel == nullptr)
	{// 生成失敗
		return INVALID_ID; // エラー値
	}

	if (FAILED(pModel->Init(offSet, rot, scale, nIdxParent)))
	{// 初期化失敗
		pModel->Uninit();
		delete pModel;
		pModel = nullptr;
		return INVALID_ID;
	}

	m_apModel.push_back(pModel); // モデルを追加
	m_aModelSource[tag].countUse++; // 使われている数を追加

	return m_apModel.size() - 1;
}

//----------------------------------------------------
// 終了処理
//----------------------------------------------------
void CModelManager::Release(void)
{
	SAFE_UNINIT_ARRAY(m_apModel); // 全てのモデルをアンイニット
}

//----------------------------------------------------
// Xファイルの読み込み
//----------------------------------------------------
HRESULT CModelManager::LoadX(LPDIRECT3DDEVICE9 pDevice, model::Source* pSource)
{
	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(
		pSource->Path.string().c_str(),
		D3DXMESH_MANAGED,
		pDevice,
		NULL,
		&pSource->mesh.pBuffMat,
		NULL,
		&pSource->mesh.dwNumMat,
		&pSource->mesh.pMesh)))
	{
		return E_FAIL;
	}

	//マテリアルデータへのポインタを取得
	pSource->mesh.pMat = static_cast<D3DXMATERIAL*>(pSource->mesh.pBuffMat->GetBufferPointer());

	// AttributeTableを取得
	pSource->mesh.pMesh->GetAttributeTable(nullptr, &pSource->mesh.dwNumSubSet);
	D3DXATTRIBUTERANGE* pAttributeTable = new D3DXATTRIBUTERANGE[pSource->mesh.dwNumSubSet];
	pSource->mesh.pMesh->GetAttributeTable(pAttributeTable, &pSource->mesh.dwNumSubSet);

	pSource->pSubSet = new model::SubSetSource[pSource->mesh.dwNumSubSet]; // サブセット情報を生成

	for (DWORD cntSubset = 0; cntSubset < pSource->mesh.dwNumSubSet; cntSubset++)
	{//マテリアル数繰り返す
		pSource->pSubSet[cntSubset].AttribId = pAttributeTable[cntSubset].AttribId; // IDをコピー

		if (pAttributeTable[cntSubset].FaceCount > 0)
		{// サブセットが使用されているか
			pSource->pSubSet[cntSubset].bUse = true; // つかわれている

			if (pSource->mesh.pMat[pSource->pSubSet[cntSubset].AttribId].pTextureFilename != nullptr)
			{//テクスチャがある
				pSource->pSubSet[cntSubset].texIdx = CTextureManager::GetInstance().Register(static_cast<path>(pSource->mesh.pMat[pSource->pSubSet[cntSubset].AttribId].pTextureFilename)); // テクスチャID
			}
			else
			{//テクスチャがない
				pSource->pSubSet[cntSubset].texIdx = INVALID_ID; // テクスチャがない
			}
		}
		else
		{// サブセットが使用されていない
			pSource->pSubSet[cntSubset].bUse = false;        // つかわれていない
			pSource->pSubSet[cntSubset].texIdx = INVALID_ID; // テクスチャがない
		}
	}
	delete[] pAttributeTable; // メモリ解放

	//頂点数
	pSource->mesh.dwNumVertex = pSource->mesh.pMesh->GetNumVertices();
	//頂点サイズ
	pSource->mesh.sizeFVF = D3DXGetFVFVertexSize(pSource->mesh.pMesh->GetFVF());
	//頂点バッファの取得
	pSource->mesh.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pSource->mesh.pVtxBuff);

	for (DWORD cntVtx = 0; cntVtx < pSource->mesh.dwNumVertex; cntVtx++)
	{//頂点数繰り返す
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pSource->mesh.pVtxBuff;

		//頂点の大きさの反映
		if (vtx.x > pSource->mesh.vtxMax.x)
		{
			pSource->mesh.vtxMax.x = vtx.x;
		}
		if (vtx.x < pSource->mesh.vtxMin.x)
		{
			pSource->mesh.vtxMin.x = vtx.x;
		}
		if (vtx.y > pSource->mesh.vtxMax.y)
		{
			pSource->mesh.vtxMax.y = vtx.y;
		}
		if (vtx.y < pSource->mesh.vtxMin.y)
		{
			pSource->mesh.vtxMin.y = vtx.y;
		}
		if (vtx.z > pSource->mesh.vtxMax.z)
		{
			pSource->mesh.vtxMax.z = vtx.z;
		}
		if (vtx.z < pSource->mesh.vtxMin.z)
		{
			pSource->mesh.vtxMin.z = vtx.z;
		}

		pSource->mesh.pVtxBuff += pSource->mesh.sizeFVF;
	}

	// 頂点バッファのアンロック
	pSource->mesh.pMesh->UnlockVertexBuffer();

	return S_OK; // 成功値
}