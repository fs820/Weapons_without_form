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

#if 0

//-------------------------------
// �C���v�b�g�N���X �V���O���g��
//-------------------------------
class CInput
{
// ���J
public:
	// ���ۃ{�^���^�C�v
	using BUTTON = enum
	{
		INTERACT = 0,
		ATTACK,
		JUMP,
		MAX
	};
	static constexpr float RELEASE_TIME = 2.0f;          // Release�\����
	static constexpr float REPEAT_START_TIME = 1.0f;     // Repeat�J�n����
	static constexpr float REPEAT_INTERVAL_TIME = 1.0f;  // Repeat�Ԋu

	static CInput& GetInstance(void) { return m_instance; }
	CInput(const CInput&) = delete;
	CInput& operator=(const CInput&) = delete;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool IsPress(BUTTON button);
	bool IsTrigger(BUTTON button);
	bool IsRelease(BUTTON button);
	bool IsRepeat(BUTTON button);
	bool IsTriggerRepeat(BUTTON button) { return IsTrigger(button) || IsRepeat(button); }

// ����J
private:
	static CInput m_instance; // �C���X�^���X

	CInputKeyboard* m_pKeyboard;     // �L�[�{�[�h
	CInputMouse* m_pMouse;           // �}�E�X
	CInputController* m_pController; // �R���g���[���[

	CInput() :m_pKeyboard{}, m_pMouse{}, m_pController{} {};
	~CInput() = default;
};

//-----------------------------
// �L�[�{�[�h�N���X
//-----------------------------
class CInputKeyboard
{
// ���J
public:
	CInputKeyboard() : m_pRaw{}, m_pDirect{} {}
	~CInputKeyboard() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool IsPress(CInput::BUTTON button);
	bool IsTrigger(CInput::BUTTON button);
	bool IsRelease(CInput::BUTTON button);
	bool IsRepeat(CInput::BUTTON button);

// ����J
private:
	CInputRawInputKeyboard* m_pRaw;       // rawInput
	CInputDirectInputKeyboard* m_pDirect; // DirectInput
};

//-----------------------------
// �}�E�X�N���X
//-----------------------------
class CInputMouse
{
// ���J
public:
	// �}�E�X�̈ړ��l
	using MouseMove = struct
	{
		float x;
		float y;
	};

	CInputMouse() : m_pRaw{}, m_pDirect{} {}
	~CInputMouse() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool IsPress(CInput::BUTTON button);
	bool IsTrigger(CInput::BUTTON button);
	bool IsRelease(CInput::BUTTON button);
	bool IsRepeat(CInput::BUTTON button);
	MouseMove GetMove(void);
	float GetWheel(void);

	// ����J
private:
	static constexpr float MOUSE_INA = 15.0f;         // �}�E�X�ړ���
	static constexpr float MOUSE_WHEEL_INA = 300.0f;  // �}�E�X�z�C�[���ړ���

	CInputRawInputMouse* m_pRaw;       // rawInput
	CInputDirectInputMouse* m_pDirect; // DirectInput
};

//-----------------------------
// �R���g���[���[�N���X
//-----------------------------
class CInputController
{
// ���J
public:
	//�R���g���[���[�̎��
	using CONTROLLER = enum
	{
		P1 = 0,
		P2,
		P3,
		P4,
		MAX
	};

	CInputController();
	~CInputController();

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool IsPress(CONTROLLER controller, CInput::BUTTON button);
	bool IsTrigger(CONTROLLER controller, CInput::BUTTON button);
	bool IsRelease(CONTROLLER controller, CInput::BUTTON button);
	bool IsRepeat(CONTROLLER controller, CInput::BUTTON button);
	void SetVibrate(CONTROLLER controller, float fLeftPower, float fReghtPower);

