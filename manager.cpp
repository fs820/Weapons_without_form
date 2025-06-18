//------------------------------------------
//
// �}�l�[�W���[ [manager.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "manager.h"
#include "renderer.h"
#include "file.h"
#include "input.h"
#include "object.h"
#include "object2D.h"
#include "object3D.h"
#include "player.h"
#include "bg.h"
#include "bullet.h"
#include "explosion.h"
#include "math.h"
#include "enemy.h"
#include "effect.h"
#include "debug.h"
#include "sound.h"
#include "number.h"
#include "score.h"
#include "time.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "objectbillboard.h"
#include "model.h"
#include "objectX.h"
#include "gui.h"

//------------------------------
//
// �}�l�[�W���[�N���X
//
//------------------------------

// �ÓI�����o�ϐ��̒�`
const path CManager::m_textureFile = "data/TXT/texture.txt"; // �e�N�X�`�����X�g
// �g���e�N�X�`�����w�� (txt�̃^�O����ɂ���)
const string_view CManager::m_textureTagList[] =
{
	"Number",
	"Enemy1","Enemy2",
	"Bullet1","Bullet2",
	"EX1","EX2",
	"Shadow_Effect"
};

CRenderer* CManager::m_pRenderer{};
CFile* CManager::m_pFile{};
CCamera* CManager::m_pCamera{};
CLight* CManager::m_pLight{};
CObject3D* CManager::m_p3DObject{};
CPlayer* CManager::m_pPlayer{};
CTime* CManager::m_pTime{};
CScore* CManager::m_pScore{};
CGui* CManager::m_pGui{};

// �|�[�Y��Ԃ��ǂ���
bool CManager::m_bPause{};

//---------------
// ������
//---------------
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// �`��N���X
	if (m_pRenderer == nullptr)
	{
		m_pRenderer = new CRenderer;
	}

	if (m_pRenderer != nullptr)
	{
		if (FAILED(m_pRenderer->Init(hInstance, hWnd, bWindow)))
		{
			return E_FAIL;
		}
	}

	// �t�@�C��
	if (m_pFile == nullptr)
	{
		m_pFile = new CFile;
	}

	// �J����
	if (m_pCamera == nullptr)
	{
		m_pCamera = new CCamera;
	}

	if (m_pCamera != nullptr)
	{
		m_pCamera->Init();
	}

	// ���C�g
	if (m_pLight == nullptr)
	{
		m_pLight = new CLight;
	}

	if (m_pLight != nullptr)
	{
		m_pLight->Init();
	}

	// Gui
	if (m_pGui == nullptr)
	{
		m_pGui = new CGui;
	}

	if (m_pGui != nullptr)
	{
		m_pGui->Init();
	}

	//// �C���v�b�g
	//if (FAILED(CInput::GetInstance().Init(hInstance, hWnd)))return E_FAIL;

	// �T�E���h
	if (FAILED(CSoundManager::GetInstance().Init(CMain::GethWnd())))return E_FAIL;

	// �f�o�b�N�\��
	if (FAILED(CDebugProc::Init()))return E_FAIL;

	// �ǂݍ���
	
	//-------------------------------
	// �K��e�N�X�`���̃p�X���擾����
	//-------------------------------
	vector<path> sTextureList{};
	vector<D3DXVECTOR2> textureBlockList{};
	m_pFile->ChangeFile(m_textureFile);    // �e�N�X�`�����X�g�t�@�C�����Z�b�g
	string readData = m_pFile->ReadText(); // �e�N�X�`���t�@�C���̕�������擾
	for (const auto& tag : m_textureTagList)
	{// �ǂݍ��������t���O
		// �t���O����Ƀp�X�ƃu���b�N�����擾����
		auto sPath = StringToData<string>(readData, tag);
		if (sPath.has_value())
		{
			sTextureList.push_back(sPath.value());
		}

		auto blockX = StringToData<float>(readData, tag, 1);
		auto blockY = StringToData<float>(readData, tag, 2);
		if (blockX.has_value() && blockY.has_value())
		{
			D3DXVECTOR2 textureBlock{ blockX.value(),blockY.value() };
			textureBlockList.push_back(textureBlock);
		}
	}
	if (FAILED(CTextureManager::GetInstance().Load(sTextureList, textureBlockList)))return E_FAIL; // �K��e�N�X�`����ǂݍ���

	if (FAILED(CModelManager::GetInstance().Load()))return E_FAIL;   // �K�胂�f��

	// �Q�[��
	if (FAILED(GameLoad()))return E_FAIL;  // ���[�h
	if (FAILED(GameStart()))return E_FAIL; // �X�^�[�g

	return S_OK;
}

