//------------------------------------------
//
// ���f�������̒�` [model.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "renderer.h"

namespace model
{
	// ���b�V�����
	struct MeshSource
	{
		LPD3DXMESH pMesh{};		  // ���b�V��(���_���ւ̃|�C���^)
		LPD3DXBUFFER pBuffMat{};  // �}�e���A���ւ̃|�C���^
		D3DXMATERIAL* pMat{};	  // �}�e���A���f�[�^�ւ̃|�C���^
		DWORD dwNumMat{};	      // �}�e���A���̐�
		DWORD dwNumSubSet{};      // �T�u�Z�b�g��
		size_t dwNumVertex{};     // ���_��
		size_t sizeFVF{};         // �t���[���o�b�t�@�̃T�C�Y 
		BYTE* pVtxBuff{};         // ���_�o�b�t�@�ւ̃|�C���^
		D3DXVECTOR3 vtxMin{};     // ���_�̍ŏ��l
		D3DXVECTOR3 vtxMax{};     // ���_�̍ő�l
	};
	// �T�u�Z�b�g���
	struct SubSetSource
	{
		bool bUse{};      // �g���Ă��邩
		DWORD AttribId{}; // ID
		Index texIdx{};   // �e�N�X�`���C���f�b�N�X
	};
	// ���
	struct Source
	{
		path Path{};             // �p�X
		MeshSource mesh{};       // ���b�V�����
		SubSetSource* pSubSet{}; // �T�u�Z�b�g���̃|�C���^
		size_t countUse{};       // �g���Ă��鐔
	};
}

//-----------------------------------------
// ���f���N���X
//-----------------------------------------
class CModel final
{
	// �N���Xusing
	using Source = model::Source;
// ���J
public:
	CModel(const Source* pSource) : m_pSource(pSource), m_pModel{}, m_offSet{}, m_transform{} {}
	~CModel() = default;

	HRESULT Init(void);
	HRESULT Init(const D3DXVECTOR3 offSet, const D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f }, const D3DXVECTOR3 scale = { 1.0f,1.0f,1.0f }, const int nIdxParent = -1);
	void Uninit(void) {};
	void Draw(void);

	path GetPath(void) const { return m_pSource->Path; }

	const LPD3DXMESH GetMesh(void) const { return m_pSource->mesh.pMesh; }
	const LPD3DXBUFFER GetBuffMat(void) const { return m_pSource->mesh.pBuffMat; }
	const D3DXMATERIAL* GetMat(void) const { return m_pSource->mesh.pMat; }
	DWORD GetNumMat(void) const { return  m_pSource->mesh.dwNumMat; }
	DWORD GetNumSubSet(void) const { return m_pSource->mesh.dwNumSubSet; }
	size_t GetNumVertex(void) const { return m_pSource->mesh.dwNumVertex; }
	size_t GetSizeFVF(void) const { return m_pSource->mesh.sizeFVF; }
	LPBYTE GetVtxBuff(void) const { return m_pSource->mesh.pVtxBuff; }
	D3DXVECTOR3 GetVtxMin(void) const { return m_pSource->mesh.vtxMin; }
	D3DXVECTOR3 GetVtxMax(void) const { return m_pSource->mesh.vtxMax; }

	bool IsSubSetUse(Index idx) const { return (idx >= 0 && idx < m_pSource->mesh.dwNumMat) ? m_pSource->pSubSet[idx].bUse : false; }
	DWORD GetSubSetAttribId(Index idx) const { return (idx >= 0 && idx < m_pSource->mesh.dwNumMat) ? m_pSource->pSubSet[idx].AttribId : DWORD(-1); }
	Index GetSubSetTexIdx(Index idx) const { return (idx >= 0 && idx < m_pSource->mesh.dwNumMat) ? m_pSource->pSubSet[idx].texIdx : INVALID_ID; }

	D3DXVECTOR3 GetPos(void) const { return m_transform.pos; }
	D3DXVECTOR3 GetRot(void) const { return m_transform.rot; }
	D3DXVECTOR3 GetScale(void) const { return m_transform.scale; }
	D3DXMATRIX GetMatrix(void) const { return m_transform.mtxWorld; }
	const D3DXVECTOR3 GetOffSet(void) const { return m_offSet.Get(); }
	
	void SetPos(const D3DXVECTOR3 pos)
	{
		try { m_transform.pos = pos + m_offSet.Get(); }
		catch (const exception&) { m_transform.pos = pos; }
	}
	void SetRot(const D3DXVECTOR3 rot) { m_transform.rot = rot; }
	void SetScale(const D3DXVECTOR3 scale) { m_transform.scale = scale; }
	void SetOffSet(const D3DXVECTOR3 offSet) { m_offSet.Set(offSet); m_transform.pos = m_offSet.Get(); }
	void SetParent(const int nIdxParent);

// ����J
private:
	const Source* m_pSource;         // �\�[�X���̃|�C���^
	const CModel* m_pModel;          // �e���f���̃|�C���^
	WriteOnce<D3DXVECTOR3> m_offSet; // �I�t�Z�b�g
	DirectX::Transform m_transform;  // �g�����X�t�H�[�����
};

//----------------------------------------------
// ���f���Ǘ��N���X �V���O���g�� �R�s�[�֎~
//----------------------------------------------
class CModelManager final
{
	// �N���Xusing
	using Source = model::Source;
// ���J
public:
	static CModelManager& GetInstance(void) { return m_instance; }

	CModelManager(const CModelManager&) = delete;
	CModelManager& operator=(const CModelManager&) = delete;

	HRESULT Load(const vector<path> pathList);
	void Unload(void);

	HRESULT Register(const path Path);

