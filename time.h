//------------------------------------------
//
// �^�C�}�[�����̒�`�E�錾[time.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "object.h"

class CNumber;

//-----------------------------------------
// �^�C���N���X (�I�u�W�F�N�g�h��)
//-----------------------------------------
class CTime : public CObject
{
	// ���J
public:
	CTime() : m_apNumber{}, m_digits{}, m_space{}, m_fTime{}, m_count{} {}
	CTime(size_t digits, float space, int priority) : CObject(priority), m_apNumber{}, m_digits(digits), m_space(space), m_fTime{}, m_count{} {}
	virtual ~CTime() = default;

	static CTime* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, size_t digits = 5, float space = 0.05f, int priority = 3);
	static CTime* Create(Transform transform, size_t digits = 5, float space = 0.05f, int priority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale);
	HRESULT Init(Transform transform);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void CountUp(void) { m_count = UP; }
	void CountDown(void) { m_count = DOWN; }
	void Stop(void) { m_count = NONE; }

	void SetTime(const float time);
	void AddTime(const float time);
	float GetTime(void) const { return m_fTime; }

	// ����J
private:
	// �J�E���g��Ԃ̗�
	using COUNT = enum
	{
		NONE = 0,
		UP,
		DOWN,
		COUNT_MAX
	};

	void OnCollision(const CObject& other) override {};

	const size_t m_digits;   // ����
	const float m_space;     // �����̊Ԋu
	CNumber* m_apNumber[32]; // ����
	float m_fTime;           // ����
	COUNT m_count;           // �J�E���g���
};