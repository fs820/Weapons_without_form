//---------------------------------------
//
// ���͏����̒�` [input.h]
// Author: Fuma Sato
//
//---------------------------------------
#pragma once
#include"main.h" // ���C��

// dInput��XInput�������N
#define DIRECTINPUT_VERSION  0x0800
#include <dinput.h>
#include <xinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "xinput.lib")

// �g���K�[�̃f�W�^�����͂悤�}�N��
#ifndef XINPUT_GAMEPAD_LEFT_TRIGGER
#define XINPUT_GAMEPAD_LEFT_TRIGGER  0x0400
#endif

#ifndef XINPUT_GAMEPAD_RIGHT_TRIGGER
#define XINPUT_GAMEPAD_RIGHT_TRIGGER 0x0800
#endif

// input�p��`
namespace input
{
	// time
	constexpr float RELEASE_TIME = 2.0f;          // Release�\����
	constexpr float REPEAT_START_TIME = 1.0f;     // Repeat�J�n����
	constexpr float REPEAT_INTERVAL_TIME = 1.0f;  // Repeat�Ԋu

	// Keyboard
	constexpr size_t MAX_KEY = 256u; // �L�[��

	// Mouse
	constexpr float MOUSE_INA = 15.0f;         // �}�E�X�ړ���
	constexpr float MOUSE_WHEEL_INA = 300.0f;  // �}�E�X�z�C�[���ړ���
	
	// Controller
	constexpr WORD VIBRATION_MAX = static_cast<WORD>(65535); // �o�C�u���[�V�����l
	constexpr SHORT STICK_NUM = static_cast<SHORT>(32767);   // �X�e�B�b�N�̒l

	constexpr int KEY_MAX = 32; //dinput�{�^���̍ő吔

	// GUID
	constexpr GUID ELECOM = GUID(0);
	constexpr GUID PlayStation = GUID(0);
	constexpr GUID Nintendo = GUID(0);

	// POV
	constexpr int POV_MAX = 4;                // POV�̐�
	constexpr int POV_SYS_MAX = 8;            // POV�̕�����
	constexpr float POV_NUM = 4500.0f;        // POV�̕����W��
	constexpr float POV_UP = 0.0f;            // ��
	constexpr float POV_RIGHTUP = 4500.0f;    // �E��
	constexpr float POV_RIGHT = 9000.0f;      // �E
	constexpr float POV_RIGHTDOWN = 13500.0f; // �E��
	constexpr float POV_DOWN = 18000.0f;      // ��
	constexpr float POV_LEFTDOWN = 22500.0f;  // ����
	constexpr float POV_LEFT = 27000.0f;      // ��
	constexpr float POV_LEFTUP = 31500.0f;    // ����

	// DirectInputRange
	constexpr LONG DIRECTINPUT_TRIGGER_MIN = 0l;     // �g���K�[��X���C�_�[�̍ŏ��l
	constexpr LONG DIRECTINPUT_TRIGGER_MAX = 10000l; // �g���K�[��X���C�_�[�̍ő�l
	constexpr LONG DIRECTINPUT_AXIS_MIN = -1000l;    // ���̍ŏ��l
	constexpr LONG DIRECTINPUT_AXIS_MAX = 1000l;     // ���̍ő�l

	// ���ۃ{�^���^�C�v
	enum class BUTTON : Index8
	{
		Interact,
		Attack,
		Jump,
		Max
	};
	
	//�}�E�X�{�^���̎��
	enum class MOUSE_BUTTON : Index8
	{
		Left,
		Right,
		Senter,
		B1,
		B2,
		B3,
		B4,
		B5,
		Max
	};

	// �{�^���̎��
	enum class CONTROLLER_BUTTON : Index8
	{
		Up,
		Down,
		Left,
		Right,
		Start,
		Back,
		L3,
		R3,
		LB,
		RB,
		LT,
		RT,
		BottomButton,
		RightFaceButton,
		LeftFaceButton,
		TopButton,
		Max
	};

