//------------------------------------------
//
// モデル描画用の処理[objectX.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "objectX.h"
#include "object3D.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "model.h"

//---------------------------------------
//
// オブジェクトXクラス
//
//---------------------------------------

Index CObjectX::m_modelIdx{};           // モデルタグ

//------------------------------
// 生成
//------------------------------
CObjectX* CObjectX::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, int priority)
{
	CObjectX* pObjectX = new CObjectX(priority); // インスタンス生成

	if (pObjectX == nullptr)
	{// 生成失敗
		return nullptr;
	}

	m_modelIdx = CModelManager::GetInstance().Create(Index(MODELTAG::Test));

	// 初期化
	if (FAILED(pObjectX->Init(pos, rot, scale, type)))
	{
		delete pObjectX;
		pObjectX = nullptr;
		return nullptr;
	}

	return pObjectX;
}

//------------------------------
// 生成
//------------------------------
CObjectX* CObjectX::Create(Transform transform, TYPE type, int priority)
{
	CObjectX* pObjectX = new CObjectX(priority); // インスタンス生成

	if (pObjectX == nullptr)
	{// 生成失敗
		return nullptr;
	}

	m_modelIdx = CModelManager::GetInstance().Create(Index(MODELTAG::Test));

	// 初期化
	if (FAILED(pObjectX->Init(transform, type)))
	{
		delete pObjectX;
		pObjectX = nullptr;
		return nullptr;
	}

	return pObjectX;
}

//------------------------------
//初期化処理
//------------------------------
HRESULT CObjectX::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type)
{
	CObject::Init(type); // 親クラスの初期化

	SetTransform(Transform(pos, rot, scale));
	//SetSize(D3DXVECTOR3(CModelManager::GetInstance().GetSize(m_modelIdx)));

	return S_OK;
}

//------------------------------
//初期化処理
//------------------------------
HRESULT CObjectX::Init(Transform transform, TYPE type)
{
	CObject::Init(type); // 親クラスの初期化

	SetTransform(transform);
	//SetSize(D3DXVECTOR3(CModelManager::GetInstance().GetSize(m_modelIdx)));

	return S_OK;
}

//------------------------------
//終了処理
//------------------------------
void CObjectX::Uninit(void)
{

}

//------------------------------
//更新処理
//------------------------------
void CObjectX::Update(void)
{
	SetPosition(D3DXVECTOR3(GetPosition().x, CManager::Get3D().GetHeight(GetPosition()), GetPosition().z)); // Transformの更新
}

//------------------------------
//描画処理
//------------------------------
void CObjectX::Draw(void)
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

	CModelManager::GetInstance().Draw(m_modelIdx);
}