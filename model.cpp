//----------------------------------------------------
//
// ���f������ [model.cpp]
// Author: fuma sato
//
//----------------------------------------------------
#include "model.h"
#include "manager.h"
#include "texture.h"

using namespace model; // ���f����Ԃ̎g�p

//----------------------------------------------------
//
// ���f���N���X
//
//----------------------------------------------------

//----------------------------------------------------
// ����������
//----------------------------------------------------
HRESULT CModel::Init(void)
{
	m_transform.pos = { 0.0f,0.0f,0.0f };    // �����ʒu�̓I�t�Z�b�g
	m_transform.rot = { 0.0f,0.0f,0.0f };    // ��]
	m_transform.scale = { 1.0f,1.0f,1.0f };  // �傫��
	return S_OK;
}

//----------------------------------------------------
// ����������
//----------------------------------------------------
HRESULT CModel::Init(const D3DXVECTOR3 offSet, const D3DXVECTOR3 rot, const D3DXVECTOR3 scale, const int nIdxParent)
{
	m_offSet.Set(offSet);                // �I�t�Z�b�g��ݒ�
	m_transform.pos = m_offSet.Get();    // �����ʒu�̓I�t�Z�b�g
	m_transform.rot = rot;               // ��]
	m_transform.scale = scale;           // �傫��
	SetParent(nIdxParent);               // �e�̐ݒ�
	return S_OK;
}

//----------------------------------------------------
// ���f���`��
//----------------------------------------------------
void CModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice(); // �����_���[����f�o�C�X���擾

	if (pDevice == nullptr) return;

	D3DXMATRIX mtxRot{}, mtxTrans{}, mtxScale{}; // �v�Z�p�}�g���b�N�X

	// �p�[�c�̃��[�J���}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_transform.mtxWorld);

	// �p�[�c�̑傫���𔽉f
	D3DXMatrixScaling(&mtxScale, m_transform.scale.x, m_transform.scale.y, m_transform.scale.z);
	D3DXMatrixMultiply(&m_transform.mtxWorld, &m_transform.mtxWorld, &mtxScale);

	// �p�[�c�̌����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_transform.rot.y, m_transform.rot.x, m_transform.rot.z);
	D3DXMatrixMultiply(&m_transform.mtxWorld, &m_transform.mtxWorld, &mtxRot);

	// �p�[�c�̈ʒu(�I�t�Z�b�g)�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_transform.pos.x, m_transform.pos.y, m_transform.pos.z);
	D3DXMatrixMultiply(&m_transform.mtxWorld, &m_transform.mtxWorld, &mtxTrans);

	// �e�̃}�g���b�N�X���|���ă��[���h���W�ɕϊ�
	D3DXMATRIX mtxParent;
	if (m_pModel != nullptr)
	{// �e���f��������
		mtxParent = m_pModel->GetMatrix();
	}
	else
	{// �e���f�������Ȃ� (Player���e)
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	D3DXMatrixMultiply(&m_transform.mtxWorld, &m_transform.mtxWorld, &mtxParent);

	// �}�g���b�N�X�̓K��
	pDevice->SetTransform(D3DTS_WORLD, &m_transform.mtxWorld);

	// �}�e���A�����擾
	D3DMATERIAL9 matDef{};
	pDevice->GetMaterial(&matDef);

	// �T�u�Z�b�g��
	for (DWORD cntSubSet = 0; cntSubSet < m_pSource->mesh.dwNumSubSet; cntSubSet++)
	{// �T�u�Z�b�g�J��Ԃ�
		if (!m_pSource->pSubSet[cntSubSet].bUse) continue; // �T�u�Z�b�g���g���Ă��Ȃ��Ȃ�X�L�b�v

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&m_pSource->mesh.pMat[m_pSource->pSubSet[cntSubSet].AttribId].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CTextureManager::GetInstance().GetTexture(m_pSource->pSubSet[cntSubSet].texIdx));

		// �T�u�Z�b�g�̕`��
		m_pSource->mesh.pMesh->DrawSubset(m_pSource->pSubSet[cntSubSet].AttribId);
	}

	// �}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);
}

void CModel::SetParent(const int nIdxParent) { m_pModel = CModelManager::GetInstance().GetModel(nIdxParent); }

//----------------------------------------------------
//
// ���f���Ǘ��N���X
//
//----------------------------------------------------

// �ÓI�����o�ϐ�
CModelManager CModelManager::m_instance{};     // �����̃C���X�^���X

