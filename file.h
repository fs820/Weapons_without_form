//------------------------------------------
//
// ファイルの処理の定義 [file.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include <fstream>     // ファイル入出力
#include <sstream>     // stringstream

using std::fstream;                        // ファイルIO
using std::ifstream;                       // ファイルI
using std::ofstream;                       // ファイルO
using std::stringstream;                   // 文字列とファイルの連結
using std::ios;                            // ios
using std::streamsize;                     // サイズ
using std::istreambuf_iterator;            // インプットイテレータ
using std::streamoff;					   // ストリームオフセット

using std::bit_cast;                     // ビットキャスト

//----------------------
// ファイルクラス
//----------------------
class CFile
{
// 公開
public:
	CFile() : defaultPath{} {};
	explicit CFile(path filePath) : defaultPath(filePath) {};
    ~CFile() = default;

	void ChangeFile(const path filePath) { defaultPath = filePath; };

	template<typename T>
	requires std::is_trivially_copyable_v<T>
	T ReadBinary(streamoff offSet = 0);
	template<typename T>
	requires std::is_trivially_copyable_v<T>
	bool WriteBinary(const T& data);
	template<typename T>
	requires std::is_trivially_copyable_v<T>
	bool AddWriteBinary(const T& data);

	string ReadText(void);
	string ReadTextLine(size_t lineNumber);
	vector<string> ReadTextLines(void);
	bool WriteText(const string_view content);
	bool WriteTextLines(const vector<string_view> lines);
	bool AddWriteText(const string_view content);
	bool AddWriteTextLines(const vector<string_view> lines);

	unordered_map<string, string> ReadJson(void);
	bool WriteJson(const unordered_map<string, string>& map);

	bool Exists(void);
	size_t GetFileSize(void);
	string GetExtension(void);

	template<typename T>
	requires std::is_trivially_copyable_v<T>
	T ReadBinary(const path filePath, streamoff offSet = 0);
	template<typename T>
	requires std::is_trivially_copyable_v<T>
    bool WriteBinary(const path filePath, const T& data);
	template<typename T>
	requires std::is_trivially_copyable_v<T>
    bool AddWriteBinary(const path filePath, const T& data);

    string ReadText(const path filePath);
	string ReadTextLine(const path filePath, size_t lineNumber);
    vector<string> ReadTextLines(const path filePath);
    bool WriteText(const path filePath, const string_view content);
    bool WriteTextLines(const path filePath, const vector<string_view> lines);
    bool AddWriteText(const path filePath, const string_view content);
    bool AddWriteTextLines(const path filePath, const vector<string_view> lines);

    unordered_map<string, string> ReadJson(const path filePath);
    bool WriteJson(const path filePath, const unordered_map<string, string>& map);

    bool Exists(const path filePath);
    size_t GetFileSize(const path filePath);
    string GetExtension(const path filePath);

// 非公開
private:
	path defaultPath; // ファイルパス
};

//------------------------------
// バイナリファイル読み込み
//------------------------------
template<typename T>
requires std::is_trivially_copyable_v<T>
T CFile::ReadBinary(streamoff offSet)
{
	// インプットファイル作成 (バイナリ)
	ifstream file(defaultPath, ios::binary);

	if (!file.is_open())
	{// 開けない
		return {};
	}

	// オフセットにシーク
	file.seekg(offSet, ios::beg);
	if (!file.good())
	{// シーク失敗
		return {};
	}

	// 読み取り
	std::array<uint8_t, sizeof(T)> buffer{};
	file.read(reinterpret_cast<char*>(buffer.data()), sizeof(T));

	if (file.gcount() != sizeof(T))
	{// サイズ不足
		return {};
	}

	// T型に変換して返す
	return std::bit_cast<T>(buffer);
}

//----------------------------------------------
// バイナリファイル書き込み
//----------------------------------------------
template<typename T>
requires std::is_trivially_copyable_v<T>
bool CFile::WriteBinary(const T& data)
{
	// なかったらディレクトリを作成
	if (defaultPath.has_parent_path())
	{
		create_directories(defaultPath.parent_path());
	}

	// アウトプットファイル作成（バイナリ）
	ofstream file(defaultPath, ios::binary);

	if (!file.is_open())
	{// 開けない
		return false;
	}

	// 書き込み
	auto bitData = bit_cast<array<uint8_t, sizeof(T)>>(data);
	file.write(reinterpret_cast<const char*>(bitData.data()), sizeof(T));

	// 成功を返す
	return file.good();
}

//----------------------------
// バイナリファイル追加書き込み
//----------------------------
template<typename T>
requires std::is_trivially_copyable_v<T>
bool CFile::AddWriteBinary(const T& data)
{
	// アウトプットファイル作成（バイナリ,追求）
	ofstream file(defaultPath, ios::binary | ios::app);

	if (!file.is_open())
	{// 開けない
		return false;
	}

	// 書き込み
	auto bitData = bit_cast<array<uint8_t, sizeof(T)>>(data);
	file.write(reinterpret_cast<const char*>(bitData.data()), sizeof(T));

	// 成功を返す
	return file.good();
}

//------------------------------
// バイナリファイル読み込み
//------------------------------
template<typename T>
requires std::is_trivially_copyable_v<T>
T CFile::ReadBinary(const path filePath, streamoff offSet)
{
	// インプットファイル作成 (バイナリ)
	ifstream file(filePath, ios::binary);

	if (!file.is_open())
	{// 開けない
		return {};
	}

	// オフセットにシーク
	file.seekg(offSet, ios::beg);
	if (!file.good())
	{// シーク失敗
		return {};
	}

	// 読み取り
	std::array<uint8_t, sizeof(T)> buffer{};
	file.read(reinterpret_cast<char*>(buffer.data()), sizeof(T));

	if (file.gcount() != sizeof(T))
	{// サイズ不足
		return {};
	}

	// T型に変換して返す
	return std::bit_cast<T>(buffer);
}

//----------------------------------------------
// バイナリファイル書き込み
//----------------------------------------------
template<typename T>
requires std::is_trivially_copyable_v<T>
bool CFile::WriteBinary(const path filePath, const T& data)
{
	// なかったらディレクトリを作成
	if (filePath.has_parent_path())
	{
		create_directories(filePath.parent_path());
	}

	// アウトプットファイル作成（バイナリ）
	ofstream file(filePath, ios::binary);

	if (!file.is_open())
	{// 開けない
		return false;
	}

	// 書き込み
	auto bitData = bit_cast<array<uint8_t, sizeof(T)>>(data);
	file.write(reinterpret_cast<const char*>(bitData.data()), sizeof(T));

	// 成功を返す
	return file.good();
}

//----------------------------
// バイナリファイル追加書き込み
//----------------------------
template<typename T>
requires std::is_trivially_copyable_v<T>
bool CFile::AddWriteBinary(const path filePath, const T& data)
{
	// アウトプットファイル作成（バイナリ,追求）
	ofstream file(defaultPath, ios::binary | ios::app);

	if (!file.is_open())
	{// 開けない
		return false;
	}

	// 書き込み
	auto bitData = bit_cast<array<uint8_t, sizeof(T)>>(data);
	file.write(reinterpret_cast<const char*>(bitData.data()), sizeof(T));

	// 成功を返す
	return file.good();
}