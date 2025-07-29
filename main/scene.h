//------------------------------------------
//
// シーンの定義 [scene.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "object.h"

namespace scene
{
	// シーンモード
	enum class MODE : Index8
	{
		Title,      // タイトル
		Game,       // ゲーム
		Result,     // リザルド
		Max         // 最大数
	};
}

//-----------------------------
// シーンクラス
//-----------------------------
class CScene : public CObject
{
// 公開
public:
	CScene() : m_mode{} {}
	~CScene() = default;

	static CScene* Create(scene::MODE mode);
	virtual HRESULT Init(void) = 0;

	scene::MODE GetMode(void) const { return m_mode; }
// 非公開
private:
	scene::MODE m_mode;
};
