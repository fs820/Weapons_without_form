//---------------------------------------
//
// 入力処理の定義 [input.h]
// Author: Fuma Sato
//
//---------------------------------------
#pragma once
#include"main.h" // メイン

// dInputとXInputをリンク
#define DIRECTINPUT_VERSION  0x0800
#include <dinput.h>
#include <xinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "xinput.lib")

#if 0

//-------------------------------
// インプットクラス シングルトン
//-------------------------------
class CInput
{
// 公開
public:
	// 抽象ボタンタイプ
	using BUTTON = enum
	{
		INTERACT = 0,
		ATTACK,
		JUMP,
		MAX
	};
	static constexpr float RELEASE_TIME = 2.0f;          // Release可能時間
	static constexpr float REPEAT_START_TIME = 1.0f;     // Repeat開始時間
	static constexpr float REPEAT_INTERVAL_TIME = 1.0f;  // Repeat間隔

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

// 非公開
private:
	static CInput m_instance; // インスタンス

	CInputKeyboard* m_pKeyboard;     // キーボード
	CInputMouse* m_pMouse;           // マウス
	CInputController* m_pController; // コントローラー

	CInput() :m_pKeyboard{}, m_pMouse{}, m_pController{} {};
	~CInput() = default;
};

//-----------------------------
// キーボードクラス
//-----------------------------
class CInputKeyboard
{
// 公開
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

// 非公開
private:
	CInputRawInputKeyboard* m_pRaw;       // rawInput
	CInputDirectInputKeyboard* m_pDirect; // DirectInput
};

//-----------------------------
// マウスクラス
//-----------------------------
class CInputMouse
{
// 公開
public:
	// マウスの移動値
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

	// 非公開
private:
	static constexpr float MOUSE_INA = 15.0f;         // マウス移動量
	static constexpr float MOUSE_WHEEL_INA = 300.0f;  // マウスホイール移動量

	CInputRawInputMouse* m_pRaw;       // rawInput
	CInputDirectInputMouse* m_pDirect; // DirectInput
};

//-----------------------------
// コントローラークラス
//-----------------------------
class CInputController
{
// 公開
public:
	//コントローラーの種類
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

// 非公開
private:
	CInputXInputController* m_pX;           // XInput
	CInputDirectInputController* m_pDirect; // DirectInput
};

//-----------------------------
// rawInputキーボードクラス
//-----------------------------
class CInputRawInputKeyboard
{
// 公開
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

// 非公開
private:
	static constexpr size_t MAX_KEY = 256; // キー数

	BYTE m_KeyState[MAX_KEY];	  // キー状態
	BYTE m_KeyStateOld[MAX_KEY];  // 前回キー状態

	bool m_bKeyPress[MAX_KEY];	  // キー押下状態
	bool m_bKeyTrigger[MAX_KEY];  // キー入力状態
	bool m_bKeyRelease[MAX_KEY];  // キー離し状態
	bool m_bKeyRepeat[MAX_KEY];   // キー長押し状態

	int m_KeyPressTime[MAX_KEY];  // キー押下時間
};

//-----------------------------
// rawInputマウスクラス
//-----------------------------
class CInputRawInputMouse
{
	// 公開
public:
	//マウスボタンの種類
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

	// 非公開
private:
	LPDIRECTINPUTDEVICE8 m_pDevice;        // インプットデバイス

	DIMOUSESTATE2 m_MouseState;	   // 状態
	DIMOUSESTATE2 m_MouseStateOld; // 前回状態

	bool m_bPress[MAX];	           // ボタン押下状態
	bool m_bTrigger[MAX];          // ボタン入力状態
	bool m_bRelease[MAX];          // ボタン離し状態
	bool m_bRepeat[MAX];           // ボタン長押し状態
	CInputMouse::MouseMove m_Move; // マウス移動量
	float m_WheelMove;             // ホイール移動量

	float m_PressTime[MAX];  // ボタン押下時間
	float m_RepeatTime[MAX]; // ボタン押下時間
};

//-----------------------------
// XInputコントローラークラス
//-----------------------------
class CInputXInputController
{
// 公開
public:
	//Xパッドボタンの種類
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

// 非公開
private:
	static constexpr WORD VIBRATION_MAX = WORD(65535); // バイブレーション値
	static constexpr SHORT STICK_NUM = SHORT(32767);   // スティックの値

