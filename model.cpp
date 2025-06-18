//----------------------------------------------------
//
// ���f������ [model.cpp]
// Author: fuma sato
//
//----------------------------------------------------
#include "model.h"
#include "manager.h"
#include "texture.h"

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
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice(); // �f�o�C�X

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(
		m_sPass.data(),
		D3DXMESH_MANAGED,
		pDevice,
		NULL,
		&m_info.pBuffMat,
		NULL,
		&m_info.dwNumMat,
		&m_info.pMesh)))
	{
		return E_FAIL;
	}

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	m_info.pMat = static_cast<D3DXMATERIAL*>(m_info.pBuffMat->GetBufferPointer());

	// AttributeTable���擾
	m_info.pMesh->GetAttributeTable(nullptr, &m_info.dwNumSubSet);
	D3DXATTRIBUTERANGE* pAttributeTable = new D3DXATTRIBUTERANGE[m_info.dwNumSubSet];
	m_info.pMesh->GetAttributeTable(pAttributeTable, &m_info.dwNumSubSet);

	m_info.pSubSet = new SubSetInfo[m_info.dwNumSubSet]; // �T�u�Z�b�g���𐶐�

	for (DWORD cntSubset = 0; cntSubset < m_info.dwNumSubSet; cntSubset++)
	{//�}�e���A�����J��Ԃ�
		m_info.pSubSet[cntSubset].AttribId = pAttributeTable[cntSubset].AttribId; // ID���R�s�[

		if (pAttributeTable[cntSubset].FaceCount > 0)
		{// �T�u�Z�b�g���g�p����Ă��邩
			m_info.pSubSet[cntSubset].bUse = true; // �����Ă���

			if (m_info.pMat[m_info.pSubSet[cntSubset].AttribId].pTextureFilename != nullptr)
			{//�e�N�X�`��������
				m_info.pSubSet[cntSubset].texIdx = CTextureManager::GetInstance().Register(m_info.pMat[m_info.pSubSet[cntSubset].AttribId].pTextureFilename); // �e�N�X�`��ID
			}
			else
			{//�e�N�X�`�����Ȃ�
				m_info.pSubSet[cntSubset].texIdx = INVALID_ID; // �e�N�X�`�����Ȃ�
			}
		}
		else
		{// �T�u�Z�b�g���g�p����Ă��Ȃ�
			m_info.pSubSet[cntSubset].bUse = false;        // �����Ă��Ȃ�
			m_info.pSubSet[cntSubset].texIdx = INVALID_ID; // �e�N�X�`�����Ȃ�
		}
	}
	delete[] pAttributeTable; // ���������

	m_info.pos = m_info.offSet;        // �I�t�Z�b�g��K������
	m_info.rot = { 0.0f,0.0f,0.0f };   // ������
	m_info.scale = { 1.0f,1.0f,1.0f }; // ������

	m_pModel = CModelManager::GetInstance().GetModel(m_info.nIdxParent);

	return S_OK;
}

