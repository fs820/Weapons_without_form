//------------------------------------------
//
// �v���C���[�̏��� [player.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "player.h"
#include "renderer.h"
#include "manager.h"
#include "math.h"
#include "bullet.h"
#include "explosion.h"
#include "input.h"
#include "particle.h"
#include "debug.h"
#include "sound.h"
#include "score.h"
#include "enemy.h"
#include "texture.h"
#include "model.h"
#include "object3D.h"

//---------------------------------------
//
// �v���C���[�N���X
//
//---------------------------------------

// �ÓI�����o�ϐ�
Index* CPlayer::m_modelID{};                       // ���f���̃C���f�b�N�X
size_t CPlayer::m_modelNum{};                      // ���f���̐�

//-----------------------------
// �\�[�X�ǂݍ���
//-----------------------------
HRESULT CPlayer::Load(const size_t modelNum, const string_view sTexturePass[], const D3DXVECTOR3 offSet[], const int modelParentIdx[])
{
	m_modelNum = modelNum;
	m_modelID = new Index[m_modelNum];

	if (offSet == nullptr)return E_POINTER;

	for (size_t cntModel = 0; cntModel < m_modelNum; cntModel++)
	{
		m_modelID[cntModel] = CModelManager::GetInstance().Register(sTexturePass[cntModel], offSet[cntModel]);
	}

	if (modelParentIdx != nullptr)
	{
		for (size_t cntModel = 0; cntModel < m_modelNum; cntModel++)
		{
			CModelManager::GetInstance().SetParent(m_modelID[cntModel], m_modelID[modelParentIdx[cntModel]]);
		}
	}

	return S_OK;
}

//------------------------------
// ����
//------------------------------
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float fSpeed, float fRotSpeed, int priority)
{
	CPlayer* pPlayer = new CPlayer(priority); // �C���X�^���X����

	if (pPlayer == nullptr)
	{// �������s
		return nullptr;
	}

	// ������
	if (FAILED(pPlayer->Init(pos, rot, scale, fSpeed, fRotSpeed)))
	{
		delete pPlayer;
		pPlayer = nullptr;
		return nullptr;
	}

	return pPlayer;
}

//------------------------------
//����������
//------------------------------
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float fSpeed, float fRotSpeed)
{
	CObject::Init(PLAYER); // �e�̏�����

	IsCollision(true); // �����蔻�������	

	// �X�N���[���T�C�Y�̎擾
	D3DXVECTOR2 screenSize = {};
	if (FAILED(CManager::GetRenderer().GetDxScreenSize(&screenSize)))
	{
		return E_FAIL;
	}

	SetTransform(Transform(pos, rot, scale));

	m_state = APPEAR;
	m_StateTime = CMain::GetElapsedTimeGameSpeed();

	m_nLife = LIFE; // �v���C���[�̃��C�t

	m_fSpeed = fSpeed;             // �|���S���̈ړ����x
	m_fRotSpeed = fRotSpeed;       // �|���S���̉�]���x

	return S_OK;
}

//------------------------------
//�I������
//------------------------------
void CPlayer::Uninit(void)
{
	
}