	XINPUT_STATE m_state;
};

//-----------------------------
// DirectInput管理クラス
//-----------------------------
class CInputDirectInput
{
// 公開
public:
	CInputDirectInput() = delete;

	static HRESULT Register(HINSTANCE hInstanse, HWND hWnd);
	static void Unregister(void);

	static LPDIRECTINPUT8 GetDirectInput(void) { return m_pInput; }

	static HRESULT SetEnum(void);

// 非公開
private:
	static BOOL CALLBACK EnumDevicesCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);

	static LPDIRECTINPUT8 m_pInput;
	static size_t m_useCount;
};

//-----------------------------
// DirectInputキーボードクラス
//-----------------------------
class CInputDirectInputKeyboard
{
	// 公開
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

	// 非公開
private:
	static constexpr size_t MAX_KEY = 256; // キー数

	LPDIRECTINPUTDEVICE8 m_pDevice;        // インプットデバイス

	BYTE m_KeyState[MAX_KEY];	  // キー状態
	BYTE m_KeyStateOld[MAX_KEY];  // 前回キー状態

	bool m_bKeyPress[MAX_KEY];	  // キー押下状態
	bool m_bKeyTrigger[MAX_KEY];  // キー入力状態
	bool m_bKeyRelease[MAX_KEY];  // キー離し状態
	bool m_bKeyRepeat[MAX_KEY];   // キー長押し状態

	float m_KeyPressTime[MAX_KEY];  // キー押下時間
	float m_KeyRepeatTime[MAX_KEY]; // キー押下時間
};

//-----------------------------
// DirectInputマウスクラス
//-----------------------------
class CInputDirectInputMouse
{
// 公開
public:
	//マウスボタンの種類
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

// 非公開
private:
	LPDIRECTINPUTDEVICE8 m_pDevice;        // インプットデバイス

	DIMOUSESTATE2 m_MouseState;	   // 状態
	DIMOUSESTATE2 m_MouseStateOld; // 前回状態

	bool m_bPress[MAX];	           // ボタン押下状態
	bool m_bTrigger[MAX];          // ボタン入力状態
	bool m_bRelease[MAX];          // ボタン離し状態
	bool m_bRepeat[MAX];           // ボタン長押し状態
	CInputMouse::MouseMove m_Move; // マウス移動量
	float m_WheelMove;             // ホイール移動量

	float m_PressTime[MAX];  // ボタン押下時間
	float m_RepeatTime[MAX]; // ボタン押下時間
};

//-----------------------------
// DirectInputコントローラークラス
//-----------------------------
class CInputDirectInputController
{
	// 公開
public:
	//ELEパッドボタンの種類
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

	//PSパッドボタンの種類
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

	//NINパッドボタンの種類
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
		－,
		＋,
		L3,
		R3,
		HOME,
		CAP,
		MAX
	};

	//Dパッドボタンの種類
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

	// 非公開
private:
	static constexpr int KEY_MAX = 32; //dinputボタンの最大数

	// POV
	static constexpr int POV_MAX = 4;                // POVの数
	static constexpr int POV_SYS_MAX = 8;            // POVの方向数
	static constexpr float POV_NUM = 4500.0f;        // POVの方向係数
	static constexpr float POV_UP = 0.0f;            //上
	static constexpr float POV_RIGHTUP = 4500.0f;    //右上
	static constexpr float POV_RIGHT = 9000.0f;      //右
	static constexpr float POV_RIGHTDOWN = 13500.0f; //右下
	static constexpr float POV_DOWN = 18000.0f;      //下
	static constexpr float POV_LEFTDOWN = 22500.0f;  //左下
	static constexpr float POV_LEFT = 27000.0f;      //左
	static constexpr float POV_LEFTUP = 31500.0f;    //左上

	static constexpr float DSTICK_NUM = -1.0f; // スティック計算用

	LPDIRECTINPUTDEVICE8 m_pDevice;        // インプットデバイス

	DIJOYSTATE m_state;//dパッド情報
	LPDIRECTINPUTEFFECT m_effect;
	string_view m_sName;
	const DIDEVICEINSTANCE* m_pDiInstance;
};

#endif