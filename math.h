//------------------------------------------
//
// ���l�����������Q�̒�` [math.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once

#include "main.h"
#include <random>
#include <cmath>
#include <d3dx9math.h>

// �����_���}�N��
#define RandomAngle CMath::Random(-D3DX_PI, D3DX_PI)
#define RandomColor D3DXCOLOR(CMath::Random(0.0f, 1.0f),CMath::Random(0.0f, 1.0f),CMath::Random(0.0f, 1.0f),CMath::Random(0.0f, 1.0f))
#define RandomColorTranslucent D3DXCOLOR(CMath::Random(0.0f, 1.0f),CMath::Random(0.0f, 1.0f),CMath::Random(0.0f, 1.0f),CMath::Random(0.0f, 0.5f))
#define RandomColorA(alpha) D3DXCOLOR(CMath::Random(0.0f, 1.0f),CMath::Random(0.0f, 1.0f),CMath::Random(0.0f, 1.0f),alpha)

//------------------------------------------
// ���w�N���X (�C���X�^���X�͎����Ȃ�)
//------------------------------------------
class CMath
{
public:
	CMath() = delete;

	static HRESULT Rotation(D3DXVECTOR3 resultPos[], const D3DXVECTOR2 size, const float fAngle);

	static float NormalizeAngle(float angle);
	static void NormalizeAngle(float* const pAngle);
    static D3DXVECTOR3 NormalizeRot(const D3DXVECTOR3 rot);
    static void NormalizeRot(D3DXVECTOR3* pRot);

    static void NormalizeAngleDeltaAndAdd(float* pDestAngle, float angle, const float coefficient, const float deltaTime);

	static void SpherePos(D3DXVECTOR3* const pOutPos, const D3DXVECTOR3 basePos, const D3DXVECTOR3 rot, const float Length);

	template<typename T>
	static T Random(T min, T max);
	static int RandomInt(int min, int max);
	static float RandomFloat(float min, float max);
private:
	static std::random_device rd;
	static std::mt19937 gen;
};

//-------------------------------------------
// �����𐶐�����
//--------------------------------------------
template<typename T>
T CMath::Random(T min, T max)
{
    if constexpr (std::is_integral<T>::value)
    {
        std::uniform_int_distribution<T> dist(min, max);
        return dist(gen);
    }
    else if constexpr (std::is_floating_point<T>::value)
    {
        std::uniform_real_distribution<T> dist(min, max);
        return dist(gen);
    }
    else if constexpr (std::is_enum<T>::value)
    {
        std::uniform_int_distribution<int> dist(static_cast<int>(min), static_cast<int>(max));
        return static_cast<T>(dist(gen));
    }
    else
    {
        static_assert(std::is_arithmetic<T>::value, "T must be numeric");
        return T{};
    }
}