	//X�p�b�h�{�^���̎��
	enum class JOYKEY : Index8
	{
		Up,
		Down,
		Left,
		Right,
		Start,
		Back,
		L3,
		R3,
		LB,
		RB,
		LT,
		RT,
		A,
		B,
		X,
		Y,
		Max
	};

	//ELE�p�b�h�{�^���̎��
	enum class ELEKEY : Index8
	{
		A,
		B,
		X,
		Y,
		LB,
		RB,
		LT,
		RT,
		L3,
		R3,
		Back,
		Start,
		Senter,
		Max
	};

	//PS�p�b�h�{�^���̎��
	enum class PSKEY : Index8
	{
		Sq,
		Cr,
		Ci,
		Tra,
		LB,
		RB,
		LT,
		RT,
		Back,
		Start,
		L3,
		R3,
		Senter,
		Pad,
		Max
	};

	//NIN�p�b�h�{�^���̎��
	enum class NINKEY : Index8
	{
		B,
		A,
		Y,
		X,
		LB,
		RB,
		LT,
		RT,
		�|,
		�{,
		L3,
		R3,
		Home,
		Cap,
		Max
	};

	//D�p�b�h�{�^���̎��
	enum class DKEY : Index8
	{
		A,
		B,
		X,
		Y,
		LB,
		RB,
		LT,
		RT,
		L3,
		R3,
		Back,
		Start,
		Senter,
		B1,
		B2,
		Max
	};

	//D�p�b�h�{�^���̎��
	enum class CONTROLLER_TYPE : Index8
	{
		XInput,
		Ele,
		Ps,
		Nin,
		DirectInput,
		Max
	};

