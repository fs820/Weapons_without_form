//------------------------------------------
//
// ファイルの処理 [file.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "file.h"      // ファイル
#include <memory>   // メモリ
#include <json/json.h> // jsonファイル

using namespace Json;                      // Json::Valueなどを使うため

using namespace std::filesystem;           // ファイルシステム
using std::unique_ptr;                     // ユニークポインタ
using std::fstream;                        // ファイルIO
using std::ifstream;                       // ファイルI
using std::ofstream;                       // ファイルO
using std::stringstream;                   // 文字列とファイルの連結
using std::ios;                            // ios
using std::streamsize;                     // サイズ
using std::istreambuf_iterator;            // インプットイテレータ
using std::streamoff;					   // ストリームオフセット
using std::string;                        // 文字列
using std::string_view;                   // 文字列ビュー
using std::vector;                        // ベクター
using std::unordered_map;                 // ハッシュマップ
using std::optional;                      // オプショナル
using std::nullopt;                       // オプショナルのnull

using std::bit_cast;                     // ビットキャスト

//-------------------
//
// cpp内ヘルパー
//
//-------------------

namespace
{
//-----------------------
// JsonをMapに変換
//-----------------------
	unordered_map<string, string> JsonToMap(const Value& root)
	{
		unordered_map<string, string> map;

		for (const auto& key : root.getMemberNames())
		{
			map[key] = root[key].asString();
		}

		return map;
	}

	//-----------------------
	// MapをJsonに変換
	//-----------------------
	Value MapToJson(const unordered_map<string, string>& map)
	{
		Value root;

		for (const auto& [key, value] : map)
		{
			root[key] = value;
		}

		return root;
	}
}

//-------------------------------
//
// ファイルクラス
//
//-------------------------------

//----------------------------
// テキストファイル読み込み
//----------------------------
string CFile::ReadText(void)
{
	// インプットファイル作成 (テキスト)
	ifstream file(defaultPath, ios::in);
	if (!file.is_open())
	{// 開けない
		return {};
	}

	// ファイルの内容を文字列に読み込む
	string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

	// UTF-8 BOM (0xEF,0xBB,0xBF) の検出と除去
	if (content.size() >= 3 &&
		static_cast<unsigned char>(content[0]) == 0xEF &&
		static_cast<unsigned char>(content[1]) == 0xBB &&
		static_cast<unsigned char>(content[2]) == 0xBF)
	{
		content.erase(0, 3);
	}

	return content;
}

//--------------------------
// テキストファイル読み込み (行)
//--------------------------
string CFile::ReadTextLine(size_t lineNumber)
{
	// インプットファイル作成 (テキスト)
	ifstream file(defaultPath, ios::in);

	if (!file.is_open())
	{// 開けない
		return "";
	}

	// ライン取得
	string line{};
	size_t currentLine = 0;
	while (getline(file, line))
	{
		if (currentLine == lineNumber)
		{
			return line; // 指定行を見つけたら返す
		}
		++currentLine;
	}

	return "";
}

//--------------------------
// テキストファイル読み込み (行)
//--------------------------
vector<string> CFile::ReadTextLines(void)
{
	vector<string> lines; // ラインごとの配列
	// インプットファイル作成 (テキスト)
	ifstream file(defaultPath, ios::in);

	if (!file.is_open())
	{// 開けない
		return lines;
	}

	// ライン取得
	string line;
	while (getline(file, line))
	{// ファイルが終わるまで
		lines.push_back(line);
	}

	return lines;
}

//--------------------------
// テキストファイル書き込み
//--------------------------
bool CFile::WriteText(const string_view content)
{
	// なかったらディレクトリを作成
	if (defaultPath.has_parent_path())
	{
		create_directories(defaultPath.parent_path());
	}

	// アウトプットファイル作成（テキスト）
	ofstream file(defaultPath, ios::out, ios::trunc);

	if (!file.is_open())
	{// 開けない
		return false;
	}

	// 書き込み
	file << content;

	// 成功を返す
	return file.good();
}

