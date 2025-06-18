//----------------------------------------------------
//
// モデル処理 [model.cpp]
// Author: fuma sato
//
//----------------------------------------------------
#include "model.h"
#include "manager.h"
#include "texture.h"

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
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice(); // デバイス

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(
		m_sPass.data(),
		D3DXMESH_MANAGED,
		pDevice,
		NULL,
		&m_info.pBuffMat,
		NULL,
		&m_info.dwNumMat,
		&m_info.pMesh)))
	{
		return E_FAIL;
	}

	//マテリアルデータへのポインタを取得
	m_info.pMat = static_cast<D3DXMATERIAL*>(m_info.pBuffMat->GetBufferPointer());

	// AttributeTableを取得
	m_info.pMesh->GetAttributeTable(nullptr, &m_info.dwNumSubSet);
	D3DXATTRIBUTERANGE* pAttributeTable = new D3DXATTRIBUTERANGE[m_info.dwNumSubSet];
	m_info.pMesh->GetAttributeTable(pAttributeTable, &m_info.dwNumSubSet);

	m_info.pSubSet = new SubSetInfo[m_info.dwNumSubSet]; // サブセット情報を生成

	for (DWORD cntSubset = 0; cntSubset < m_info.dwNumSubSet; cntSubset++)
	{//マテリアル数繰り返す
		m_info.pSubSet[cntSubset].AttribId = pAttributeTable[cntSubset].AttribId; // IDをコピー

		if (pAttributeTable[cntSubset].FaceCount > 0)
		{// サブセットが使用されているか
			m_info.pSubSet[cntSubset].bUse = true; // つかわれている

			if (m_info.pMat[m_info.pSubSet[cntSubset].AttribId].pTextureFilename != nullptr)
			{//テクスチャがある
				m_info.pSubSet[cntSubset].texIdx = CTextureManager::GetInstance().Register(m_info.pMat[m_info.pSubSet[cntSubset].AttribId].pTextureFilename); // テクスチャID
			}
			else
			{//テクスチャがない
				m_info.pSubSet[cntSubset].texIdx = INVALID_ID; // テクスチャがない
			}
		}
		else
		{// サブセットが使用されていない
			m_info.pSubSet[cntSubset].bUse = false;        // つかわれていない
			m_info.pSubSet[cntSubset].texIdx = INVALID_ID; // テクスチャがない
		}
	}
	delete[] pAttributeTable; // メモリ解放

	m_info.pos = m_info.offSet;        // オフセットを適応する
	m_info.rot = { 0.0f,0.0f,0.0f };   // 初期化
	m_info.scale = { 1.0f,1.0f,1.0f }; // 初期化

	m_pModel = CModelManager::GetInstance().GetModel(m_info.nIdxParent);

	return S_OK;
}

