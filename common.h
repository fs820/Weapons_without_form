//-------------------------------
//
// 汎用定数リスト [common.h]
// Author: fuma sato
//
//-------------------------------
#pragma once

#define SAFE_FREE(p) { if (p != nullptr) { free(p); (p) = nullptr; } }           // ポインタの安全な解放マクロ
#define SAFE_DELETE(p) { if (p != nullptr) { delete (p); (p) = nullptr; } }      // ポインタの安全な削除マクロ
#define SAFE_DELETE_ARRAY(p) { for (auto& item : p) { if (item != nullptr) { delete item; item = nullptr; } } } // 配列ポインタの安全なアンイニットマクロ
#define SAFE_DELETE_ARRAYPTR(p) { if (p != nullptr) { delete[] (p); (p) = nullptr; } } // 配列ポインタの安全な削除マクロ
#define SAFE_UNINIT(p) { if (p != nullptr) { (p)->Uninit(); delete (p); (p) = nullptr; } } // ポインタの安全なアンイニットマクロ
#define SAFE_UNINIT_ARRAY(p) { for (auto& item : p) { if (item != nullptr) { item->Uninit(); delete item; item = nullptr; } } } // 配列ポインタの安全なアンイニットマクロ
#define SAFE_RELEASE(p) { if (p != nullptr) { (p)->Release(); (p) = nullptr; } } // ポインタの安全な解放マクロ
#define SAFE_RELEASE_ARRAY(p) { for (auto& item : p) { if (item != nullptr) { item->Release(); item = nullptr; } } } // 配列ポインタの安全な解放マクロ

constexpr bool OK = true;
constexpr bool NO = false;
constexpr bool ON = true;
constexpr bool OFF = false;
using Index = unsigned int;
constexpr Index INVALID_ID = Index(-1);

using VECTORTAG = enum { X, Y, Z, W };