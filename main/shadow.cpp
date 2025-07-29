//------------------------------------------
//
// �e[shadow.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "shadow.h"
#include "renderer.h"

using namespace DirectX3D; // DirectX���O��Ԃ̎g�p

//-----------------------------
// 
// �e�N���X
// 
//-----------------------------

LPDIRECT3DVERTEXBUFFER9 CShadow::m_pVtxBuff{}; // �ÓI�����o�ϐ��̒�`

//------------------------------
// ��������
//------------------------------
CShadow* CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, Index priority)
{
	CShadow* pShadow = new CShadow(priority); // �C���X�^���X����

	if (pShadow == nullptr)
	{// �������s
		return nullptr;
	}

	// ������
	if (FAILED(pShadow->Init(pos, rot, scale)))
	{
		SAFE_DELETE(pShadow);
		return nullptr;
	}

	return pShadow;
}

//------------------------------
// ��������
//------------------------------
CShadow* CShadow::Create(DirectX::Transform transform, Index priority)
{
	CShadow* pShadow = new CShadow(priority); // �C���X�^���X����

	if (pShadow == nullptr)
	{// �������s
		return nullptr;
	}

	// ������
	if (FAILED(pShadow->Init(transform)))
	{
		SAFE_DELETE(pShadow);
		return nullptr;
	}

	return pShadow;
}

//------------------------------
// ����������
//------------------------------
HRESULT CShadow::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	if (m_pVtxBuff == nullptr)
	{
		CManager::GetRenderer().CreateViewportVertexBuffer(&m_pVtxBuff, Black(0.1f)); // �r���[�|�[�g�̒��_�o�b�t�@�𐶐�
	}
	CObjectX::Init(pos, rot, scale,MODELTAG::Shadow, TYPE::Shadow);
	return S_OK;
}

//------------------------------
// ����������
//------------------------------
HRESULT CShadow::Init(DirectX::Transform transform)
{
	if (m_pVtxBuff == nullptr)
	{
		CManager::GetRenderer().CreateViewportVertexBuffer(&m_pVtxBuff, Black(0.1f)); // �r���[�|�[�g�̒��_�o�b�t�@�𐶐�
	}
	CObjectX::Init(transform, MODELTAG::Shadow, TYPE::Shadow);
	return S_OK;
}

//------------------------------
// �`�揈��
//------------------------------
void CShadow::Draw(void)
{
	if (m_pVtxBuff == nullptr)
	{
		return; // ���_�o�b�t�@���Ȃ��ꍇ�͕`�悵�Ȃ�
	}

	// �����_���[����f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();
	D3DCAPS9 caps{};
	if (SUCCEEDED(pDevice->GetDeviceCaps(&caps)))
	{
		if (!(caps.StencilCaps & (D3DSTENCILCAPS_INCR | D3DSTENCILCAPS_DECR)))
		{
			return; // �X�e���V�����삪�T�|�[�g����Ă��Ȃ��ꍇ�͕`�悵�Ȃ�
		}

		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE); // ���C�e�B���O�𖳌���

		// �X�e���V���p�̐ݒ�
		pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);          // �X�e���V���o�b�t�@��L����
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);          // Z�o�b�t�@�������݂𖳌���
		//pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000); // �J���[�o�b�t�@�������݂𖳌���

		if (caps.StencilCaps & D3DSTENCILCAPS_TWOSIDED)
		{
			// ���ʃX�e���V�����[�h��L����
			pDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, TRUE);

			pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);     // �X�e���V���֐�����ɒʉ�
			pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);  // �X�e���V���p�X���L�[�v�ɐݒ�
			pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR); // �X�e���V��Z�t�F�C�����C���N�������g�ɐݒ�
			pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);  // �X�e���V���t�F�C�����L�[�v�ɐݒ�

			pDevice->SetRenderState(D3DRS_CCW_STENCILPASS, D3DSTENCILOP_KEEP);  // �X�e���V���p�X���L�[�v�ɐݒ�
			pDevice->SetRenderState(D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_DECR); // �X�e���V��Z�t�F�C�����f�N�������g�ɐݒ�
			pDevice->SetRenderState(D3DRS_CCW_STENCILFAIL, D3DSTENCILOP_KEEP);  // �X�e���V���t�F�C�����L�[�v�ɐݒ�

			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // �J�����O���[�h�𖳌���

			CObjectX::Draw(); // �`�揈�����Ăяo��

			// ���ʃX�e���V�����[�h�𖳌���
			pDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, FALSE);
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // �J�����O���[�h��CCW�ɐݒ�
		}
		else
		{
			pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);     // �X�e���V���֐�����ɒʉ�
			pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);  // �X�e���V���p�X���L�[�v�ɐݒ�
			pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR); // �X�e���V��Z�t�F�C�����C���N�������g�ɐݒ�
			pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);  // �X�e���V���t�F�C�����L�[�v�ɐݒ�

			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW); // �J�����O���[�h��CW�ɐݒ�

			CObjectX::Draw(); // �`�揈�����Ăяo��

			pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);  // �X�e���V���p�X���L�[�v�ɐݒ�
			pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_DECR); // �X�e���V��Z�t�F�C�����f�N�������g�ɐݒ�
			pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);  // �X�e���V���t�F�C�����L�[�v�ɐݒ�

			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // �J�����O���[�h��CCW�ɐݒ�

			CObjectX::Draw(); // �`�揈�����Ăяo��
		}

		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F); // �J���[�o�b�t�@�������݂�L����

		pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);                   // �X�e���V�����t�@�����X�l��ݒ�
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);          // �X�e���V���֐��𓙂����ɐݒ�
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);     // �X�e���V���p�X���L�[�v�ɐݒ�
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);    // �X�e���V��Z�t�F�C�����L�[�v�ɐݒ�
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);     // �X�e���V���t�F�C�����L�[�v�ɐݒ�

		// ���_�o�b�t�@
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(DirectX2D::Vertex));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(DirectX2D::FVF_VERTEX);

		// �|���S���̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//�^�C�v
			0,                  // �C���f�b�N�X
			2                   //�|���S����
		);

		// ���ɖ߂�
		CManager::GetRenderer().SetRender();
	}
	else
	{
		return; // �f�o�C�X�̎擾�Ɏ��s�����ꍇ�͕`�悵�Ȃ�
	}
}