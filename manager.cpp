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
#include "hierarchy.h"
#include "motion.h"

//------------------------------
//
// �}�l�[�W���[�N���X
//
//------------------------------

// �ÓI�����o�ϐ��̒�`

// �e�N�X�`��
const path CManager::m_textureFile = "data/TXT/texture.txt";       // �t�@�C���p�X
const string_view CManager::m_textureTagList[Index(TEXTAG::Max)] = // �^�O���X�g
{
	"Number",
	"Ground",
	"Enemy1","Enemy2",
	"Bullet",
	"EX1","EX2",
	"Shadow_Effect"
};

// ���f��
const path CManager::m_modelFile = "data/TXT/model.txt";           // �t�@�C���p�X
const string_view CManager::m_modelTagList[Index(MODELTAG::Max)] = // �^�O���X�g
{
	"XModel",
	"PBody",
	"PHead",
	"PRArm",
	"PRHand",
	"PLArm",
	"PLHand",
	"PRLeg",
	"PRFoot",
	"PLLeg",
	"PLFoot",
	"PHat"
};

// ���[�V����
const path CManager::m_motionFile = "data/TXT/motion.txt";           // �t�@�C���p�X
const string_view CManager::m_motionTagList[Index(MOTIONTAG::Max)] = // �^�O���X�g
{
	"Player"
};

// �K�w�\��
const path CManager::m_hierarchyFile = "data/TXT/hierarchy.txt";           // �t�@�C���p�X
const string_view CManager::m_hierarchyTagList[Index(HIERARCHYTAG::Max)] = // �^�O���X�g
{
	"Player"
};

// �v���C���[
const path CManager::m_playerFile = "data/TXT/player000.txt"; // �t�@�C���p�X

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

	//// �T�E���h
	//if (FAILED(CSoundManager::GetInstance().Init(CMain::GethWnd())))return E_FAIL;

	// �f�o�b�N�\��
	if (FAILED(CDebugProc::Init()))return E_FAIL;

	// �ǂݍ���
	if (FAILED(LoadTexture()))return E_FAIL;   // �e�N�X�`��
	if (FAILED(LoadModel()))return E_FAIL;     // ���f��
	if (FAILED(LoadMotion()))return E_FAIL;    // ���[�V����
	if (FAILED(LoadHierarchy()))return E_FAIL; // �K�w�\��

	// �Q�[��
	if (FAILED(LoadGame()))return E_FAIL;  // ���[�h
	if (FAILED(StartGame()))return E_FAIL; // �X�^�[�g

	return S_OK;
}