//------------------------------
//�X�V����
//------------------------------
void CPlayer::Update(void)
{
	if (m_state != DEATH)
	{// ����łȂ����
		//�ړ��֌W
		//CInputKeyboard keyboard = CManager::GetInputKeyboard();
		Transform transform = GetTransform();
		float deltaTime = CMain::GetDeltaTimeGameSpeed();

		D3DXVECTOR3 move = { 0.0f, 0.0f, 0.0f };
		float fAngle = GetRotation().y;

		//if (keyboard.GetPress(DIK_A))//A�L�[
		//{
		//	if (keyboard.GetPress(DIK_W))//W�L�[
		//	{//����
		//		move.x += sinf(D3DX_PI * -0.25f) * m_fSpeed * deltaTime;
		//		move.z += cosf(D3DX_PI * -0.25f) * m_fSpeed * deltaTime;
		//		fAngle = D3DX_PI * 0.75f;
		//	}
		//	else if (keyboard.GetPress(DIK_S))//S�L�[
		//	{//����
		//		move.x += sinf(D3DX_PI * -0.75f) * m_fSpeed * deltaTime;
		//		move.z += cosf(D3DX_PI * -0.75f) * m_fSpeed * deltaTime;
		//		fAngle = D3DX_PI * 0.25f;
		//	}
		//	else
		//	{//��
		//		move.x += sinf(D3DX_PI * -0.5f) * m_fSpeed * deltaTime;
		//		move.z += cosf(D3DX_PI * -0.5f) * m_fSpeed * deltaTime;
		//		fAngle = D3DX_PI * 0.5f;
		//	}
		//}
		//else if (keyboard.GetPress(DIK_D))//D�L�[
		//{
		//	if (keyboard.GetPress(DIK_W) == true)//W�L�[
		//	{//�E��
		//		move.x += sinf(D3DX_PI * 0.25f) * m_fSpeed * deltaTime;
		//		move.z += cosf(D3DX_PI * 0.25f) * m_fSpeed * deltaTime;
		//		fAngle = D3DX_PI * -0.75f;
		//	}
		//	else if (keyboard.GetPress(DIK_S) == true)//S�L�[
		//	{//�E��
		//		move.x += sinf(D3DX_PI * 0.75f) * m_fSpeed * deltaTime;
		//		move.z += cosf(D3DX_PI * 0.75f) * m_fSpeed * deltaTime;
		//		fAngle = D3DX_PI * -0.25f;
		//	}
		//	else
		//	{//�E
		//		move.x += sinf(D3DX_PI * 0.5f) * m_fSpeed * deltaTime;
		//		move.z += cosf(D3DX_PI * 0.5f) * m_fSpeed * deltaTime;
		//		fAngle = D3DX_PI * -0.5f;
		//	}
		//}
		//else if (keyboard.GetPress(DIK_W) == true)//W�L�[
		//{//��
		//	move.x += sinf(0.0f) * m_fSpeed * deltaTime;
		//	move.z += cosf(0.0f) * m_fSpeed * deltaTime;
		//	fAngle = D3DX_PI;
		//}
		//else if (keyboard.GetPress(DIK_S) == true)//S�L�[
		//{//��
		//	move.x += sinf(D3DX_PI) * m_fSpeed * deltaTime;
		//	move.z += cosf(D3DX_PI) * m_fSpeed * deltaTime;
		//	fAngle = 0.0f;
		//}

		move.x += 10.0f * deltaTime; // �d�͂�������
		move.z -= 10.0f * deltaTime; // �d�͂�������

		move.y += -10.0f * deltaTime; // �d�͂�������
		AddPosition(move);

		Vector3 pos = GetPosition();
		SetPosition(D3DXVECTOR3(pos.x, CManager::Get3D().GetHeight(pos), pos.z)); // �ړ�
		CMath::NormalizeAngle(&fAngle);
		SetRotation(D3DXVECTOR3(0.0f, fAngle, 0.0f));

		//if (keyboard.GetTrigger(DIK_SPACE))
		//{
		//	size_t randomNum = CMath::Random(1, 3);
		//	float angleSet = 2.0f * D3DX_PI / (float)randomNum; // �e�̊p�x�Ԋu
		//	for (size_t cntBullet = 0; cntBullet < randomNum; cntBullet++)
		//	{
		//		float angle = transform.rot.y - cntBullet * angleSet;
		//		CMath::NormalizeAngle(&angle);
		//		CBullet::Create(transform.pos, D3DXVECTOR3(0.0f, 0.0f, atan2f(sinf(angle), cosf(angle))), transform.scale * 10.0f, CMath::Random(0.05f, 0.5f), CBullet::PLAYER, 2);
		//	}
		//}
	}

	Transform transform = GetTransform();

	// ��ԊǗ�
	switch (m_state)
	{
	case NONE:
		break;
	case APPEAR:
		if (CMain::GetElapsedTimeGameSpeed() - m_StateTime >= APPEAR_TIME)
		{// ��莞�Ԍo��
			m_state = NORMAL; // �ʏ���
			m_StateTime = CMain::GetElapsedTimeGameSpeed(); // ���ԋL�^
		}
		break;
	case NORMAL:
		break;
	case DAMAGE:
		if (CMain::GetElapsedTimeGameSpeed() - m_StateTime >= DAMAGE_TIME)
		{// ��莞�Ԍo��
			m_state = NORMAL; // �ʏ���
			m_StateTime = CMain::GetElapsedTimeGameSpeed(); // ���ԋL�^
		}
		break;
	case DEATH:
		if (CMain::GetElapsedTimeGameSpeed() - m_StateTime >= DEATH_TIME)
		{// ��莞�Ԍo��
			Release();
			return;
		}
		break;
	}

	CDebugProc::Print(CDebugProc::OBJECT, "Player%d:%d", GetID(), m_state);
	CDebugProc::Print(CDebugProc::OBJECT, "Player%d ��ԃJ�E���^�[:%f", GetID(), m_StateTime);
}

//------------------------------
//�`�揈��
//------------------------------
void CPlayer::Draw(void)
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

	// ���f���̕`��
	for (size_t cntModel = 0; cntModel < m_modelNum; cntModel++)
	{
		CModelManager::GetInstance().Draw(m_modelID[cntModel]);
	}
}

//------------------------------
// �Փˏ���
//------------------------------
void CPlayer::OnCollision(const CObject& other)
{

}

//--------------------------
// �_���[�W����
//--------------------------
void CPlayer::Hit(int damage)
{
	m_nLife -= damage; // �_���[�W
	if (m_nLife <= 0)
	{// �̗͂��Ȃ��Ȃ���
		m_state = DEATH; // ��

		Transform transform = GetTransform();
		CExplosion::Create(transform.pos, transform.rot, transform.scale * 100.0f, CMath::Random(CExplosion::DEFAULT, CExplosion::EXTRA),4); // �����G�t�F�N�g�𐶐�

		CManager::GetScore()->AddScore(-100);
	}
	else
	{
		m_state = DAMAGE; // �_���[�W
		CManager::GetScore()->AddScore(-10);
	}
	m_StateTime = CMain::GetElapsedTimeGameSpeed(); // ����
}