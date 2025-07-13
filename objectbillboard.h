//------------------------------------------
//
// オブジェクト2Dの処理の定義・宣言[objectbillboard.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "object.h"

//-----------------------------------------
// オブジェクト3Dクラス (オブジェクト派生)
//-----------------------------------------
class CObjectBillboard : public CObject
{
// 公開
public:
	CObjectBillboard() : m_pVtxBuff{}, m_mtxWorld{} {}
	CObjectBillboard(Index priority) : CObject(priority), m_pVtxBuff{}, m_mtxWorld{} {}
	virtual ~CObjectBillboard() = default;

	static CObjectBillboard* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, Index priority = 3);
	static CObjectBillboard* Create(DirectX::Transform transform, TYPE type, Index priority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type);
	HRESULT Init(DirectX::Transform transform, TYPE type);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void GetVtxBuff(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff) const { *ppVtxBuff = m_pVtxBuff; }
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) const { return m_pVtxBuff; }

// 非公開
private:
	void OnCollision(const CObject& other) override {}

	static size_t textureIdx;           // テクスチャ番号

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; // 頂点
	D3DXMATRIX m_mtxWorld;              // マトリックス
};