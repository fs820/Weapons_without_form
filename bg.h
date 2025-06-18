//------------------------------------------
//
// Playerの処理の定義・宣言[player.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "object.h"

class CObject2D; // 前方宣言

//--------------------------
// プレイヤークラス
//--------------------------
class CBg final : public CObject
{
	// 公開
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

	// 非公開
private:
	void OnCollision(const CObject& other) override {};

	static constexpr int MAX_TEXTURE = 3;            // 背景の最大数

	static const string_view m_sTexturePass[MAX_TEXTURE]; // テクスチャのパス
	static const float m_afScrollSpeed[MAX_TEXTURE]; // スクロール速度

	static CObject2D* m_pBg[MAX_TEXTURE];  // 背景のポインタ配列

	static size_t* textureIdx;             // テクスチャID

	float m_TextureU[MAX_TEXTURE]; // テクスチャのU座標
};