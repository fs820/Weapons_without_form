//------------------------------------------
//
// �X�R�A�̏����̒�`�E�錾[score.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "object.h"

class CNumber;

//-----------------------------------------
// �X�R�A�N���X (�I�u�W�F�N�g�h��)
//-----------------------------------------
class CScore : public CObject
{
// ���J
public:
	CScore() : m_apNumber{}, m_digits{}, m_space{}, m_nScore{} {}
	CScore(size_t digits, float space, int priority) : CObject(priority), m_apNumber{}, m_digits(digits), m_space(space), m_nScore{} {}
	virtual ~CScore() = default;

	static CScore* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, size_t digits = 5, float space = 0.05f, int priority = 3);
	static CScore* Create(DirectX::Transform transform, size_t digits = 5, float space = 0.05f, int priority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale);
	HRESULT Init(DirectX::Transform transform);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetScore(const int score);
	void AddScore(const int score);
	int GetScore(void) const { return m_nScore; }

// ����J
private:
	void OnCollision(const CObject& other) override {};

	const size_t m_digits;         // ����
	const float m_space;           // �����̊Ԋu
	array<CNumber*,32> m_apNumber; // ����
	int m_nScore;                  // �X�R�A
};