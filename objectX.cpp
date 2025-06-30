//------------------------------------------
//
// ���f���`��p�̏���[objectX.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "objectX.h"
#include "object3D.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "model.h"

//---------------------------------------
//
// �I�u�W�F�N�gX�N���X
//
//---------------------------------------

Index CObjectX::m_modelIdx{};           // ���f���^�O

//------------------------------
// ����
//------------------------------
CObjectX* CObjectX::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, int priority)
{
	CObjectX* pObjectX = new CObjectX(priority); // �C���X�^���X����

	if (pObjectX == nullptr)
	{// �������s
		return nullptr;
	}

	m_modelIdx = CModelManager::GetInstance().Create(Index(MODELTAG::Test));

	// ������
	if (FAILED(pObjectX->Init(pos, rot, scale, type)))
	{
		delete pObjectX;
		pObjectX = nullptr;
		return nullptr;
	}

	return pObjectX;
}

//------------------------------
// ����
//------------------------------
CObjectX* CObjectX::Create(Transform transform, TYPE type, int priority)
{
	CObjectX* pObjectX = new CObjectX(priority); // �C���X�^���X����

	if (pObjectX == nullptr)
	{// �������s
		return nullptr;
	}

	m_modelIdx = CModelManager::GetInstance().Create(Index(MODELTAG::Test));

	// ������
	if (FAILED(pObjectX->Init(transform, type)))
	{
		delete pObjectX;
		pObjectX = nullptr;
		return nullptr;
	}

	return pObjectX;
}

//------------------------------
//����������
//------------------------------
HRESULT CObjectX::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type)
{
	CObject::Init(type); // �e�N���X�̏�����

	SetTransform(Transform(pos, rot, scale));
	//SetSize(D3DXVECTOR3(CModelManager::GetInstance().GetSize(m_modelIdx)));

	return S_OK;
}

//------------------------------
//����������
//------------------------------
HRESULT CObjectX::Init(Transform transform, TYPE type)
{
	CObject::Init(type); // �e�N���X�̏�����

	SetTransform(transform);
	//SetSize(D3DXVECTOR3(CModelManager::GetInstance().GetSize(m_modelIdx)));

	return S_OK;
}

//------------------------------
//�I������
//------------------------------
void CObjectX::Uninit(void)
{

}

//------------------------------
//�X�V����
//------------------------------
void CObjectX::Update(void)
{
	SetPosition(D3DXVECTOR3(GetPosition().x, CManager::Get3D().GetHeight(GetPosition()), GetPosition().z)); // Transform�̍X�V
}

//------------------------------
//�`�揈��
//------------------------------
void CObjectX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice(); // �����_���[����f�o�C�X���擾

	if (pDevice == nullptr) return;

	Transform transform = GetTransform();
	D3DXMATRIX mtxRot{}, mtxTrans{}, mtxScale{};//�v�Z�}�g���b�N�X

	//�}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);

	//�傫���̔��f
	D3DXMatrixScaling(&mtxScale, transform.scale.x, transform.scale.y, transform.scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//�����̔��f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, transform.rot.y, transform.rot.x, transform.rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�̌v�Z
	D3DXMatrixTranslation(&mtxTrans, transform.pos.x, transform.pos.y, transform.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	CModelManager::GetInstance().Draw(m_modelIdx);
}