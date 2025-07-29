//------------------------------------------
//
// タイマー処理の定義・宣言[time.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "object.h"

class CNumber;

//-----------------------------------------
// タイムクラス (オブジェクト派生)
//-----------------------------------------
class CTime : public CObject
{
	// 公開
public:
	CTime() : m_apNumber{}, m_digits{}, m_space{}, m_fTime{}, m_count{} {}
	CTime(size_t digits, float space, int priority) : CObject(priority), m_apNumber{}, m_digits(digits), m_space(space), m_fTime{}, m_count{} {}
	virtual ~CTime() = default;

	static CTime* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, size_t digits = 5, float space = 0.05f, int priority = 3);
	static CTime* Create(DirectX::Transform transform, size_t digits = 5, float space = 0.05f, int priority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale);
	HRESULT Init(DirectX::Transform transform);
	void Uninit(void) override;
	void Update(const context::UpdateContext updateContext) override;
	void Draw(const context::DrawContext drawContext) override;

	void CountUp(void) { m_count = Count::Up; }
	void CountDown(void) { m_count = Count::Down; }
	void Stop(void) { m_count = Count::None; }

	void SetTime(const float time);
	void AddTime(const float time);
	float GetTime(void) const { return m_fTime; }
	
	// 非公開
private:
	// カウント状態の列挙
	enum class Count : Index8
	{
		None,
		Up,
		Down,
		Max
	};

	void OnCollision(const CObject& other) override {};

	const size_t m_digits;          // 桁数
	const float m_space;            // 数字の間隔
	array<CNumber*, 32> m_apNumber; // 数字
	float m_fTime;                  // 時間
	Count m_count;                  // カウント状態
};