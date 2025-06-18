//------------------------------------------
//
// オブジェクト2Dの処理の定義・宣言[object2D.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "object.h"

using namespace DirectX3D; // DirectX3D名前空間の使用

//-----------------------------------------
// オブジェクト3Dクラス (オブジェクト派生)
//-----------------------------------------
class CObject3D : public CObject
{
// 公開
public:
	CObject3D() : m_pVtxBuff{}, m_mtxWorld{} {}
	CObject3D(int priority) : CObject(priority), m_pVtxBuff{}, m_mtxWorld{} {}
	~CObject3D() = default;

	static CObject3D* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, int priority = 3);
	static CObject3D* Create(Transform transform, TYPE type, int priority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type);
	HRESULT Init(Transform transform, TYPE type);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	float GetHeight(const D3DXVECTOR3 pos) const;

	void GetVtxBuff(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff) const { *ppVtxBuff = m_pVtxBuff; }
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) const { return m_pVtxBuff; }

// 非公開
private:
	void OnCollision(const CObject& other) override {};
	bool CheakPolygon(Vector3 pPos[3], const Vector3 pos) const;

	static size_t m_textureIdx;           // テクスチャ番号

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; // 頂点
	D3DXMATRIX m_mtxWorld;              // マトリックス
};