//------------------------------------------
//
// 3D�`��p�̏���[objectbillboard.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "objectbillboard.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

using namespace DirectX3D; // DirectX3D���O��Ԃ̎g�p

//---------------------------------------
//
// �I�u�W�F�N�g2D�N���X
//
//---------------------------------------

size_t CObjectBillboard::textureIdx{};           // �e�N�X�`���ԍ�

//------------------------------
// ����
//------------------------------
CObjectBillboard* CObjectBillboard::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, int priority)
{
	CObjectBillboard* pObjectBillboard = new CObjectBillboard(priority); // �C���X�^���X����

	if (pObjectBillboard == nullptr)
	{// �������s
		return nullptr;
	}

	textureIdx = CTextureManager::GetInstance().Register("data\\TEXTURE\\enemy001.png");

	// ������
	if (FAILED(pObjectBillboard->Init(pos, rot, scale, type)))
	{
		delete pObjectBillboard;
		pObjectBillboard = nullptr;
		return nullptr;
	}

	return pObjectBillboard;
}

//------------------------------
// ����
//------------------------------
CObjectBillboard* CObjectBillboard::Create(Transform transform, TYPE type, int priority)
{
	CObjectBillboard* pObjectBillboard = new CObjectBillboard(priority); // �C���X�^���X����

	if (pObjectBillboard == nullptr)
	{// �������s
		return nullptr;
	}

	textureIdx = CTextureManager::GetInstance().Register("data\\TEXTURE\\enemy001.png");

	// ������
	if (FAILED(pObjectBillboard->Init(transform, type)))
	{
		delete pObjectBillboard;
		pObjectBillboard = nullptr;
		return nullptr;
	}

	return pObjectBillboard;
}

//------------------------------
//����������
//------------------------------
HRESULT CObjectBillboard::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type)
{
	CObject::Init(type); // �e�N���X�̏�����

	CRenderer renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer.GetDevice(); // �����_���[����f�o�C�X���擾

	if (pDevice == nullptr)
	{
		return E_POINTER;
	}

	SetTransform(Transform(pos, rot, scale));
	SetSize(D3DXVECTOR3(CTextureManager::GetInstance().GetSize(textureIdx)));

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

	// ���_���̐ݒ�
	Vertex* pVtx;
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0))) { return E_FAIL; }

	D3DXVECTOR3 resultPos[VT_DEF] = {};
	D3DXVECTOR3 size = GetSize(); // �T�C�Y�̎擾

	resultPos[0] = D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, 0.0f);
	resultPos[1] = D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, 0.0f);
	resultPos[2] = D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, 0.0f);
	resultPos[3] = D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, 0.0f);

	for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
	{
		pVtx[cntVtx].pos = resultPos[cntVtx];
		pVtx[cntVtx].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[cntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[cntVtx].tex = D3DXVECTOR2((float)(cntVtx % 2) * 1.0f, (float)(cntVtx / 2) * 1.0f);
	}

	if (FAILED(m_pVtxBuff->Unlock())) { return E_FAIL; }

	return S_OK;
}

//------------------------------
//����������
//------------------------------
HRESULT CObjectBillboard::Init(Transform transform, TYPE type)
{
	CObject::Init(type); // �e�N���X�̏�����

	CRenderer renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer.GetDevice(); // �����_���[����f�o�C�X���擾

	if (pDevice == nullptr)
	{
		return E_POINTER;
	}

	SetTransform(transform);

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

	// ���_���̐ݒ�
	Vertex* pVtx;
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0))) { return E_FAIL; }

	D3DXVECTOR3 resultPos[VT_DEF] = {};
	D3DXVECTOR3 size = GetSize(); // �T�C�Y�̎擾

	resultPos[0] = D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, 0.0f);
	resultPos[1] = D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, 0.0f);
	resultPos[2] = D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, 0.0f);
	resultPos[3] = D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, 0.0f);

	for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
	{
		pVtx[cntVtx].pos = resultPos[cntVtx];
		pVtx[cntVtx].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[cntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[cntVtx].tex = D3DXVECTOR2((float)(cntVtx % 2) * 1.0f, (float)(cntVtx / 2) * 1.0f);
	}

	if (FAILED(m_pVtxBuff->Unlock())) { return E_FAIL; }

	return S_OK;
}

//------------------------------
//�I������
//------------------------------
void CObjectBillboard::Uninit(void)
{
	// ���_�o�b�t�@�̉��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//------------------------------
//�X�V����
//------------------------------
void CObjectBillboard::Update(void)
{

}

//------------------------------
//�`�揈��
//------------------------------
void CObjectBillboard::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice(); // �����_���[����f�o�C�X���擾

	if (pDevice == nullptr || m_pVtxBuff == nullptr)
	{
		return;
	}

	Transform transform = GetTransform();
	D3DXMATRIX mtxRot{}, mtxTrans{}, mtxScale{};//�v�Z�}�g���b�N�X

	//�}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);

	//�r���[�}�g���b�N�X�擾
	D3DXMATRIX mtxView;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�J�����̋t�s���ݒ�
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	//�傫���̔��f
	D3DXMatrixScaling(&mtxScale, transform.scale.x, transform.scale.y, transform.scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//�ʒu�̌v�Z
	D3DXMatrixTranslation(&mtxTrans, transform.pos.x, transform.pos.y, transform.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(Vertex));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CTextureManager::GetInstance().GetTexture(textureIdx));

	// �|���S���̕`��
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//�^�C�v
		0,                  // �C���f�b�N�X
		2                   //�|���S����
	);
}