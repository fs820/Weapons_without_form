//------------------------------------------
//
// Number���� [number.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "number.h"
#include "manager.h"
#include "math.h"
#include "debug.h"
#include "texture.h"

using namespace DirectX2D;

//---------------------------------------
//
// Number
//
//---------------------------------------

//------------------------------
// ����
//------------------------------
CNumber* CNumber::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int startNumber)
{
	CNumber* pNumber = new CNumber; // �C���X�^���X����

	if (pNumber == nullptr)
	{// �������s
		return nullptr;
	}

	// ������
	if (FAILED(pNumber->Init(pos, rot, scale, startNumber)))
	{
		delete pNumber;
		pNumber = nullptr;
		return nullptr;
	}

	return pNumber;
}

//------------------------------
// ����������
//------------------------------
HRESULT CNumber::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int startNumber)
{
	CRenderer renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer.GetDevice();

	if (pDevice == nullptr)
	{
		return E_POINTER;
	}

	m_number = startNumber; // �������l�̓o�^

	//�o�b�t�@�[�̐ݒ�
	if (FAILED(pDevice->CreateVertexBuffer
	(
		sizeof(Vertex) * VT_DEF,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr
	)))
	{
		return E_FAIL;
	}

	// �X�N���[���T�C�Y�̎擾
	D3DXVECTOR2 screenSize = {};
	if (FAILED(renderer.GetDxScreenSize(&screenSize)))
	{
		return E_FAIL;
	}

	// ���_���̐ݒ�
	Vertex* pVtx;
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0))) { return E_FAIL; }

	D3DXVECTOR3 screenPos = D3DXVECTOR3(pos.x * screenSize.x, pos.y * screenSize.y, 0.0f);

	D3DXVECTOR3 resultPos[VT_DEF] = {};

	D3DXVECTOR2 imageSize = CTextureManager::GetInstance().GetSize(CManager::Number);
	D3DXVECTOR2 imageBlock = CTextureManager::GetInstance().GetBlock(CManager::Number);
	m_size.x = imageSize.x * scale.x;
	m_size.y = imageSize.y * scale.y;

	m_size.x /= imageBlock.x;
	m_size.y /= imageBlock.y;

	resultPos[0] = D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y * 0.5f, 0.0f);
	resultPos[1] = D3DXVECTOR3(m_size.x * 0.5f, -m_size.y * 0.5f, 0.0f);
	resultPos[2] = D3DXVECTOR3(-m_size.x * 0.5f, m_size.y * 0.5f, 0.0f);
	resultPos[3] = D3DXVECTOR3(m_size.x * 0.5f, m_size.y * 0.5f, 0.0f);

	for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
	{
		resultPos[cntVtx] += screenPos;

		pVtx[cntVtx].pos = resultPos[cntVtx];
		pVtx[cntVtx].rhw = 1.0f;
		pVtx[cntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[cntVtx].tex = D3DXVECTOR2((float)(cntVtx % 2) * 1.0f / imageBlock.x + m_number / imageBlock.x, (float)(cntVtx / 2) * 1.0f);
	}

	if (FAILED(m_pVtxBuff->Unlock())) { return E_FAIL; }

	return S_OK;
}

//------------------------------
// �I������
//------------------------------
void CNumber::Uninit(void)
{
	// ���_
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//------------------------------
// �`�揈��
//------------------------------
void CNumber::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice(); // �����_���[����f�o�C�X���擾

	if (pDevice == nullptr || m_pVtxBuff == nullptr)
	{
		return;
	}

	// ���_�o�b�t�@
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(Vertex));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CTextureManager::GetInstance().GetTexture(CManager::Number));

	// �|���S���̕`��
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//�^�C�v
		0,                  // �C���f�b�N�X
		2                   //�|���S����
	);
}

//------------------------------
// �`�揈��
//------------------------------
void CNumber::SetNumber(const int number)
{
	m_number = number; // ���l��ύX

	// ���_���̐ݒ�
	Vertex* pVtx;
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0))) { return; }

	D3DXVECTOR2 imageBlock = CTextureManager::GetInstance().GetBlock(CManager::Number);
	for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
	{
		pVtx[cntVtx].tex = D3DXVECTOR2((float)(cntVtx % 2) * 1.0f / imageBlock.x + m_number / imageBlock.x, (float)(cntVtx / 2) * 1.0f);
	}

	if (FAILED(m_pVtxBuff->Unlock())) { return; }

	return;
}

//------------------------------
// �`�揈��
//------------------------------
void CNumber::AddNumber(const int number)
{
	m_number += number; // ���l�𑫂�

	// ���_���̐ݒ�
	Vertex* pVtx;
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0))) { return; }

	D3DXVECTOR2 imageBlock = CTextureManager::GetInstance().GetBlock(CManager::Number);
	for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
	{
		pVtx[cntVtx].tex = D3DXVECTOR2((float)(cntVtx % 2) * 1.0f / imageBlock.x + m_number / imageBlock.x, (float)(cntVtx / 2) * 1.0f);
	}

	if (FAILED(m_pVtxBuff->Unlock())) { return; }

	return;
}

//--------------------
//����������
//--------------------
void CNumber::Digit(int* aNumber, const int nData, const size_t digits)
{
	int nRank = 1;        // ����
	int nCpyData = nData; // ���l���R�s�[

	// �������擾
	while (nCpyData >= 10)
	{// 1���ɂȂ�܂ŌJ��Ԃ�
		nCpyData /= 10; // �ʂ�������
		nRank += 1;    // ����+1
	}

	// ���ꂼ��̌��̒l���擾
	aNumber[0] = nData % 10;
	for (size_t cntRank = 1; cntRank < size_t(nRank); cntRank++)
	{// �ʂ����[�v���Ċm�F
		aNumber[cntRank] = nData / int(powf(10.0f, float(cntRank))) % 10;
	}
}