	bool IsXInputControllerConnected(CONTROLLER Controller);
	bool IsDirectInputControllerConnected(CONTROLLER Controller);
	string_view ControllerName(CONTROLLER Controller);
	int ControllerNum(void);

// ����J
private:
	CInputXInputController* m_pX;           // XInput
	CInputDirectInputController* m_pDirect; // DirectInput
};

//-----------------------------
// rawInput�L�[�{�[�h�N���X
//-----------------------------
class CInputRawInputKeyboard
{
// ���J
public:
	CInputRawInputKeyboard();
	~CInputRawInputKeyboard();

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool IsPress(size_t key);
	bool IsTrigger(size_t key);
	bool IsRelease(size_t key);
	bool IsRepeat(size_t key);

// ����J
private:
	static constexpr size_t MAX_KEY = 256; // �L�[��

	BYTE m_KeyState[MAX_KEY];	  // �L�[���
	BYTE m_KeyStateOld[MAX_KEY];  // �O��L�[���

	bool m_bKeyPress[MAX_KEY];	  // �L�[�������
	bool m_bKeyTrigger[MAX_KEY];  // �L�[���͏��
	bool m_bKeyRelease[MAX_KEY];  // �L�[�������
	bool m_bKeyRepeat[MAX_KEY];   // �L�[���������

	int m_KeyPressTime[MAX_KEY];  // �L�[��������
};

//-----------------------------
// rawInput�}�E�X�N���X
//-----------------------------
class CInputRawInputMouse
{
	// ���J
public:
	//�}�E�X�{�^���̎��
	using BUTTON = enum
	{
		LEFT = 0,
		RIGHT,
		SENTER,
		B1,
		B2,
		B3,
		B4,
		B5,
		MAX
	};

	CInputRawInputMouse() : m_MouseState{}, m_MouseStateOld{}, m_bPress{}, m_bTrigger{}, m_bRelease{}, m_bRepeat{}, m_PressTime{}, m_RepeatTime{}, m_Move{}, m_WheelMove{} {}
	~CInputRawInputMouse() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool IsPress(BUTTON button);
	bool IsTrigger(BUTTON button);
	bool IsRelease(BUTTON button);
	bool IsRepeat(BUTTON button);
	CInputMouse::MouseMove GetMove(void);
	float GetWheel(void);

	// ����J
private:
	LPDIRECTINPUTDEVICE8 m_pDevice;        // �C���v�b�g�f�o�C�X

	DIMOUSESTATE2 m_MouseState;	   // ���
	DIMOUSESTATE2 m_MouseStateOld; // �O����

	bool m_bPress[MAX];	           // �{�^���������
	bool m_bTrigger[MAX];          // �{�^�����͏��
	bool m_bRelease[MAX];          // �{�^���������
	bool m_bRepeat[MAX];           // �{�^�����������
	CInputMouse::MouseMove m_Move; // �}�E�X�ړ���
	float m_WheelMove;             // �z�C�[���ړ���

	float m_PressTime[MAX];  // �{�^����������
	float m_RepeatTime[MAX]; // �{�^����������
};

//-----------------------------
// XInput�R���g���[���[�N���X
//-----------------------------
class CInputXInputController
{
// ���J
public:
	//X�p�b�h�{�^���̎��
	using JOYKEY = enum
	{
		UP = 0,
		DOWN,
		LEFT,
		RIGHT,
		START,
		BACK,
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
		MAX
	};

	CInputXInputController() :m_state{} {};
	~CInputXInputController() = default;

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	bool IsPress(JOYKEY key);
	bool IsTrigger(JOYKEY key);
	bool IsRelease(JOYKEY key);
	bool IsRepeat(JOYKEY key);
	float GetLeftStickAngle(void);
	float GetRightStickAngle(void);
	float GetLeftStickLength(void);
	float GetRightStickLength(void);
	void Vibrate(WORD wLeftMotorSpeed, WORD wRightMotorSpeed);

// ����J
private:
	static constexpr WORD VIBRATION_MAX = WORD(65535); // �o�C�u���[�V�����l
	static constexpr SHORT STICK_NUM = SHORT(32767);   // �X�e�B�b�N�̒l