//--------------------------
// テキストファイル書き込み (行)
//--------------------------
bool CFile::WriteTextLines(const vector<string_view> lines)
{
	// なかったらディレクトリを作成
	if (defaultPath.has_parent_path())
	{
		create_directories(defaultPath.parent_path());
	}

	// アウトプットファイル作成（テキスト）
	ofstream file(defaultPath, ios::out, ios::trunc);

	if (!file.is_open())
	{// 開けない
		return false;
	}

	for (const auto& line : lines)
	{// 改行をしながら書き入れる
		file << line << '\n';
	}

	// 成功を返す
	return file.good();
}

//--------------------------
// テキストファイル書き込み
//--------------------------
bool CFile::AddWriteText(const string_view content)
{
	// アウトプットファイル作成（テキスト,追記）
	ofstream file(defaultPath, ios::out, ios::app);

	if (!file.is_open())
	{// 開けない
		return false;
	}

	// 書き込み
	file << content;

	// 成功を返す
	return file.good();
}

//--------------------------
// テキストファイル書き込み (行)
//--------------------------
bool CFile::AddWriteTextLines(const vector<string_view> lines)
{
	// アウトプットファイル作成（テキスト）
	ofstream file(defaultPath, ios::out, ios::app);

	if (!file.is_open())
	{// 開けない
		return false;
	}

	file << '\n'; // 改行
	for (const auto& line : lines)
	{// 改行をしながら書き入れる
		file << line << '\n';
	}

	// 成功を返す
	return file.good();
}

//--------------------------
// jsonファイル読み込み
//--------------------------
unordered_map<string, string> CFile::ReadJson(void)
{
	// 既存のReadText関数でファイル内容を読み込む（BOMはここで除去される）
	string jsonContent = ReadText();
	if (jsonContent.empty()) {
		return {};
	}

	Value root;
	CharReaderBuilder builder;
	string errs;

	// 文字列からパースする
    unique_ptr<CharReader> reader(builder.newCharReader());
	if (!reader->parse(jsonContent.c_str(), jsonContent.c_str() + jsonContent.length(), &root, &errs))
	{
		// 失敗
		return {};
	}

	return JsonToMap(root);
}

//--------------------------
// jsonファイル書き込み
//--------------------------
bool CFile::WriteJson(const unordered_map<string, string>& map)
{
	// アウトプットファイル作成（バイナリ）
	ofstream file(defaultPath, ofstream::binary);

	if (!file.is_open())
	{// 開けない
		return {};
	}

	StreamWriterBuilder writer;    // 設定
	writer["indentation"] = "  ";  // インデント設定（見やすく）

	StreamWriter* jsonWriter(writer.newStreamWriter()); // ライターの生成

	Value root = MapToJson(map); // Jsonデータに変換

	// 書き込み
	if (jsonWriter->write(root, &file) != 0)
	{// 失敗
		delete jsonWriter; // ライター破棄
		return false;
	}

	delete jsonWriter; // ライター破棄
	return true;
}

//--------------------------
// yamlファイル読み込み
//--------------------------
YAML::Node CFile::ReadYaml(const std::string_view nodeName)
{
    // YAMLファイルの読み込み
    YAML::Node config = YAML::LoadFile(defaultPath.string());
    if (config)
    {
        return config[nodeName];
    }
    return YAML::Node(); // 空のノードを返す
}

//-----------------------
// ファイル存在チェック
//-----------------------
bool CFile::Exists(void)
{
	return exists(defaultPath);
}

//-----------------------
// ファイルサイズ
//-----------------------
size_t CFile::GetFileSize(void)
{
	try
	{
		if (exists(defaultPath) && is_regular_file(defaultPath))
		{
			return static_cast<size_t>(file_size(defaultPath));
		}
	}
	catch (const filesystem_error&)
	{

	}
	return 0;
}

//-----------------------
// 拡張子
//-----------------------
string CFile::GetExtension(void)
{
	return defaultPath.has_extension() ? defaultPath.extension().string() : "";
}

