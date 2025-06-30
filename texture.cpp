//----------------------------------------------------
//
// �e�N�X�`������ [texture.cpp]
// Author: fuma sato
//
//----------------------------------------------------
#include "texture.h"
#include "manager.h"

//----------------------------------------------------
//
// �e�N�X�`���N���X
//
//----------------------------------------------------

//----------------------------------------------------
// ����������
//----------------------------------------------------
HRESULT CTexture::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();

	// �e�N�X�`��
	D3DXIMAGE_INFO imageInfo = {};
	if (FAILED(D3DXGetImageInfoFromFile
	(
		m_path.string().c_str(),
		&imageInfo
	)))
	{
		return E_FAIL;
	}
	m_size = D3DXVECTOR2((float)imageInfo.Width, (float)imageInfo.Height); // �T�C�Y���擾
	m_blockSize = D3DXVECTOR2(m_size.x / m_block.x, m_size.y / m_block.y); // 1�u���b�N���̃T�C�Y
	m_countUse = 0;                                                        // �����g���Ă��

	if (FAILED(D3DXCreateTextureFromFile
	(
		pDevice,
		m_path.string().c_str(),
		&m_pTexture
	)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//----------------------------------------------------
// �I������
//----------------------------------------------------
void CTexture::Uninit(void)
{
	if (m_pTexture != nullptr)
	{// �����
		m_pTexture->Release(); // �J��
		m_pTexture = nullptr;  // null
	}
}

//----------------------------------------------------
//
// �e�N�X�`���Ǘ��N���X
//
//----------------------------------------------------

// �ÓI�����o�ϐ�
CTextureManager CTextureManager::m_instance{};     // �����̃C���X�^���X

//----------------------------------------------------
// ����������
//----------------------------------------------------
HRESULT CTextureManager::Load(vector<path> texturePassList, vector<D3DXVECTOR2> textureBlockList)
{
	// vector�̃T�C�Y�����O�Ɋm��
	if (m_apTexture.size() < texturePassList.size())
	{
		m_apTexture.resize(texturePassList.size());
	}

	// �K��e�N�X�`���̐���
	for (size_t cntTex = 0; cntTex < m_apTexture.size(); cntTex++)
	{// �K��^�C�v��
		if (m_apTexture[cntTex] == nullptr)
		{// �Ȃ����
			m_apTexture[cntTex] = new CTexture(texturePassList[cntTex], textureBlockList[cntTex]); // ����

			if (m_apTexture[cntTex] != nullptr)
			{// �����
				if (FAILED(m_apTexture[cntTex]->Init()))
				{
					m_apTexture[cntTex]->Uninit();
					delete m_apTexture[cntTex];
					m_apTexture[cntTex] = nullptr;
					return E_FAIL;
				}
				m_apTexture[cntTex]->AddUseCount();                     // �g���Ă��鐔�̒ǉ�
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
void CTextureManager::Unload(void)
{
	for (auto& pTeture : m_apTexture)
	{// �S�Ẵe�N�X�`��
		if (pTeture != nullptr)
		{// �����
			pTeture->Uninit(); // �I��
			delete pTeture;    // �J��
			pTeture = nullptr; // null
		}
	}
}

//----------------------------------------------------
// �X�V
//----------------------------------------------------
size_t CTextureManager::Register(const path sTexturePath, const D3DXVECTOR2 block)
{
	if (sTexturePath.empty())
	{
		return INVALID_ID; // �G���[�l
	}

	// ����e�N�X�`���m�F
	for (size_t cntTexture = 0; cntTexture < m_apTexture.size(); cntTexture++)
	{// ���܂ł̃e�N�X�`��
		if (m_apTexture[cntTexture]->GetPath() == sTexturePath)
		{// �����e�N�X�`���𔭌�
			m_apTexture[cntTexture]->AddUseCount(); // �g���Ă��鐔�̒ǉ�
			return cntTexture;                      // ���̃e�N�X�`�������Ă�
		}
	}

	// �V�e�N�X�`���̐���
	CTexture* pTexture = new CTexture(sTexturePath, block);
	if (FAILED(pTexture->Init()))
	{
		pTexture->Uninit();
		delete pTexture;
		pTexture = nullptr;
		return INVALID_ID;
	}
	else
	{
		pTexture->AddUseCount(); // �g���Ă��鐔�̒ǉ�
		m_apTexture.push_back(pTexture); // �ǉ�
		return m_apTexture.size() - 1;
	}
}