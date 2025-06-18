//------------------------------------------
//
// �I�u�W�F�N�g2D�̏����̒�`�E�錾[objectbillboard.h]
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
class CObjectBillboard : public CObject
{
// ���J
public:
	CObjectBillboard() : m_pVtxBuff{}, m_mtxWorld{} {}
	CObjectBillboard(int priority) : CObject(priority), m_pVtxBuff{}, m_mtxWorld{} {}
	~CObjectBillboard() = default;

	static CObjectBillboard* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, int priority = 3);
	static CObjectBillboard* Create(Transform transform, TYPE type, int priority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type);
	HRESULT Init(Transform transform, TYPE type);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void GetVtxBuff(LPDIRECT3DVERTEXBUFFER9* ppVtxBuff) const { *ppVtxBuff = m_pVtxBuff; }
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) const { return m_pVtxBuff; }

// ����J
private:
	void OnCollision(const CObject& other) override {}

	static size_t textureIdx;           // �e�N�X�`���ԍ�

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; // ���_
	D3DXMATRIX m_mtxWorld;              // �}�g���b�N�X
};