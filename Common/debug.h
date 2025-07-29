//------------------------------------------
//
// デバッグ処理の定義 [debug.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include <array>
#include <string>
#include "common.h"

//------------------------
// デバッグクラス
//------------------------
class CDebugProc final
{
// 公開
public:
	// 表示切替
	enum class MODE : common::Index8
	{
		Static,
		Window,
		System,
		Object,
		Max
	};

    CDebugProc() : m_sDebug{}, m_bDraw{}, m_drawMode{} {}
    ~CDebugProc() = default;

    CDebugProc(const CDebugProc&) = delete;
    CDebugProc& operator=(const CDebugProc&) = delete;
    CDebugProc(const CDebugProc&&) = default;
    CDebugProc& operator=(const CDebugProc&&) = default;

	bool Init(void);
	void Uninit(void);
    void Draw(void);

	void  Print(const char* format, ...);
	void  Print(const MODE mode, const char* format, ...);

	void ToggleDebugDraw(void) { m_bDraw = !m_bDraw; };
	void ChangeDrawMode(void) { m_drawMode = MODE((common::Index8(m_drawMode) + 1) % common::Index8(MODE::Max)); };

// 非公開
private:
    std::array<std::string, common::Index8(MODE::Max)> m_sDebug; // 表示文字列
    bool m_bDraw;                                                // 表示切替
    MODE m_drawMode;                                             // 表示するモード
};
