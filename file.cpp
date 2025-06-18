//------------------------------------------
//
// �t�@�C���̏��� [file.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "file.h"      // �t�@�C��
#include <json/json.h> // json�t�@�C��

using namespace Json;  // Json::Value�Ȃǂ��g������

//-------------------
//
// cpp���w���p�[
//
//-------------------

namespace
{
//-----------------------
// Json��Map�ɕϊ�
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
	// Map��Json�ɕϊ�
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
// �t�@�C���N���X
//
//-------------------------------

//----------------------------
// �e�L�X�g�t�@�C���ǂݍ���
//----------------------------
string CFile::ReadText(void)
{
	// �C���v�b�g�t�@�C���쐬 (�e�L�X�g)
	ifstream file(defaultPath);

	if (!file.is_open())
	{// �J���Ȃ�
		return {};
	}

	// stringstream�ŕԂ�
	stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

//--------------------------
// �e�L�X�g�t�@�C���ǂݍ��� (�s)
//--------------------------
string CFile::ReadTextLine(size_t lineNumber)
{
	// �C���v�b�g�t�@�C���쐬 (�e�L�X�g)
	ifstream file(defaultPath);

	if (!file.is_open())
	{// �J���Ȃ�
		return "";
	}

	// ���C���擾
	string line{};
	size_t currentLine = 0;
	while (getline(file, line))
	{
		if (currentLine == lineNumber)
		{
			return line; // �w��s����������Ԃ�
		}
		++currentLine;
	}

	return "";
}

//--------------------------
// �e�L�X�g�t�@�C���ǂݍ��� (�s)
//--------------------------
vector<string> CFile::ReadTextLines(void)
{
	vector<string> lines; // ���C�����Ƃ̔z��
	// �C���v�b�g�t�@�C���쐬 (�e�L�X�g)
	ifstream file(defaultPath);

	if (!file.is_open())
	{// �J���Ȃ�
		return lines;
	}

	// ���C���擾
	string line;
	while (getline(file, line))
	{// �t�@�C�����I���܂�
		lines.push_back(line);
	}

	return lines;
}

//--------------------------
// �e�L�X�g�t�@�C����������
//--------------------------
bool CFile::WriteText(const string_view content)
{
	// �Ȃ�������f�B���N�g�����쐬
	if (defaultPath.has_parent_path())
	{
		create_directories(defaultPath.parent_path());
	}

	// �A�E�g�v�b�g�t�@�C���쐬�i�e�L�X�g�j
	ofstream file(defaultPath);

	if (!file.is_open())
	{// �J���Ȃ�
		return false;
	}

	// ��������
	file << content;

	// ������Ԃ�
	return file.good();
}

//--------------------------
// �e�L�X�g�t�@�C���������� (�s)
//--------------------------
bool CFile::WriteTextLines(const vector<string_view> lines)
{
	// �Ȃ�������f�B���N�g�����쐬
	if (defaultPath.has_parent_path())
	{
		create_directories(defaultPath.parent_path());
	}

	// �A�E�g�v�b�g�t�@�C���쐬�i�e�L�X�g�j
	ofstream file(defaultPath);

	if (!file.is_open())
	{// �J���Ȃ�
		return false;
	}

	for (const auto& line : lines)
	{// ���s�����Ȃ��珑�������
		file << line << '\n';
	}

	// ������Ԃ�
	return file.good();
}

//--------------------------
// �e�L�X�g�t�@�C����������
//--------------------------
bool CFile::AddWriteText(const string_view content)
{
	// �A�E�g�v�b�g�t�@�C���쐬�i�e�L�X�g,�ǋL�j
	ofstream file(defaultPath, ios::app);

	if (!file.is_open())
	{// �J���Ȃ�
		return false;
	}

	// ��������
	file << content;

	// ������Ԃ�
	return file.good();
}

//--------------------------
// �e�L�X�g�t�@�C���������� (�s)
//--------------------------
bool CFile::AddWriteTextLines(const vector<string_view> lines)
{
	// �A�E�g�v�b�g�t�@�C���쐬�i�e�L�X�g�j
	ofstream file(defaultPath, ios::app);

	if (!file.is_open())
	{// �J���Ȃ�
		return false;
	}

	file << '\n'; // ���s
	for (const auto& line : lines)
	{// ���s�����Ȃ��珑�������
		file << line << '\n';
	}

	// ������Ԃ�
	return file.good();
}

//--------------------------
// json�t�@�C���ǂݍ���
//--------------------------
unordered_map<string, string> CFile::ReadJson(void)
{
	// �C���v�b�g�t�@�C���쐬�i�o�C�i���j
	ifstream file(defaultPath, ifstream::binary);

	if (!file.is_open())
	{// �J���Ȃ�
		return {};
	}

	Value root;                // Json�f�[�^�i�[
	CharReaderBuilder builder; // �ݒ�
	string errs;               // �G���[�i�[

	// �ǂݍ���
	if (!parseFromStream(builder, file, &root, &errs))
	{// ���s
		return {};
	}

	return JsonToMap(root);
}

//--------------------------
// json�t�@�C����������
//--------------------------
bool CFile::WriteJson(const unordered_map<string, string>& map)
{
	// �A�E�g�v�b�g�t�@�C���쐬�i�o�C�i���j
	ofstream file(defaultPath, ofstream::binary);

	if (!file.is_open())
	{// �J���Ȃ�
		return {};
	}

	StreamWriterBuilder writer;    // �ݒ�
	writer["indentation"] = "  ";  // �C���f���g�ݒ�i���₷���j

	StreamWriter* jsonWriter(writer.newStreamWriter()); // ���C�^�[�̐���

	Value root = MapToJson(map); // Json�f�[�^�ɕϊ�

	// ��������
	if (jsonWriter->write(root, &file) != 0)
	{// ���s
		delete jsonWriter; // ���C�^�[�j��
		return false;
	}

	delete jsonWriter; // ���C�^�[�j��
	return true;
}

//-----------------------
// �t�@�C�����݃`�F�b�N
//-----------------------
bool CFile::Exists(void)
{
	return exists(defaultPath);
}

//-----------------------
// �t�@�C���T�C�Y
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
// �g���q
//-----------------------
string CFile::GetExtension(void)
{
	return defaultPath.has_extension() ? defaultPath.extension().string() : "";
}

//----------------------------
// �e�L�X�g�t�@�C���ǂݍ���
//----------------------------
string CFile::ReadText(const path filePath)
{
	// �C���v�b�g�t�@�C���쐬 (�e�L�X�g)
	ifstream file(filePath);

	if (!file.is_open())
	{// �J���Ȃ�
		return {};
	}

	// stringstream�ŕԂ�
	stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

//--------------------------
// �e�L�X�g�t�@�C���ǂݍ��� (�s)
//--------------------------
string CFile::ReadTextLine(const path filePath, size_t lineNumber)
{
	// �C���v�b�g�t�@�C���쐬 (�e�L�X�g)
	ifstream file(filePath);

	if (!file.is_open())
	{// �J���Ȃ�
		return "";
	}

	// ���C���擾
	string line{};
	size_t currentLine = 0;
	while (getline(file, line))
	{
		if (currentLine == lineNumber)
		{
			return line; // �w��s����������Ԃ�
		}
		++currentLine;
	}

	return "";
}

//--------------------------
// �e�L�X�g�t�@�C���ǂݍ��� (�s)
//--------------------------
vector<string> CFile::ReadTextLines(const path filePath)
{
	vector<string> lines; // ���C�����Ƃ̔z��
	// �C���v�b�g�t�@�C���쐬 (�e�L�X�g)
	ifstream file(filePath);

	if (!file.is_open())
	{// �J���Ȃ�
		return lines;
	}

	// ���C���擾
	string line;
	while (getline(file, line))
	{// �t�@�C�����I���܂�
		lines.push_back(line);
	}

	return lines;
}

//--------------------------
// �e�L�X�g�t�@�C����������
//--------------------------
bool CFile::WriteText(const path filePath, const string_view content)
{
	// �Ȃ�������f�B���N�g�����쐬
	if (filePath.has_parent_path())
	{
		create_directories(filePath.parent_path());
	}

	// �A�E�g�v�b�g�t�@�C���쐬�i�e�L�X�g�j
	ofstream file(filePath);

	if (!file.is_open())
	{// �J���Ȃ�
		return false;
	}

	// ��������
	file << content;

	// ������Ԃ�
	return file.good();
}

//--------------------------
// �e�L�X�g�t�@�C���������� (�s)
//--------------------------
bool CFile::WriteTextLines(const path filePath, const vector<string_view> lines)
{
	// �Ȃ�������f�B���N�g�����쐬
	if (filePath.has_parent_path())
	{
		create_directories(filePath.parent_path());
	}

	// �A�E�g�v�b�g�t�@�C���쐬�i�e�L�X�g�j
	ofstream file(filePath);

	if (!file.is_open())
	{// �J���Ȃ�
		return false;
	}

	for (const auto& line : lines)
	{// ���s�����Ȃ��珑�������
		file << line << '\n';
	}

	// ������Ԃ�
	return file.good();
}

//--------------------------
// �e�L�X�g�t�@�C����������
//--------------------------
bool CFile::AddWriteText(const path filePath, const string_view content)
{
	// �A�E�g�v�b�g�t�@�C���쐬�i�e�L�X�g,�ǋL�j
	ofstream file(filePath, ios::app);

	if (!file.is_open())
	{// �J���Ȃ�
		return false;
	}

	// ��������
	file << content;

	// ������Ԃ�
	return file.good();
}

//--------------------------
// �e�L�X�g�t�@�C���������� (�s)
//--------------------------
bool CFile::AddWriteTextLines(const path filePath, const vector<string_view> lines)
{
	// �A�E�g�v�b�g�t�@�C���쐬�i�e�L�X�g�j
	ofstream file(filePath, ios::app);

	if (!file.is_open())
	{// �J���Ȃ�
		return false;
	}

	file << '\n'; // ���s
	for (const auto& line : lines)
	{// ���s�����Ȃ��珑�������
		file << line << '\n';
	}

	// ������Ԃ�
	return file.good();
}

//--------------------------
// json�t�@�C���ǂݍ���
//--------------------------
unordered_map<string, string> CFile::ReadJson(const path filePath)
{
	// �C���v�b�g�t�@�C���쐬�i�o�C�i���j
	ifstream file(filePath, ifstream::binary);

	if (!file.is_open())
	{// �J���Ȃ�
		return {};
	}

	Value root;                // Json�f�[�^�i�[
	CharReaderBuilder builder; // �ݒ�
	string errs;               // �G���[�i�[

	// �ǂݍ���
	if (!parseFromStream(builder, file, &root, &errs))
	{// ���s
		return {};
	}

	return JsonToMap(root);
}

//--------------------------
// json�t�@�C����������
//--------------------------
bool CFile::WriteJson(const path filePath, const unordered_map<string, string>& map)
{
	// �A�E�g�v�b�g�t�@�C���쐬�i�o�C�i���j
	ofstream file(filePath, ofstream::binary);

	if (!file.is_open())
	{// �J���Ȃ�
		return {};
	}

	StreamWriterBuilder writer;    // �ݒ�
	writer["indentation"] = "  ";  // �C���f���g�ݒ�i���₷���j

	StreamWriter* jsonWriter(writer.newStreamWriter()); // ���C�^�[�̐���

	Value root = MapToJson(map); // Json�f�[�^�ɕϊ�

	// ��������
	if (jsonWriter->write(root, &file) != 0)
	{// ���s
		delete jsonWriter; // ���C�^�[�j��
		return false;
	}

	delete jsonWriter; // ���C�^�[�j��
	return true;
}

//-----------------------
// �t�@�C�����݃`�F�b�N
//-----------------------
bool CFile::Exists(const path filePath)
{
	return exists(filePath);
}

//-----------------------
// �t�@�C���T�C�Y
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
// �g���q
//-----------------------
string CFile::GetExtension(const path filePath)
{
	return filePath.has_extension() ? filePath.extension().string() : "";
}

#if 0
//------------------------------
// �o�C�i���t�@�C���ǂݍ���
//------------------------------
vector<uint8_t> CFile::ReadBinary(const string_view filename)
{
	// �C���v�b�g�t�@�C���쐬 (�o�C�i��,�Ōォ��)
	ifstream file(filename.data(), ios::binary | ios::ate);

	if (!file.is_open())
	{// �J���Ȃ�
		return {};
	}

	// �t�@�C���T�C�Y
	streamsize size = file.tellg();
	if (size <= 0)
	{// �����Ȃ�
		return {};
	}

	// �擪��
	file.seekg(0, ios::beg);

	// �t�@�C���T�C�Y���̃o�b�t�@���m��
	vector<uint8_t> buffer(static_cast<size_t>(size));

	// �ǂݍ���
	if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
	{// ���s
		return {};
	}

	return buffer;
}


//----------------------------
// �e�L�X�g�t�@�C���ǂݍ���
//----------------------------
string CFile::ReadText(const string_view filename)
{
	// �C���v�b�g�t�@�C���쐬 (�e�L�X�g,�Ōォ��)
	ifstream file(filename.data(), ios::ate);

	if (!file.is_open())
	{// �J���Ȃ�
		return "";
	}

	// �t�@�C���T�C�Y
	streamsize size = file.tellg();
	if (size <= 0)
	{// �����Ȃ�
		return "";
	}

	// �擪�ɖ߂�
	file.seekg(0, ios::beg);

	// �t�@�C���T�C�Y���̕�������m��
	string content(static_cast<size_t>(size), '\0');

	// �ǂݍ���
	if (!file.read(content.data(), size))
	{
		return "";  // �ǂݍ��ݎ��s���͋󕶎����Ԃ�
	}

	return content;
}

//----------------------------
// �e�L�X�g�t�@�C���ǂݍ���
//----------------------------
string CFile::ReadText(const string_view filename)
{
	// �C���v�b�g�t�@�C���쐬 (�e�L�X�g)
	ifstream file(filename.data());

	if (!file.is_open())
	{// �J���Ȃ�
		return {};
	}

	// �C�e���[�^�ŕԂ�
	return string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
}

//-----------------------
// �t�@�C�����݃`�F�b�N
//-----------------------
bool CFile::Exists(const path filePath)
{
	ifstream file(filename.data());
	return file.good();
}
#endif