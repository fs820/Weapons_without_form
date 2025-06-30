//------------------------------------------
//
// �^�C������[time.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "time.h"
#include "renderer.h"
#include "manager.h"
#include "number.h"

//---------------------------------------
//
// �^�C���N���X
//
//---------------------------------------

//------------------------------
// ����
//------------------------------
CTime* CTime::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, size_t digits, float space, int priority)
{
	CTime* pTime = new CTime(digits, space, priority); // �C���X�^���X����

	if (pTime == nullptr)
	{// �������s
		return nullptr;
	}

	// ������
	if (FAILED(pTime->Init(pos, rot, scale)))
	{
		delete pTime;
		pTime = nullptr;
		return nullptr;
	}

	return pTime;
}

//------------------------------
// ����
//------------------------------
CTime* CTime::Create(Transform transform, size_t digits, float space, int priority)
{
	CTime* pTime = new CTime(digits, space, priority); // �C���X�^���X����

	if (pTime == nullptr)
	{// �������s
		return nullptr;
	}

	// ������
	if (FAILED(pTime->Init(transform)))
	{
		delete pTime;
		pTime = nullptr;
		return nullptr;
	}

	return pTime;
}

//------------------------------
// ����������
//------------------------------
HRESULT CTime::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	CObject::Init(TYPE::Score); // �e�N���X�̏�����

	CRenderer renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer.GetDevice(); // �����_���[����f�o�C�X���擾

	if (pDevice == nullptr)
	{
		return E_POINTER;
	}

	m_fTime = 0.0f; // ����
	m_count = NONE; // �J�E���g���

	// �X�N���[���T�C�Y�̎擾
	D3DXVECTOR2 screenSize = {};
	if (FAILED(renderer.GetDxScreenSize(&screenSize)))
	{
		return E_FAIL;
	}

	SetTransform(Transform(pos, rot, scale));

	for (size_t cntDigits = 0; cntDigits < m_digits; cntDigits++)
	{// ���ꂼ��̌��̐���
		if (m_apNumber[cntDigits] == nullptr)
		{// �Ȃ����
			m_apNumber[cntDigits] = CNumber::Create(pos + D3DXVECTOR3(m_space * (float(m_digits) / 2.0f) - float(cntDigits) * m_space, 0.0f, 0.0f), rot, scale, 0);
		}
	}

	return S_OK;
}

//------------------------------
// ����������
//------------------------------
HRESULT CTime::Init(Transform transform)
{
	CObject::Init(TYPE::Score); // �e�N���X�̏�����

	CRenderer renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer.GetDevice(); // �����_���[����f�o�C�X���擾

	if (pDevice == nullptr)
	{
		return E_POINTER;
	}

	m_fTime = 0.0f; // ����
	m_count = NONE; // �J�E���g���

	// �X�N���[���T�C�Y�̎擾
	D3DXVECTOR2 screenSize = {};
	if (FAILED(renderer.GetDxScreenSize(&screenSize)))
	{
		return E_FAIL;
	}

	SetTransform(transform);

	for (size_t cntDigits = 0; cntDigits < m_digits; cntDigits++)
	{// ���ꂼ��̌��̐���
		if (m_apNumber[cntDigits] == nullptr)
		{// �Ȃ����
			m_apNumber[cntDigits] = CNumber::Create(transform.pos + D3DXVECTOR3(m_space * (float(m_digits) / 2.0f) - float(cntDigits) * m_space, 0.0f, 0.0f), transform.rot, transform.scale, 0);
		}
	}

	return S_OK;
}

//------------------------------
// �I������
//------------------------------
void CTime::Uninit(void)
{
	for (size_t cntDigits = 0; cntDigits < m_digits; cntDigits++)
	{// ���ꂼ��̌��̐���
		if (m_apNumber[cntDigits] != nullptr)
		{// �����
			m_apNumber[cntDigits]->Uninit(); // �I��
			delete[] m_apNumber[cntDigits];  // �j��
			m_apNumber[cntDigits] = nullptr; // null
		}
	}
}

//------------------------------
// �X�V����
//------------------------------
void CTime::Update(void)
{
	// �J�E���g���
	switch (m_count)
	{
		// �J�E���g���Ȃ�
	case NONE:
		break;
		// �J�E���g�A�b�v
	case UP:
	{
		m_fTime += CMain::GetDeltaTimeGameSpeed();

		m_fTime = clamp(m_fTime, 0.0f, FLT_MAX);

		int aScore[64]{};
		CNumber::Digit(aScore, int(floorf(m_fTime)), m_digits);

		for (size_t cntDigits = 0; cntDigits < m_digits; cntDigits++)
		{// ���ꂼ��̌��̐���
			if (m_apNumber[cntDigits] != nullptr)
			{// �����
				m_apNumber[cntDigits]->SetNumber(aScore[cntDigits]);
			}
		}
		break;
	}
	// �J�E���g�_�E��
	case DOWN:
	{
		m_fTime -= CMain::GetDeltaTimeGameSpeed();

		m_fTime = clamp(m_fTime, 0.0f, FLT_MAX);

		int aScore[64]{};
		CNumber::Digit(aScore, int(floorf(m_fTime)), m_digits);

		for (size_t cntDigits = 0; cntDigits < m_digits; cntDigits++)
		{// ���ꂼ��̌��̐���
			if (m_apNumber[cntDigits] != nullptr)
			{// �����
				m_apNumber[cntDigits]->SetNumber(aScore[cntDigits]);
			}
		}

		if (m_fTime <= 0.0f) m_count = NONE; // 0�ɂȂ�����J�E���g�_�E���I��
		break;
	}
	}
}

//------------------------------
// �`�揈��
//------------------------------
void CTime::Draw(void)
{
	for (size_t cntDigits = 0; cntDigits < m_digits; cntDigits++)
	{// ���ꂼ��̌��̐���
		if (m_apNumber[cntDigits] != nullptr)
		{// �����
			m_apNumber[cntDigits]->Draw(); // �`��
		}
	}
}

//------------------------------
// �Z�b�g����
//------------------------------
void CTime::SetTime(const float time)
{
	m_fTime = time;

	m_fTime = clamp(m_fTime, 0.0f, FLT_MAX);

	int aScore[64]{};
	CNumber::Digit(aScore, int(floorf(m_fTime)), m_digits);

	for (size_t cntDigits = 0; cntDigits < m_digits; cntDigits++)
	{// ���ꂼ��̌��̐���
		if (m_apNumber[cntDigits] != nullptr)
		{// �����
			m_apNumber[cntDigits]->SetNumber(aScore[cntDigits]);
		}
	}
}

//------------------------------
// ���Z����
//------------------------------
void CTime::AddTime(const float time)
{
	m_fTime += time;

	m_fTime = clamp(m_fTime, 0.0f, FLT_MAX);

	int aScore[64]{};
	CNumber::Digit(aScore, int(floorf(m_fTime)), m_digits);

	for (size_t cntDigits = 0; cntDigits < m_digits; cntDigits++)
	{// ���ꂼ��̌��̐���
		if (m_apNumber[cntDigits] != nullptr)
		{// �����
			m_apNumber[cntDigits]->SetNumber(aScore[cntDigits]);
		}
	}
}