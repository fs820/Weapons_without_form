//------------------------------------------
//
//���C���̏����̒�`�E�錾[main.h]
//Author: fuma sato
//
//------------------------------------------
#pragma once

// �v���W�F�N�g���ʃw�b�_
#define NOMINMAX         // ��max,min �𖳌���
#include <windows.h>     // windows�W��
#include <algorithm>     // �Vmin,max��L����
#include <chrono>        // ����
#include <string>        // ������
#include <string_view>   // ������Q��
#include <sstream>       // �����񑀍�
#include <bit>           // �r�b�g�P�ʑ���
#include <optional>      // �G���[�Ή�
#include <list>          // ���X�g
#include <unordered_map> // �A�z(�y�A)�z��
#include <set>           // ��ӂ̒l
#include <filesystem>    // �t�@�C���Ǘ�
#include <array>         // �z��
#include <span>          // �A���f�[�^�̎Q��

#include "common.h"    // �ėp��`

using namespace std::chrono;     // chrono
using namespace std::filesystem; // �t�@�C���V�X�e��
using std::string;           // ������
using std::string_view;      // ������Q��
using std::istringstream;    // �����񑀍�
using std::ostringstream;    // �����񑀍�
using std::vector;           // ���I�z�� (�C�e���[�V�����p)
using std::list;             // ���I�z�� (�폜�E�ǉ�)
using std::unordered_map;    // �A�z(�y�A)�z��
using std::set;              // ��ӂ̔z��
using std::optional;         // �G���[�Ή�
using std::array;            // �z��
using std::span;             // �A���f�[�^�̎Q��
using std::logic_error;      // �_���G���[
using std::exception;        // ��{��O
using std::bit_cast;         // �r�b�g�P�ʂ̃L���X�g
using std::clamp;            // clamp
using std::max;              // max
using std::min;              // min
using std::swap;             // swap
using std::nullopt;          // null

//-------------------------------------------------
// ���C���̃N���X (�C���X�^���X�֎~) (�p���֎~)
//-------------------------------------------------
class CMain final
{
	// ���J
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
	// �v���W�F�N�g�萔
	//--------------------

	// ���O
	static constexpr const char* CLASS_NAME = "WindowClass";      // �N���X
	static constexpr const char* WINDOW_NAME = "Telepathy_Wepon"; // �E�C���h�E

	// �E�C���h�E�̃T�C�Y
	//static constexpr float SCREEN_WIDTH = 1280.0f;  // ��
	//static constexpr float SCREEN_HEIGHT = 720.0f;  // ����
	static constexpr float SCREEN_WIDTH = 1920.0f;   // ��
	static constexpr float SCREEN_HEIGHT = 1080.0f;  // ����
	//static constexpr float SCREEN_WIDTH = 2560.0f;  // ��
	//static constexpr float SCREEN_HEIGHT = 1440.0f; // ����

	// �t���X�N���[�����{�[�_�[���X�ŕ\��
	static constexpr bool ISBORDERLESS = true;

	// Fps(1�b�Ԃɉ���`�悷�邩)
	static constexpr int FPS = 60;

	// ����J
private:
	//--------------------
	// �v���W�F�N�g�ϐ�
	//--------------------
	static HWND m_hWnd;         // �E�C���h�E�n���h��
	static bool m_bFullScreen;  // �t���X�N���[���t���O
	static float m_elapsedTime; // �N������
	static float m_deltaTime;   // �t���[������
	static bool m_bStop;        // �X�g�b�v�t���O
	static float m_gameSpeed;   // �Q�[���X�s�[�h
};


//--------------------------
// ��x�����������\��t�^
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

	// �l��ݒ�
	void Set(const T& value)
	{
		if (m_storage.has_value())
		{// ���ɒl���ݒ肳��Ă���ꍇ�̓G���[
			throw logic_error("Value already set");
		}
		m_storage = value;
	}

	// �l���擾
	const T& Get() const
	{
		if (!m_storage.has_value())
		{// �l���ݒ肳��Ă��Ȃ��ꍇ�̓G���[
			throw logic_error("Value not set");
		}
		return m_storage.value();
	}

private:
	optional<T> m_storage;
};

// >> ���g����^���ǂ���
template<typename T>
concept StreamExtractable = requires(std::istream & is, T & t)
{
	{ is >> t } -> std::same_as<std::istream&>;
};

//-----------------------
// �g����
//-----------------------
inline string_view Trim(string_view sv)
{
	size_t start = sv.find_first_not_of(" \t");
	size_t end = sv.find_last_not_of(" \t");
	return (start == string_view::npos) ? "" : sv.substr(start, end - start + 1);
}

//----------------------------
// tag = val ��T���Ēl��Ԃ�
//----------------------------
template<StreamExtractable T>
optional<T> StringToData(const string_view source, const string_view tag, const Index index = 0)
{
	size_t pos = 0;
	while (pos < source.size())
	{
		// �s����s���o��
		size_t lineEnd = source.find('\n', pos);
		if (lineEnd == string_view::npos) lineEnd = source.size();

		string_view line = source.substr(pos, lineEnd - pos);
		pos = lineEnd + 1;

		// �R�����g�𖳎��i#�ȍ~�j
		size_t hashPos = line.find('#');
		if (hashPos != string_view::npos)
			line = line.substr(0, hashPos);

		// �T���t���O�̈ʒu
		size_t tagPos = line.find(tag);
		while (tagPos != string_view::npos)
		{
			bool leftOK = (tagPos == 0 || isspace(line[tagPos - 1]));
			bool rightOK = (tagPos + tag.size() == line.size() || isspace(line[tagPos + tag.size()]) || line[tagPos + tag.size()] == '=');

			if (leftOK && rightOK)
			{
				// '=' ��T��
				size_t eqPos = line.find('=', tagPos + tag.size());
				if (eqPos == string_view::npos) break;

				string_view valPart = line.substr(eqPos + 1);
				valPart = Trim(valPart);

				// �󔒋�؂�� index �Ԗڂ̒l���擾
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
				return nullopt; // index���͈͊O
			}
			tagPos = line.find(tag, tagPos + 1);
		}
	}
	return nullopt;
}

//-----------------------
// ������̎w��͈͂𔲂��o���֐�
//-----------------------
inline optional<string_view> ExtractBlock(const string_view source, const string_view tag, const string_view endtag, const Index index = 0)
{
	size_t pos = 0;
	size_t currentIndex = 0;

	while (pos < source.size())
	{
		// �J�n�^�O��T��
		size_t startPos = source.find(tag, pos);
		if (startPos == std::string_view::npos) break;

		// ���g�̊J�n�ʒu
		size_t contentStart = startPos + tag.size();

		// �I���^�O��T���i�J�n�^�O�̌�j
		size_t endPos = source.find(endtag, contentStart);
		if (endPos == std::string_view::npos) break;

		// �K�v�� index �ɓ��B������^�O���܂܂Ȃ����g�̂ݕԂ�
		if (currentIndex == index)
		{
			return source.substr(contentStart, endPos - contentStart);
		}

		++currentIndex;
		pos = endPos + endtag.size(); // ���̌�����
	}

	return std::nullopt;
}

//-----------------------
// Swap���g����vector�̗v�f�폜
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