	//---------------------------------------------------------------------
	// �R���g���[���[���Ƃ̃{�^�����R���g���[���[���ʃ{�^���z�u�ɂ���Ƃ��̑Ή��\
	//---------------------------------------------------------------------
	constexpr array<array<CONTROLLER_BUTTON, Index8(CONTROLLER_BUTTON::Max)>, Index8(CONTROLLER_TYPE::Max)> ControllerMapList =
	{
		(
			CONTROLLER_BUTTON::Up,
			CONTROLLER_BUTTON::Down,
			CONTROLLER_BUTTON::Left,
			CONTROLLER_BUTTON::Right,
			CONTROLLER_BUTTON::Start,
			CONTROLLER_BUTTON::Back,
			CONTROLLER_BUTTON::L3,
			CONTROLLER_BUTTON::R3,
			CONTROLLER_BUTTON::LB,
			CONTROLLER_BUTTON::RB,
			CONTROLLER_BUTTON::LT,
			CONTROLLER_BUTTON::RT,
			CONTROLLER_BUTTON::BottomButton,
			CONTROLLER_BUTTON::RightFaceButton,
			CONTROLLER_BUTTON::LeftFaceButton,
			CONTROLLER_BUTTON::TopButton
		),
		(
			CONTROLLER_BUTTON::Up,
			CONTROLLER_BUTTON::Down,
			CONTROLLER_BUTTON::Left,
			CONTROLLER_BUTTON::Right,
			CONTROLLER_BUTTON::Start,
			CONTROLLER_BUTTON::Back,
			CONTROLLER_BUTTON::L3,
			CONTROLLER_BUTTON::R3,
			CONTROLLER_BUTTON::LB,
			CONTROLLER_BUTTON::RB,
			CONTROLLER_BUTTON::LT,
			CONTROLLER_BUTTON::RT,
			CONTROLLER_BUTTON::BottomButton,
			CONTROLLER_BUTTON::RightFaceButton,
			CONTROLLER_BUTTON::LeftFaceButton,
			CONTROLLER_BUTTON::TopButton
		),
		(
			CONTROLLER_BUTTON::Up,
			CONTROLLER_BUTTON::Down,
			CONTROLLER_BUTTON::Left,
			CONTROLLER_BUTTON::Right,
			CONTROLLER_BUTTON::Start,
			CONTROLLER_BUTTON::Back,
			CONTROLLER_BUTTON::L3,
			CONTROLLER_BUTTON::R3,
			CONTROLLER_BUTTON::LB,
			CONTROLLER_BUTTON::RB,
			CONTROLLER_BUTTON::LT,
			CONTROLLER_BUTTON::RT,
			CONTROLLER_BUTTON::BottomButton,
			CONTROLLER_BUTTON::RightFaceButton,
			CONTROLLER_BUTTON::LeftFaceButton,
			CONTROLLER_BUTTON::TopButton
		),
		(
			CONTROLLER_BUTTON::Up,
			CONTROLLER_BUTTON::Down,
			CONTROLLER_BUTTON::Left,
			CONTROLLER_BUTTON::Right,
			CONTROLLER_BUTTON::Start,
			CONTROLLER_BUTTON::Back,
			CONTROLLER_BUTTON::L3,
			CONTROLLER_BUTTON::R3,
			CONTROLLER_BUTTON::LB,
			CONTROLLER_BUTTON::RB,
			CONTROLLER_BUTTON::LT,
			CONTROLLER_BUTTON::RT,
			CONTROLLER_BUTTON::BottomButton,
			CONTROLLER_BUTTON::RightFaceButton,
			CONTROLLER_BUTTON::LeftFaceButton,
			CONTROLLER_BUTTON::TopButton
		),
		(
			CONTROLLER_BUTTON::Up,
			CONTROLLER_BUTTON::Down,
			CONTROLLER_BUTTON::Left,
			CONTROLLER_BUTTON::Right,
			CONTROLLER_BUTTON::Start,
			CONTROLLER_BUTTON::Back,
			CONTROLLER_BUTTON::L3,
			CONTROLLER_BUTTON::R3,
			CONTROLLER_BUTTON::LB,
			CONTROLLER_BUTTON::RB,
			CONTROLLER_BUTTON::LT,
			CONTROLLER_BUTTON::RT,
			CONTROLLER_BUTTON::BottomButton,
			CONTROLLER_BUTTON::RightFaceButton,
			CONTROLLER_BUTTON::LeftFaceButton,
			CONTROLLER_BUTTON::TopButton
		)
	};

	// ������
	struct Axis
	{
		float x;
		float y;

		float Angle(void) { return atan2f(y, x); }
		void Angle(float* pAngle) { *pAngle = atan2f(y, x); }
		float Length(void) { return sqrtf(x * x + y * y); }
		void Length(float* pLength) { *pLength = sqrtf(x * x + y * y); }

		Axis() : x{}, y{} {}
		~Axis() = default;
	};
}

// �Ǘ�
class CInputKeyboardManager;   // �L�[�{�[�h
class CInputMouseManager;      // �}�E�X
class CInputControllerManager; // �R���g���[���[

// ���
class CInputKeyboard;   // �L�[�{�[�h
class CInputMouse;		// �}�E�X
class CInputController;	// �R���g���[���[

// rawInput�{��
class CInputRawInput;

// �h��
class CInputRawInputKeyboard; // �L�[�{�[�h
class CInputRawInputMouse;	  // �}�E�X
class CInputXInputController; // �R���g���[���[

// DirectInput�{��
class CInputDirectInput;

// �h��
class CInputDirectInputKeyboard;   // �L�[�{�[�h
class CInputDirectInputMouse;	   // �}�E�X
class CInputDirectInputController; // �R���g���[���[

//-------------------------------
// �C���v�b�g�N���X �V���O���g��
//-------------------------------
class CInput final
{
// ���J
public:
	static CInput& GetInstance(void) { return m_instance; }
	CInput(const CInput&) = delete;
	CInput& operator=(const CInput&) = delete;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool IsPress(Index8 idx, input::BUTTON button);
	bool IsTrigger(Index8 idx, input::BUTTON button);
	bool IsRelease(Index8 idx, input::BUTTON button);
	bool IsRepeat(Index8 idx, input::BUTTON button);
	bool IsTriggerRepeat(Index8 idx, input::BUTTON button) { return IsTrigger(idx, button) || IsRepeat(idx, button); }