//----------------------------------------------------
// ����������
//----------------------------------------------------
HRESULT CModelManager::Load(const vector<path> pathList)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice(); // �����_���[����f�o�C�X���擾

	if (pDevice == nullptr) return E_FAIL;

	// vector�̃T�C�Y�����O�Ɋm��
	if (m_aModelSource.size() < pathList.size())
	{
		m_aModelSource.resize(pathList.size());
	}

	// �K��e�N�X�`���̐���
	for (size_t cntModel = 0; cntModel < m_aModelSource.size(); cntModel++)
	{// �K��^�C�v��
		m_aModelSource[cntModel].Path = pathList[cntModel]; // �p�X���Z�b�g
		// X�t�@�C���̓ǂݍ���
		if (FAILED(LoadX(pDevice, &m_aModelSource[cntModel])))
		{
			return E_FAIL; // �G���[�l
		}
	}

	return S_OK;
}

//----------------------------------------------------
// �I������
//----------------------------------------------------
void CModelManager::Unload(void)
{
	for (auto& modelSource : m_aModelSource)
	{// �S�Ẵ��f���\�[�X
		SAFE_DELETE_ARRAYPTR(modelSource.pSubSet);  // �T�u�Z�b�g���

		SAFE_RELEASE(modelSource.mesh.pBuffMat);    // �}�e���A��

		SAFE_RELEASE(modelSource.mesh.pMesh);       // ���_
	}
}

//----------------------------------------------------
// �o�^
//----------------------------------------------------
HRESULT CModelManager::Register(const path Path)
{
	// ���ꃂ�f���m�F
	for (size_t cntModel = 0; cntModel < m_aModelSource.size(); cntModel++)
	{// ���܂ł̃��f��
		if (m_aModelSource[cntModel].Path == Path)
		{// �������f���𔭌�
			return E_FAIL;             // �G���[�l
		}
	}

	// �V���f���̐���
	m_aModelSource.push_back(model::Source{}); // �V�������f������ǉ�
	m_aModelSource.back().Path = Path; // �p�X���Z�b�g
	if (FAILED(LoadX(CManager::GetRenderer().GetDevice(), &m_aModelSource.back())))
	{// X�t�@�C���̓ǂݍ��ݎ��s
		m_aModelSource.pop_back(); // �ǉ��������f�������폜
		return E_FAIL;             // �G���[�l
	}

	return S_OK; // �����l
}

//----------------------------------------------------
// ���f�����g��
//----------------------------------------------------
Index CModelManager::Create(Index tag)
{
	if (tag < 0 || tag >= m_aModelSource.size())
	{// �^�O���͈͊O
		return INVALID_ID; // �G���[�l
	}

	// ���f���̐���
	CModel* pModel = nullptr;
	pModel = new CModel(&m_aModelSource[tag]);
	if (pModel == nullptr)
	{// �������s
		return INVALID_ID; // �G���[�l
	}

	if (FAILED(pModel->Init()))
	{// ���������s
		pModel->Uninit();
		delete pModel;
		pModel = nullptr;
		return INVALID_ID;
	}

	m_apModel.push_back(pModel); // ���f����ǉ�
	m_aModelSource[tag].countUse++; // �g���Ă��鐔��ǉ�

	return m_apModel.size() - 1;
}

//----------------------------------------------------
// ���f�����g��
//----------------------------------------------------
Index CModelManager::Create(Index tag, const D3DXVECTOR3 offSet, const D3DXVECTOR3 rot, const D3DXVECTOR3 scale, const int nIdxParent)
{
	if (tag < 0 || tag >= m_aModelSource.size())
	{// �^�O���͈͊O
		return INVALID_ID; // �G���[�l
	}

	// ���f���̐���
	CModel* pModel = nullptr;
	pModel = new CModel(&m_aModelSource[tag]);
	if (pModel == nullptr)
	{// �������s
		return INVALID_ID; // �G���[�l
	}

	if (FAILED(pModel->Init(offSet, rot, scale, nIdxParent)))
	{// ���������s
		pModel->Uninit();
		delete pModel;
		pModel = nullptr;
		return INVALID_ID;
	}

	m_apModel.push_back(pModel); // ���f����ǉ�
	m_aModelSource[tag].countUse++; // �g���Ă��鐔��ǉ�

	return m_apModel.size() - 1;
}

//----------------------------------------------------
// �I������
//----------------------------------------------------
void CModelManager::Release(void)
{
	SAFE_UNINIT_ARRAY(m_apModel); // �S�Ẵ��f�����A���C�j�b�g
}

