//------------------------------------------
//
// �V�[�� [scene.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "scene.h"
#include "game.h"

using namespace scene; // �V�[����Ԃ̎g�p

//-----------------------------
// 
// �V�[���N���X
// 
//-----------------------------

//------------------------------
// ��������
//------------------------------
CScene* CScene::Create(MODE mode)
{
	CScene* pScene{};
	switch (mode)
	{
	case MODE::Title:
		break;
	case MODE::Game:
		pScene = new CGame; // �Q�[���V�[���̐���
		break;
	case MODE::Result:
		break;
	}
	if (pScene == nullptr)
	{
		return nullptr; // �������s
	}

	if (FAILED(pScene->Init()))
	{
		delete pScene;    // ���������s�Ȃ�폜
		pScene = nullptr; // �|�C���^��nullptr�ɂ���
		return nullptr;   // �������s
	}
	return pScene; // ����
}