//------------------------------------------
//
// �v���C���[�̏��� [player.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "bg.h"
#include "renderer.h"
#include "manager.h"
#include "object2D.h"
#include "texture.h"

using namespace DirectX2D; // DirectX2D��Ԃ̎g�p

//---------------------------------------
//
// �v���C���[�N���X
//
//---------------------------------------

// �ÓI�����o�ϐ��̒�`

// �e�N�X�`���̃p�X
const string_view CBg::m_sTexturePass[CBg::MAX_TEXTURE] =
{
	"data\\TEXTURE\\bg100.png",
	"data\\TEXTURE\\bg101.png",
	"data\\TEXTURE\\bg102.png"
};
const float CBg::m_afScrollSpeed[CBg::MAX_TEXTURE] = { 0.01f, 0.05f, 0.1f }; // �X�N���[�����x

CObject2D* CBg::m_pBg[CBg::MAX_TEXTURE] = { nullptr };      // �w�i�̃|�C���^�z��
size_t* CBg::textureIdx{};                                  // �e�N�X�`��ID

//------------------------------
// �\�[�X�ǂݍ���
//------------------------------
HRESULT CBg::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();

	textureIdx = new size_t[MAX_TEXTURE];

	// �e�N�X�`��
	for (size_t cntTex = 0; cntTex < MAX_TEXTURE; cntTex++)
	{
		if (m_sTexturePass[cntTex].empty())
		{// �e�N�X�`���p�X����Ȃ�I��
			break;
		}

		textureIdx[cntTex] = CTextureManager::GetInstance().Register(m_sTexturePass[cntTex].data());
	}

	return S_OK;
}

//------------------------------
// ����
//------------------------------
CBg* CBg::Create(int priority)
{
	CBg* pBg = new CBg(priority);

	if (FAILED(pBg->Init()))
	{
		delete pBg;
		pBg = nullptr;
		return nullptr;
	}
	return pBg;
}

//------------------------------
//����������
//------------------------------
HRESULT CBg::Init(void)
{
	CRenderer renderer = CManager::GetRenderer();

	CObject::Init(TYPE::BackGround);

	// �X�N���[���T�C�Y�̎擾
	D3DXVECTOR2 screenSize = {};
	if (FAILED(renderer.GetViewportSize(&screenSize)))
	{
		return E_FAIL;
	}

	// �w�i�T�C�Y�̎擾
	for (size_t cntTex = 0; cntTex < MAX_TEXTURE; cntTex++)
	{
		m_pBg[cntTex] = CObject2D::Create(CTextureManager::GetInstance().GetTexture(textureIdx[cntTex]), D3DXVECTOR3(screenSize.x * 0.5f, screenSize.y * 0.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), TYPE::BackGround, GetPriority());

		D3DXVECTOR2 screenSizeRatio = D3DXVECTOR2(screenSize.x / CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).x, screenSize.y / CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).y);
		if (screenSizeRatio.x > screenSizeRatio.y)
		{
			SetSize(D3DXVECTOR3(CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).x * screenSizeRatio.x, CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).y * screenSizeRatio.x, 0.0f)); // �T�C�Y��ݒ�
		}
		else
		{
			SetSize(D3DXVECTOR3(CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).x * screenSizeRatio.y, CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).y * screenSizeRatio.y, 0.0f)); // �T�C�Y��ݒ�
		}

		// ���_���̐ݒ�
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pBg[cntTex]->GetVtxBuff();
		Vertex* pVtx;
		if (FAILED(pVtxBuff->Lock(0, 0, (void**)&pVtx, 0))) { return E_FAIL; }

		D3DXVECTOR3 size = GetSize(); // �T�C�Y���擾
		for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
		{
			pVtx[cntVtx].pos = D3DXVECTOR3(size.x * (float)(cntVtx % 2) + screenSize.x * 0.5f - size.x * 0.5f, size.y * (float)(cntVtx / 2) + screenSize.y * 0.5f - size.y * 0.5f, 0.0f);
			pVtx[cntVtx].rhw = 1.0f;
			pVtx[cntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[cntVtx].tex = D3DXVECTOR2((float)(cntVtx % 2) + m_TextureU[cntTex], (float)(cntVtx / 2));
		}

		if (FAILED(pVtxBuff->Unlock())) { return E_FAIL; }
	}

	return S_OK;
}

//------------------------------
//�I������
//------------------------------
void CBg::Uninit(void)
{

}

//------------------------------
//�X�V����
//------------------------------
void CBg::Update(void)
{
	const CRenderer renderer = CManager::GetRenderer();

	// �X�N���[���T�C�Y�̎擾
	D3DXVECTOR2 screenSize = {};
	if (FAILED(renderer.GetViewportSize(&screenSize)))
	{
		return;
	}

	// �w�i�T�C�Y�̎擾
	for (size_t cntTex = 0; cntTex < MAX_TEXTURE; cntTex++)
	{
		if (m_pBg[cntTex] == nullptr)
		{
			break;
		}

		if (CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).x == 0.0f || CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).y == 0.0f)
		{
			continue;
		}

		// �X�N���[������
		m_TextureU[cntTex] += m_afScrollSpeed[cntTex] * CMain::GetDeltaTimeGameSpeed();

		D3DXVECTOR2 screenSizeRatio = D3DXVECTOR2(screenSize.x / CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).x, screenSize.y / CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).y);
		if (screenSizeRatio.x > screenSizeRatio.y)
		{
			SetSize(D3DXVECTOR3(CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).x * screenSizeRatio.x, CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).y * screenSizeRatio.x, 0.0f)); // �T�C�Y��ݒ�
		}
		else
		{
			SetSize(D3DXVECTOR3(CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).x * screenSizeRatio.y, CTextureManager::GetInstance().GetSize(textureIdx[cntTex]).y * screenSizeRatio.y, 0.0f)); // �T�C�Y��ݒ�
		}

		// ���_���̐ݒ�
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pBg[cntTex]->GetVtxBuff();
		Vertex* pVtx;
		if (FAILED(pVtxBuff->Lock(0, 0, (void**)&pVtx, 0))) { return; }

		D3DXVECTOR3 size = GetSize(); // �T�C�Y���擾
		for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
		{
			pVtx[cntVtx].pos = D3DXVECTOR3(size.x * (float)(cntVtx % 2) + screenSize.x * 0.5f - size.x * 0.5f, size.y * (float)(cntVtx / 2) + screenSize.y * 0.5f - size.y * 0.5f, 0.0f);
			pVtx[cntVtx].rhw = 1.0f;
			pVtx[cntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[cntVtx].tex = D3DXVECTOR2((float)(cntVtx % 2) + m_TextureU[cntTex], (float)(cntVtx / 2));
		}

		if (FAILED(pVtxBuff->Unlock())) { return; }
	}
}

//------------------------------
//�`�揈��
//------------------------------
void CBg::Draw(void)
{

}