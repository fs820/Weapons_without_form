//---------------------------------------
//
// ライト処理の定義 [light.h]
// Author: fuma sato
//
//---------------------------------------
#pragma once
#include "main.h"
#include "renderer.h"

//-----------------
// ライトクラス
//-----------------
class CLight
{
// 公開
public:
	void Init(void);                //初期化処理
	void Uninit(void);              //終了処理
	void Update(void);              //更新処理
	size_t Set(D3DLIGHT9* pLight);  //ライト設定

// 非公開
private:
	static constexpr size_t MAX_LIGHT = 4; // ライトの最大数

	D3DLIGHT9 m_light[MAX_LIGHT]; // ライト情報
};