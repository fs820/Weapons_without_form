//------------------------------------------
//
// Player�̏����̒�`�E�錾[player.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "object.h"

class CObject2D; // �O���錾

//--------------------------
// �v���C���[�N���X
//--------------------------
class CBg final : public CObject
{
	// ���J
public:
	CBg() : m_TextureU{} {}
	CBg(int priority) :CObject(priority), m_TextureU{} {}
	~CBg() = default;

	static HRESULT Load(void);
	static CBg* Create(int priority = 3);

	HRESULT Init(void);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	// ����J
private:
	void OnCollision(const CObject& other) override {};

	static constexpr int MAX_TEXTURE = 3;            // �w�i�̍ő吔

	static const string_view m_sTexturePass[MAX_TEXTURE]; // �e�N�X�`���̃p�X
	static const float m_afScrollSpeed[MAX_TEXTURE]; // �X�N���[�����x

	static CObject2D* m_pBg[MAX_TEXTURE];  // �w�i�̃|�C���^�z��

	static size_t* textureIdx;             // �e�N�X�`��ID

	float m_TextureU[MAX_TEXTURE]; // �e�N�X�`����U���W
};