//----------------------------
// テキストファイル読み込み
//----------------------------
string CFile::ReadText(const path filePath)
{
    // インプットファイル作成 (テキスト)
    ifstream file(filePath, ios::in);
    if (!file.is_open())
    {// 開けない
        return {};
    }

    // ファイルの内容を文字列に読み込む
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    // UTF-8 BOM (0xEF,0xBB,0xBF) の検出と除去
    if (content.size() >= 3 &&
        static_cast<unsigned char>(content[0]) == 0xEF &&
        static_cast<unsigned char>(content[1]) == 0xBB &&
        static_cast<unsigned char>(content[2]) == 0xBF)
    {
        content.erase(0, 3);
    }

    return content;
}

//--------------------------
// テキストファイル読み込み (行)
//--------------------------
string CFile::ReadTextLine(const path filePath, size_t lineNumber)
{
	// インプットファイル作成 (テキスト)
	ifstream file(filePath, ios::in);

	if (!file.is_open())
	{// 開けない
		return "";
	}

	// ライン取得
	string line{};
	size_t currentLine = 0;
	while (getline(file, line))
	{
		if (currentLine == lineNumber)
		{
			return line; // 指定行を見つけたら返す
		}
		++currentLine;
	}

	return "";
}

//--------------------------
// テキストファイル読み込み (行)
//--------------------------
vector<string> CFile::ReadTextLines(const path filePath)
{
	vector<string> lines; // ラインごとの配列
	// インプットファイル作成 (テキスト)
	ifstream file(filePath, ios::in);

	if (!file.is_open())
	{// 開けない
		return lines;
	}

	// ライン取得
	string line;
	while (getline(file, line))
	{// ファイルが終わるまで
		lines.push_back(line);
	}

	return lines;
}

//--------------------------
// テキストファイル書き込み
//--------------------------
bool CFile::WriteText(const path filePath, const string_view content)
{
	// なかったらディレクトリを作成
	if (filePath.has_parent_path())
	{
		create_directories(filePath.parent_path());
	}

	// アウトプットファイル作成（テキスト）
	ofstream file(filePath, ios::out, ios::trunc);

	if (!file.is_open())
	{// 開けない
		return false;
	}

	// 書き込み
	file << content;

	// 成功を返す
	return file.good();
}

//--------------------------
// テキストファイル書き込み (行)
//--------------------------
bool CFile::WriteTextLines(const path filePath, const vector<string_view> lines)
{
	// なかったらディレクトリを作成
	if (filePath.has_parent_path())
	{
		create_directories(filePath.parent_path());
	}

	// アウトプットファイル作成（テキスト）
	ofstream file(filePath, ios::out, ios::trunc);

	if (!file.is_open())
	{// 開けない
		return false;
	}

	for (const auto& line : lines)
	{// 改行をしながら書き入れる
		file << line << '\n';
	}

	// 成功を返す
	return file.good();
}

//--------------------------
// テキストファイル書き込み
//--------------------------
bool CFile::AddWriteText(const path filePath, const string_view content)
{
	// アウトプットファイル作成（テキスト,追記）
	ofstream file(filePath, ios::out, ios::app);

	if (!file.is_open())
	{// 開けない
		return false;
	}

	// 書き込み
	file << content;

	// 成功を返す
	return file.good();
}

//--------------------------
// テキストファイル書き込み (行)
//--------------------------
bool CFile::AddWriteTextLines(const path filePath, const vector<string_view> lines)
{
	// アウトプットファイル作成（テキスト）
	ofstream file(filePath, ios::out, ios::app);

	if (!file.is_open())
	{// 開けない
		return false;
	}

	file << '\n'; // 改行
	for (const auto& line : lines)
	{// 改行をしながら書き入れる
		file << line << '\n';
	}

	// 成功を返す
	return file.good();
}

//--------------------------
// jsonファイル読み込み
//--------------------------
unordered_map<string, string> CFile::ReadJson(const path filePath)
{
    // 既存のReadText関数でファイル内容を読み込む（BOMはここで除去される）
    string jsonContent = ReadText(filePath);
    if (jsonContent.empty()) {
        return {};
    }

    Value root;
    CharReaderBuilder builder;
    string errs;

    // 文字列からパースする
    unique_ptr<CharReader> reader(builder.newCharReader());
    if (!reader->parse(jsonContent.c_str(), jsonContent.c_str() + jsonContent.length(), &root, &errs))
    {
        // 失敗
        return {};
    }

    return JsonToMap(root);
}