//---------------
// �I��
//---------------
void CManager::Uninit(void)
{
	EndGame();    // �Q�[���̏I��
	UnloadGame(); // �\�[�X�̔j��

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
		EndGame();   // �I��点��
		StartGame(); // ���߂ăX�^�[�g����
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
HRESULT CManager::Draw(void)
{
	// �����_���[�̕`��
	if (m_pRenderer != nullptr)
	{
		if (FAILED(m_pRenderer->Draw())) { return E_FAIL; }
	}

	return S_OK;
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

//---------------------------------
// texture�̃��[�h����
//---------------------------------
HRESULT CManager::LoadTexture()
{
	// �e�N�X�`���̃p�X���擾����
	vector<path> textureList{};
	vector<D3DXVECTOR2> textureBlockList{};
	m_pFile->ChangeFile(m_textureFile);    // �e�N�X�`�����X�g�t�@�C�����Z�b�g
	string readData = m_pFile->ReadText(); // �e�N�X�`���t�@�C���̕�������擾
	for (const auto& tag : m_textureTagList)
	{// �ǂݍ��������t���O
		// �t���O����Ƀp�X�ƃu���b�N�����擾����
		auto sPath = StringToData<string>(readData, tag);
		if (sPath.has_value())
		{
			textureList.push_back(sPath.value());
		}

		auto blockX = StringToData<float>(readData, tag, 1);
		auto blockY = StringToData<float>(readData, tag, 2);
		if (blockX.has_value() && blockY.has_value())
		{
			D3DXVECTOR2 textureBlock{ blockX.value(),blockY.value() };
			textureBlockList.push_back(textureBlock);
		}
	}
	return CTextureManager::GetInstance().Load(textureList, textureBlockList); // �e�N�X�`����ǂݍ���
}

//---------------------------------
// model�̃��[�h����
//---------------------------------
HRESULT CManager::LoadModel()
{
    // �K�胂�f���̃p�X���擾����
	vector<path> modelList{};
	m_pFile->ChangeFile(m_modelFile);    // �e�N�X�`�����X�g�t�@�C�����Z�b�g
	string readData = m_pFile->ReadText(); // �e�N�X�`���t�@�C���̕�������擾
	for (const auto& tag : m_modelTagList)
	{// �ǂݍ��������t���O
		path modelPath{};
		// �t���O����Ƀp�X�ƃu���b�N�����擾����
		auto sPath = StringToData<string>(readData, tag);
		if (sPath.has_value())
		{
			modelList.push_back(sPath.value());
		}
	}
	if (FAILED(CModelManager::GetInstance().Load(modelList)))return E_FAIL;   // �K�胂�f��

	//Index cnt{};
	//while (true)
	//{
	//	auto block = ExtractBlock(readData, "SetModel", "SetModelEnd", cnt);
	//	if (!block.has_value())
	//	{// �擾�ł��Ȃ�������
	//		break; // �I��
	//	}
	//	auto tag = StringToData<string>(block.value(), "Tag");
	//	if (tag.has_value())
	//	{
	//		for (Index cntTag = 0; cntTag < Index(MODELTAG::MAX); cntTag++)
	//		{
	//			if (tag.value() == m_modelTagList[cntTag])
	//			{
	//				// �ʒu
	//				optional<float> pos[3]{};
	//				for (Index cnt = 0; cnt < 3; cnt++)
	//				{
	//					pos[cnt] = StringToData<float>(block.value(), "Pos", cnt);
	//				}
	//				if (pos[X].has_value() && pos[Y].has_value() && pos[Z].has_value())
	//				{
	//					D3DXVECTOR3 posData = { pos[X].value(),pos[Y].value(),pos[Z].value() };
	//					CModelManager::GetInstance().SetPos(cntTag, posData);  // �ʒu���Z�b�g
	//				}

	//				// ��]
	//				optional<float> rot[3]{};
	//				for (Index cnt = 0; cnt < 3; cnt++)
	//				{
	//					rot[cnt] = StringToData<float>(block.value(), "Rot", cnt);
	//				}
	//				if (rot[X].has_value() && rot[Y].has_value() && rot[Z].has_value())
	//				{
	//					D3DXVECTOR3 rotData = { rot[X].value(),rot[Y].value(),rot[Z].value() };
	//					CModelManager::GetInstance().SetRot(cntTag, rotData);     // ��]���Z�b�g
	//				}

	//				// �X�P�[��
	//				optional<float> scale[3]{};
	//				for (Index cnt = 0; cnt < 3; cnt++)
	//				{
	//					scale[cnt] = StringToData<float>(block.value(), "Scale", cnt);
	//				}
	//				if (scale[X].has_value() && scale[Y].has_value() && scale[Z].has_value())
	//				{
	//					D3DXVECTOR3 scaleData = { scale[X].value(),scale[Y].value(),scale[Z].value() };
	//					CModelManager::GetInstance().SetScale(cntTag, scaleData); // �X�P�[�����Z�b�g
	//				}
	//			}
	//		}
	//	}
	//	cnt++; // ���̃u���b�N��
	//}

	return S_OK; // ����
}

//---------------------------------
// ���[�V�����̃��[�h����
//---------------------------------	
HRESULT CManager::LoadMotion(void)
{
	// �K�胂�f���̃p�X���擾����
	vector<path> modelList{};
	m_pFile->ChangeFile(m_motionFile);    // ���[�V�������X�g�t�@�C�����Z�b�g
	string readData = m_pFile->ReadText(); // ���[�V�����t�@�C���̕�������擾

	vector<motion::Source> aSource{};
	for (const auto& tag : m_motionTagList)
	{
		auto block = ExtractBlock(readData, tag, string_view(string(tag) + "End"));
		if (!block.has_value())
		{// �擾�ł��Ȃ�������
			break; // �I��
		}

		motion::Source source{};
		Index cntMotion{};
		while (true)
		{
			auto motionBlock = ExtractBlock(block.value(), "Motion", "MotionEnd", cntMotion);
			if (!motionBlock.has_value())
			{
				break;
			}

			motion::Motion Motion{};

			auto loop = StringToData<bool>(motionBlock.value(), "Loop");
			if (!loop.has_value())
			{
				break;
			}
			Motion.bLoop = loop.value();

			Index cntKey{};
			while (true)
			{
				auto keyBlock = ExtractBlock(motionBlock.value(), "Key", "KeyEnd", cntKey);
				if (!keyBlock.has_value())
				{
					break;
				}

				motion::Key key{};

				auto frame = StringToData<Index>(keyBlock.value(), "Frame");
				if (!frame.has_value())
				{
					break;
				}
				key.frame = frame.value();

				Index cntParts{};
				while (true)
				{
					auto partsBlock = ExtractBlock(keyBlock.value(), "Set", "End", cntParts);
					if (!partsBlock.has_value())
					{
						break;
					}

					motion::Parts parts{};

					// �ʒu
					optional<float> pos[3]{};
					for (Index cnt = 0; cnt < 3; cnt++)
					{
						pos[cnt] = StringToData<float>(partsBlock.value(), "Pos", cnt);
					}
					if (pos[X].has_value() && pos[Y].has_value() && pos[Z].has_value())
					{
						parts.pos = { pos[X].value(),pos[Y].value(),pos[Z].value() };
					}

					// ��]
					optional<float> rot[3]{};
					for (Index cnt = 0; cnt < 3; cnt++)
					{
						rot[cnt] = StringToData<float>(partsBlock.value(), "Rot", cnt);
					}
					if (rot[X].has_value() && rot[Y].has_value() && rot[Z].has_value())
					{
						parts.rot = { rot[X].value(),rot[Y].value(),rot[Z].value() };
					}
					key.aParts.push_back(parts); // �p�[�c���ǉ�
					++cntParts; // ���̃p�[�c��
				}
				Motion.aKey.push_back(key); // �L�[���ǉ�
				++cntKey; // ���̃L�[��
			}
			source.aMotion.push_back(Motion); // ���[�V�������ǉ�
			++cntMotion; // ���̃��[�V������
		}
		aSource.push_back(source); // �\�[�X�ǉ�
	}

	CMotionManager::GetInstance().Load(aSource); // ���[�V������ǂݍ���

	return S_OK; // ����
}

//---------------------------------
// �q�G�����L�[�̃��[�h����
//---------------------------------
HRESULT CManager::LoadHierarchy(void)
{
	// �K�胂�f���̃p�X���擾����
	m_pFile->ChangeFile(m_hierarchyFile);  // �K�w�\���t�@�C�����Z�b�g
	string readData = m_pFile->ReadText(); // �K�w�\���t�@�C���̕�������擾

	for (const auto& tag : m_hierarchyTagList)
	{// �ǂݍ��������t���O
		auto Hierarchy = ExtractBlock(readData, tag, string_view(string(tag) + "End"));

		vector < hierarchy::PartsInfo> hierarchy{}; // �K�w�\�����

		Index cnt{};
		while (true)
		{
			hierarchy::PartsInfo partsInfo{}; // �p�[�c���

			auto block = ExtractBlock(Hierarchy.value(), "Set", "End", cnt);
			if (!block.has_value())
			{// �擾�ł��Ȃ�������
				break; // �I��
			}
			auto tag = StringToData<string>(block.value(), "Tag");
			if (tag.has_value())
			{
				for (Index cntTag = 0; cntTag < Index(MODELTAG::Max); cntTag++)
				{
					if (tag.value() == m_modelTagList[cntTag])
					{
						partsInfo.modelTag = cntTag; // ���f���̃C���f�b�N�X��ǉ�
					}
				}
			}
			auto Parent = StringToData<int>(block.value(), "Parent");
			if (Parent.has_value())
			{
				partsInfo.parentIdx = Parent.value(); // ���f���̃C���f�b�N�X��ǉ�
			}
			float offSet[3]{};
			for (size_t cntOffSet = 0; cntOffSet < 3; cntOffSet++)
			{
				auto OffSet = StringToData<float>(block.value(), "OffSet", cntOffSet);
				if (OffSet.has_value())
				{
					offSet[cntOffSet] = OffSet.value();
				}
				else
				{
					offSet[cntOffSet] = 0.0f; // �f�t�H���g�l
				}
			}
			partsInfo.offSet = { offSet[X], offSet[Y], offSet[Z] }; // ���f���̃C���f�b�N�X��ǉ�

			hierarchy.push_back(partsInfo); // �p�[�c����ǉ�
			cnt++; // ���̃u���b�N��
		}

		CHierarchyManager::GetInstance().Register(hierarchy); // �K�w�\������o�^
	}

	return S_OK; // ����
}

//---------------------------------
// �Q�[���̃��[�h����
//---------------------------------
HRESULT CManager::LoadGame(void)
{
	return S_OK;
}

//---------------------------------
// �Q�[���̃A�����[�h����
//---------------------------------
void CManager::UnloadGame(void)
{
	CModelManager::GetInstance().Release();
	CMotionManager::GetInstance().Release();
}

//---------------------------------
// �Q�[���̃X�^�[�g����
//---------------------------------
HRESULT CManager::StartGame(void)
{
	//--------------------
	// �C���X�^���X����
	//--------------------

	// �v���C���[
	float scale = CMath::Random(1.0f, 1.0f);
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, RandomAngle, 0.0f), D3DXVECTOR3(scale, scale, scale), CMath::Random(100.0f, 500.0f), CMath::Random(-0.01f, 0.1f));
	if (m_pPlayer == nullptr)
	{
		return E_POINTER;
	}

	// 3D
	m_p3DObject = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), CObject::TYPE::None, 2);
	if (m_p3DObject==nullptr)
	{
		return E_POINTER;
	}

	// X
	if (CObjectX::Create(D3DXVECTOR3(0.0f, 0.0f, 50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.5f, 0.5f, 0.5f), CObject::TYPE::None) == nullptr)
	{
		return E_POINTER;
	}

	// Billboard
	if (CObjectBillboard::Create(D3DXVECTOR3(0.0f, 10.0f, 10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), CObject::TYPE::None, 2) == nullptr)
	{
		return E_POINTER;
	}

	// �^�C��
	m_pTime = CTime::Create(D3DXVECTOR3(0.5f, 0.05f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2, 0.03f, 7);
	if (m_pTime == nullptr)
	{
		return E_POINTER;
	}

	// �X�R�A
	m_pScore = CScore::Create(D3DXVECTOR3(0.9f, 0.05f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 5, 0.03f, 7);
	if (m_pScore == nullptr)
	{
		return E_POINTER;
	}

	m_bPause = false; // �|�[�Y��Ԃ�������

	m_pTime->SetTime(120); // ���ԃZ�b�g
	m_pTime->CountDown(); // �J�E���g�_�E��

	CSoundManager::GetInstance().Play(Index8(sound::LABEL::TestBgm)); // BGM�Đ�

	return S_OK;
}

//---------------------------------
// �Q�[���̃G���h����
//---------------------------------
void CManager::EndGame(void)
{
	CSoundManager::GetInstance().Stop(); // �T�E���h��~

	// �I�u�W�F�N�g�j��
	CObject::ReleaseAll();
}