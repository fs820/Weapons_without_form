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

// input�p��`
namespace input
{
	// time
	static constexpr float RELEASE_TIME = 2.0f;          // Release�\����
	static constexpr float REPEAT_START_TIME = 1.0f;     // Repeat�J�n����
	static constexpr float REPEAT_INTERVAL_TIME = 1.0f;  // Repeat�Ԋu

	// Keyboard
	static constexpr size_t MAX_KEY = 256; // �L�[��

	// Mouse
	static constexpr float MOUSE_INA = 15.0f;         // �}�E�X�ړ���
	static constexpr float MOUSE_WHEEL_INA = 300.0f;  // �}�E�X�z�C�[���ړ���

	// Controller
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

	// ���ۃ{�^���^�C�v
	enum class BUTTON : Index
	{
		Interact,
		Attack,
		Jump,
		Max
	};

	//�}�E�X�{�^���̎��
	enum class MOUSE_BUTTON : Index
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
	enum class CONTROLLER_BUTTON : Index
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
	enum class JOYKEY : Index
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
	enum class ELEKEY : Index
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
	enum class PSKEY : Index
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
	enum class NINKEY : Index
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
	enum class DKEY : Index
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

	// �}�E�X�̈ړ��l
	struct MouseMove
	{
		float x;
		float y;
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
	// �N���Xusing
	using BUTTON = input::BUTTON;

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

	CInputKeyboardManager* m_pKeyboard;     // �L�[�{�[�h
	CInputMouseManager* m_pMouse;           // �}�E�X
	CInputControllerManager* m_pController; // �R���g���[���[

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
	// �N���Xusing
	using BUTTON = input::BUTTON;

	CInputKeyboardManager() : m_apKeyboard{} {}
	~CInputKeyboardManager() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool IsPress(BUTTON button);
	bool IsTrigger(BUTTON button);
	bool IsRelease(BUTTON button);
	bool IsRepeat(BUTTON button);

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
	// �N���Xusing
	using BUTTON = input::BUTTON;
	using MouseMove = input::MouseMove;

	CInputMouseManager() : m_apMouse{} {}
	~CInputMouseManager() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool IsPress(BUTTON button);
	bool IsTrigger(BUTTON button);
	bool IsRelease(BUTTON button);
	bool IsRepeat(BUTTON button);
	MouseMove GetMove(void);
	float GetWheel(void);

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
	// �N���Xusing
	using BUTTON = input::BUTTON;

	CInputControllerManager();
	~CInputControllerManager();

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool IsPress(Index controller, BUTTON button);
	bool IsTrigger(Index controller, BUTTON button);
	bool IsRelease(Index controller, BUTTON button);
	bool IsRepeat(Index controller, BUTTON button);
	void SetVibrate(Index controller, float fLeftPower, float fReghtPower);

	bool IsXInputControllerConnected(Index Controller);
	bool IsDirectInputControllerConnected(Index Controller);
	string_view ControllerName(Index Controller);
	int ControllerNum(void);

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
	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool IsPress(size_t key);
	bool IsTrigger(size_t key);
	bool IsRelease(size_t key);
	bool IsRepeat(size_t key);

	// ����J
private:
	BYTE m_KeyState[input::MAX_KEY];	 // �L�[���
	BYTE m_KeyStateOld[input::MAX_KEY];  // �O��L�[���

	bool m_bKeyPress[input::MAX_KEY];	  // �L�[�������
	bool m_bKeyTrigger[input::MAX_KEY];  // �L�[���͏��
	bool m_bKeyRelease[input::MAX_KEY];  // �L�[�������
	bool m_bKeyRepeat[input::MAX_KEY];   // �L�[���������

	int m_KeyPressTime[input::MAX_KEY];  // �L�[��������
};

//-----------------------------
// �}�E�X�N���X
//-----------------------------
class CInputMouse
{
// ���J
public:
	// �N���Xusing
	using MOUSE_BUTTON = input::MOUSE_BUTTON;
	using MouseMove = input::MouseMove;

	CInputMouse() : m_MouseState{}, m_MouseStateOld{}, m_bPress{}, m_bTrigger{}, m_bRelease{}, m_bRepeat{}, m_PressTime{}, m_RepeatTime{}, m_Move{}, m_WheelMove{} {}
	~CInputMouse() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool IsPress(MOUSE_BUTTON button);
	bool IsTrigger(MOUSE_BUTTON button);
	bool IsRelease(MOUSE_BUTTON button);
	bool IsRepeat(MOUSE_BUTTON button);
	MouseMove GetMove(void);
	float GetWheel(void);

	// ����J
private:
	LPDIRECTINPUTDEVICE8 m_pDevice;        // �C���v�b�g�f�o�C�X

	DIMOUSESTATE2 m_MouseState;	   // ���
	DIMOUSESTATE2 m_MouseStateOld; // �O����

	bool m_bPress[Index(MOUSE_BUTTON::Max)];	  // �{�^���������
	bool m_bTrigger[Index(MOUSE_BUTTON::Max)];    // �{�^�����͏��
	bool m_bRelease[Index(MOUSE_BUTTON::Max)];    // �{�^���������
	bool m_bRepeat[Index(MOUSE_BUTTON::Max)];     // �{�^�����������
	MouseMove m_Move;                             // �}�E�X�ړ���
	float m_WheelMove;                            // �z�C�[���ړ���

	float m_PressTime[Index(MOUSE_BUTTON::Max)];  // �{�^����������
	float m_RepeatTime[Index(MOUSE_BUTTON::Max)]; // �{�^����������
};

//-----------------------------
// �R���g���[���[�N���X
//-----------------------------
class CInputController
{
	// ���J
public:
	// �N���Xusing
	using CONTROLLER_BUTTON = input::CONTROLLER_BUTTON;