//---------------
// �I��
//---------------
void CManager::Uninit(void)
{
	GameEnd();    // �Q�[���̏I��
	GameUnload(); // �\�[�X�̔j��

	// �f�o�b�N�\��
	CDebugProc::Uninit();

	// �T�E���h
	CSoundManager::GetInstance().Uninit();

	// ���f��
	CModelManager::GetInstance().Unload();

	// �e�N�X�`��
	CTextureManager::GetInstance().Unload();

	//// �C���v�b�g
	//CInput::GetInstance().Uninit();

	// Gui
	if (m_pGui != nullptr)
	{
		m_pGui->Uninit(); //�I������
		delete m_pGui;    // �C���X�^���X�j��
		m_pGui = nullptr; // nullptr
	}

	// ���C�g
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit(); //�I������
		delete m_pLight;    // �C���X�^���X�j��
		m_pLight = nullptr;    // nullptr
	}

	// �J����
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit(); //�I������
		delete m_pCamera;    // �C���X�^���X�j��
		m_pCamera = nullptr; // nullptr
	}

	// �t�@�C��
	if (m_pFile != nullptr)
	{
		delete m_pFile;    // �C���X�^���X�j��
		m_pFile = nullptr; // nullptr
	}

	// �����_���[
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit(); //�I������
		delete m_pRenderer;    // �C���X�^���X�j��
		m_pRenderer = nullptr;    // nullptr
	}
}

//---------------
// �X�V
//---------------
void CManager::Update(void)
{
	//// �C���v�b�g�X�V
	//CInput::GetInstance().Update();

	//-----------------------------
    // �Ǘ�&Debug����֌W
    //-----------------------------
	if (m_pTime->GetTime() <= 0.0f)
	{// ���Ԃ��Ȃ��Ȃ�����
		GameEnd();   // �I��点��
		GameStart(); // ���߂ăX�^�[�g����
	}

	// �J�[�\���ʒu
	D3DXVECTOR2 cursorPos{};
	m_pRenderer->GetCursorPos(&cursorPos);
	CDebugProc::Print("Cursor�ʒu X:%.2f Y:%.2f", cursorPos.x, cursorPos.y);

	//CDebugProc::Print("�|�[�Y:[P]");
	//if (m_pInputKeyboard->GetTrigger(DIK_P))
	//{// �|�[�Y�ؑ�
	//	m_bPause = !m_bPause;
	//}

	//CDebugProc::Print("�f�o�b�N�\���ؑ�:[F2]");
	//if (m_pInputKeyboard->GetTrigger(DIK_F2))
	//{// �f�o�b�N�\���ؑ�
	//	CDebugProc::ToggleDebugDraw();
	//}
	//CDebugProc::Print("�f�o�b�N���[�h�ؑ�:[F3]");
	//if (m_pInputKeyboard->GetTrigger(DIK_F3))
	//{// �f�o�b�N���[�h�ؑ�
	//	CDebugProc::ChangeDrawMode();
	//}

	//CDebugProc::Print(CDebugProc::SYSTEM, "�����X�R�A����:[1][2]");
	//if (m_pInputKeyboard->GetPress(DIK_1))
	//{// �X�R�A
	//	m_pScore->AddScore(1);
	//}
	//if (m_pInputKeyboard->GetPress(DIK_2))
	//{// �X�R�A
	//	m_pScore->AddScore(-1);
	//}

	//CDebugProc::Print(CDebugProc::SYSTEM, "����BGM:[B][N]");
	//if (m_pInputKeyboard->GetTrigger(DIK_B))
	//{// �T�E���h
	//	CSoundManager::GetInstance().Play(CSoundManager::LABEL_TEST_BGM);
	//}
	//if (m_pInputKeyboard->GetTrigger(DIK_N))
	//{// �T�E���h
	//	CSoundManager::GetInstance().Stop(CSoundManager::LABEL_TEST_BGM, 0);
	//}

	//CDebugProc::Print(CDebugProc::SYSTEM, "BGM����:[J][K]");
	//if (m_pInputKeyboard->GetTrigger(DIK_J))
	//{// �T�E���h
	//	CSoundManager::GetInstance().AddVolume(CSoundManager::LABEL_TEST_BGM, 0, 0.1f);
	//}
	//if (m_pInputKeyboard->GetTrigger(DIK_K))
	//{// �T�E���h
	//	CSoundManager::GetInstance().AddVolume(CSoundManager::LABEL_TEST_BGM, 0, -0.1f);
	//}

	//CDebugProc::Print(CDebugProc::SYSTEM, "����SE:[G][H]");
	//if (m_pInputKeyboard->GetTrigger(DIK_G))
	//{// �T�E���h
	//	CSoundManager::GetInstance().Play(CSoundManager::LABEL_TEST_SE);
	//}
	//if (m_pInputKeyboard->GetTrigger(DIK_H))
	//{// �T�E���h
	//	CSoundManager::GetInstance().Stop(CSoundManager::LABEL_TEST_SE, 0);
	//}

	//-----------------------------
    // �Ǘ�&Debug����֌W End
    //-----------------------------

	m_pCamera->Update();

	// �����_���[�̍X�V
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Update();
	}

	// �T�E���h�X�V
	CSoundManager::GetInstance().Update();
}

//---------------
// �`��
//---------------
void CManager::Draw(void)
{
	// �����_���[�̕`��
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Draw();
	}
}