	size_t Count(void);

// ����J
private:
	static CInput m_instance; // �C���X�^���X

	CInputKeyboardManager* m_pKeyboard;     // �L�[�{�[�h�Ǘ�
	CInputMouseManager* m_pMouse;           // �}�E�X�Ǘ�
	CInputControllerManager* m_pController; // �R���g���[���[�Ǘ�

	CInput() :m_pKeyboard{}, m_pMouse{}, m_pController{} {};
	~CInput() = default;
};

//-----------------------------
// �L�[�{�[�h�Ǘ��N���X
//-----------------------------
class CInputKeyboardManager
{
// ���J
public:
	CInputKeyboardManager() : m_apKeyboard{} {}
	~CInputKeyboardManager() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool IsPress(Index8 idx, input::BUTTON button);
	bool IsTrigger(Index8 idx, input::BUTTON button);
	bool IsRelease(Index8 idx, input::BUTTON button);
	bool IsRepeat(Index8 idx, input::BUTTON button);
	size_t Count(void) { return m_apKeyboard.size(); }

// ����J
private:
	vector<CInputKeyboard*> m_apKeyboard; // Keyboard�N���X�|�C���^
};

//-----------------------------
// �}�E�X�Ǘ��N���X
//-----------------------------
class CInputMouseManager
{
// ���J
public:
	CInputMouseManager() : m_apMouse{} {}
	~CInputMouseManager() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool IsPress(Index8 idx, input::BUTTON button);
	bool IsTrigger(Index8 idx, input::BUTTON button);
	bool IsRelease(Index8 idx, input::BUTTON button);
	bool IsRepeat(Index8 idx, input::BUTTON button);
	input::Axis GetAxis(Index8 idx);
	float GetWheel(Index8 idx);
	size_t Count(void) { return m_apMouse.size(); }

	// ����J
private:
	vector<CInputMouse*> m_apMouse;    // Mouse�N���X�|�C���^
};

//-----------------------------
// �R���g���[���[�Ǘ��N���X
//-----------------------------
class CInputControllerManager
{
// ���J
public:
	CInputControllerManager() : m_apController{} {};
	~CInputControllerManager() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool IsPress(Index idx, input::BUTTON button);
	bool IsTrigger(Index idx, input::BUTTON button);
	bool IsRelease(Index idx, input::BUTTON button);
	bool IsRepeat(Index idx, input::BUTTON button);
	void SetVibrate(Index idx, float fLeftPower, float fReghtPower);

	void Register(CInputController* pController) { m_apController.push_back(pController); }

	bool IsXInputControllerConnected(Index8 idx);
	bool IsDirectInputControllerConnected(Index8 idx);
	size_t Count(void) { return m_apController.size(); }

// ����J
private:
	vector<CInputController*> m_apController; // Controller�N���X�|�C���^
};

//-----------------------------
// �L�[�{�[�h�N���X
//-----------------------------
class CInputKeyboard
{
// ���J
public:
	CInputKeyboard() : m_key{}, m_keyOld{}, m_bPress{}, m_bTrigger{}, m_bRelease{}, m_bRepeat{}, m_PressTime{}, m_RepeatTime{} {};
	virtual ~CInputKeyboard() = default;

	virtual HRESULT Init(HINSTANCE hInstanse, HWND hWnd) = 0;
	virtual void Uninit(void) = 0;
	void Update(void);

