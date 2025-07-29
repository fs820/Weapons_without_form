//------------------------------------------
//
// Numberの処理の定義・宣言[number.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include <Windows.h>

//--------------------------
// 数字 (末端)
//--------------------------
class CGui final
{
	// 公開
public:
	CGui() = default;
	~CGui() = default;

	bool Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const { return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam); }

	void Init(void) const;
	void Uninit(void) const;
	void Update(void) const;
	void Draw(void) const;

	void InvalidateDevice(void) const { ImGui_ImplDX9_InvalidateDeviceObjects(); }
	void CreateDevice(void) const { ImGui_ImplDX9_CreateDeviceObjects(); }

// 非公開
private:

};
