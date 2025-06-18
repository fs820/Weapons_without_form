//------------------------------------------
//
// デバッグ処理 [debug.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "debug.h"
#include "manager.h"

//------------------------
// 
// デバッグクラス
// 
//------------------------

// 静的メンバ変数
LPD3DXFONT CDebugProc::m_pFont{};                                // フォントポインタ
string CDebugProc::m_sDebug[MODE_MAX]{};                         // 文字列
D3DXCOLOR CDebugProc::m_color{ D3DXCOLOR(1.0f,1.0f,1.0f,1.0f) }; // 色
UINT CDebugProc::m_flag{ DT_LEFT | DT_TOP | DT_WORDBREAK };      // 表示方法
bool CDebugProc::m_bDraw{ true };                                // 辨官するか
CDebugProc::MODE CDebugProc::m_drawMode{};                       // 表示するモード

//---------------------
// 初期化
//---------------------
HRESULT CDebugProc::Init(void)
{
	// フォントの生成
	return D3DXCreateFont
	(
		CManager::GetRenderer().GetDevice(),
		32, 0,                        // 高さ32 幅は高さに合わせる
		FW_BOLD,                      // 太字
		1,                            // ミップマップ数 1でいい
		FALSE,                        // 斜体文字
		SHIFTJIS_CHARSET,             // 日本語環境
		OUT_TT_PRECIS,                // TrueTypeフォント優先
		CLEARTYPE_QUALITY,            // アンチエイリアス有効
		DEFAULT_PITCH | FF_DONTCARE,  // 通常
		"Yu Gothic",                  // 書体
		&m_pFont
	);
}

//---------------------
// 終了
//---------------------
void CDebugProc::Uninit(void)
{
	// フォント破棄
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
}

//---------------------
// 描画
//---------------------
void CDebugProc::Draw(void)
{
	if (m_bDraw)
	{// 表示状態
		D3DXVECTOR2 screenSize{};
		CManager::GetRenderer().GetDxScreenSize(&screenSize);
		RECT rect{ 0,0,LONG(screenSize.x),LONG(screenSize.y) };

		m_pFont->DrawText(nullptr, m_sDebug[m_drawMode].c_str(), -1, &rect, m_flag, D3DCOLOR_COLORVALUE(m_color.r, m_color.g, m_color.b, m_color.a));
	}

	for (size_t cntMode = WINDOW; cntMode < MODE_MAX; cntMode++)
	{
		m_sDebug[cntMode].clear(); // 文字列をクリア
	}
}

////---------------------
//// 文字列追加
////---------------------
//void CDebugProc::Print(const char* format, ...)
//{
//	va_list args{}; // 可変引数のアドレスを見るためのcharポインタ
//	va_start(args, format); // ポインタの位置を可変引数の始まりへ
//
//	while (*format != '\0')
//	{// 文字列が終わるまでループ
//		if (*format == '%')
//		{// %があった
//			format++; // その次に行く
//			switch (*format)
//			{// 型の判別
//			case 'd': // int
//				m_sDebug += std::to_string_view(va_arg(args, int)); // argsの現時点の位置の引数をintとしてとる
//				break;
//			case 'f': // double
//				m_sDebug += std::to_string_view(va_arg(args, double)); // argsの現時点の位置の引数をdoubleとしてとる
//				break;
//			case 's': // string_view
//				m_sDebug += va_arg(args, char*); // argsの現時点の位置の引数を文字列としてとる
//				break;
//			}
//		}
//		else
//		{// 普通の文字
//			m_sDebug += *format; // 追加
//		}
//		format++; // 次の文字へ
//	}
//
//	va_end(args); // ポインタ解放
//}

//---------------------
// 文字列追加
//---------------------
void CDebugProc::Print(const char* format, ...)
{
	// エラー処理
	if (format == nullptr)
	{// 無効なformat
		for (size_t cntMode = WINDOW; cntMode < MODE_MAX; cntMode++)
		{// STATIC以外の文字列に適応する
			m_sDebug[cntMode] += "無効なformat\n";
		}
		return;
	}

	va_list args{};         // 可変引数のアドレスを見るためのcharポインタ
	va_start(args, format); // ポインタの位置を可変引数の始まりへ

	// 事前に長さを取得する
	va_list checkArgs{};                                       // 長さ取得用のコピー
	va_copy(checkArgs, args);                                  // コピー
	int formatSize = vsnprintf(nullptr, 0, format, checkArgs); // フォーマットの長さを取得
	va_end(checkArgs);                                         // ポインタ開放
	
	// エラー処理
	if (formatSize < 0)
	{// 無効なサイズ
		va_end(args);
		for (size_t cntMode = WINDOW; cntMode < MODE_MAX; cntMode++)
		{// STATIC以外の文字列に適応する
			m_sDebug[cntMode] += "無効なformat\n";
		}
		return;
	}
	else if (formatSize == 0)
	{// formatが空
		va_end(args);
		for (size_t cntMode = WINDOW; cntMode < MODE_MAX; cntMode++)
		{// STATIC以外の文字列に適応する
			m_sDebug[cntMode] += "\n";
		}
		return;
	}

	for (size_t cntMode = WINDOW; cntMode < MODE_MAX; cntMode++)
	{// STATIC以外の文字列に適応する
		// 文字列に追加スペースを用意する
		size_t oldSize = m_sDebug[cntMode].size();
		m_sDebug[cntMode].resize(oldSize + formatSize + 1);

		// 文字列に追加
		vsnprintf(&m_sDebug[cntMode][oldSize], formatSize + 1, format, args); // 文字列を追加
		m_sDebug[cntMode][oldSize + formatSize] = '\n';                       // nullを改行に上書き
	}
	va_end(args);                                                        // ポインタ解放
}

//---------------------
// 文字列追加
//---------------------
void CDebugProc::Print(const MODE mode, const char* format, ...)
{
	// エラー処理
	if (format == nullptr)
	{// 無効なformat
		m_sDebug[mode] += "無効なformat\n";
		return;
	}

	va_list args{};         // 可変引数のアドレスを見るためのcharポインタ
	va_start(args, format); // ポインタの位置を可変引数の始まりへ

	// 事前に長さを取得する
	va_list checkArgs{};                                       // 長さ取得用のコピー
	va_copy(checkArgs, args);                                  // コピー
	int formatSize = vsnprintf(nullptr, 0, format, checkArgs); // フォーマットの長さを取得
	va_end(checkArgs);                                         // ポインタ開放

	// エラー処理
	if (formatSize < 0)
	{// 無効なサイズ
		va_end(args);
		m_sDebug[mode] += "無効なformat\n";
		return;
	}
	else if (formatSize == 0)
	{// formatが空
		va_end(args);
		m_sDebug[mode] += "\n";
		return;
	}

	// 文字列に追加スペースを用意する
	size_t oldSize = m_sDebug[mode].size();
	m_sDebug[mode].resize(oldSize + formatSize + 1);

	// 文字列に追加
	vsnprintf(&m_sDebug[mode][oldSize], formatSize + 1, format, args); // 文字列を追加
	m_sDebug[mode][oldSize + formatSize] = '\n';                       // nullを改行に上書き
	va_end(args);                                                      // ポインタ解放
}