	bool IsPress(Index8 key) const { return m_bPress[key]; }
	bool IsTrigger(Index8 key) const { return m_bTrigger[key]; }
	bool IsRelease(Index8 key) const { return m_bRelease[key]; }
	bool IsRepeat(Index8 key) const { return m_bRepeat[key]; }

// �Ƒ����J
protected:
	virtual HRESULT GetKey(span<bool> key) const = 0;
// ����J
private:
	array <bool, input::MAX_KEY> m_key;	   // ����̃L�[���
	array <bool, input::MAX_KEY> m_keyOld; // �O��̃L�[���

	array<bool, input::MAX_KEY> m_bPress;	  // �L�[�������
	array<bool, input::MAX_KEY> m_bTrigger;   // �L�[���͏��
	array<bool, input::MAX_KEY> m_bRelease;   // �L�[�������
	array<bool, input::MAX_KEY> m_bRepeat;    // �L�[���������

	array<float, input::MAX_KEY> m_PressTime;  // �L�[��������
	array<float, input::MAX_KEY> m_RepeatTime; // �L�[��������
};

//-----------------------------
// �}�E�X�N���X
//-----------------------------
class CInputMouse
{
// ���J
public:
	CInputMouse() : m_button{}, m_buttonOld{}, m_bPress{}, m_bTrigger{}, m_bRelease{}, m_bRepeat{}, m_PressTime{}, m_RepeatTime{}, m_Move{}, m_WheelMove{} {}
	virtual ~CInputMouse() = default;

	virtual HRESULT Init(HINSTANCE hInstanse, HWND hWnd) = 0;
	virtual void Uninit(void) = 0;
	void Update(void);

	bool IsPress(input::MOUSE_BUTTON button) const { return m_bPress[Index8(button)]; }
	bool IsTrigger(input::MOUSE_BUTTON button) const { return m_bTrigger[Index8(button)]; }
	bool IsRelease(input::MOUSE_BUTTON button) const { return m_bRelease[Index8(button)]; }
	bool IsRepeat(input::MOUSE_BUTTON button) const { return m_bRepeat[Index8(button)]; }
	input::Axis GetAxis(void) const { return m_Move; }
	float GetWheel(void) const { return m_WheelMove; }

// �Ƒ����J
protected:
	virtual HRESULT GetButton(span<bool> button) const = 0;
	virtual HRESULT GetMove(input::Axis* pMove) const = 0;
	virtual HRESULT GetWheel(float* pWheel) const = 0;

// ����J
private:
	array<bool, Index8(input::MOUSE_BUTTON::Max)> m_button;	   // ����̃{�^�����
	array<bool, Index8(input::MOUSE_BUTTON::Max)> m_buttonOld; // �O��̃{�^�����

	array<bool, Index8(input::MOUSE_BUTTON::Max)> m_bPress;	  // �{�^���������
	array<bool, Index8(input::MOUSE_BUTTON::Max)> m_bTrigger; // �{�^�����͏��
	array<bool, Index8(input::MOUSE_BUTTON::Max)> m_bRelease; // �{�^���������
	array<bool, Index8(input::MOUSE_BUTTON::Max)> m_bRepeat;  // �{�^�����������

	input::Axis m_Move; // �}�E�X�ړ���
	float m_WheelMove;  // �z�C�[���ړ���

	array<float, Index8(input::MOUSE_BUTTON::Max)> m_PressTime;  // �{�^����������
	array<float, Index8(input::MOUSE_BUTTON::Max)> m_RepeatTime; // �{�^����������
};

//-----------------------------
// �R���g���[���[�N���X
//-----------------------------
class CInputController
{
	// ���J
public:
	CInputController() : m_button{}, m_buttonOld{}, m_bPress{}, m_bTrigger{}, m_Slider{}, m_bRelease{}, m_bRepeat{}, m_Stick{}, m_Trigger{}, m_PressTime{}, m_RepeatTime{} {};
	virtual ~CInputController() = default;

