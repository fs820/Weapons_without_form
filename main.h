//------------------------------------------
//
//メインの処理の定義・宣言[main.h]
//Author: fuma sato
//
//------------------------------------------
#pragma once

// プロジェクト共通ヘッダ
#define NOMINMAX         // 旧max,min を無効化
#include <windows.h>     // windows標準
#include <algorithm>     // 新min,maxを有効化
#include <chrono>        // 時間
#include <string>        // 文字列
#include <string_view>   // 文字列参照
#include <sstream>       // 文字列操作
#include <bit>           // ビット単位操作
#include <optional>      // エラー対応
#include <list>          // リスト
#include <unordered_map> // 連想(ペア)配列
#include <set>           // 一意の値
#include <filesystem>    // ファイル管理
#include <array>         // 配列

#include "common.h"    // 汎用定義

using namespace std::chrono;     // chrono
using namespace std::filesystem; // ファイルシステム
using std::string;           // 文字列
using std::string_view;      // 文字列参照
using std::istringstream;    // 文字列操作
using std::ostringstream;    // 文字列操作
using std::vector;           // 動的配列 (イテレーション用)
using std::list;             // 動的配列 (削除・追加)
using std::unordered_map;    // 連想(ペア)配列
using std::set;              // 一意の配列
using std::optional;         // エラー対応
using std::array;            // 配列
using std::bit_cast;         // ビット単位のキャスト
using std::clamp;            // clamp
using std::max;              // max
using std::min;              // min
using std::nullopt;          // null

//-------------------------------------------------
// メインのクラス (インスタンス禁止) (継承禁止)
//-------------------------------------------------
class CMain final
{
	// 公開
public:
	CMain() = delete;

	static HWND GethWnd(void) { return m_hWnd; }
	static void SethWnd(HWND hWnd) { m_hWnd = hWnd; }
	static bool IsStop(void) { return m_bStop; }
	static bool IsFullScreen(void) { return m_bFullScreen; }

	static float GetElapsedTime(void) { return m_elapsedTime; }
	static float GetElapsedTimeGameSpeed(void) { return m_elapsedTime * m_gameSpeed; }
	static void SetElapsedTime(float elapsedTime) { m_elapsedTime = elapsedTime; }
	static float GetDeltaTime(void) { return m_deltaTime; }
	static float GetDeltaTimeGameSpeed(void) { return m_deltaTime * m_gameSpeed; }
	static void SetDeltaTime(float deltaTime) { m_deltaTime = deltaTime; }

	static float GetGameSpeed(void) { return m_gameSpeed; }
	static void SetGameSpeed(float gameSpeed) { m_gameSpeed = gameSpeed; }

	static string_view GetFilePath(void);
	static HRESULT GetClientRect(RECT* rect);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void ToggleFullScreen(void);
	static void ToggleBorderless(void);

	static void RegisterStandardInputDevices(void);
	static void RegisterAllInputDevices(void);
	static void DebugPrintDeviceInfo(void);

	static void Error(HRESULT hr, const char func[]);
	static void ConfineCursorToWindow(void);
	static void ReleaseCursor(void);
	static void CursorSwitch(bool Switch);
	static POINT GetCursorPoint(void);

	//--------------------
	// プロジェクト定数
	//--------------------

	// 名前
	static constexpr const char* CLASS_NAME = "WindowClass";        // クラス
	static constexpr const char* WINDOW_NAME = "シューティング";    // ウインドウ

	// ウインドウのサイズ
	//static constexpr float SCREEN_WIDTH = 1280.0f;  // 幅
	//static constexpr float SCREEN_HEIGHT = 720.0f;  // 高さ
	static constexpr float SCREEN_WIDTH = 1920.0f;   // 幅
	static constexpr float SCREEN_HEIGHT = 1080.0f;  // 高さ
	//static constexpr float SCREEN_WIDTH = 2560.0f;  // 幅
	//static constexpr float SCREEN_HEIGHT = 1440.0f; // 高さ