	XINPUT_STATE m_state;
};

//-----------------------------
// DirectInput�Ǘ��N���X
//-----------------------------
class CInputDirectInput
{
// ���J
public:
	CInputDirectInput() = delete;

	static HRESULT Register(HINSTANCE hInstanse, HWND hWnd);
	static void Unregister(void);

	static LPDIRECTINPUT8 GetDirectInput(void) { return m_pInput; }

	static HRESULT SetEnum(void);

// ����J
private:
	static BOOL CALLBACK EnumDevicesCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);

	static LPDIRECTINPUT8 m_pInput;
	static size_t m_useCount;
};

//-----------------------------
// DirectInput�L�[�{�[�h�N���X
//-----------------------------
class CInputDirectInputKeyboard
{
	// ���J
public:
	CInputDirectInputKeyboard() : m_KeyState{}, m_KeyStateOld{}, m_bKeyPress{}, m_bKeyTrigger{}, m_bKeyRelease{}, m_bKeyRepeat{}, m_KeyPressTime{}, m_KeyRepeatTime{} {}
	~CInputDirectInputKeyboard() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool IsPress(size_t key);
	bool IsTrigger(size_t key);
	bool IsRelease(size_t key);
	bool IsRepeat(size_t key);

	// ����J
private:
	static constexpr size_t MAX_KEY = 256; // �L�[��

	LPDIRECTINPUTDEVICE8 m_pDevice;        // �C���v�b�g�f�o�C�X

	BYTE m_KeyState[MAX_KEY];	  // �L�[���
	BYTE m_KeyStateOld[MAX_KEY];  // �O��L�[���

	bool m_bKeyPress[MAX_KEY];	  // �L�[�������
	bool m_bKeyTrigger[MAX_KEY];  // �L�[���͏��
	bool m_bKeyRelease[MAX_KEY];  // �L�[�������
	bool m_bKeyRepeat[MAX_KEY];   // �L�[���������

	float m_KeyPressTime[MAX_KEY];  // �L�[��������
	float m_KeyRepeatTime[MAX_KEY]; // �L�[��������
};

//-----------------------------
// DirectInput�}�E�X�N���X
//-----------------------------
class CInputDirectInputMouse
{
// ���J
public:
	//�}�E�X�{�^���̎��
	using BUTTON = enum
	{
		LEFT = 0,
		RIGHT,
		SENTER,
		B1,
		B2,
		B3,
		B4,
		B5,
		MAX
	};

	CInputDirectInputMouse() : m_MouseState{}, m_MouseStateOld{}, m_bPress{}, m_bTrigger{}, m_bRelease{}, m_bRepeat{}, m_PressTime{}, m_RepeatTime{}, m_Move{}, m_WheelMove{} {}
	~CInputDirectInputMouse() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool IsPress(BUTTON button);
	bool IsTrigger(BUTTON button);
	bool IsRelease(BUTTON button);
	bool IsRepeat(BUTTON button);
	CInputMouse::MouseMove GetMove(void);
	float GetWheel(void);

// ����J
private:
	LPDIRECTINPUTDEVICE8 m_pDevice;        // �C���v�b�g�f�o�C�X

	DIMOUSESTATE2 m_MouseState;	   // ���
	DIMOUSESTATE2 m_MouseStateOld; // �O����

	bool m_bPress[MAX];	           // �{�^���������
	bool m_bTrigger[MAX];          // �{�^�����͏��
	bool m_bRelease[MAX];          // �{�^���������
	bool m_bRepeat[MAX];           // �{�^�����������
	CInputMouse::MouseMove m_Move; // �}�E�X�ړ���
	float m_WheelMove;             // �z�C�[���ړ���

	float m_PressTime[MAX];  // �{�^����������
	float m_RepeatTime[MAX]; // �{�^����������
};

