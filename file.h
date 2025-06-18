//------------------------------------------
//
// �t�@�C���̏����̒�` [file.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include <fstream>     // �t�@�C�����o��
#include <sstream>     // stringstream

using std::fstream;                        // �t�@�C��IO
using std::ifstream;                       // �t�@�C��I
using std::ofstream;                       // �t�@�C��O
using std::stringstream;                   // ������ƃt�@�C���̘A��
using std::ios;                            // ios
using std::streamsize;                     // �T�C�Y
using std::istreambuf_iterator;            // �C���v�b�g�C�e���[�^
using std::streamoff;					   // �X�g���[���I�t�Z�b�g

using std::bit_cast;                     // �r�b�g�L���X�g

//----------------------
// �t�@�C���N���X
//----------------------
class CFile
{
// ���J
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

// ����J
private:
	path defaultPath; // �t�@�C���p�X
};

//------------------------------
// �o�C�i���t�@�C���ǂݍ���
//------------------------------
template<typename T>
requires std::is_trivially_copyable_v<T>
T CFile::ReadBinary(streamoff offSet)
{
	// �C���v�b�g�t�@�C���쐬 (�o�C�i��)
	ifstream file(defaultPath, ios::binary);

	if (!file.is_open())
	{// �J���Ȃ�
		return {};
	}

	// �I�t�Z�b�g�ɃV�[�N
	file.seekg(offSet, ios::beg);
	if (!file.good())
	{// �V�[�N���s
		return {};
	}

	// �ǂݎ��
	std::array<uint8_t, sizeof(T)> buffer{};
	file.read(reinterpret_cast<char*>(buffer.data()), sizeof(T));

	if (file.gcount() != sizeof(T))
	{// �T�C�Y�s��
		return {};
	}

	// T�^�ɕϊ����ĕԂ�
	return std::bit_cast<T>(buffer);
}

//----------------------------------------------
// �o�C�i���t�@�C����������
//----------------------------------------------
template<typename T>
requires std::is_trivially_copyable_v<T>
bool CFile::WriteBinary(const T& data)
{
	// �Ȃ�������f�B���N�g�����쐬
	if (defaultPath.has_parent_path())
	{
		create_directories(defaultPath.parent_path());
	}

	// �A�E�g�v�b�g�t�@�C���쐬�i�o�C�i���j
	ofstream file(defaultPath, ios::binary);

	if (!file.is_open())
	{// �J���Ȃ�
		return false;
	}

	// ��������
	auto bitData = bit_cast<array<uint8_t, sizeof(T)>>(data);
	file.write(reinterpret_cast<const char*>(bitData.data()), sizeof(T));

	// ������Ԃ�
	return file.good();
}

//----------------------------
// �o�C�i���t�@�C���ǉ���������
//----------------------------
template<typename T>
requires std::is_trivially_copyable_v<T>
bool CFile::AddWriteBinary(const T& data)
{
	// �A�E�g�v�b�g�t�@�C���쐬�i�o�C�i��,�ǋ��j
	ofstream file(defaultPath, ios::binary | ios::app);

	if (!file.is_open())
	{// �J���Ȃ�
		return false;
	}

	// ��������
	auto bitData = bit_cast<array<uint8_t, sizeof(T)>>(data);
	file.write(reinterpret_cast<const char*>(bitData.data()), sizeof(T));

	// ������Ԃ�
	return file.good();
}

//------------------------------
// �o�C�i���t�@�C���ǂݍ���
//------------------------------
template<typename T>
requires std::is_trivially_copyable_v<T>
T CFile::ReadBinary(const path filePath, streamoff offSet)
{
	// �C���v�b�g�t�@�C���쐬 (�o�C�i��)
	ifstream file(filePath, ios::binary);

	if (!file.is_open())
	{// �J���Ȃ�
		return {};
	}

	// �I�t�Z�b�g�ɃV�[�N
	file.seekg(offSet, ios::beg);
	if (!file.good())
	{// �V�[�N���s
		return {};
	}

	// �ǂݎ��
	std::array<uint8_t, sizeof(T)> buffer{};
	file.read(reinterpret_cast<char*>(buffer.data()), sizeof(T));

	if (file.gcount() != sizeof(T))
	{// �T�C�Y�s��
		return {};
	}

	// T�^�ɕϊ����ĕԂ�
	return std::bit_cast<T>(buffer);
}

//----------------------------------------------
// �o�C�i���t�@�C����������
//----------------------------------------------
template<typename T>
requires std::is_trivially_copyable_v<T>
bool CFile::WriteBinary(const path filePath, const T& data)
{
	// �Ȃ�������f�B���N�g�����쐬
	if (filePath.has_parent_path())
	{
		create_directories(filePath.parent_path());
	}

	// �A�E�g�v�b�g�t�@�C���쐬�i�o�C�i���j
	ofstream file(filePath, ios::binary);

	if (!file.is_open())
	{// �J���Ȃ�
		return false;
	}

	// ��������
	auto bitData = bit_cast<array<uint8_t, sizeof(T)>>(data);
	file.write(reinterpret_cast<const char*>(bitData.data()), sizeof(T));

	// ������Ԃ�
	return file.good();
}

//----------------------------
// �o�C�i���t�@�C���ǉ���������
//----------------------------
template<typename T>
requires std::is_trivially_copyable_v<T>
bool CFile::AddWriteBinary(const path filePath, const T& data)
{
	// �A�E�g�v�b�g�t�@�C���쐬�i�o�C�i��,�ǋ��j
	ofstream file(defaultPath, ios::binary | ios::app);

	if (!file.is_open())
	{// �J���Ȃ�
		return false;
	}

	// ��������
	auto bitData = bit_cast<array<uint8_t, sizeof(T)>>(data);
	file.write(reinterpret_cast<const char*>(bitData.data()), sizeof(T));

	// ������Ԃ�
	return file.good();
}