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
// �I�u�W�F�N�g3D�N���X (�I�u�W�F�N�g�h��)
//-----------------------------------------
class CObject3D : public CObject
{
// ���J
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

// ����J
private:
	void OnCollision(const CObject& other) override {};
	bool CheakPolygon(DirectX::Vector3 pPos[3], const DirectX::Vector3 pos) const;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; // ���_
	D3DXMATRIX m_mtxWorld;              // �}�g���b�N�X
};