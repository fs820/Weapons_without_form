//------------------------------------------
//
// enemy���� [enemy.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "enemy.h"
#include "renderer.h"
#include "manager.h"
#include "math.h"
#include "explosion.h"
#include "particle.h"
#include "bullet.h"
#include "debug.h"
#include "sound.h"
#include "score.h"
#include "player.h"

//---------------------------------------
//
// Enemy
//
//---------------------------------------

// �ÓI�����o�ϐ�
LPDIRECT3DTEXTURE9 CEnemy::m_apTexture[TYPE_MAX] = { nullptr }; // ���L�e�N�X�`���̃|�C���^
D3DXVECTOR2 CEnemy::m_aImageSize[TYPE_MAX] = {};             // �e�N�X�`���T�C�Y
int CEnemy::m_nNumAll = 0;                                   // �o����

//------------------------------
// �\�[�X�ǂݍ���
//------------------------------
HRESULT CEnemy::Load(const string_view sTexturePass[TYPE_MAX])
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();

	// �e�N�X�`��
	for (size_t cntTex = 0; cntTex < TYPE_MAX; cntTex++)
	{
		D3DXIMAGE_INFO imageInfo = {};
		if (FAILED(D3DXGetImageInfoFromFile
		(
			sTexturePass[cntTex].data(),
			&imageInfo
		)))
		{
			return E_FAIL;
		}
		m_aImageSize[cntTex] = D3DXVECTOR2((float)imageInfo.Width, (float)imageInfo.Height);

		if (FAILED(D3DXCreateTextureFromFile
		(
			pDevice,
			sTexturePass[cntTex].data(),
			&m_apTexture[cntTex]
		)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//------------------------------
// �\�[�X�j��
//------------------------------
void CEnemy::Unload(void)
{
	// �e�N�X�`��
	for (size_t cntTex = 0; cntTex < TYPE_MAX; cntTex++)
	{
		if (m_apTexture[cntTex] != nullptr)
		{
			m_apTexture[cntTex]->Release();
			m_apTexture[cntTex] = nullptr;
		}
	}
}

//------------------------------
// ����
//------------------------------
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float fSpeed, TYPE type, int priority)
{
	CEnemy* pBullet = new CEnemy(priority); // �C���X�^���X����

	if (pBullet == nullptr)
	{// �������s
		return nullptr;
	}

	pBullet->SetSize(D3DXVECTOR3(m_aImageSize[type].x, m_aImageSize[type].y, 0.0f)); // �T�C�Y�̐ݒ�

	// ������
	if (FAILED(pBullet->Init(pos, rot, scale, fSpeed, type)))
	{
		delete pBullet;
		pBullet = nullptr;
		return nullptr;
	}

	return pBullet;
}

//------------------------------
//����������
//------------------------------
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float fSpeed, TYPE type)
{
	CObjectBillboard::Init(pos, rot, scale, ENEMY); // �e�̏�����

	IsCollision(true); // �����蔻�������	

	// �X�N���[���T�C�Y�̎擾
	D3DXVECTOR2 screenSize = {};
	if (FAILED(CManager::GetRenderer().GetDxScreenSize(&screenSize)))
	{
		return E_FAIL;
	}

	m_nLife = LIFE;                // ���C�t�̏�����
	m_state = NORMAL;              // ������
	m_StateTime = 0.0f;  // �o������
	m_AttackTime = 0.0f; // �U������

	m_fSpeed = fSpeed;             // �|���S���̈ړ����x
	m_type = type;                 // �G�̎��

	// ���_���̐ݒ�
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	Vertex* pVtx;
	if (FAILED(pVtxBuff->Lock(0, 0, (void**)&pVtx, 0))) { return E_FAIL; }

	Transform transform = GetTransform();
	D3DXVECTOR3 screenPos = D3DXVECTOR3(transform.pos.x * screenSize.x, transform.pos.y * screenSize.y, 0.0f);

	D3DXVECTOR3 resultPos[VT_DEF] = {};
	D3DXVECTOR3 size = GetSize(); // �T�C�Y�̎擾
	size.x *= transform.scale.x;
	size.y *= transform.scale.y;
	CMath::Rotation(resultPos, D3DXVECTOR2(size.x, size.y), GetRotation().z);

	for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
	{
		resultPos[cntVtx] += screenPos;

		pVtx[cntVtx].pos = resultPos[cntVtx];
		pVtx[cntVtx].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[cntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[cntVtx].tex = D3DXVECTOR2((float)(cntVtx % 2) * 1.0f, (float)(cntVtx / 2) * 1.0f);
	}

	if (FAILED(pVtxBuff->Unlock())) { return E_FAIL; }

	return S_OK;
}

//------------------------------
//�I������
//------------------------------
void CEnemy::Uninit(void)
{
	CObjectBillboard::Uninit(); // �e�̏I��
}

//------------------------------
//�X�V����
//------------------------------
void CEnemy::Update(void)
{
	CObjectBillboard::Update(); // �e�̍X�V

	CRenderer renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer.GetDevice(); // �����_���[����f�o�C�X���擾

	float deltaTime = CMain::GetDeltaTimeGameSpeed();

	// �X�N���[���T�C�Y�̎擾
	D3DXVECTOR2 screenSize = {};
	if (FAILED(renderer.GetDxScreenSize(&screenSize)))
	{
		return;
	}

	// ���_���̐ݒ�
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	Vertex* pVtx;
	if (FAILED(pVtxBuff->Lock(0, 0, (void**)&pVtx, 0))) { return; }

	Transform transform = GetTransform(); // �ό`���̎擾

	// ��]�֌W����
	D3DXVECTOR3 screenPos = D3DXVECTOR3(transform.pos.x * screenSize.x, transform.pos.y * screenSize.y, 0.0f);
	D3DXVECTOR3 resultPos[VT_DEF] = {};

	D3DXVECTOR3 size = GetSize(); // �e�N�X�`���T�C�Y�̎擾
	size.x *= transform.scale.x;
	size.y *= transform.scale.y;
	CMath::Rotation(resultPos, D3DXVECTOR2(size.x, size.y), transform.rot.z);

	// ���_���̐ݒ�
	for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
	{
		resultPos[cntVtx] += screenPos;

		pVtx[cntVtx].pos = resultPos[cntVtx];
		pVtx[cntVtx].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[cntVtx].col = m_state == DAMAGE ? RED : WHITE; // �_���[�W��Ԃ͐�
		pVtx[cntVtx].tex = D3DXVECTOR2((float)(cntVtx % 2) * 1.0f, (float)(cntVtx / 2) * 1.0f);
	}

	if (FAILED(pVtxBuff->Unlock())) { return; }

	// ��ԊǗ�
	switch (m_state)
	{
	case NONE:
		break;
	case NORMAL:
		if ((m_AttackTime += CMain::GetDeltaTimeGameSpeed()) >= ATTACK_TIME)
		{// ��莞�Ԍo��
			D3DXVECTOR3 playerPos{};
			D3DXVECTOR3 space = playerPos - GetPosition();
			float angle = atan2f(space.x, space.y);
			CMath::NormalizeAngle(&angle);
			CBullet::Create(transform.pos, D3DXVECTOR3(0.0f, 0.0f, angle), transform.scale, CMath::Random(0.05f, 0.5f), CBullet::ENEMY, 2);

			m_AttackTime = 0.0f; // �߂�
		}
		break;
	case DAMAGE:
		if ((m_StateTime += CMain::GetDeltaTimeGameSpeed()) >= DAMAGE_TIME)
		{// ��莞�Ԍo��
			m_state = NORMAL; // �ʏ���
			m_StateTime = 0.0f; // �߂�
		}
		break;
	case DEATH:
		if ((m_StateTime += CMain::GetDeltaTimeGameSpeed()) >= DEATH_TIME)
		{// ��莞�Ԍo��
			Release();
			return;
		}
		break;
	}

	CDebugProc::Print(CDebugProc::OBJECT, "Enemy%d:%d", GetID(), m_state);
	CDebugProc::Print(CDebugProc::OBJECT, "Enemy%d ��ԃJ�E���^�[:%f", GetID(), m_StateTime);
	CDebugProc::Print(CDebugProc::OBJECT, "Enemy%d �U���J�E���^�[:%f", GetID(), m_AttackTime);
}

//------------------------------
//�`�揈��
//------------------------------
void CEnemy::Draw(void)
{
	CObjectBillboard::Draw(); // �e�̕`��
}

//------------------------------
// �Փˏ���
//------------------------------
void CEnemy::OnCollision(const CObject& other)
{

}

//--------------------------
// �_���[�W����
//--------------------------
void CEnemy::Hit(int damage)
{
	if (m_state == NORMAL || m_state == DAMAGE)
	{// ���G���ԂłȂ�
		m_nLife -= damage; // �_���[�W
		if (m_nLife <= 0)
		{// �̗͂��Ȃ��Ȃ���
			m_state = DEATH; // ��

			Transform transform = GetTransform();
			CExplosion::Create(transform.pos, transform.rot, transform.scale * 50.0f, CMath::Random(CExplosion::DEFAULT, CExplosion::EXTRA), 4); // �����G�t�F�N�g�𐶐�

			CManager::GetScore()->AddScore(10);
		}
		else
		{
			m_state = DAMAGE; // �_���[�W
			CManager::GetScore()->AddScore(1);
		}
		m_StateTime = 0.0f; // �߂�
	}
}