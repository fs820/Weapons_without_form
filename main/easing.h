//---------------------------------------
//
//イージング処理の定義・宣言[easing.h]
//Author null
//
//---------------------------------------
#pragma once
#include <cmath> // 計算関数使用

//---------------------------------------
// double型のイージング処理の名前空間
//---------------------------------------
namespace easing
{
    // 数学定数
    inline constexpr double PI = 3.14159265358979323846;
    inline constexpr double TWO_PI = 6.28318530717958647692;
    inline constexpr double HALF_PI = 1.57079632679489661923;
	inline constexpr double QUARTER_PI = 0.78539816339744830962;

    // Linear
    inline constexpr double easeLinear(double x)
    {
        return x;
    }

    // Sine
    inline constexpr double easeInSine(double x)
    {
        return 1 - std::cos((x * PI) / 2);
    }

    inline constexpr double easeOutSine(double x)
    {
        return std::sin((x * PI) / 2);
    }

    inline constexpr double easeInOutSine(double x)
    {
        return -(std::cos(PI * x) - 1) / 2;
    }

    // Quad
    inline constexpr double easeInQuad(double x)
    {
        return x * x;
    }

    inline constexpr double easeOutQuad(double x)
    {
        return 1 - (1 - x) * (1 - x);
    }

    inline constexpr double easeInOutQuad(double x)
    {
        return x < 0.5 ? 2 * x * x : 1 - std::pow(-2 * x + 2, 2) / 2;
    }

    // Cubic
    inline constexpr double easeInCubic(double x)
    {
        return x * x * x;
    }

    inline constexpr double easeOutCubic(double x)
    {
        return 1 - std::pow(1 - x, 3);
    }

    inline constexpr double easeInOutCubic(double x)
    {
        return x < 0.5 ? 4 * x * x * x : 1 - std::pow(-2 * x + 2, 3) / 2;
    }

    // Quart
    inline constexpr double easeInQuart(double x)
    {
        return x * x * x * x;
    }

    inline constexpr double easeOutQuart(double x)
    {
        return 1 - std::pow(1 - x, 4);
    }

    inline constexpr double easeInOutQuart(double x)
    {
        return x < 0.5 ? 8 * x * x * x * x : 1 - std::pow(-2 * x + 2, 4) / 2;
    }

    // Quint
    inline constexpr double easeInQuint(double x)
    {
        return x * x * x * x * x;
    }

    inline constexpr double easeOutQuint(double x)
    {
        return 1 - std::pow(1 - x, 5);
    }

    inline constexpr double easeInOutQuint(double x)
    {
        return x < 0.5 ? 16 * x * x * x * x * x : 1 - std::pow(-2 * x + 2, 5) / 2;
    }

    // Expo
    inline constexpr double easeInExpo(double x)
    {
        return x == 0 ? 0 : std::pow(2, 10 * x - 10);
    }

    inline constexpr double easeOutExpo(double x)
    {
        return x == 1 ? 1 : 1 - std::pow(2, -10 * x);
    }

    inline constexpr double easeInOutExpo(double x)
    {
        return x == 0
            ? 0
            : x == 1
            ? 1
            : x < 0.5 ? std::pow(2, 20 * x - 10) / 2
            : (2 - std::pow(2, -20 * x + 10)) / 2;
    }

    // Circ
    inline constexpr double easeInCirc(double x)
    {
        return 1 - std::sqrt(1 - std::pow(x, 2));
    }

    inline constexpr double easeOutCirc(double x)
    {
        return std::sqrt(1 - std::pow(x - 1, 2));
    }

    inline constexpr double easeInOutCirc(double x)
    {
        return x < 0.5
            ? (1 - std::sqrt(1 - std::pow(2 * x, 2))) / 2
            : (std::sqrt(1 - std::pow(-2 * x + 2, 2)) + 1) / 2;
    }

    // Back
    inline constexpr double easeInBack(double x)
    {
        const double c1 = 1.70158;
        const double c3 = c1 + 1;

        return c3 * x * x * x - c1 * x * x;
    }

    inline constexpr double easeOutBack(double x)
    {
        const double c1 = 1.70158;
        const double c3 = c1 + 1;

        return 1 + c3 * std::pow(x - 1, 3) + c1 * std::pow(x - 1, 2);
    }