	// フルスクリーンをボーダーレスで表現
	static constexpr bool ISBORDERLESS = true;

	// Fps(1秒間に何回描画するか)
	static constexpr int FPS = 60;

	// 非公開
private:
	//--------------------
	// プロジェクト変数
	//--------------------
	static HWND m_hWnd;         // ウインドウハンドル
	static bool m_bFullScreen;  // フルスクリーンフラグ
	static float m_elapsedTime; // 起動時間
	static float m_deltaTime;   // フレーム時間
	static bool m_bStop;        // ストップフラグ
	static float m_gameSpeed;   // ゲームスピード

};

// >> が使える型かどうか
template<typename T>
concept StreamExtractable = requires(std::istream & is, T & t)
{
	{ is >> t } -> std::same_as<std::istream&>;
};

//-----------------------
// トリム
//-----------------------
inline string_view Trim(string_view sv)
{
	size_t start = sv.find_first_not_of(" \t");
	size_t end = sv.find_last_not_of(" \t");
	return (start == string_view::npos) ? "" : sv.substr(start, end - start + 1);
}

//----------------------------
// tag = val を探して値を返す
//----------------------------
template<StreamExtractable T>
optional<T> StringToData(const string_view source, const string_view tag, const Index index = 0)
{
	size_t pos = 0;
	while (pos < source.size())
	{
		// 行を一行取り出す
		size_t lineEnd = source.find('\n', pos);
		if (lineEnd == string_view::npos) lineEnd = source.size();

		string_view line = source.substr(pos, lineEnd - pos);
		pos = lineEnd + 1;

		// コメントを無視（#以降）
		size_t hashPos = line.find('#');
		if (hashPos != string_view::npos)
			line = line.substr(0, hashPos);

		// 探すフラグの位置
		size_t tagPos = line.find(tag);
		while (tagPos != string_view::npos)
		{
			bool leftOK = (tagPos == 0 || isspace(line[tagPos - 1]));
			bool rightOK = (tagPos + tag.size() == line.size() || isspace(line[tagPos + tag.size()]) || line[tagPos + tag.size()] == '=');

			if (leftOK && rightOK)
			{
				// '=' を探す
				size_t eqPos = line.find('=', tagPos + tag.size());
				if (eqPos == string_view::npos) break;

				string_view valPart = line.substr(eqPos + 1);
				valPart = Trim(valPart);

				// 空白区切りで index 番目の値を取得
				istringstream iss{ string(valPart) };
				string token;
				size_t currentIndex = 0;

				while (iss >> token)
				{
					if (currentIndex == index)
					{
						istringstream valueStream(token);
						T value;
						valueStream >> value;
						if (!valueStream.fail())
							return value;
						else
							return nullopt;
					}
					++currentIndex;
				}
				return nullopt; // indexが範囲外
			}
			tagPos = line.find(tag, tagPos + 1);
		}
	}
	return nullopt;
}

//-----------------------
// 文字列の指定範囲を抜き出す関数
//-----------------------
inline optional<string_view> ExtractBlock(const string_view source, const string_view tag, const string_view endtag, const Index index)
{
	size_t pos = 0;
	size_t currentIndex = 0;

	while (pos < source.size())
	{
		// 開始タグを探す
		size_t startPos = source.find(tag, pos);
		if (startPos == string_view::npos) break;

		// 終了タグを探す（開始より後）
		size_t endPos = source.find(endtag, startPos + tag.size());
		if (endPos == string_view::npos) break;

		endPos += endtag.size(); // 終了タグも含める

		// 必要な index に到達したらそのブロックを返す
		if (currentIndex == index)
		{
			// コメント行や無関係な行を含めたくない場合、さらに行調整してもOK
			return source.substr(startPos, endPos - startPos);
		}

		++currentIndex;
		pos = endPos;
	}

	return nullopt;
}