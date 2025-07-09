//------------------------------------------
//
// デバッグ処理の定義 [debug.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "renderer.h"

//------------------------
// デバッグクラス
//------------------------
class CDebugProc final
{
// 公開
public:
	// 表示切替
	enum class MODE
	{
		Static,
		Window,
		System,
		Object,
		Max
	};

	CDebugProc() = delete;

	static HRESULT Init(void);
	static void Uninit(void);
    static void Draw(void);

	static void  Print(const char* format, ...);
	static void  Print(const MODE mode, const char* format, ...);

	static void SetColor(D3DXCOLOR color) { m_color = color; };
	static void SetDebugOp(UINT flag) { m_flag = flag; }

	static void ToggleDebugDraw(void) { m_bDraw = !m_bDraw; };
	static void ChangeDrawMode(void) { m_drawMode = MODE((Index(m_drawMode) + 1) % Index(MODE::Max)); };

// 非公開
private:
	static LPD3DXFONT m_pFont;                        // フォントポインタ
	static array<string, Index8(MODE::Max)> m_sDebug; // 表示文字列
	static D3DXCOLOR m_color;                         // 文字色
	static UINT m_flag;                               // DrawText設定
	static bool m_bDraw;                              // 表示切替
	static MODE m_drawMode;                           // 表示するモード
};