//------------------------------------------
//
// ���[�V���������̒�` [motion.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "renderer.h"

namespace motion
{
	// 1�̃p�[�c�\��
	struct Parts
	{
		D3DXVECTOR3 pos; // �ʒu
		D3DXVECTOR3 rot; // ����
	};

	// 1�̃L�[�\��
	struct Key
	{
		Index frame;			  // �Đ��t���[��
		vector<Parts> aParts; // �e�p�[�c�̃L�[�v�f
	};

	// 1�̃��[�V�����\��
	struct Motion
	{
		bool bLoop;		  // ���[�v���邩�ǂ���
		vector<Key> aKey; // �L�[���
	};

	// ���[�V��������p�f�[�^
	struct Data
	{
		bool bLoop;                     // ���[�v���邩�ǂ���
		Index key;						// ���݂̃L�[No.
		Index nextKey;	                // ���̃L�[No.
		Index counterKey;				// �L�[�J�E���^�[
		Index counterMotion;			// ���[�V�����J�E���^�[
		bool MotionEnd;                 // ���[�V�������I����Ă��邩

		bool bLoopBlend;                // �u�����h�悪���[�v���邩�ǂ���
		Index keyBlend;					// �u�����h��̌��݂̃L�[No.
		Index nextKeyBlend;	            // �u�����h��̎��̃L�[No.
		Index counterKeyBlend;		    // �u�����h�L�[�J�E���^�[
		Index counterMotionBlend;		// �u�����h���[�V�����J�E���^�[
		bool MotionEndBlend;            // �u�����h��̃��[�V�������I����Ă��邩

		bool bBlend;                    // �u�����h���邩
		Index blendFrame;               // �u�����h�t���[��
		Index blendCounter;             // �u�����h�J�E���^�[
	};

	// �^�O���Ƃ̃��[�V�����̉�
	struct Source
	{
		vector<Motion> aMotion; // ���[�V�����\�����X�g
	};
}

//-----------------------------------------
// ���[�V�����N���X
//-----------------------------------------
class CMotion final
{
	// �N���Xusing
	using Motion = motion::Motion;
	using Data = motion::Data;
	using Key = motion::Key;
	using Parts = motion::Parts;
// ���J
public:
	CMotion() : m_modelIndex{}, m_pMotion{}, m_pBlendMotion{}, m_data{} {}
	~CMotion() = default;

	void RegisterModel(const vector<Index> modelIndex) { m_modelIndex = modelIndex; };
	void Set(const Motion* pMotion, bool bBlend, int blendFrame);
	bool Update(void);

// ����J
private:
	vector<Index> m_modelIndex;   // �K�����郂�f���̃C���f�b�N�X(���f���N���X�ˑ�)
	const Motion* m_pMotion;      // ���̃��[�V�������̃|�C���^
	const Motion* m_pBlendMotion; // �u�����h��̃��[�V�������̃|�C���^
	Data m_data;                  // ���[�V�����f�[�^
};

//----------------------------------------------
// ���[�V�����Ǘ��N���X �V���O���g�� �R�s�[�֎~
//----------------------------------------------
class CMotionManager final
{
	// �N���Xusing
	using Source = motion::Source;
// ���J
public:
	static CMotionManager& GetInstance(void) { return m_instance; }

	CMotionManager(const CMotionManager&) = delete;
	CMotionManager& operator=(const CMotionManager&) = delete;

	void Load(const vector<Source> aSource) { m_aSource = aSource; }

	HRESULT Register(const Source source) { m_aSource.push_back(source); }

	Index Create(void);
	void Release(void);

	void RegisterModel(Index idx, const vector<Index> modelIndex) { if (idx >= 0 && idx < m_apMotion.size()) m_apMotion[idx]->RegisterModel(modelIndex); }
	void Set(Index idx, Index motionTag, Index motionIdx, bool bBlend = false, int blendFrame = 60) { if (idx >= 0 && idx < m_apMotion.size() && motionTag >= 0 && motionTag < m_aSource.size() && motionIdx >= 0 && motionIdx < m_aSource[motionTag].aMotion.size()) m_apMotion[idx]->Set(&m_aSource[motionTag].aMotion[motionIdx], bBlend, blendFrame); }
	void Update(Index idx) const { if (idx >= 0 && idx < m_apMotion.size()) m_apMotion[idx]->Update(); }
// ����J
private:
	static CMotionManager m_instance;  // �����̃C���X�^���X

	CMotionManager() : m_apMotion{}, m_aSource{} {}
	~CMotionManager() = default;

	vector<Source> m_aSource;     // �ǂݍ��񂾃��[�V�����f�[�^�̊i�[��
	vector<CMotion*> m_apMotion;  // ���[�V�����̃|�C���^�z��
};