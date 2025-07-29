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
// オブジェクト2Dクラス (オブジェクト派生)
//-----------------------------------------
class CObject2D : public CObject
{
// 公開
public:
	CObject2D() : m_pVtxBuff{} {}
	CObject2D(Index priority) : CObject(priority), m_pVtxBuff{} {}
	virtual ~CObject2D() = default;

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type);
	HRESULT Init(DirectX::Transform transform, TYPE type);
	void Uninit(void) override;
	void Draw(const context::DrawContext drawContext) override;

	void GetVtxBuff(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff) const { *ppVtxBuff = m_pVtxBuff; }
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) const { return m_pVtxBuff; }

// 非公開
private:
	void OnCollision(const CObject& other) override {};

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; // 頂点
};