//-----------------------------
// DirectInput�R���g���[���[�N���X
//-----------------------------
class CInputDirectInputController
{
	// ���J
public:
	//ELE�p�b�h�{�^���̎��
	using ELEKEY = enum
	{
		A = 0,
		B,
		X,
		Y,
		LB,
		RB,
		LT,
		RT,
		L3,
		R3,
		BACK,
		START,
		SENTER,
		MAX
	};

	//PS�p�b�h�{�^���̎��
	using PSKEY = enum
	{
		SQ = 0,
		CR,
		CI,
		TRA,
		LB,
		RB,
		LT,
		RT,
		BACK,
		START,
		L3,
		R3,
		SENTER,
		PAD,
		MAX
	};

	//NIN�p�b�h�{�^���̎��
	using NINKEY = enum
	{
		B = 0,
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
		HOME,
		CAP,
		MAX
	};

	//D�p�b�h�{�^���̎��
	using DKEY = enum
	{
		A = 0,
		B,
		X,
		Y,
		LB,
		RB,
		LT,
		RT,
		L3,
		R3,
		BACK,
		START,
		SENTER,
		B1,
		B2,
		MAX
	};

	CInputDirectInputController() : m_pDevice{}, m_state{}, m_effect{}, m_sName{}, m_pDiInstance{} {}
	~CInputDirectInputController() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool IsPress(size_t key);
	bool IsTrigger(size_t key);
	bool IsRelease(size_t key);
	bool IsRepeat(size_t key);
	float GetLeftStickAngle();
	float GetRightStickAngle();
	float GetLeftStickLength();
	float GetRightStickLength();
	float GetSlider(int nSlider);
	bool IsPressUp(int nPov = 0);
	bool IsPressDown(int nPov = 0);
	bool IsPressLeft(int nPov = 0);
	bool IsPressRight(int nPov = 0);
	bool IsTriggerUp(int nPov = 0);
	bool IsTriggerDown(int nPov = 0);
	bool IsTriggerLeft(int nPov = 0);
	bool IsTriggerRight(int nPov = 0);
	bool IsReleaseUp(int nPov = 0);
	bool IsReleaseDown(int nPov = 0);
	bool IsReleaseLeft(int nPov = 0);
	bool IsReleaseRight(int nPov = 0);
	bool IsRepeatUp(int nPov = 0);
	bool IsRepeatDown(int nPov = 0);
	bool IsRepeatLeft(int nPov = 0);
	bool IsRepeatRight(int nPov = 0);
	void Vibrate(LONG lMotorPower);
	void VibrateStop();

	// ����J
private:
	static constexpr int KEY_MAX = 32; //dinput�{�^���̍ő吔

	// POV
	static constexpr int POV_MAX = 4;                // POV�̐�
	static constexpr int POV_SYS_MAX = 8;            // POV�̕�����
	static constexpr float POV_NUM = 4500.0f;        // POV�̕����W��
	static constexpr float POV_UP = 0.0f;            //��
	static constexpr float POV_RIGHTUP = 4500.0f;    //�E��
	static constexpr float POV_RIGHT = 9000.0f;      //�E
	static constexpr float POV_RIGHTDOWN = 13500.0f; //�E��
	static constexpr float POV_DOWN = 18000.0f;      //��
	static constexpr float POV_LEFTDOWN = 22500.0f;  //����
	static constexpr float POV_LEFT = 27000.0f;      //��
	static constexpr float POV_LEFTUP = 31500.0f;    //����

	static constexpr float DSTICK_NUM = -1.0f; // �X�e�B�b�N�v�Z�p

	LPDIRECTINPUTDEVICE8 m_pDevice;        // �C���v�b�g�f�o�C�X

	DIJOYSTATE m_state;//d�p�b�h���
	LPDIRECTINPUTEFFECT m_effect;
	string_view m_sName;
	const DIDEVICEINSTANCE* m_pDiInstance;
};

#endif