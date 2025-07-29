//------------------------------------------
//
// �e�̒�`�E�錾[shadow.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "objectX.h"

//----------------------------
// �e�N���X (�I�u�W�F�N�gX�h��)
//----------------------------
class CShadow : public CObjectX
{
// ���J
public:
	CShadow() = default;
	CShadow(Index priority) : CObjectX(priority) {}
	~CShadow() = default;

	static CShadow* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, Index priority = 0);
	static CShadow* Create(DirectX::Transform transform, Index priority = 0);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale);
	HRESULT Init(DirectX::Transform transform);
	void Uninit(void) override {};
	void Draw(const context::DrawContext drawContext) override;

// ����J
private:
	void OnCollision(const CObject& other) override {};

	static LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; // ���_
};