	CInputController() :m_state{} {};
	~CInputController() = default;

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	bool IsPress(CONTROLLER_BUTTON button);
	bool IsTrigger(CONTROLLER_BUTTON button);
	bool IsRelease(CONTROLLER_BUTTON button);
	bool IsRepeat(CONTROLLER_BUTTON button);
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
// rawInput�L�[�{�[�h�N���X
//-----------------------------
class CInputRawInputKeyboard : public CInputKeyboard
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
	BYTE m_KeyState[input::MAX_KEY];	 // �L�[���
	BYTE m_KeyStateOld[input::MAX_KEY];  // �O��L�[���

	bool m_bKeyPress[input::MAX_KEY];	 // �L�[�������
	bool m_bKeyTrigger[input::MAX_KEY];  // �L�[���͏��
	bool m_bKeyRelease[input::MAX_KEY];  // �L�[�������
	bool m_bKeyRepeat[input::MAX_KEY];   // �L�[���������

	int m_KeyPressTime[input::MAX_KEY];  // �L�[��������
};

//-----------------------------
// rawInput�}�E�X�N���X
//-----------------------------
class CInputRawInputMouse : public CInputMouse
{
	// ���J
public:
	CInputRawInputMouse() : m_MouseState{}, m_MouseStateOld{}, m_bPress{}, m_bTrigger{}, m_bRelease{}, m_bRepeat{}, m_PressTime{}, m_RepeatTime{}, m_Move{}, m_WheelMove{} {}
	~CInputRawInputMouse() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool IsPress(MOUSE_BUTTON button);
	bool IsTrigger(MOUSE_BUTTON button);
	bool IsRelease(MOUSE_BUTTON button);
	bool IsRepeat(MOUSE_BUTTON button);
	CInputMouseManager::MouseMove GetMove(void);
	float GetWheel(void);

	// ����J
private:
	LPDIRECTINPUTDEVICE8 m_pDevice;        // �C���v�b�g�f�o�C�X

	DIMOUSESTATE2 m_MouseState;	   // ���
	DIMOUSESTATE2 m_MouseStateOld; // �O����

	bool m_bPress[Index(MOUSE_BUTTON::Max)];	           // �{�^���������
	bool m_bTrigger[Index(MOUSE_BUTTON::Max)];          // �{�^�����͏��
	bool m_bRelease[Index(MOUSE_BUTTON::Max)];          // �{�^���������
	bool m_bRepeat[Index(MOUSE_BUTTON::Max)];           // �{�^�����������
	CInputMouseManager::MouseMove m_Move; // �}�E�X�ړ���
	float m_WheelMove;             // �z�C�[���ړ���

	float m_PressTime[Index(MOUSE_BUTTON::Max)];  // �{�^����������
	float m_RepeatTime[Index(MOUSE_BUTTON::Max)]; // �{�^����������
};

//-----------------------------
// XInput�R���g���[���[�N���X
//-----------------------------
class CInputXInputController : public CInputController
{
// ���J
public:
	// �N���Xusing
	using JOYKEY = input::JOYKEY;

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
class CInputDirectInputKeyboard : public CInputKeyboard
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
class CInputDirectInputMouse : public CInputMouse
{
// ���J
public:
	CInputDirectInputMouse() : m_MouseState{}, m_MouseStateOld{}, m_bPress{}, m_bTrigger{}, m_bRelease{}, m_bRepeat{}, m_PressTime{}, m_RepeatTime{}, m_Move{}, m_WheelMove{} {}
	~CInputDirectInputMouse() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool IsPress(MOUSE_BUTTON button);
	bool IsTrigger(MOUSE_BUTTON button);
	bool IsRelease(MOUSE_BUTTON button);
	bool IsRepeat(MOUSE_BUTTON button);
	CInputMouseManager::MouseMove GetMove(void);
	float GetWheel(void);

// ����J
private:
	LPDIRECTINPUTDEVICE8 m_pDevice;        // �C���v�b�g�f�o�C�X

	DIMOUSESTATE2 m_MouseState;	   // ���
	DIMOUSESTATE2 m_MouseStateOld; // �O����

	bool m_bPress[Index(MOUSE_BUTTON::Max)];	           // �{�^���������
	bool m_bTrigger[Index(MOUSE_BUTTON::Max)];          // �{�^�����͏��
	bool m_bRelease[Index(MOUSE_BUTTON::Max)];          // �{�^���������
	bool m_bRepeat[Index(MOUSE_BUTTON::Max)];           // �{�^�����������
	CInputMouseManager::MouseMove m_Move; // �}�E�X�ړ���
	float m_WheelMove;             // �z�C�[���ړ���

	float m_PressTime[Index(MOUSE_BUTTON::Max)];  // �{�^����������
	float m_RepeatTime[Index(MOUSE_BUTTON::Max)]; // �{�^����������
};

//-----------------------------
// DirectInput�R���g���[���[�N���X
//-----------------------------
class CInputDirectInputController : public CInputController
{
// ���J
public:
	CInputDirectInputController() : m_pDevice{}, m_state{}, m_effect{}, m_sName{}, m_pDiInstance{} {}
	~CInputDirectInputController() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool IsPress(Index key);
	bool IsTrigger(Index key);
	bool IsRelease(Index key);
	bool IsRepeat(Index key);
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
	LPDIRECTINPUTDEVICE8 m_pDevice;        // �C���v�b�g�f�o�C�X

	DIJOYSTATE m_state;//d�p�b�h���
	LPDIRECTINPUTEFFECT m_effect;
	string_view m_sName;
	const DIDEVICEINSTANCE* m_pDiInstance;
};