//----------------------------------------------------
// �I������
//----------------------------------------------------
void CModel::Uninit(void)
{
	// �T�u�Z�b�g���
	if (m_info.pSubSet != nullptr)
	{// �����
		delete[] m_info.pSubSet;  // �J��
		m_info.pSubSet = nullptr; // null
	}

	// �}�e���A��
	if (m_info.pBuffMat != nullptr)
	{// �����
		m_info.pBuffMat->Release(); // �J��
		m_info.pBuffMat = nullptr;  // null
	}

	// ���_
	if (m_info.pMesh != nullptr)
	{// �����
		m_info.pMesh->Release(); // �J��
		m_info.pMesh = nullptr;  // null
	}
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
	D3DXMatrixIdentity(&m_info.mtxWorld);

	// �p�[�c�̑傫���𔽉f
	D3DXMatrixScaling(&mtxScale, m_info.scale.x, m_info.scale.y, m_info.scale.z);
	D3DXMatrixMultiply(&m_info.mtxWorld, &m_info.mtxWorld, &mtxScale);

	// �p�[�c�̌����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_info.rot.y, m_info.rot.x, m_info.rot.z);
	D3DXMatrixMultiply(&m_info.mtxWorld, &m_info.mtxWorld, &mtxRot);

	// �p�[�c�̈ʒu(�I�t�Z�b�g)�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_info.pos.x, m_info.pos.y, m_info.pos.z);
	D3DXMatrixMultiply(&m_info.mtxWorld, &m_info.mtxWorld, &mtxTrans);

	// �e�̃}�g���b�N�X���|���ă��[���h���W�ɕϊ�
	D3DXMATRIX mtxParent;
	if (m_pModel != nullptr)
	{// �e���f��������
		mtxParent = m_pModel->GetInfo().mtxWorld;
	}
	else
	{// �e���f�������Ȃ� (Player���e)
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	D3DXMatrixMultiply(&m_info.mtxWorld, &m_info.mtxWorld, &mtxParent);

	// �}�g���b�N�X�̓K��
	pDevice->SetTransform(D3DTS_WORLD, &m_info.mtxWorld);

	// �}�e���A�����擾
	D3DMATERIAL9 matDef{};
	pDevice->GetMaterial(&matDef);

	// �T�u�Z�b�g��
	for (DWORD cntSubSet = 0; cntSubSet < m_info.dwNumSubSet; cntSubSet++)
	{// �T�u�Z�b�g�J��Ԃ�
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&m_info.pMat[m_info.pSubSet[cntSubSet].AttribId].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CTextureManager::GetInstance().GetTexture(m_info.pSubSet[cntSubSet].texIdx));

		// �T�u�Z�b�g�̕`��
		m_info.pMesh->DrawSubset(m_info.pSubSet[cntSubSet].AttribId);
	}

	// �}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//----------------------------------------------------
// �e�̐ݒ�
//----------------------------------------------------
void CModel::SetParent(const int nIdxParent)
{
	m_info.nIdxParent = nIdxParent;
	m_pModel = CModelManager::GetInstance().GetModel(m_info.nIdxParent);
}

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
HRESULT CModelManager::Load(void)
{
	// �K��e�N�X�`���̐���
	for (size_t cntType = 0; cntType < TYPE_MAX; cntType++)
	{// �K��^�C�v��
		if (m_apModel[cntType] == nullptr)
		{// �Ȃ����
			m_apModel[cntType] = new CModel(m_typePass[cntType], m_typeOffSet[cntType], m_typeParent[cntType]); // ����

			if (m_apModel[cntType] != nullptr)
			{// �����
				if (FAILED(m_apModel[cntType]->Init()))
				{
					m_apModel[cntType]->Uninit();
					delete m_apModel[cntType];
					m_apModel[cntType] = nullptr;
					return E_FAIL;
				}
				m_apModel[cntType]->AddUseCount();                     // �g���Ă��鐔�̒ǉ�
				m_nNumAll++; // ����
			}
			else
			{
				return E_FAIL; // �G���[�l
			}
		}
	}

	return S_OK;
}

//----------------------------------------------------
// �I������
//----------------------------------------------------
void CModelManager::Unload(void)
{
	for (size_t cntModel = 0; cntModel < m_nNumAll; cntModel++)
	{// �S�Ẵe�N�X�`��
		if (m_apModel[cntModel] != nullptr)
		{// �����
			m_apModel[cntModel]->Uninit(); // �I��
			delete m_apModel[cntModel];    // �J��
			m_apModel[cntModel] = nullptr; // null
		}
	}
}

//----------------------------------------------------
// �X�V
//----------------------------------------------------
Index CModelManager::Register(const string_view sModelPass, const D3DXVECTOR3 offSet, const int nIdxParent)
{
	// ���ꃂ�f���m�F
	for (size_t cntModel = 0; cntModel < m_nNumAll; cntModel++)
	{// ���܂ł̃��f��
		if (m_apModel[cntModel]->GetPass() == sModelPass)
		{// �������f���𔭌�
			m_apModel[cntModel]->AddUseCount(); // �g���Ă��鐔�̒ǉ�
			return cntModel;                      // ���̃��f�������Ă�
		}
	}

	// �V�e�N�X�`���̐���
	if (m_apModel[m_nNumAll] == nullptr)
	{// �Ȃ����
		m_apModel[m_nNumAll] = new CModel(sModelPass, offSet, nIdxParent); // ����

		if (m_apModel[m_nNumAll] != nullptr)
		{// �����
			if (FAILED(m_apModel[m_nNumAll]->Init()))
			{
				m_apModel[m_nNumAll]->Uninit();
				delete m_apModel[m_nNumAll];
				m_apModel[m_nNumAll] = nullptr;
				return INVALID_ID;
			}
			m_apModel[m_nNumAll]->AddUseCount();                         // �g���Ă��鐔�̒ǉ�
			return m_nNumAll++; // �����O�̒l��Ԃ����̌㑫��
		}
		else
		{
			return INVALID_ID; // �G���[�l
		}
	}
	return INVALID_ID; // �G���[�l
}