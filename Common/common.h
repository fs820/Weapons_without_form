//-------------------------------
//
// 汎用定数リスト [common.h]
// Author: fuma sato
//
//-------------------------------
#pragma once

// 破棄マクロ (非推奨)
#define SAFE_FREE(p) { if (p != nullptr) { free(p); (p) = nullptr; } }           // ポインタの安全な解放マクロ
#define SAFE_DELETE(p) { if (p != nullptr) { delete (p); (p) = nullptr; } }      // ポインタの安全な削除マクロ
#define SAFE_DELETE_ARRAY(p) { for (auto& item : p) { if (item != nullptr) { delete item; item = nullptr; } } } // 配列ポインタの安全なアンイニットマクロ
#define SAFE_DELETE_ARRAYPTR(p) { if (p != nullptr) { delete[] (p); (p) = nullptr; } } // 配列ポインタの安全な削除マクロ
#define SAFE_UNINIT(p) { if (p != nullptr) { (p)->Uninit(); delete (p); (p) = nullptr; } } // ポインタの安全なアンイニットマクロ
#define SAFE_UNINIT_ARRAY(p) { for (auto& item : p) { if (item != nullptr) { item->Uninit(); delete item; item = nullptr; } } } // 配列ポインタの安全なアンイニットマクロ
#define SAFE_RELEASE(p) { if (p != nullptr) { (p)->Release(); (p) = nullptr; } } // ポインタの安全な解放マクロ
#define SAFE_RELEASE_ARRAY(p) { for (auto& item : p) { if (item != nullptr) { item->Release(); item = nullptr; } } } // 配列ポインタの安全な解放マクロ

namespace common
{
    // 汎用定数
    static constexpr float PI = 3.14159265358979323846f; // 円周率
    static constexpr float EPSILON = 1e-6f;              // 非常に小さい値 (浮動小数点の比較用)

    // 固有フラグ
    constexpr bool OK = true;
    constexpr bool NO = false;
    constexpr bool ON = true;
    constexpr bool OFF = false;

    // Index定義
#if _WIN64
    using Index = unsigned long long;
#else
    using Index = unsigned int;
#endif
    using Index8 = unsigned char;
    using Index16 = unsigned short;
    using Index32 = unsigned int;
    using Index64 = unsigned long long;
    constexpr Index INVALID_ID = Index(-1);
    constexpr Index8 INVALID_ID8 = Index8(-1);
    constexpr Index16 INVALID_ID16 = Index16(-1);
    constexpr Index32 INVALID_ID32 = Index32(-1);
    constexpr Index64 INVALID_ID64 = Index64(-1);

    // 汎用Vectorタグ
    enum class VectorTag { X, Y, Z, W };

    // 汎用左右
    enum class Direction { Left, Right };

    // Uninit() を呼び出してから delete するデリータ
    template <typename T>
    struct UninitDeleter
    {
        void operator()(T* p) const
        {
            if (p)
            {
                p->Uninit();
                delete p;
            }
        }
    };

    // Release() を呼び出すデリータ (deleteはしない)
    template <typename T>
    struct ReleaseDeleter
    {
        void operator()(T* p) const
        {
            if (p)
            {
                p->Release();
            }
        }
    };
}
