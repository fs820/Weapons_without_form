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

// input用定義
namespace input
{
	// time
	static constexpr float RELEASE_TIME = 2.0f;          // Release可能時間
	static constexpr float REPEAT_START_TIME = 1.0f;     // Repeat開始時間
	static constexpr float REPEAT_INTERVAL_TIME = 1.0f;  // Repeat間隔

	// Keyboard
	static constexpr size_t MAX_KEY = 256; // キー数

	// Mouse
	static constexpr float MOUSE_INA = 15.0f;         // マウス移動量
	static constexpr float MOUSE_WHEEL_INA = 300.0f;  // マウスホイール移動量

	// Controller
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

	// 抽象ボタンタイプ
	enum class BUTTON : Index
	{
		Interact,
		Attack,
		Jump,
		Max
	};

	//マウスボタンの種類
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

	// ボタンの種類
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

	//Xパッドボタンの種類
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

	//ELEパッドボタンの種類
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

	//PSパッドボタンの種類
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

	//NINパッドボタンの種類
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
		－,
		＋,
		L3,
		R3,
		Home,
		Cap,
		Max
	};

	//Dパッドボタンの種類
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

	// マウスの移動値
	struct MouseMove
	{
		float x;
		float y;
	};
}

// 管理
class CInputKeyboardManager;   // キーボード
class CInputMouseManager;      // マウス
class CInputControllerManager; // コントローラー

// 基底
class CInputKeyboard;   // キーボード
class CInputMouse;		// マウス
class CInputController;	// コントローラー

// 派生
class CInputRawInputKeyboard; // キーボード
class CInputRawInputMouse;	  // マウス
class CInputXInputController; // コントローラー

// DirectInput本体
class CInputDirectInput;

// 派生
class CInputDirectInputKeyboard;   // キーボード
class CInputDirectInputMouse;	   // マウス
class CInputDirectInputController; // コントローラー

//-------------------------------
// インプットクラス シングルトン
//-------------------------------
class CInput final
{
// 公開
public:
	// クラスusing
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

// 非公開
private:
	static CInput m_instance; // インスタンス

	CInputKeyboardManager* m_pKeyboard;     // キーボード
	CInputMouseManager* m_pMouse;           // マウス
	CInputControllerManager* m_pController; // コントローラー

	CInput() :m_pKeyboard{}, m_pMouse{}, m_pController{} {};
	~CInput() = default;
};

//-----------------------------
// キーボード管理クラス
//-----------------------------
class CInputKeyboardManager
{
// 公開
public:
	// クラスusing
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

// 非公開
private:
	vector<CInputKeyboard*> m_apKeyboard; // Keyboardクラスポインタ
};

//-----------------------------
// マウス管理クラス
//-----------------------------
class CInputMouseManager
{
// 公開
public:
	// クラスusing
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

	// 非公開
private:
	vector<CInputMouse*> m_apMouse;    // Mouseクラスポインタ
};

//-----------------------------
// コントローラー管理クラス
//-----------------------------
class CInputControllerManager
{
// 公開
public:
	// クラスusing
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

// 非公開
private:
	vector<CInputController*> m_apController; // Controllerクラスポインタ
};

//-----------------------------
// キーボードクラス
//-----------------------------
class CInputKeyboard
{
	// 公開
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

	// 非公開
private:
	BYTE m_KeyState[input::MAX_KEY];	 // キー状態
	BYTE m_KeyStateOld[input::MAX_KEY];  // 前回キー状態

	bool m_bKeyPress[input::MAX_KEY];	  // キー押下状態
	bool m_bKeyTrigger[input::MAX_KEY];  // キー入力状態
	bool m_bKeyRelease[input::MAX_KEY];  // キー離し状態
	bool m_bKeyRepeat[input::MAX_KEY];   // キー長押し状態

	int m_KeyPressTime[input::MAX_KEY];  // キー押下時間
};

//-----------------------------
// マウスクラス
//-----------------------------
class CInputMouse
{
// 公開
public:
	// クラスusing
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

	// 非公開
private:
	LPDIRECTINPUTDEVICE8 m_pDevice;        // インプットデバイス

	DIMOUSESTATE2 m_MouseState;	   // 状態
	DIMOUSESTATE2 m_MouseStateOld; // 前回状態

