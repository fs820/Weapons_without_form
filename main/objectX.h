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
	CObjectX() : m_modelIdx{} {};
	CObjectX(Index priority) : CObject(priority), m_modelIdx{} {}
	virtual ~CObjectX() = default;

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, MODELTAG model, TYPE type);
	HRESULT Init(DirectX::Transform transform, MODELTAG model, TYPE type);
	void Uninit(void) override;
	void Draw(const context::DrawContext drawContext) override;

// ����J
private:
	void OnCollision(const CObject& other) override {};

	Index m_modelIdx;  // ���f���C���f�b�N�X
};