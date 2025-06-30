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
	static CTime* Create(Transform transform, size_t digits = 5, float space = 0.05f, int priority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale);
	HRESULT Init(Transform transform);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void CountUp(void) { m_count = UP; }
	void CountDown(void) { m_count = DOWN; }
	void Stop(void) { m_count = NONE; }

	void SetTime(const float time);
	void AddTime(const float time);
	float GetTime(void) const { return m_fTime; }

	// 非公開
private:
	// カウント状態の列挙
	using COUNT = enum
	{
		NONE = 0,
		UP,
		DOWN,
		COUNT_MAX
	};

	void OnCollision(const CObject& other) override {};

	const size_t m_digits;   // 桁数
	const float m_space;     // 数字の間隔
	CNumber* m_apNumber[32]; // 数字
	float m_fTime;           // 時間
	COUNT m_count;           // カウント状態
};