//------------------------------------------
//
// 便利定義 [useful.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include <string>         // std::string, std::wstring
#include <string_view>    // std::string_view
#include <vector>         // std::vector
#include <optional>       // std::optional, std::nullopt
#include <stdexcept>      // std::logic_error
#include <sstream>        // std::istringstream
#include <concepts>       // std::same_as (C++20)
#include <utility>        // std::swap
#include <cctype>         // isspace

#define NOMINMAX
#include <windows.h>      // MultiByteToWideChar, WideCharToMultiByte

#include "common.h"       // common::Index

//--------------------------
// 一度だけ初期化可能を付与
//--------------------------
template<typename T>
class WriteOnce
{
public:
    WriteOnce() :m_storage{} {};
    ~WriteOnce() = default;

    WriteOnce(WriteOnce&&) = delete;
    WriteOnce(const WriteOnce&) = delete;
    WriteOnce& operator=(const WriteOnce&) = delete;
    WriteOnce& operator=(WriteOnce&&) = delete;

    // 値を設定
    void Set(const T& value)
    {
        if (m_storage.has_value())
        {// 既に値が設定されている場合はエラー
            throw logic_error("Value already set");
        }
        m_storage = value;
    }

    // 値を取得
    const T& Get() const
    {
        if (!m_storage.has_value())
        {// 値が設定されていない場合はエラー
            throw logic_error("Value not set");
        }
        return m_storage.value();
    }

private:
    optional<T> m_storage;
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
inline std::string_view Trim(std::string_view sv)
{
    size_t start = sv.find_first_not_of(" \t");
    size_t end = sv.find_last_not_of(" \t");
    return (start == std::string_view::npos) ? "" : sv.substr(start, end - start + 1);
}

//----------------------------
// tag = val を探して値を返す
//----------------------------
template<StreamExtractable T>
std::optional<T> StringToData(const std::string_view source, const std::string_view tag, const common::Index index = 0)
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
inline std::optional<std::string_view> ExtractBlock(const std::string_view source, const std::string_view tag, const std::string_view endtag, const common::Index index = 0)
{
    size_t pos = 0;
    size_t currentIndex = 0;

    while (pos < source.size())
    {
        // 開始タグを探す
        size_t startPos = source.find(tag, pos);
        if (startPos == std::string_view::npos) break;

        // 中身の開始位置
        size_t contentStart = startPos + tag.size();

        // 終了タグを探す（開始タグの後）
        size_t endPos = source.find(endtag, contentStart);
        if (endPos == std::string_view::npos) break;

        // 必要な index に到達したらタグを含まない中身のみ返す
        if (currentIndex == index)
        {
            return source.substr(contentStart, endPos - contentStart);
        }

        ++currentIndex;
        pos = endPos + endtag.size(); // 次の検索へ
    }

    return std::nullopt;
}

//-----------------------
// Swapを使ったvectorの要素削除
//-----------------------
template<typename T>
void SwapRemove(std::vector<T>& vec, size_t index)
{
    if (index < vec.size())
    {
        std::swap(vec[index], vec.back());
        vec.pop_back();
    }
}

//----------------------------------------------------
// UTF-8文字列をワイド文字列に変換
//----------------------------------------------------
std::wstring Utf8ToWstring(const std::string& utf8)
{
    int size = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, nullptr, 0);
    std::wstring result(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &result[0], size);
    return result;
}

//----------------------------------------------------
// ワイド文字列をUTF-8文字列に変換
//----------------------------------------------------
std::string WstringToUtf8(const std::wstring& wstr)
{
    if (wstr.empty()) return {};

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);

    std::string str(size_needed - 1, 0); // -1 to exclude null terminator
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], size_needed, nullptr, nullptr);

    return str;
}
