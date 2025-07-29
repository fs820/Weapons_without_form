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

using namespace DirectX3D; // DirectX3D名前空間の使用

//---------------------------------------
//
// オブジェクトXクラス
//
//---------------------------------------

//------------------------------
//初期化処理
//------------------------------
HRESULT CObjectX::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, MODELTAG model, TYPE type)
{
	CObject::Init(type); // 親クラスの初期化

	m_modelIdx = CModelManager::GetInstance().Create(Index(model));

	SetTransform(Transform(pos, rot, scale));

	return S_OK;
}

//------------------------------
//初期化処理
//------------------------------
HRESULT CObjectX::Init(Transform transform, MODELTAG model, TYPE type)
{
	CObject::Init(type); // 親クラスの初期化

	m_modelIdx = CModelManager::GetInstance().Create(Index(model));

	SetTransform(transform);

	return S_OK;
}

//------------------------------
//終了処理
//------------------------------
void CObjectX::Uninit(void)
{

}

//------------------------------
//描画処理
//------------------------------
void CObjectX::Draw(const context::DrawContext drawContext)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice(); // レンダラーからデバイスを取得

	Transform transform = GetTransform();
	transform.SetMatrix();
	pDevice->SetTransform(D3DTS_WORLD,&transform.mtxWorld);
	CModelManager::GetInstance().Draw(m_modelIdx);
}
