//------------------------------------------
//
// ファイルの処理の定義 [file.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include <fstream>       // ファイル入出力
#include <sstream>       // stringstream
#include <string>        // string
#include <string_view>   // string_view
#include <vector>        // vector
#include <unordered_map> // unordered_map
#include <optional>      // optional
#include <filesystem>    // filesystem
#include <bit>           // bit
#include <yaml-cpp/yaml.h> // yaml-cpp

//----------------------
// ファイルクラス
//----------------------
class CFile
{
// 公開
public:
	CFile() : defaultPath{} {};
	explicit CFile(std::filesystem::path filePath) : defaultPath(filePath) {};
    ~CFile() = default;

	void ChangeFile(const std::filesystem::path filePath) { defaultPath = filePath; };

	template<typename T>
	requires std::is_trivially_copyable_v<T>
	std::optional<T> ReadBinary(std::streamoff offSet = 0);
	template<typename T>
	requires std::is_trivially_copyable_v<T>
	bool WriteBinary(const T& data);
	template<typename T>
	requires std::is_trivially_copyable_v<T>
	bool AddWriteBinary(const T& data);

    std::string ReadText(void);
    std::string ReadTextLine(size_t lineNumber);
    std::vector<std::string> ReadTextLines(void);
	bool WriteText(const std::string_view content);
	bool WriteTextLines(const std::vector<std::string_view> lines);
	bool AddWriteText(const std::string_view content);
	bool AddWriteTextLines(const std::vector<std::string_view> lines);

    std::unordered_map<std::string, std::string> ReadJson(void);
	bool WriteJson(const std::unordered_map<std::string, std::string>& map);

    YAML::Node ReadYaml(const std::string_view nodeName);

	bool Exists(void);
	size_t GetFileSize(void);
    std::string GetExtension(void);

	template<typename T>
	requires std::is_trivially_copyable_v<T>
    std::optional<T> ReadBinary(const std::filesystem::path filePath, std::streamoff offSet = 0);
	template<typename T>
	requires std::is_trivially_copyable_v<T>
    bool WriteBinary(const std::filesystem::path filePath, const T& data);
	template<typename T>
	requires std::is_trivially_copyable_v<T>
    bool AddWriteBinary(const std::filesystem::path filePath, const T& data);

    std::string ReadText(const std::filesystem::path filePath);
    std::string ReadTextLine(const std::filesystem::path filePath, size_t lineNumber);
    std::vector<std::string> ReadTextLines(const std::filesystem::path filePath);
    bool WriteText(const std::filesystem::path filePath, const std::string_view content);
    bool WriteTextLines(const std::filesystem::path filePath, const std::vector<std::string_view> lines);
    bool AddWriteText(const std::filesystem::path filePath, const std::string_view content);
    bool AddWriteTextLines(const std::filesystem::path filePath, const std::vector<std::string_view> lines);

    std::unordered_map<std::string, std::string> ReadJson(const std::filesystem::path filePath);
    bool WriteJson(const std::filesystem::path filePath, const std::unordered_map<std::string, std::string>& map);

    YAML::Node ReadYaml(const std::filesystem::path filePath, const std::string_view nodeName);

    bool Exists(const std::filesystem::path filePath);
    size_t GetFileSize(const std::filesystem::path filePath);
    std::string GetExtension(const std::filesystem::path filePath);

// 非公開
private:
    std::filesystem::path defaultPath; // ファイルパス
};

//------------------------------
// バイナリファイル読み込み
//------------------------------
template<typename T>
requires std::is_trivially_copyable_v<T>
std::optional<T> CFile::ReadBinary(std::streamoff offSet)
{
	// インプットファイル作成 (バイナリ)
	ifstream file(defaultPath, ios::binary);

	if (!file.is_open())
	{// 開けない
		return nullopt;
	}

	// オフセットにシーク
	file.seekg(offSet, ios::beg);
	if (!file.good())
	{// シーク失敗
		return nullopt;
	}

	// 読み取り
	std::array<uint8_t, sizeof(T)> buffer{};
	file.read(reinterpret_cast<char*>(buffer.data()), sizeof(T));

	if (file.gcount() != sizeof(T))
	{// サイズ不足
		return nullopt;
	}

	// T型に変換して返す
	return bit_cast<T>(buffer);
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
	// なかったらディレクトリを作成
	if (filePath.has_parent_path())
	{
		create_directories(filePath.parent_path());
	}

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
std::optional<T> CFile::ReadBinary(const std::filesystem::path filePath, std::streamoff offSet)
{
	// インプットファイル作成 (バイナリ)
	ifstream file(filePath, ios::binary);

	if (!file.is_open())
	{// 開けない
		return nullopt;
	}

	// オフセットにシーク
	file.seekg(offSet, ios::beg);
	if (!file.good())
	{// シーク失敗
		return nullopt;
	}

	// 読み取り
	std::array<uint8_t, sizeof(T)> buffer{};
	file.read(reinterpret_cast<char*>(buffer.data()), sizeof(T));

	if (file.gcount() != sizeof(T))
	{// サイズ不足
		return nullopt;
	}

	// T型に変換して返す
	return bit_cast<T>(buffer);
}

//----------------------------------------------
// バイナリファイル書き込み
//----------------------------------------------
template<typename T>
requires std::is_trivially_copyable_v<T>
bool CFile::WriteBinary(const std::filesystem::path filePath, const T& data)
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
bool CFile::AddWriteBinary(const std::filesystem::path filePath, const T& data)
{
	// なかったらディレクトリを作成
	if (filePath.has_parent_path())
	{
		create_directories(filePath.parent_path());
	}

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
