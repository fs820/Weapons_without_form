//------------------------------------------
//
// オブジェクトの処理の定義・宣言[object.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "renderer.h"

//---------------------
// オブジェクトクラス
//---------------------
class CObject
{
// 公開
public:
	// タイプ
	enum class TYPE : Index
	{
		None,       // 無し
		BackGround, // 背景
		Player,     // プレイヤー
		Enemy,      // 敵
		Bullet,     // 弾
		Shadow,    // 影
		Explosion,  // 爆発
		Effect,     // エフェクト
		Score,      // スコア
		Max         // 最大数
	};

	CObject(Index priority = 3) :m_Priority(priority), m_ID(INVALID_ID), m_type{}, m_transform{}, m_bCollision{}, m_bRelease{}, m_bAfterimage{} {} // デフォルト引数によりデフォルトコンストラクタを兼用している
	virtual ~CObject() = default;

	static void UpdateAll(const context::UpdateContext updateContext);
	static void DrawAll(const context::DrawContext drawContext, bool bAfterimage = false);
	static void DrawPriority(Index priority, const context::DrawContext drawContext, bool bAfterimage = false);
	static void ReleaseAll(void);

	static size_t GetObjectAll(void) { return m_nAll; }

	virtual void Uninit(void) = 0;
	virtual void Update(const context::UpdateContext updateContext) = 0;
	virtual void Draw(const context::DrawContext drawContext) = 0;

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

// 家族公開
protected:
	virtual void OnCollision(const CObject& other) = 0;

	void Init(TYPE type);

// 非公開
private:
	static void Release(CObject* pObject);
	static void CallCollision(void);
	static bool CallCollisionHelper(const CObject& HostObject, const CObject& GuestObject);

	static array<vector<CObject*>, 8> m_apObject;   // オブジェクトのポインタ配列
	static int m_nAll;                              // オブジェクトの数

	Index m_Priority;                 // 描画順位
	Index m_ID;                       // オブジェクトのID
	TYPE m_type;                      // オブジェクトの種類
	DirectX::Transform m_transform;   // 位置・回転・スケール
	DirectX::Vector3 m_size;          // サイズ（当たり判定用）
	bool m_bCollision;                // あたり判定の有無
	bool m_bRelease;                  // 破棄フラグ
	bool m_bAfterimage;               // 残像フラグ
};