	virtual HRESULT Init(HINSTANCE hInstanse, HWND hWnd, Index8 idx = INVALID_ID8) = 0;
	virtual void Uninit(void) = 0;
	void Update(void);

	bool IsPress(input::CONTROLLER_BUTTON button) const { return m_bPress[Index8(button)]; }
	bool IsTrigger(input::CONTROLLER_BUTTON button) const { return m_bTrigger[Index8(button)]; }
	bool IsRelease(input::CONTROLLER_BUTTON button) const { return m_bRelease[Index8(button)]; }
	bool IsRepeat(input::CONTROLLER_BUTTON button) const { return m_bRepeat[Index8(button)]; }
	input::Axis GetAxis(LR lr) const { return m_Stick[lr]; }
	float GetTrigger(LR lr) const { return m_Trigger[lr]; }
	float GetSlider(LR lr) const { return m_Slider[lr]; }
	virtual void Vibrate(float leftMotorSpeed = 1.0f, float rightMotorSpeed = 1.0f) = 0;

// �Ƒ����J
protected:
	virtual HRESULT GetButton(span<bool> button) const = 0;
	virtual HRESULT GetStick(span<input::Axis> axis) const = 0;
	virtual HRESULT GetTrigger(span<float> trigger) const = 0;
	virtual HRESULT GetSlider(span<float> slider) const = 0;

// ����J
private:
	array<bool, Index8(input::CONTROLLER_BUTTON::Max)> m_button;    // ����̃{�^�����
	array<bool, Index8(input::CONTROLLER_BUTTON::Max)> m_buttonOld; // �O��̃{�^�����

	array<bool, Index8(input::CONTROLLER_BUTTON::Max)> m_bPress;   // �{�^���������
	array<bool, Index8(input::CONTROLLER_BUTTON::Max)> m_bTrigger; // �{�^�����͏��
	array<bool, Index8(input::CONTROLLER_BUTTON::Max)> m_bRelease; // �{�^���������
	array<bool, Index8(input::CONTROLLER_BUTTON::Max)> m_bRepeat;  // �{�^�����������

	array <input::Axis, 2> m_Stick; // �X�e�B�b�N
	array <float, 2> m_Trigger;     // �g���K�[����
	array <float, 2>  m_Slider;     // �X���C�_�[����

	array<float, Index8(input::CONTROLLER_BUTTON::Max)> m_PressTime;  // �{�^����������
	array<float, Index8(input::CONTROLLER_BUTTON::Max)> m_RepeatTime; // �{�^����������
};

//-----------------------------
// rawInput�Ǘ��N���X
//-----------------------------
class CInputRawInput
{
	// ���J
public:
	CInputRawInput() = delete;

	static size_t GetCount(void) { return m_useCount; }

	static HRESULT AnalysisRawData(RAWINPUT* pRawData);

// ����J
private:
	static size_t m_useCount;
};

//-----------------------------
// rawInput�L�[�{�[�h�N���X
//-----------------------------
class CInputRawInputKeyboard : public CInputKeyboard
{
// ���J
public:
	CInputRawInputKeyboard() = default;
	~CInputRawInputKeyboard() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd) override;
	void Uninit(void) override;
	HRESULT GetKey(span<bool> key) const override;

// ����J
private:
};

//-----------------------------
// rawInput�}�E�X�N���X
//-----------------------------
class CInputRawInputMouse : public CInputMouse
{
	// ���J
public:
	CInputRawInputMouse() = default;
	~CInputRawInputMouse() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd) override;
	void Uninit(void) override;
	HRESULT GetButton(span<bool> button) const override;
	HRESULT GetMove(input::Axis* pMove) const override;
	HRESULT GetWheel(float* pWheel) const override;

	// ����J
private:
};