    inline constexpr double easeInOutBack(double x)
    {
        const double c1 = 1.70158;
        const double c2 = c1 * 1.525;

        return x < 0.5
            ? (std::pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
            : (std::pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
    }

    // Elastic
    inline constexpr double easeInElastic(double x)
    {
        const double c4 = (2 * PI) / 3;

        return x == 0
            ? 0
            : x == 1
            ? 1
            : -std::pow(2, 10 * x - 10) * std::sin((x * 10 - 10.75) * c4);
    }

    inline constexpr double easeOutElastic(double x)
    {
        const double c4 = (2 * PI) / 3;

        return x == 0
            ? 0
            : x == 1
            ? 1
            : std::pow(2, -10 * x) * std::sin((x * 10 - 0.75) * c4) + 1;
    }

    inline constexpr double easeInOutElastic(double x)
    {
        const double c5 = (2 * PI) / 4.5;

        return x == 0
            ? 0
            : x == 1
            ? 1
            : x < 0.5
            ? -(std::pow(2, 20 * x - 10) * std::sin((20 * x - 11.125) * c5)) / 2
            : (std::pow(2, -20 * x + 10) * std::sin((20 * x - 11.125) * c5)) / 2 + 1;
    }

    // Bounce
    inline constexpr double easeOutBounce(double x)
    {
        const double n1 = 7.5625;
        const double d1 = 2.75;

        if (x < 1 / d1)
        {
            return n1 * x * x;
        }
        else if (x < 2 / d1)
        {
            return n1 * (x -= 1.5 / d1) * x + 0.75;
        }
        else if (x < 2.5 / d1)
        {
            return n1 * (x -= 2.25 / d1) * x + 0.9375;
        }
        else
        {
            return n1 * (x -= 2.625 / d1) * x + 0.984375;
        }
    }

    inline constexpr double easeInBounce(double x)
    {
        return 1 - easeOutBounce(1 - x);
    }

    inline constexpr double easeInOutBounce(double x)
    {
        return x < 0.5
            ? (1 - easeOutBounce(1 - 2 * x)) / 2
            : (1 + easeOutBounce(2 * x - 1)) / 2;
    }
}

//---------------------------------------
// float型のイージング処理の名前空間
//---------------------------------------
namespace easingf
{
    // 数学定数
    inline constexpr float PI = 3.14159265358979323846f;
    inline constexpr float TWO_PI = 6.28318530717958647692f;
    inline constexpr float HALF_PI = 1.57079632679489661923f;
    inline constexpr float QUARTER_PI = 0.78539816339744830962f;

    // Linear
    inline constexpr float easeLinear(float x)
    {
        return x;
    }

    // Sine
    inline constexpr float easeInSine(float x)
    {
        return 1.0f - std::cosf((x * PI) / 2.0f);
    }

    inline constexpr float easeOutSine(float x)
    {
        return std::sinf((x * PI) / 2.0f);
    }

    inline constexpr float easeInOutSine(float x)
    {
        return -(std::cosf(PI * x) - 1.0f) / 2.0f;
    }

    // Quad
    inline constexpr float easeInQuad(float x)
    {
        return x * x;
    }

    inline constexpr float easeOutQuad(float x)
    {
        return 1.0f - (1.0f - x) * (1.0f - x);
    }

    inline constexpr float easeInOutQuad(float x)
    {
        return x < 0.5f ? 2.0f * x * x : 1.0f - std::powf(-2.0f * x + 2.0f, 2.0f) / 2.0f;
    }

    // Cubic
    inline constexpr float easeInCubic(float x)
    {
        return x * x * x;
    }

    inline constexpr float easeOutCubic(float x)
    {
        return 1.0f - std::powf(1.0f - x, 3.0f);
    }

    inline constexpr float easeInOutCubic(float x)
    {
        return x < 0.5f ? 4.0f * x * x * x : 1.0f - std::powf(-2.0f * x + 2.0f, 3.0f) / 2.0f;
    }

    // Quart
    inline constexpr float easeInQuart(float x)
    {
        return x * x * x * x;
    }

    inline constexpr float easeOutQuart(float x)
    {
        return 1.0f - std::powf(1.0f - x, 4.0f);
    }

    inline constexpr float easeInOutQuart(float x)
    {
        return x < 0.5f ? 8.0f * x * x * x * x : 1.0f - std::powf(-2.0f * x + 2.0f, 4.0f) / 2.0f;
    }

    // Quint
    inline constexpr float easeInQuint(float x)
    {
        return x * x * x * x * x;
    }

    inline constexpr float easeOutQuint(float x)
    {
        return 1.0f - std::powf(1.0f - x, 5.0f);
    }

    inline constexpr float easeInOutQuint(float x)
    {
        return x < 0.5f ? 16.0f * x * x * x * x * x : 1.0f - std::powf(-2.0f * x + 2.0f, 5.0f) / 2.0f;
    }

    // Expo
    inline constexpr float easeInExpo(float x)
    {
        return x == 0.0f ? 0.0f : std::powf(2.0f, 10.0f * x - 10.0f);
    }

    inline constexpr float easeOutExpo(float x)
    {
        return x == 1.0f ? 1.0f : 1.0f - std::powf(2.0f, -10.0f * x);
    }

    inline constexpr float easeInOutExpo(float x)
    {
        return x == 0.0f
            ? 0.0f
            : x == 1.0f
            ? 1.0f
            : x < 0.5f ? std::powf(2.0f, 20.0f * x - 10.0f) / 2.0f
            : (2.0f - std::powf(2.0f, -20.0f * x + 10.0f)) / 2.0f;
    }

    // Circ
    inline constexpr float easeInCirc(float x)
    {
        return 1.0f - std::sqrtf(1.0f - std::powf(x, 2.0f));
    }

    inline constexpr float easeOutCirc(float x)
    {
        return std::sqrtf(1.0f - std::powf(x - 1.0f, 2.0f));
    }

    inline constexpr float easeInOutCirc(float x)
    {
        return x < 0.5f
            ? (1.0f - std::sqrtf(1.0f - std::powf(2.0f * x, 2.0f))) / 2.0f
            : (std::sqrtf(1.0f - std::powf(-2.0f * x + 2.0f, 2.0f)) + 1.0f) / 2.0f;
    }

    // Back
    inline constexpr float easeInBack(float x)
    {
        const float c1 = 1.70158f;
        const float c3 = c1 + 1.0f;

        return c3 * x * x * x - c1 * x * x;
    }

    inline constexpr float easeOutBack(float x)
    {
        const float c1 = 1.70158f;
        const float c3 = c1 + 1.0f;

        return 1.0f + c3 * std::powf(x - 1.0f, 3.0f) + c1 * std::powf(x - 1.0f, 2.0f);
    }

    inline constexpr float easeInOutBack(float x)
    {
        const float c1 = 1.70158f;
        const float c2 = c1 * 1.525f;

        return x < 0.5f
            ? (std::powf(2.0f * x, 2.0f) * ((c2 + 1.0f) * 2.0f * x - c2)) / 2.0f
            : (std::powf(2.0f * x - 2.0f, 2.0f) * ((c2 + 1.0f) * (x * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
    }

    // Elastic
    inline constexpr float easeInElastic(float x)
    {
        const float c4 = (2.0f * PI) / 3.0f;

        return x == 0.0f
            ? 0.0f
            : x == 1.0f
            ? 1.0f
            : -std::powf(2.0f, 10.0f * x - 10.0f) * std::sinf((x * 10.0f - 10.75f) * c4);
    }

    inline constexpr float easeOutElastic(float x)
    {
        const float c4 = (2.0f * PI) / 3.0f;

        return x == 0.0f
            ? 0.0f
            : x == 1.0f
            ? 1.0f
            : std::powf(2.0f, -10.0f * x) * std::sinf((x * 10.0f - 0.75f) * c4) + 1.0f;
    }

    inline constexpr float easeInOutElastic(float x)
    {
        const float c5 = (2.0f * PI) / 4.5f;

        return x == 0.0f
            ? 0.0f
            : x == 1.0f
            ? 1.0f
            : x < 0.5f
            ? -(std::powf(2.0f, 20.0f * x - 10.0f) * std::sinf((20.0f * x - 11.125f) * c5)) / 2.0f
            : (std::powf(2.0f, -20.0f * x + 10.0f) * std::sinf((20.0f * x - 11.125f) * c5)) / 2.0f + 1.0f;
    }

    // Bounce
    inline constexpr float easeOutBounce(float x)
    {
        const float n1 = 7.5625f;
        const float d1 = 2.75f;

        if (x < 1.0f / d1)
        {
            return n1 * x * x;
        }
        else if (x < 2.0f / d1)
        {
            return n1 * (x -= 1.5f / d1) * x + 0.75f;
        }
        else if (x < 2.5f / d1)
        {
            return n1 * (x -= 2.25f / d1) * x + 0.9375f;
        }
        else
        {
            return n1 * (x -= 2.625f / d1) * x + 0.984375f;
        }
    }

    inline constexpr float easeInBounce(float x)
    {
        return 1.0f - easeOutBounce(1.0f - x);
    }

    inline constexpr float easeInOutBounce(float x)
    {
        return x < 0.5f
            ? (1.0f - easeOutBounce(1.0f - 2.0f * x)) / 2.0f
            : (1.0f + easeOutBounce(2.0f * x - 1.0f)) / 2.0f;
    }
}