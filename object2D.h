//------------------------------------------
//
// �I�u�W�F�N�g2D�̏����̒�`�E�錾[object2D.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "object.h"

//-----------------------------------------
// �I�u�W�F�N�g2D�N���X (�I�u�W�F�N�g�h��)
//-----------------------------------------
class CObject2D : public CObject
{
// ���J
public:
	CObject2D() : m_pVtxBuff{} {}
	CObject2D(int priority) : CObject(priority), m_pVtxBuff{} {}
	virtual ~CObject2D() = default;

	static CObject2D* Create(LPDIRECT3DTEXTURE9 pTexture, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, int priority = 3);
	static CObject2D* Create(LPDIRECT3DTEXTURE9 pTexture, DirectX::Transform transform, TYPE type, int priority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type);
	HRESULT Init(DirectX::Transform transform, TYPE type);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void GetVtxBuff(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff) const { *ppVtxBuff = m_pVtxBuff; }
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) const { return m_pVtxBuff; }

// ����J
private:
	void OnCollision(const CObject& other) override {};

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; // ���_
};