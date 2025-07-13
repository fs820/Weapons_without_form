//-------------------------------
//
// �ėp�萔���X�g [common.h]
// Author: fuma sato
//
//-------------------------------
#pragma once

// �j���}�N��
#define SAFE_FREE(p) { if (p != nullptr) { free(p); (p) = nullptr; } }           // �|�C���^�̈��S�ȉ���}�N��
#define SAFE_DELETE(p) { if (p != nullptr) { delete (p); (p) = nullptr; } }      // �|�C���^�̈��S�ȍ폜�}�N��
#define SAFE_DELETE_ARRAY(p) { for (auto& item : p) { if (item != nullptr) { delete item; item = nullptr; } } } // �z��|�C���^�̈��S�ȃA���C�j�b�g�}�N��
#define SAFE_DELETE_ARRAYPTR(p) { if (p != nullptr) { delete[] (p); (p) = nullptr; } } // �z��|�C���^�̈��S�ȍ폜�}�N��
#define SAFE_UNINIT(p) { if (p != nullptr) { (p)->Uninit(); delete (p); (p) = nullptr; } } // �|�C���^�̈��S�ȃA���C�j�b�g�}�N��
#define SAFE_UNINIT_ARRAY(p) { for (auto& item : p) { if (item != nullptr) { item->Uninit(); delete item; item = nullptr; } } } // �z��|�C���^�̈��S�ȃA���C�j�b�g�}�N��
#define SAFE_RELEASE(p) { if (p != nullptr) { (p)->Release(); (p) = nullptr; } } // �|�C���^�̈��S�ȉ���}�N��
#define SAFE_RELEASE_ARRAY(p) { for (auto& item : p) { if (item != nullptr) { item->Release(); item = nullptr; } } } // �z��|�C���^�̈��S�ȉ���}�N��

// �ŗL�t���O
constexpr bool OK = true;
constexpr bool NO = false;
constexpr bool ON = true;
constexpr bool OFF = false;

// Index��`
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

// �ėpVector�^�O
using VECTORTAG = enum { X, Y, Z, W };

// �ėp���E
using LR = enum { Left, Right };