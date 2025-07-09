//---------------------------------------
//
// ���C�g�����̒�` [light.h]
// Author: fuma sato
//
//---------------------------------------
#pragma once
#include "main.h"
#include "renderer.h"

//-----------------
// ���C�g�N���X
//-----------------
class CLight
{
// ���J
public:
	void Init(void);                //����������
	void Uninit(void);              //�I������
	void Update(void);              //�X�V����
	size_t Set(D3DLIGHT9* pLight);  //���C�g�ݒ�

// ����J
private:
	array<D3DLIGHT9, 8> m_light; // ���C�g���
};