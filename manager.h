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

//----------------------------
// �}�l�[�W���[�N���X
//----------------------------
class CManager
{
// ���J
public:
	// �e�N�X�`���^�O
	using TEXTAG = enum
	{
		Number,
		Enemy1, Enemy2,
		Bullet1, Bullet2,
		EX1, EX2,
		Shadow_Effect
	};

	CManager() = default;
	~CManager() = default;

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Update(void);
	void Draw(void);
	void Uninit(void);

	static const CRenderer& GetRenderer(void) { return *m_pRenderer; }
	static HRESULT RenererReset(const BOOL bWindow);

	static HRESULT GameLoad(void);
	static HRESULT GameStart(void);
	static void GameEnd(void);
	static void GameUnload(void);

	static const CObject3D& Get3D(void) { return *m_p3DObject; }

	static CCamera* const GetCamera(void) { return m_pCamera; }
	static CLight* const GetLight(void) { return m_pLight; }
	static CTime* const GetTime(void) { return m_pTime; }
	static CScore* const GetScore(void) { return m_pScore; }
	static const CGui& GetGui(void) { return *m_pGui; }

	static bool IsPause(void) { return m_bPause; }

// ����J
private:
	static const path m_textureFile; // �e�N�X�`�����X�g
	static const string_view m_textureTagList[]; // �g���e�N�X�`�����w�� (txt�̃^�O����ɂ���)

	static CRenderer* m_pRenderer;           // �����_���[�̃|�C���^
	static CFile* m_pFile;                   // �t�@�C���̃|�C���^
	static CCamera* m_pCamera;               // �J�����̃|�C���^
	static CLight* m_pLight;                 // ���C�g�̃|�C���^
	static CPlayer* m_pPlayer;               // �v���C���[�̃|�C���^
	static CObject3D* m_p3DObject;           // 3D�I�u�W�F�N�g�̃|�C���^
	static CDebugProc* m_pDebugProc;         // �f�o�b�N�\���̃|�C���^
	static CTime* m_pTime;                   // �^�C���̃|�C���^
	static CScore* m_pScore;                 // �X�R�A�̃|�C���^
	static CGui* m_pGui;                     // Gui�̃|�C���^

	static bool m_bPause;                    // �|�[�Y��Ԃ��ǂ���
};