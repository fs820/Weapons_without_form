//------------------------------------------
//
// �I�u�W�F�N�g�̏����̒�`�E�錾[object.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "renderer.h"

//---------------------
// �I�u�W�F�N�g�N���X
//---------------------
class CObject
{
// ���J
public:
	// �^�C�v
	enum class TYPE : Index
	{
		None,       // ����
		BackGround, // �w�i
		Player,     // �v���C���[
		Enemy,      // �G
		Bullet,     // �e
		Explosion,  // ����
		Effect,     // �G�t�F�N�g
		Score,      // �X�R�A
		Max         // �ő吔
	};

	CObject(int priority = 3) :m_Priority(priority), m_ID(INVALID_ID), m_type{}, m_transform{}, m_bCollision{}, m_bRelease{}, m_bAfterimage{} {} // �f�t�H���g�����ɂ��f�t�H���g�R���X�g���N�^�����p���Ă���
	virtual ~CObject() = default;

	static void UpdateAll(void);
	static void DrawAll(bool bAfterimage = false);
	static void DrawPriority(Index priority, bool bAfterimage = false);
	static void ReleaseAll(void);

	static size_t GetObjectAll(void) { return m_nAll; }

	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	Index GetPriority(void) const { return m_Priority; }
	Index GetID(void) const { return m_ID; }
	TYPE GetType(void) const { return m_type; }

	DirectX::Transform GetTransform(void) const { return m_transform; }
	DirectX::Vector3 GetPosition(void) const { return m_transform.pos; }
	DirectX::Vector3 GetRotation(void) const { return m_transform.rot; }
	DirectX::Vector3 GetScale(void) const { return m_transform.scale; }

	DirectX::Vector3 GetSize(void) const { return m_size; }

	void SetTransform(const DirectX::Transform transform) { m_transform = transform; }
	void SetPosition(const DirectX::Vector3 pos) { m_transform.pos = pos; }
	void SetRotation(const DirectX::Vector3 rot) { m_transform.rot = rot; }
	void SetScale(const DirectX::Vector3 scale) { m_transform.scale = scale; }

	void SetSize(const DirectX::Vector3 size) { m_size = size; }

	void AddTransform(const DirectX::Transform transform) { m_transform.pos += transform.pos; m_transform.rot += transform.rot; m_transform.scale += transform.scale; }
	void AddPosition(const DirectX::Vector3 pos) { m_transform.pos += pos; }
	void AddRotation(const DirectX::Vector3 rot) { m_transform.rot += rot; }
	void AddScale(const DirectX::Vector3 scale) { m_transform.scale += scale; }

	void SetCollision(const bool bCollision) { m_bCollision = bCollision; }
	bool IsCollision(void) const { return m_bCollision; }

	void SetRelease(const bool bRelease) { m_bRelease = bRelease; }
	bool IsRelease(void) const { return m_bRelease; }

	void SetAfterimage(const bool bAfterimage) { m_bAfterimage = bAfterimage; }
	bool IsAfterimage(void) const { return m_bAfterimage; }

// �Ƒ����J
protected:
	virtual void OnCollision(const CObject& other) = 0;

	void Init(TYPE type);

// ����J
private:
	static void Release(CObject* pObject);
	static void CallCollision(void);
	static bool CallCollisionHelper(const CObject& HostObject, const CObject& GuestObject);

	static array<vector<CObject*>, 8> m_apObject;   // �I�u�W�F�N�g�̃|�C���^�z��
	static int m_nAll;                              // �I�u�W�F�N�g�̐�

	Index m_Priority;                 // �`�揇��
	Index m_ID;                       // �I�u�W�F�N�g��ID
	TYPE m_type;                      // �I�u�W�F�N�g�̎��
	DirectX::Transform m_transform;   // �ʒu�E��]�E�X�P�[��
	DirectX::Vector3 m_size;          // �T�C�Y�i�����蔻��p�j
	bool m_bCollision;                // �����蔻��̗L��
	bool m_bRelease;                  // �j���t���O
	bool m_bAfterimage;               // �c���t���O
};