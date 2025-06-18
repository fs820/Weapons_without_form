//------------------------------------------
//
// ファイルの処理 [file.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "file.h"      // ファイル
#include <json/json.h> // jsonファイル

using namespace Json;  // Json::Valueなどを使うため

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
	ifstream file(defaultPath);

	if (!file.is_open())
	{// 開けない
		return {};
	}

	// stringstreamで返す
	stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

//--------------------------
// テキストファイル読み込み (行)
//--------------------------
string CFile::ReadTextLine(size_t lineNumber)
{
	// インプットファイル作成 (テキスト)
	ifstream file(defaultPath);

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
	ifstream file(defaultPath);

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
	ofstream file(defaultPath);

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
	ofstream file(defaultPath);

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
	ofstream file(defaultPath, ios::app);

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
	ofstream file(defaultPath, ios::app);

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
	// インプットファイル作成（バイナリ）
	ifstream file(defaultPath, ifstream::binary);

	if (!file.is_open())
	{// 開けない
		return {};
	}

	Value root;                // Jsonデータ格納
	CharReaderBuilder builder; // 設定
	string errs;               // エラー格納

	// 読み込み
	if (!parseFromStream(builder, file, &root, &errs))
	{// 失敗
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
	ifstream file(filePath);

	if (!file.is_open())
	{// 開けない
		return {};
	}

	// stringstreamで返す
	stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

//--------------------------
// テキストファイル読み込み (行)
//--------------------------
string CFile::ReadTextLine(const path filePath, size_t lineNumber)
{
	// インプットファイル作成 (テキスト)
	ifstream file(filePath);

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
	ifstream file(filePath);

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
	ofstream file(filePath);

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
	ofstream file(filePath);

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
	ofstream file(filePath, ios::app);

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
	ofstream file(filePath, ios::app);

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
	// インプットファイル作成（バイナリ）
	ifstream file(filePath, ifstream::binary);

	if (!file.is_open())
	{// 開けない
		return {};
	}

	Value root;                // Jsonデータ格納
	CharReaderBuilder builder; // 設定
	string errs;               // エラー格納

	// 読み込み
	if (!parseFromStream(builder, file, &root, &errs))
	{// 失敗
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