//------------------------------------------
//
// 数値を扱う処理群の定義 [math.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include <random>
#include <cmath>

// ランダムマクロ
#define RandomAngle CMath::Random(-D3DX_PI, D3DX_PI)
#define RandomColor D3DXCOLOR(CMath::Random(0.0f, 1.0f),CMath::Random(0.0f, 1.0f),CMath::Random(0.0f, 1.0f),CMath::Random(0.0f, 1.0f))
#define RandomColorTranslucent D3DXCOLOR(CMath::Random(0.0f, 1.0f),CMath::Random(0.0f, 1.0f),CMath::Random(0.0f, 1.0f),CMath::Random(0.0f, 0.5f))
#define RandomColorA(alpha) D3DXCOLOR(CMath::Random(0.0f, 1.0f),CMath::Random(0.0f, 1.0f),CMath::Random(0.0f, 1.0f),alpha)

namespace
{
    std::random_device rd;
    std::mt19937 gen;
}

namespace math
{
    // 数学定数
    constexpr double PI = 3.14159265358979323846;
    constexpr double TWO_PI = 6.28318530717958647692;
    constexpr double HALF_PI = 1.57079632679489661923;
    constexpr double QUARTER_PI = 0.78539816339744830962;

    inline HRESULT Rotation(D3DXVECTOR3 resultPos[], const D3DXVECTOR2 size, const float fAngle);

    inline float NormalizeAngle(float angle);
    inline void NormalizeAngle(float* const pAngle);
    inline D3DXVECTOR3 NormalizeRot(const D3DXVECTOR3 rot);
    inline void NormalizeRot(D3DXVECTOR3* pRot);

    inline void NormalizeAngleDeltaAndAdd(float* pDestAngle, float angle, const float coefficient, const float deltaTime);

    inline void SpherePos(D3DXVECTOR3* const pOutPos, const D3DXVECTOR3 basePos, const D3DXVECTOR3 rot, const float Length);

    inline int RandomInt(int min, int max);
    inline float RandomFloat(float min, float max);
    inline D3DXVECTOR3 VectorToEuler(const D3DXVECTOR3& direction);

    template<typename T>
    T Random(T min, T max)
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
}
