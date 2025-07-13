//------------------------------------------
//
// �I�u�W�F�N�gX�̏����̒�`�E�錾[objectX.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "object.h"
#include "manager.h"

//-----------------------------------------
// �I�u�W�F�N�gX�N���X (�I�u�W�F�N�g�h��)
//-----------------------------------------
class CObjectX : public CObject
{
// ���J
public:
	CObjectX() : m_mtxWorld{} {}
	CObjectX(Index priority) : CObject(priority), m_mtxWorld{} {}
	virtual ~CObjectX() = default;

	static CObjectX* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, Index priority = 3);
	static CObjectX* Create(DirectX::Transform transform, TYPE type, Index priority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type);
	HRESULT Init(DirectX::Transform transform, TYPE type);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

// ����J
private:
	void OnCollision(const CObject& other) override {};

	static Index m_modelIdx; // ���f���C���f�b�N�X

	D3DXMATRIX m_mtxWorld;                // �}�g���b�N�X
};