//-----------------------------
// XInput�R���g���[���[�N���X
//-----------------------------
class CInputXInputController : public CInputController
{
// ���J
public:
	CInputXInputController() : m_idx{} {}
	~CInputXInputController() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd, Index8 idx = INVALID_ID8) override;
	void Uninit(void) override {};
	HRESULT GetButton(span<bool> button) const override;
	HRESULT GetStick(span<input::Axis> axis) const override;
	HRESULT GetTrigger(span<float> trigger) const override;
	HRESULT GetSlider(span<float> slider) const override;
	void Vibrate(float leftMotorSpeed = 1.0f, float rightMotorSpeed = 1.0f)  override;

// ����J
private:
	void SetTriggerButton(PXINPUT_STATE pState) const;
	WORD GetXInputMask(input::JOYKEY key) const;

	Index8 m_idx;
};

//-----------------------------
// DirectInput�Ǘ��N���X
//-----------------------------
class CInputDirectInput
{
// ���J
public:
	CInputDirectInput() = delete;

	static HRESULT Register(HINSTANCE hInstanse);
	static void Unregister(void);

	static LPDIRECTINPUT8 GetDirectInput(void) { return m_pInput; }

	static HRESULT SetEnum(CInputControllerManager* pControllerManager);

// ����J
private:
	static BOOL CALLBACK EnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
	
	static LPDIRECTINPUT8 m_pInput;
	static size_t m_useCount;
};

//-----------------------------
// DirectInput�L�[�{�[�h�N���X
//-----------------------------
class CInputDirectInputKeyboard : public CInputKeyboard
{
// ���J
public:
	CInputDirectInputKeyboard() : m_pDevice{} {}
	~CInputDirectInputKeyboard() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd) override;
	void Uninit(void) override;
	HRESULT GetKey(span<bool> key) const override;

// ����J
private:
	HRESULT SetProperty(void);

	LPDIRECTINPUTDEVICE8 m_pDevice;        // �C���v�b�g�f�o�C�X
};

//-----------------------------
// DirectInput�}�E�X�N���X
//-----------------------------
class CInputDirectInputMouse : public CInputMouse
{
// ���J
public:
	CInputDirectInputMouse() : m_pDevice{} {}
	~CInputDirectInputMouse() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd) override;
	void Uninit(void) override;
	HRESULT GetButton(span<bool> button) const override;
	HRESULT GetMove(input::Axis* pMove) const override;
	HRESULT GetWheel(float* pWheel) const override;

// ����J
private:
	HRESULT SetProperty(void);

	LPDIRECTINPUTDEVICE8 m_pDevice;        // �C���v�b�g�f�o�C�X
};

//-----------------------------
// DirectInput�R���g���[���[�N���X
//-----------------------------
class CInputDirectInputController : public CInputController
{
// ���J
public:
	CInputDirectInputController() : m_pDevice{}, m_effect{}, m_pDiInstance{} {}
	~CInputDirectInputController() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd, Index8 idx = INVALID_ID8) override;
	void Uninit(void) override;
	HRESULT GetButton(span<bool> button) const override;
	HRESULT GetStick(span<input::Axis> axis) const override;
	HRESULT GetTrigger(span<float> trigger) const override;
	HRESULT GetSlider(span<float> slider) const override;
	void Vibrate(float leftMotorSpeed = 1.0f, float rightMotorSpeed = 1.0f)  override;

	void SetInstance(LPCDIDEVICEINSTANCE pDiInstance) { m_pDiInstance = pDiInstance; }
	LPCDIDEVICEINSTANCE GetInstance(void) { return m_pDiInstance; }

	// ����J
private:
	static BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);
	HRESULT SetProperty(void);

	LPDIRECTINPUTDEVICE8 m_pDevice;        // �C���v�b�g�f�o�C�X

	LPDIRECTINPUTEFFECT m_effect;      // �U��
	LPCDIDEVICEINSTANCE m_pDiInstance; // �f�o�C�X���
};