//---------------------------------
// �Q�[���̃��[�h����
//---------------------------------
HRESULT CManager::GameLoad(void)
{
	//--------------------
	// �\�[�X�ǂݍ���
    //--------------------

	const string_view sModelList[] =
	{
		"data\\MODEL\\Playermodel\\00_Player_Body.x",
		"data\\MODEL\\Playermodel\\01_Player_Head.x",
		"data\\MODEL\\Playermodel\\02_Player_RightArm.x",
		"data\\MODEL\\Playermodel\\03_Player_RightHand.x",
		"data\\MODEL\\Playermodel\\04_Player_LeftArm.x",
		"data\\MODEL\\Playermodel\\05_Player_LeftHand.x",
		"data\\MODEL\\Playermodel\\06_Player_RightLeg.x",
		"data\\MODEL\\Playermodel\\07_Player_RightFoot.x",
		"data\\MODEL\\Playermodel\\08_Player_LeftLeg.x",
		"data\\MODEL\\Playermodel\\09_Player_LeftFoot.x",
		"data\\MODEL\\Playermodel\\10_Player_Hat.x"
	};
	const D3DXVECTOR3 sModelOffSet[] =
	{
		{ 0.0f,20.0f,0.0f },
		{ 0.0f,25.0f,0.0f },
		{ -7.0f,20.0f,0.0f },
		{ -7.0f,0.0f,0.0f },
		{ 7.0f,20.0f,0.0f },
		{ 7.0f,0.0f,0.0f },
		{ -5.0f,0.0f,0.0f },
		{ 0.0f,-7.0f,0.0f },
		{ 5.0f,0.0f,0.0f },
		{ 0.0f,-7.0f,0.0f },
		{ 0.0f,15.0f,0.0f }
	};
	const int sModelParents[] =
	{
		-1,
		0,
		0,
		2,
		0,
		4,
		0,
		6,
		0,
        8,
		1
	};
	if (FAILED(CPlayer::Load(11, sModelList, sModelOffSet, sModelParents))) return E_FAIL;     // �v���C���[

	return S_OK;
}

//---------------------------------
// �Q�[���̃X�^�[�g����
//---------------------------------
HRESULT CManager::GameStart(void)
{
	//--------------------
	// �C���X�^���X����
	//--------------------

	// �v���C���[
	float scale = CMath::Random(1.0f, 1.0f);
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 100.0f), D3DXVECTOR3(0.0f, RandomAngle, 0.0f), D3DXVECTOR3(scale, scale, scale), CMath::Random(100.0f, 500.0f), CMath::Random(-0.01f, 0.1f));
	if (m_pPlayer == nullptr)
	{
		return E_POINTER;
	}

	// 3D
	m_p3DObject = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), CObject::NONE);
	if (m_p3DObject==nullptr)
	{
		return E_POINTER;
	}

	// X
	if (CObjectX::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 10.0f), CObject::NONE) == nullptr)
	{
		return E_POINTER;
	}

	// Billboard
	if (CObjectBillboard::Create(D3DXVECTOR3(0.0f, 100.0f, 10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.1f, 0.1f, 0.1f), CObject::NONE) == nullptr)
	{
		return E_POINTER;
	}

	// �^�C��
	m_pTime = CTime::Create(D3DXVECTOR3(0.5f, 0.05f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2, 0.03f, 6);
	if (m_pTime == nullptr)
	{
		return E_POINTER;
	}

	// �X�R�A
	m_pScore = CScore::Create(D3DXVECTOR3(0.9f, 0.05f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 5, 0.03f, 6);
	if (m_pScore == nullptr)
	{
		return E_POINTER;
	}

	m_bPause = false; // �|�[�Y��Ԃ�������

	m_pTime->SetTime(120); // ���ԃZ�b�g
	m_pTime->CountDown(); // �J�E���g�_�E��

	CSoundManager::GetInstance().Play(CSoundManager::LABEL_TEST_BGM); // BGM�Đ�

	return S_OK;
}

//---------------------------------
// �Q�[���̃G���h����
//---------------------------------
void CManager::GameEnd(void)
{
	CSoundManager::GetInstance().Stop(); // �T�E���h��~

	// �I�u�W�F�N�g�j��
	CObject::ReleaseAll();
}

//---------------------------------
// �Q�[���̃A�����[�h����
//---------------------------------
void CManager::GameUnload(void)
{
	CEffect::Unload();    // Effect
	CExplosion::Unload(); // ����
	CBullet::Unload();    // �o���b�g
	CEnemy::Unload();     // �G
	CPlayer::Unload();    // �v���C���[
}

//-------------------
// �����_���[���Z�b�g
//-------------------
HRESULT CManager::RenererReset(const BOOL bWindow)
{
	if (m_pRenderer != nullptr)
	{
		HRESULT hr = m_pRenderer->ReSize(bWindow);// ���T�C�Y����

		return hr;
	}
	else
	{
		return E_POINTER;
	}
}