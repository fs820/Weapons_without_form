//------------------------------------------
//
// �I�u�W�F�N�g2D�̏����̒�`�E�錾[object2D.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "object.h"

using namespace DirectX3D; // DirectX3D���O��Ԃ̎g�p

//-----------------------------------------
// �I�u�W�F�N�g3D�N���X (�I�u�W�F�N�g�h��)
//-----------------------------------------
class CObject3D : public CObject
{
// ���J
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

// ����J
private:
	void OnCollision(const CObject& other) override {};
	bool CheakPolygon(Vector3 pPos[3], const Vector3 pos) const;

	static size_t m_textureIdx;           // �e�N�X�`���ԍ�

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; // ���_
	D3DXMATRIX m_mtxWorld;              // �}�g���b�N�X
};