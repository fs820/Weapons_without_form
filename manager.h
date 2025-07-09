//------------------------------------------
//
// �}�l�[�W���[�̒�` [manager.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once

#include "main.h"

class CRenderer;      // �����_���[
class CFile;          // �t�@�C��
class CDebugProc;     // �f�o�b�N�\��
class CCamera;        // �J����
class CLight;         // ���C�g
class CPlayer;        // �v���C���[
class CObject3D;      // 3D�I�u�W�F�N�g
class CScore;         // �X�R�A
class CTime;          // �^�C��
class CGui;           // Gui

// �e�N�X�`���^�O
enum class TEXTAG : Index
{
	Number,           // ����
	Ground,           // �n��
	Enemy1, Enemy2,   // �G1, �G2
	Bullet,           // �e
	EX1, EX2,         // ����1, ����2
	Shadow_Effect,    // �e�G�t�F�N�g
	Max               // �ő吔
};

// ���f���^�O
enum class MODELTAG : Index
{
	Test, // �e�X�g���f��
	// �v���C���[�̃��f��
	PBody,
	PHead,
	PRArm,
	PRHand,
	PLArm,
	PLHand,
	PRLeg,
	PRFoot,
	PLLeg,
	PLFoot,
	PHat,
	// �v���C���[�̃��f��End
	Max   // �ő吔
};

// ���[�V�����^�O
enum class MOTIONTAG : Index
{
	Player, // �v���C���[
	Max   // �ő吔
};

// �K�w�\���^�O
enum class HIERARCHYTAG : Index
{
	Player, // �v���C���[
	Max   // �ő吔
};

//----------------------------
// �}�l�[�W���[�N���X
//----------------------------
class CManager
{
// ���J
public:
	CManager() = default;
	~CManager() = default;

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	HRESULT Draw(void);

	static const CRenderer& GetRenderer(void) { return *m_pRenderer; }
	static HRESULT RenererReset(const BOOL bWindow);

	static const CObject3D& Get3D(void) { return *m_p3DObject; }

	static CCamera* const GetCamera(void) { return m_pCamera; }
	static CLight* const GetLight(void) { return m_pLight; }
	static CTime* const GetTime(void) { return m_pTime; }
	static CScore* const GetScore(void) { return m_pScore; }
	static const CGui& GetGui(void) { return *m_pGui; }

	static bool IsPause(void) { return m_bPause; }

// ����J
private:
	static bool m_bPause;                    // �|�[�Y��Ԃ��ǂ���

	// �|�C���^
	static CRenderer* m_pRenderer;           // �����_���[
	static CFile* m_pFile;                   // �t�@�C��
	static CCamera* m_pCamera;               // �J����
	static CLight* m_pLight;                 // ���C�g
	static CPlayer* m_pPlayer;               // �v���C���[
	static CObject3D* m_p3DObject;           // 3D�I�u�W�F�N�g
	static CDebugProc* m_pDebugProc;         // �f�o�b�N�\��
	static CTime* m_pTime;                   // �^�C��
	static CScore* m_pScore;                 // �X�R�A
	static CGui* m_pGui;                     // Gui

	// �e�N�X�`��
	static const path m_textureFile;                               // �t�@�C���p�X
	static const string_view m_textureTagList[Index(TEXTAG::Max)]; // �^�O���X�g

	// ���f��
	static const path m_modelFile;                                 // �t�@�C���p�X
	static const string_view m_modelTagList[Index(MODELTAG::Max)]; // �^�O���X�g

	// ���[�V����
	static const path m_motionFile;                                  // �t�@�C���p�X
	static const string_view m_motionTagList[Index(MOTIONTAG::Max)]; // �^�O���X�g

	// �K�w�\��
	static const path m_hierarchyFile;                                     // �t�@�C���p�X
	static const string_view m_hierarchyTagList[Index(HIERARCHYTAG::Max)]; // �^�O���X�g

	// �v���C���[
	static const path m_playerFile; // �t�@�C���p�X

	HRESULT LoadTexture(void);
	HRESULT LoadModel(void);
	HRESULT LoadMotion(void);
	HRESULT LoadHierarchy(void);
	HRESULT LoadGame(void);
	void UnloadGame(void);
	HRESULT StartGame(void);
	void EndGame(void);
};