	bool m_bPress[Index(MOUSE_BUTTON::Max)];	  // ボタン押下状態
	bool m_bTrigger[Index(MOUSE_BUTTON::Max)];    // ボタン入力状態
	bool m_bRelease[Index(MOUSE_BUTTON::Max)];    // ボタン離し状態
	bool m_bRepeat[Index(MOUSE_BUTTON::Max)];     // ボタン長押し状態
	MouseMove m_Move;                             // マウス移動量
	float m_WheelMove;                            // ホイール移動量

	float m_PressTime[Index(MOUSE_BUTTON::Max)];  // ボタン押下時間
	float m_RepeatTime[Index(MOUSE_BUTTON::Max)]; // ボタン押下時間
};

//-----------------------------
// コントローラークラス
//-----------------------------
class CInputController
{
	// 公開
public:
	// クラスusing
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

	// 非公開
private:
	static constexpr WORD VIBRATION_MAX = WORD(65535); // バイブレーション値
	static constexpr SHORT STICK_NUM = SHORT(32767);   // スティックの値

	XINPUT_STATE m_state;
};

//-----------------------------
// rawInputキーボードクラス
//-----------------------------
class CInputRawInputKeyboard : public CInputKeyboard
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
	BYTE m_KeyState[input::MAX_KEY];	 // キー状態
	BYTE m_KeyStateOld[input::MAX_KEY];  // 前回キー状態

	bool m_bKeyPress[input::MAX_KEY];	 // キー押下状態
	bool m_bKeyTrigger[input::MAX_KEY];  // キー入力状態
	bool m_bKeyRelease[input::MAX_KEY];  // キー離し状態
	bool m_bKeyRepeat[input::MAX_KEY];   // キー長押し状態

	int m_KeyPressTime[input::MAX_KEY];  // キー押下時間
};

//-----------------------------
// rawInputマウスクラス
//-----------------------------
class CInputRawInputMouse : public CInputMouse
{
	// 公開
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

	// 非公開
private:
	LPDIRECTINPUTDEVICE8 m_pDevice;        // インプットデバイス

	DIMOUSESTATE2 m_MouseState;	   // 状態
	DIMOUSESTATE2 m_MouseStateOld; // 前回状態

	bool m_bPress[Index(MOUSE_BUTTON::Max)];	           // ボタン押下状態
	bool m_bTrigger[Index(MOUSE_BUTTON::Max)];          // ボタン入力状態
	bool m_bRelease[Index(MOUSE_BUTTON::Max)];          // ボタン離し状態
	bool m_bRepeat[Index(MOUSE_BUTTON::Max)];           // ボタン長押し状態
	CInputMouseManager::MouseMove m_Move; // マウス移動量
	float m_WheelMove;             // ホイール移動量

	float m_PressTime[Index(MOUSE_BUTTON::Max)];  // ボタン押下時間
	float m_RepeatTime[Index(MOUSE_BUTTON::Max)]; // ボタン押下時間
};

//-----------------------------
// XInputコントローラークラス
//-----------------------------
class CInputXInputController : public CInputController
{
// 公開
public:
	// クラスusing
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
class CInputDirectInputKeyboard : public CInputKeyboard
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
class CInputDirectInputMouse : public CInputMouse
{
// 公開
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

// 非公開
private:
	LPDIRECTINPUTDEVICE8 m_pDevice;        // インプットデバイス

	DIMOUSESTATE2 m_MouseState;	   // 状態
	DIMOUSESTATE2 m_MouseStateOld; // 前回状態

	bool m_bPress[Index(MOUSE_BUTTON::Max)];	           // ボタン押下状態
	bool m_bTrigger[Index(MOUSE_BUTTON::Max)];          // ボタン入力状態
	bool m_bRelease[Index(MOUSE_BUTTON::Max)];          // ボタン離し状態
	bool m_bRepeat[Index(MOUSE_BUTTON::Max)];           // ボタン長押し状態
	CInputMouseManager::MouseMove m_Move; // マウス移動量
	float m_WheelMove;             // ホイール移動量

	float m_PressTime[Index(MOUSE_BUTTON::Max)];  // ボタン押下時間
	float m_RepeatTime[Index(MOUSE_BUTTON::Max)]; // ボタン押下時間
};

//-----------------------------
// DirectInputコントローラークラス
//-----------------------------
class CInputDirectInputController : public CInputController
{
// 公開
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

	// 非公開
private:
	LPDIRECTINPUTDEVICE8 m_pDevice;        // インプットデバイス

	DIJOYSTATE m_state;//dパッド情報
	LPDIRECTINPUTEFFECT m_effect;
	string_view m_sName;
	const DIDEVICEINSTANCE* m_pDiInstance;
};