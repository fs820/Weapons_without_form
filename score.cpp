//------------------------------------------
//
// �X�R�A����[score.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "score.h"
#include "renderer.h"
#include "manager.h"
#include "number.h"

//---------------------------------------
//
// �X�R�A�N���X
//
//---------------------------------------

//------------------------------
// ����
//------------------------------
CScore* CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, size_t digits, float space, int priority)
{
	CScore* pScore = new CScore(digits, space, priority); // �C���X�^���X����

	if (pScore == nullptr)
	{// �������s
		return nullptr;
	}

	// ������
	if (FAILED(pScore->Init(pos, rot, scale)))
	{
		delete pScore;
		pScore = nullptr;
		return nullptr;
	}

	return pScore;
}

//------------------------------
// ����
//------------------------------
CScore* CScore::Create(Transform transform, size_t digits, float space, int priority)
{
	CScore* pScore = new CScore(digits, space, priority); // �C���X�^���X����

	if (pScore == nullptr)
	{// �������s
		return nullptr;
	}

	// ������
	if (FAILED(pScore->Init(transform)))
	{
		delete pScore;
		pScore = nullptr;
		return nullptr;
	}

	return pScore;
}

//------------------------------
// ����������
//------------------------------
HRESULT CScore::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	CObject::Init(TYPE::Score); // �e�N���X�̏�����

	CRenderer renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer.GetDevice(); // �����_���[����f�o�C�X���擾

	if (pDevice == nullptr)
	{
		return E_POINTER;
	}

	m_nScore = 0;

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
HRESULT CScore::Init(Transform transform)
{
	CObject::Init(TYPE::Score); // �e�N���X�̏�����

	CRenderer renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer.GetDevice(); // �����_���[����f�o�C�X���擾

	if (pDevice == nullptr)
	{
		return E_POINTER;
	}

	m_nScore = 0;

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
void CScore::Uninit(void)
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
void CScore::Update(void)
{

}

//------------------------------
// �`�揈��
//------------------------------
void CScore::Draw(void)
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
void CScore::SetScore(const int score)
{
	m_nScore = score;

	m_nScore = clamp(m_nScore, 0, INT_MAX);

	int aScore[64]{};
	CNumber::Digit(aScore, m_nScore, m_digits);

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
void CScore::AddScore(const int score)
{
	m_nScore += score;

	m_nScore = clamp(m_nScore, 0, INT_MAX);

	int aScore[64]{};
	CNumber::Digit(aScore, m_nScore, m_digits);

	for (size_t cntDigits = 0; cntDigits < m_digits; cntDigits++)
	{// ���ꂼ��̌��̐���
		if (m_apNumber[cntDigits] != nullptr)
		{// �����
			m_apNumber[cntDigits]->SetNumber(aScore[cntDigits]);
		}
	}
}