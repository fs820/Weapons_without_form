//------------------------------------------
//
// Bullet�̏����̒�`�E�錾[explosion.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "objectbillboard.h"

//--------------------------
// �o���b�g (���[)
//--------------------------
class CExplosion final : public CObjectBillboard
{
	// ���J
public:
	enum class TYPE : Index
	{
		Defalt, // �f�t�H���g
		Extra,  // �ǉ�
		Max     // �ő�
	};

	CExplosion() : m_nAnimationCount{}, m_aniLastTime{}, m_type{} {}
	CExplosion(int priority) : CObjectBillboard(priority), m_nAnimationCount{}, m_aniLastTime{}, m_type{} {}
	~CExplosion() = default;

	static HRESULT Load(const string_view sTexturePass[Index(TYPE::Max)]);
	static void Unload(void);
	static CExplosion* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, int priority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

// ����J
private:
	void OnCollision(const CObject& other) override {};

	static const D3DXVECTOR2 m_imageBlock[Index(TYPE::Max)]; // �e�N�X�`���u���b�N��
	static const float m_AnimationTime;	             // �A�j���[�V�����^�C��

	static LPDIRECT3DTEXTURE9 m_apTexture[Index(TYPE::Max)]; // ���L�e�N�X�`���̃|�C���^
	static D3DXVECTOR2 m_aImageSize[Index(TYPE::Max)];       // �e�N�X�`���T�C�Y

	int m_nAnimationCount;              // �A�j���[�V�����J�E���g
	float m_aniLastTime;                // �A�j���[�V�����X�V�^�C��

	TYPE m_type;                       // �^�C�v
};