//----------------------------------------------------
// 終了処理
//----------------------------------------------------
void CModel::Uninit(void)
{
	// サブセット情報
	if (m_info.pSubSet != nullptr)
	{// あれば
		delete[] m_info.pSubSet;  // 開放
		m_info.pSubSet = nullptr; // null
	}

	// マテリアル
	if (m_info.pBuffMat != nullptr)
	{// あれば
		m_info.pBuffMat->Release(); // 開放
		m_info.pBuffMat = nullptr;  // null
	}

	// 頂点
	if (m_info.pMesh != nullptr)
	{// あれば
		m_info.pMesh->Release(); // 開放
		m_info.pMesh = nullptr;  // null
	}
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
	D3DXMatrixIdentity(&m_info.mtxWorld);

	// パーツの大きさを反映
	D3DXMatrixScaling(&mtxScale, m_info.scale.x, m_info.scale.y, m_info.scale.z);
	D3DXMatrixMultiply(&m_info.mtxWorld, &m_info.mtxWorld, &mtxScale);

	// パーツの向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_info.rot.y, m_info.rot.x, m_info.rot.z);
	D3DXMatrixMultiply(&m_info.mtxWorld, &m_info.mtxWorld, &mtxRot);

	// パーツの位置(オフセット)を反映
	D3DXMatrixTranslation(&mtxTrans, m_info.pos.x, m_info.pos.y, m_info.pos.z);
	D3DXMatrixMultiply(&m_info.mtxWorld, &m_info.mtxWorld, &mtxTrans);

	// 親のマトリックスを掛けてワールド座標に変換
	D3DXMATRIX mtxParent;
	if (m_pModel != nullptr)
	{// 親モデルがある
		mtxParent = m_pModel->GetInfo().mtxWorld;
	}
	else
	{// 親モデルがいない (Playerが親)
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	D3DXMatrixMultiply(&m_info.mtxWorld, &m_info.mtxWorld, &mtxParent);

	// マトリックスの適応
	pDevice->SetTransform(D3DTS_WORLD, &m_info.mtxWorld);

	// マテリアルを取得
	D3DMATERIAL9 matDef{};
	pDevice->GetMaterial(&matDef);

	// サブセット数
	for (DWORD cntSubSet = 0; cntSubSet < m_info.dwNumSubSet; cntSubSet++)
	{// サブセット繰り返し
		// マテリアルの設定
		pDevice->SetMaterial(&m_info.pMat[m_info.pSubSet[cntSubSet].AttribId].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, CTextureManager::GetInstance().GetTexture(m_info.pSubSet[cntSubSet].texIdx));

		// サブセットの描画
		m_info.pMesh->DrawSubset(m_info.pSubSet[cntSubSet].AttribId);
	}

	// マテリアルを元に戻す
	pDevice->SetMaterial(&matDef);
}

//----------------------------------------------------
// 親の設定
//----------------------------------------------------
void CModel::SetParent(const int nIdxParent)
{
	m_info.nIdxParent = nIdxParent;
	m_pModel = CModelManager::GetInstance().GetModel(m_info.nIdxParent);
}

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
HRESULT CModelManager::Load(void)
{
	// 規定テクスチャの生成
	for (size_t cntType = 0; cntType < TYPE_MAX; cntType++)
	{// 規定タイプ回す
		if (m_apModel[cntType] == nullptr)
		{// なければ
			m_apModel[cntType] = new CModel(m_typePass[cntType], m_typeOffSet[cntType], m_typeParent[cntType]); // 生成

			if (m_apModel[cntType] != nullptr)
			{// あれば
				if (FAILED(m_apModel[cntType]->Init()))
				{
					m_apModel[cntType]->Uninit();
					delete m_apModel[cntType];
					m_apModel[cntType] = nullptr;
					return E_FAIL;
				}
				m_apModel[cntType]->AddUseCount();                     // 使っている数の追加
				m_nNumAll++; // 足す
			}
			else
			{
				return E_FAIL; // エラー値
			}
		}
	}

	return S_OK;
}

//----------------------------------------------------
// 終了処理
//----------------------------------------------------
void CModelManager::Unload(void)
{
	for (size_t cntModel = 0; cntModel < m_nNumAll; cntModel++)
	{// 全てのテクスチャ
		if (m_apModel[cntModel] != nullptr)
		{// あれば
			m_apModel[cntModel]->Uninit(); // 終了
			delete m_apModel[cntModel];    // 開放
			m_apModel[cntModel] = nullptr; // null
		}
	}
}

//----------------------------------------------------
// 更新
//----------------------------------------------------
Index CModelManager::Register(const string_view sModelPass, const D3DXVECTOR3 offSet, const int nIdxParent)
{
	// 同一モデル確認
	for (size_t cntModel = 0; cntModel < m_nNumAll; cntModel++)
	{// 今までのモデル
		if (m_apModel[cntModel]->GetPass() == sModelPass)
		{// 同じモデルを発見
			m_apModel[cntModel]->AddUseCount(); // 使っている数の追加
			return cntModel;                      // そのモデルつかってね
		}
	}

	// 新テクスチャの生成
	if (m_apModel[m_nNumAll] == nullptr)
	{// なければ
		m_apModel[m_nNumAll] = new CModel(sModelPass, offSet, nIdxParent); // 生成

		if (m_apModel[m_nNumAll] != nullptr)
		{// あれば
			if (FAILED(m_apModel[m_nNumAll]->Init()))
			{
				m_apModel[m_nNumAll]->Uninit();
				delete m_apModel[m_nNumAll];
				m_apModel[m_nNumAll] = nullptr;
				return INVALID_ID;
			}
			m_apModel[m_nNumAll]->AddUseCount();                         // 使っている数の追加
			return m_nNumAll++; // 足す前の値を返しその後足す
		}
		else
		{
			return INVALID_ID; // エラー値
		}
	}
	return INVALID_ID; // エラー値
}