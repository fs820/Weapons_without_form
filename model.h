//------------------------------------------
//
// ���f�������̒�` [model.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "renderer.h"

//-----------------------------------------
// ���f���N���X
//-----------------------------------------
class CModel final
{
	// ���J
public:
	// �T�u�Z�b�g���
	using SubSetInfo = struct
	{
		bool bUse;      // �g���Ă��邩
		DWORD AttribId; // ID
		size_t texIdx;  // �e�N�X�`���C���f�b�N�X
	};
	// ���
	using Info = struct
	{
		LPD3DXMESH pMesh;		  // ���b�V��(���_���ւ̃|�C���^)
		LPD3DXBUFFER pBuffMat;	  // �}�e���A���ւ̃|�C���^
		D3DXMATERIAL* pMat;	      // �}�e���A���f�[�^�ւ̃|�C���^
		DWORD dwNumMat;			  // �}�e���A���̐�
		DWORD dwNumSubSet;        // �T�u�Z�b�g��
		SubSetInfo* pSubSet;      // �T�u�Z�b�g���̃|�C���^
		int nIdxParent;	          // �e���f���̃C���f�b�N�X
		const D3DXVECTOR3 offSet; // �I�t�Z�b�g
		D3DXVECTOR3 pos;          // �ʒu
		D3DXVECTOR3 rot;          // ����
		D3DXVECTOR3 scale;        // �X�P�[��
		D3DXMATRIX mtxWorld;      // ���[���h�}�g���b�N�X
	};

	CModel(const string_view sPass, const D3DXVECTOR3 offSet, const int nIdxParent = -1) :m_sPass(sPass), m_info{ .nIdxParent = nIdxParent,.offSet = offSet }, m_countUse{}, m_pModel{} {}
	~CModel() = default;

	HRESULT Init(void);
	void Uninit(void);
	void Draw(void);
	string_view GetPass(void) const { return m_sPass; }
	const Info GetInfo(void) const { return m_info; }

	void SetPos(const D3DXVECTOR3 pos) { m_info.pos = pos; }
	void SetRot(const D3DXVECTOR3 rot) { m_info.rot = rot; }
	void SetScale(const D3DXVECTOR3 scale) { m_info.scale = scale; }

	void SetParent(const int nIdxParent);

	void AddUseCount(void) { m_countUse++; }

	// ����J
private:
	const string_view m_sPass; // �p�X
	const CModel* m_pModel;    // �e���f���̃|�C���^

	Info m_info;
	size_t m_countUse;             // �ǂꂾ���g���Ă邩
};

//----------------------------------------------
// ���f���Ǘ��N���X �V���O���g�� �R�s�[�֎~
//----------------------------------------------
class CModelManager final
{
	// ���J
public:
	// �ŏ�����\�ߓǂݍ��ރ��f����
	using TYPE = enum
	{
		PLAYER = 0,
		TYPE_MAX
	};

	static CModelManager& GetInstance(void) { return m_instance; };

	CModelManager(const CModelManager&) = delete;
	CModelManager& operator=(const CModelManager&) = delete;

	HRESULT Load(void);
	void Unload(void);

	Index Register(const string_view sModelPass, const D3DXVECTOR3 offSet, const int nIdxParent = -1);
	void Draw(size_t idx) const { if (idx >= 0 && idx < m_nNumAll) m_apModel[idx]->Draw(); }

	const CModel* const GetModel(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx] : nullptr; }
	string_view GetPass(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetPass() : string_view(); }
	const LPD3DXMESH GetMesh(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().pMesh : nullptr; }
	const LPD3DXBUFFER GetBuffMat(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().pBuffMat : nullptr; }
	const D3DXMATERIAL* GetMat(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().pMat : nullptr; }
	DWORD GetNumMat(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().dwNumMat : 0; }
	DWORD GetNumSubSet(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().dwNumSubSet : 0; }
	const CModel::SubSetInfo* GetSubSet(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().pSubSet : nullptr; }
	const int GetParent(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().nIdxParent : 0; }
	const D3DXVECTOR3 GetOffSet(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().offSet : 0; }
	D3DXMATRIX GetMatrix(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().mtxWorld : 0; }

	D3DXVECTOR3 GetPos(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().pos : 0; }
	D3DXVECTOR3 GetRot(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().rot : 0; }
	D3DXVECTOR3 GetScale(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().scale : 0; }
	void SetPos(size_t idx, const D3DXVECTOR3 pos) { if (idx >= 0 && idx < m_nNumAll)  m_apModel[idx]->SetPos(pos); }
	void SetRot(size_t idx, const D3DXVECTOR3 rot) { if (idx >= 0 && idx < m_nNumAll)  m_apModel[idx]->SetRot(rot); }
	void SetScale(size_t idx, const D3DXVECTOR3 scale) { if (idx >= 0 && idx < m_nNumAll)  m_apModel[idx]->SetScale(scale); }

	void SetParent(size_t idx, const int nIdxParent) { if (idx >= 0 && idx < m_nNumAll)  m_apModel[idx]->SetParent(nIdxParent); }

	// ����J
private:
	static constexpr size_t MAX_MODEL = 128; // ���f���ő吔

	static CModelManager m_instance;  // �����̃C���X�^���X

	CModelManager() : m_apModel{}, m_nNumAll{},
		// �K�胂�f�����X�g
		m_typePass
	{
		"data\\MODEL\\player000.x" // �v���C���[
	},  // �K�胂�f���e
		m_typeParent
	{
		-1                         // �v���C���[
	},  // �K�胂�f���I�t�Z�b�g
		m_typeOffSet
	{
		{0.0f,0.0f,0.0f}           // �v���C���[
	} {
	}
	~CModelManager() = default;

	const string_view m_typePass[TYPE_MAX];   // �K�胂�f�����X�g
	const int m_typeParent[TYPE_MAX];         // �K�胂�f���e
	const D3DXVECTOR3 m_typeOffSet[TYPE_MAX]; // �K�胂�f���I�t�Z�b�g
	CModel* m_apModel[MAX_MODEL];             // ���f�����
	size_t m_nNumAll;                         // ���f����
};