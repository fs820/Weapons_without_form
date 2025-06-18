//------------------------------------------
//
// �I�u�W�F�N�gX�̏����̒�`�E�錾[objectX.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "object.h"

using namespace DirectX3D; // DirectX3D���O��Ԃ̎g�p

//-----------------------------------------
// �I�u�W�F�N�gX�N���X (�I�u�W�F�N�g�h��)
//-----------------------------------------
class CObjectX : public CObject
{
// ���J
public:
	CObjectX() : m_mtxWorld{} {}
	CObjectX(int priority) : CObject(priority), m_mtxWorld{} {}
	~CObjectX() = default;

	static CObjectX* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, int priority = 3);
	static CObjectX* Create(Transform transform, TYPE type, int priority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type);
	HRESULT Init(Transform transform, TYPE type);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

// ����J
private:
	void OnCollision(const CObject& other) override {};

	static size_t m_modelIdx;           // ���f���ԍ�

	D3DXMATRIX m_mtxWorld;              // �}�g���b�N�X
};