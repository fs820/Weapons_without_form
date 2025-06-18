//------------------------------------------
//
// Player�̏����̒�`�E�錾[player.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "object.h"

//--------------------------
// �v���C���[�N���X (���[)
//--------------------------
class CPlayer final : public CObject
{
// ���J
public:
	// ��ԊǗ�
	using STATE = enum
	{
		NONE = 0, // ��
		APPEAR,   // �o��
		NORMAL,   // �ʏ�
		DAMAGE,   // �_���[�W
		DEATH,    // ��
		STATE_MAX // ��Ԑ�
	};

	CPlayer() : m_mtxWorld{}, m_state{}, m_StateTime{}, m_nLife{}, m_fSpeed{}, m_fRotSpeed{} {}
	CPlayer(int priority) :CObject(priority), m_mtxWorld{}, m_state {}, m_StateTime{}, m_nLife{}, m_fSpeed{}, m_fRotSpeed{} {}
	~CPlayer() = default;

	static HRESULT Load(const size_t modelNum, const string_view sTexturePass[], const D3DXVECTOR3 offSet[], const int modelParentIdx[] = nullptr);
	static void Unload(void) { delete m_modelID; m_modelID = nullptr; };
	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float fSpeed, float fRotSpeed, int priority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float fSpeed, float fRotSpeed);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	int GetLife(void) const { return m_nLife; }
	STATE GetState(void) const { return m_state; }

// ����J
private:
	void OnCollision(const CObject& other) override;
	void Hit(int damage);

	static constexpr int LIFE = 50;            // ���C�t
	static constexpr float APPEAR_TIME = 2.0f; // �o������
	static constexpr float DAMAGE_TIME = 0.5f; // �_���[�W����
	static constexpr float DEATH_TIME = 2.0f;  // ������

	static Index* m_modelID;                       // ���f���̃C���f�b�N�X
	static size_t m_modelNum;                      // ���f���̐�

	D3DXMATRIX m_mtxWorld;                  // ���[���h�}�g���b�N�X

	STATE m_state;                          // ���
	float m_StateTime;                      // ��ԃ^�C�}�[
	int m_nLife;                            // ���C�t

	float m_fSpeed;                         // �ړ����x
	float m_fRotSpeed;                      // ��]���x
};