//--------------------------
// jsonファイル書き込み
//--------------------------
bool CFile::WriteJson(const path filePath, const unordered_map<string, string>& map)
{
	// アウトプットファイル作成（バイナリ）
	ofstream file(filePath, ofstream::binary);

	if (!file.is_open())
	{// 開けない
		return {};
	}

	StreamWriterBuilder writer;    // 設定
	writer["indentation"] = "  ";  // インデント設定（見やすく）

	StreamWriter* jsonWriter(writer.newStreamWriter()); // ライターの生成

	Value root = MapToJson(map); // Jsonデータに変換

	// 書き込み
	if (jsonWriter->write(root, &file) != 0)
	{// 失敗
		delete jsonWriter; // ライター破棄
		return false;
	}

	delete jsonWriter; // ライター破棄
	return true;
}

//--------------------------
// yamlファイル読み込み
//--------------------------
YAML::Node CFile::ReadYaml(const std::filesystem::path filePath, const std::string_view nodeName)
{
    // YAMLファイルの読み込み
    YAML::Node config = YAML::LoadFile(filePath.string());
    if (config)
    {
        return config[nodeName];
    }
    return YAML::Node(); // 空のノードを返す
}

//-----------------------
// ファイル存在チェック
//-----------------------
bool CFile::Exists(const path filePath)
{
	return exists(filePath);
}

//-----------------------
// ファイルサイズ
//-----------------------
size_t CFile::GetFileSize(const path filePath)
{
	try
	{
		if (exists(filePath) && is_regular_file(filePath))
		{
			return static_cast<size_t>(file_size(filePath));
		}
	}
	catch (const filesystem_error&)
	{

	}
	return 0;
}

//-----------------------
// 拡張子
//-----------------------
string CFile::GetExtension(const path filePath)
{
	return filePath.has_extension() ? filePath.extension().string() : "";
}

#if 0
//------------------------------
// バイナリファイル読み込み
//------------------------------
vector<uint8_t> CFile::ReadBinary(const string_view filename)
{
	// インプットファイル作成 (バイナリ,最後から)
	ifstream file(filename.data(), ios::binary | ios::ate);

	if (!file.is_open())
	{// 開けない
		return {};
	}

	// ファイルサイズ
	streamsize size = file.tellg();
	if (size <= 0)
	{// 何もない
		return {};
	}

	// 先頭へ
	file.seekg(0, ios::beg);

	// ファイルサイズ分のバッファを確保
	vector<uint8_t> buffer(static_cast<size_t>(size));

	// 読み込み
	if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
	{// 失敗
		return {};
	}

	return buffer;
}


//----------------------------
// テキストファイル読み込み
//----------------------------
string CFile::ReadText(const string_view filename)
{
	// インプットファイル作成 (テキスト,最後から)
	ifstream file(filename.data(), ios::ate);

	if (!file.is_open())
	{// 開けない
		return "";
	}

	// ファイルサイズ
	streamsize size = file.tellg();
	if (size <= 0)
	{// 何もない
		return "";
	}

	// 先頭に戻る
	file.seekg(0, ios::beg);

	// ファイルサイズ分の文字列を確保
	string content(static_cast<size_t>(size), '\0');

	// 読み込み
	if (!file.read(content.data(), size))
	{
		return "";  // 読み込み失敗時は空文字列を返す
	}

	return content;
}

//----------------------------
// テキストファイル読み込み
//----------------------------
string CFile::ReadText(const string_view filename)
{
	// インプットファイル作成 (テキスト)
	ifstream file(filename.data());

	if (!file.is_open())
	{// 開けない
		return {};
	}

	// イテレータで返す
	return string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
}

//-----------------------
// ファイル存在チェック
//-----------------------
bool CFile::Exists(const path filePath)
{
	ifstream file(filename.data());
	return file.good();
}
#endif