	Index Create(Index tag);
	Index Create(Index tag, const D3DXVECTOR3 offSet, const D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f }, const D3DXVECTOR3 scale = { 1.0f,1.0f,1.0f }, const int nIdxParent = -1);
	void Release(void);

	void Draw(Index idx) const { if (idx >= 0 && idx < m_apModel.size()) m_apModel[idx]->Draw(); }

	const CModel* const GetModel(Index idx) const { return (idx >= 0 && idx < m_apModel.size()) ? m_apModel[idx] : nullptr; }
	path GetPath(Index idx) const { return (idx >= 0 && idx < m_apModel.size()) ? m_apModel[idx]->GetPath() : path(); }
	const LPD3DXMESH GetMesh(Index idx) const { return (idx >= 0 && idx < m_apModel.size()) ? m_apModel[idx]->GetMesh() : nullptr; }
	const LPD3DXBUFFER GetBuffMat(Index idx) const { return (idx >= 0 && idx < m_apModel.size()) ? m_apModel[idx]->GetBuffMat() : nullptr; }
	const D3DXMATERIAL* GetMat(Index idx) const { return (idx >= 0 && idx < m_apModel.size()) ? m_apModel[idx]->GetMat() : nullptr; }
	DWORD GetNumMat(Index idx) const { return (idx >= 0 && idx < m_apModel.size()) ? m_apModel[idx]->GetNumMat() : 0; }
	DWORD GetNumSubSet(Index idx) const { return (idx >= 0 && idx < m_apModel.size()) ? m_apModel[idx]->GetNumSubSet() : 0; }
	size_t GetNumVertex(Index idx) const { return (idx >= 0 && idx < m_apModel.size()) ? m_apModel[idx]->GetNumVertex() : 0; }
	size_t GetSizeFVF(Index idx) const { return (idx >= 0 && idx < m_apModel.size()) ? m_apModel[idx]->GetSizeFVF() : 0; }
	LPBYTE GetVtxBuff(Index idx) const { return (idx >= 0 && idx < m_apModel.size()) ? m_apModel[idx]->GetVtxBuff() : nullptr; }
	D3DXVECTOR3 GetVtxMin(Index idx) const { return (idx >= 0 && idx < m_apModel.size()) ? m_apModel[idx]->GetVtxMin() : D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	D3DXVECTOR3 GetVtxMax(Index idx) const { return (idx >= 0 && idx < m_apModel.size()) ? m_apModel[idx]->GetVtxMax() : D3DXVECTOR3(0.0f, 0.0f, 0.0f); }

	bool IsSubSetUse(Index idx, Index subsetIdx) const { return (idx >= 0 && idx < m_apModel.size()) ? m_apModel[idx]->IsSubSetUse(subsetIdx) : false; }
	DWORD GetSubSetAttribId(Index idx, Index subsetIdx) const { return (idx >= 0 && idx < m_apModel.size()) ? m_apModel[idx]->GetSubSetAttribId(subsetIdx) : DWORD(-1); }
	Index GetSubSetTexIdx(Index idx, Index subsetIdx) const { return (idx >= 0 && idx < m_apModel.size()) ? m_apModel[idx]->GetSubSetTexIdx(subsetIdx) : INVALID_ID; }
	D3DXVECTOR3 GetOffSet(Index idx) const { return (idx >= 0 && idx < m_apModel.size()) ? m_apModel[idx]->GetOffSet() : D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	D3DXMATRIX GetMatrix(Index idx) const { return (idx >= 0 && idx < m_apModel.size()) ? m_apModel[idx]->GetMatrix() : D3DXMATRIX(); }

	D3DXVECTOR3 GetPos(Index idx) const { return (idx >= 0 && idx < m_apModel.size()) ? m_apModel[idx]->GetPos() : D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	D3DXVECTOR3 GetRot(Index idx) const { return (idx >= 0 && idx < m_apModel.size()) ? m_apModel[idx]->GetRot() : D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	D3DXVECTOR3 GetScale(Index idx) const { return (idx >= 0 && idx < m_apModel.size()) ? m_apModel[idx]->GetScale() : D3DXVECTOR3(1.0f, 1.0f, 1.0f); }
	void SetPos(Index idx, const D3DXVECTOR3 pos) { if (idx >= 0 && idx < m_apModel.size())  m_apModel[idx]->SetPos(pos); }
	void SetRot(Index idx, const D3DXVECTOR3 rot) { if (idx >= 0 && idx < m_apModel.size())  m_apModel[idx]->SetRot(rot); }
	void SetScale(Index idx, const D3DXVECTOR3 scale) { if (idx >= 0 && idx < m_apModel.size())  m_apModel[idx]->SetScale(scale); }
	void SetOffSet(Index idx, const D3DXVECTOR3 offSet) { if (idx >= 0 && idx < m_apModel.size())  m_apModel[idx]->SetOffSet(offSet); }

	void SetParent(Index idx, const int nIdxParent) { if (idx >= 0 && idx < m_apModel.size())  m_apModel[idx]->SetParent(nIdxParent); }

	size_t GetUseCount(Index tag) const { return (tag >= 0 && tag < m_aModelSource.size()) ? m_aModelSource[tag].countUse : INVALID_ID; }

	// ����J
private:
	static CModelManager m_instance;  // �����̃C���X�^���X

	CModelManager() : m_apModel{}, m_aModelSource{} {}
	~CModelManager() = default;

	HRESULT LoadX(LPDIRECT3DDEVICE9 pDevice, Source* pSource);

	vector<Source> m_aModelSource; // ���f���\�[�X���
	vector<CModel*> m_apModel;     // ���f���̃|�C���^�z��
};