//----------------------------------------------------
// X�t�@�C���̓ǂݍ���
//----------------------------------------------------
HRESULT CModelManager::LoadX(LPDIRECT3DDEVICE9 pDevice, model::Source* pSource)
{
	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(
		pSource->Path.string().c_str(),
		D3DXMESH_MANAGED,
		pDevice,
		NULL,
		&pSource->mesh.pBuffMat,
		NULL,
		&pSource->mesh.dwNumMat,
		&pSource->mesh.pMesh)))
	{
		return E_FAIL;
	}

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pSource->mesh.pMat = static_cast<D3DXMATERIAL*>(pSource->mesh.pBuffMat->GetBufferPointer());

	// AttributeTable���擾
	pSource->mesh.pMesh->GetAttributeTable(nullptr, &pSource->mesh.dwNumSubSet);
	D3DXATTRIBUTERANGE* pAttributeTable = new D3DXATTRIBUTERANGE[pSource->mesh.dwNumSubSet];
	pSource->mesh.pMesh->GetAttributeTable(pAttributeTable, &pSource->mesh.dwNumSubSet);

	pSource->pSubSet = new model::SubSetSource[pSource->mesh.dwNumSubSet]; // �T�u�Z�b�g���𐶐�

	for (DWORD cntSubset = 0; cntSubset < pSource->mesh.dwNumSubSet; cntSubset++)
	{//�}�e���A�����J��Ԃ�
		pSource->pSubSet[cntSubset].AttribId = pAttributeTable[cntSubset].AttribId; // ID���R�s�[

		if (pAttributeTable[cntSubset].FaceCount > 0)
		{// �T�u�Z�b�g���g�p����Ă��邩
			pSource->pSubSet[cntSubset].bUse = true; // �����Ă���

			if (pSource->mesh.pMat[pSource->pSubSet[cntSubset].AttribId].pTextureFilename != nullptr)
			{//�e�N�X�`��������
				pSource->pSubSet[cntSubset].texIdx = CTextureManager::GetInstance().Register(static_cast<path>(pSource->mesh.pMat[pSource->pSubSet[cntSubset].AttribId].pTextureFilename)); // �e�N�X�`��ID
			}
			else
			{//�e�N�X�`�����Ȃ�
				pSource->pSubSet[cntSubset].texIdx = INVALID_ID; // �e�N�X�`�����Ȃ�
			}
		}
		else
		{// �T�u�Z�b�g���g�p����Ă��Ȃ�
			pSource->pSubSet[cntSubset].bUse = false;        // �����Ă��Ȃ�
			pSource->pSubSet[cntSubset].texIdx = INVALID_ID; // �e�N�X�`�����Ȃ�
		}
	}
	delete[] pAttributeTable; // ���������

	//���_��
	pSource->mesh.dwNumVertex = pSource->mesh.pMesh->GetNumVertices();
	//���_�T�C�Y
	pSource->mesh.sizeFVF = D3DXGetFVFVertexSize(pSource->mesh.pMesh->GetFVF());
	//���_�o�b�t�@�̎擾
	pSource->mesh.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pSource->mesh.pVtxBuff);

	for (DWORD cntVtx = 0; cntVtx < pSource->mesh.dwNumVertex; cntVtx++)
	{//���_���J��Ԃ�
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pSource->mesh.pVtxBuff;

		//���_�̑傫���̔��f
		if (vtx.x > pSource->mesh.vtxMax.x)
		{
			pSource->mesh.vtxMax.x = vtx.x;
		}
		if (vtx.x < pSource->mesh.vtxMin.x)
		{
			pSource->mesh.vtxMin.x = vtx.x;
		}
		if (vtx.y > pSource->mesh.vtxMax.y)
		{
			pSource->mesh.vtxMax.y = vtx.y;
		}
		if (vtx.y < pSource->mesh.vtxMin.y)
		{
			pSource->mesh.vtxMin.y = vtx.y;
		}
		if (vtx.z > pSource->mesh.vtxMax.z)
		{
			pSource->mesh.vtxMax.z = vtx.z;
		}
		if (vtx.z < pSource->mesh.vtxMin.z)
		{
			pSource->mesh.vtxMin.z = vtx.z;
		}

		pSource->mesh.pVtxBuff += pSource->mesh.sizeFVF;
	}

	// ���_�o�b�t�@�̃A�����b�N
	pSource->mesh.pMesh->UnlockVertexBuffer();

	return S_OK; // �����l
}