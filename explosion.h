//------------------------------------------
//
// Bulletの処理の定義・宣言[explosion.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "objectbillboard.h"

//--------------------------
// バレット (末端)
//--------------------------
class CExplosion final : public CObjectBillboard
{
	// 公開
public:
	enum class TYPE : Index
	{
		Defalt, // デフォルト
		Extra,  // 追加
		Max     // 最大
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

// 非公開
private:
	void OnCollision(const CObject& other) override {};

	static const D3DXVECTOR2 m_imageBlock[Index(TYPE::Max)]; // テクスチャブロック数
	static const float m_AnimationTime;	             // アニメーションタイム

	static LPDIRECT3DTEXTURE9 m_apTexture[Index(TYPE::Max)]; // 共有テクスチャのポインタ
	static D3DXVECTOR2 m_aImageSize[Index(TYPE::Max)];       // テクスチャサイズ

	int m_nAnimationCount;              // アニメーションカウント
	float m_aniLastTime;                // アニメーション更新タイム

	TYPE m_type;                       // タイプ
};