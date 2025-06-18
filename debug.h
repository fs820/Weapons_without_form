//------------------------------------------
//
// �f�o�b�O�����̒�` [debug.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "renderer.h"

//------------------------
// �f�o�b�O�N���X
//------------------------
class CDebugProc final
{
// ���J
public:
	// �\���ؑ�
	using MODE = enum
	{
		STATIC = 0,
		WINDOW,
		SYSTEM,
		OBJECT,
		MODE_MAX
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
	static void ChangeDrawMode(void) { m_drawMode = MODE((m_drawMode + 1) % MODE_MAX); };

// ����J
private:
	static LPD3DXFONT m_pFont;        // �t�H���g�|�C���^
	static string m_sDebug[MODE_MAX]; // �\��������
	static D3DXCOLOR m_color;         // �����F
	static UINT m_flag;               // DrawText�ݒ�
	static bool m_bDraw;              // �\���ؑ�
	static MODE m_drawMode;           // �\�����郂�[�h
};