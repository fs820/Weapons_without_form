//------------------------------------------
//
// オブジェクト2Dの処理の定義・宣言[object2D.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "object.h"

//-----------------------------------------
// オブジェクト3Dクラス (オブジェクト派生)
//-----------------------------------------
class CObject3D : public CObject
{
// 公開
public:
	CObject3D() : m_pVtxBuff{}, m_mtxWorld{} {}
	CObject3D(Index priority) : CObject(priority), m_pVtxBuff{}, m_mtxWorld{} {}
	virtual ~CObject3D() = default;

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type);
	HRESULT Init(DirectX::Transform transform, TYPE type);
	void Uninit(void) override;
	void Draw(const context::DrawContext drawContext) override;

	float GetHeight(const DirectX::Vector3 pos) const;

	void GetVtxBuff(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff) const { *ppVtxBuff = m_pVtxBuff; }
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) const { return m_pVtxBuff; }

// 非公開
private:
	void OnCollision(const CObject& other) override {};
	bool CheakPolygon(DirectX::Vector3 pPos[3], const DirectX::Vector3 pos) const;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; // 頂点
	D3DXMATRIX m_mtxWorld;              // マトリックス
};