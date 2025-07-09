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
	constexpr float RELEASE_TIME = 2.0f;          // Release可能時間
	constexpr float REPEAT_START_TIME = 1.0f;     // Repeat開始時間
	constexpr float REPEAT_INTERVAL_TIME = 1.0f;  // Repeat間隔

	// Keyboard
	constexpr size_t MAX_KEY = 256u; // キー数

	// Mouse
	constexpr float MOUSE_INA = 15.0f;         // マウス移動量
	constexpr float MOUSE_WHEEL_INA = 300.0f;  // マウスホイール移動量
	
	// Controller
	constexpr WORD VIBRATION_MAX = static_cast<WORD>(65535); // バイブレーション値
	constexpr SHORT STICK_NUM = static_cast<SHORT>(32767);   // スティックの値

	constexpr int KEY_MAX = 32; //dinputボタンの最大数

	// POV
	constexpr int POV_MAX = 4;                // POVの数
	constexpr int POV_SYS_MAX = 8;            // POVの方向数
	constexpr float POV_NUM = 4500.0f;        // POVの方向係数
	constexpr float POV_UP = 0.0f;            // 上
	constexpr float POV_RIGHTUP = 4500.0f;    // 右上
	constexpr float POV_RIGHT = 9000.0f;      // 右
	constexpr float POV_RIGHTDOWN = 13500.0f; // 右下
	constexpr float POV_DOWN = 18000.0f;      // 下
	constexpr float POV_LEFTDOWN = 22500.0f;  // 左下
	constexpr float POV_LEFT = 27000.0f;      // 左
	constexpr float POV_LEFTUP = 31500.0f;    // 左上

	constexpr float DSTICK_NUM = -1.0f; // スティック計算用

	// 抽象ボタンタイプ
	enum class BUTTON : Index8
	{
		Interact,
		Attack,
		Jump,
		Max
	};
	
	//マウスボタンの種類
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

	// ボタンの種類
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

	//Xパッドボタンの種類
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

	//ELEパッドボタンの種類
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

	//PSパッドボタンの種類
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

	//NINパッドボタンの種類
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
		－,
		＋,
		L3,
		R3,
		Home,
		Cap,
		Max
	};

	//Dパッドボタンの種類
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

	// 軸入力
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

// 非公開
private:
	static CInput m_instance; // インスタンス

	CInputKeyboardManager* m_pKeyboard;     // キーボード管理
	CInputMouseManager* m_pMouse;           // マウス管理
	CInputControllerManager* m_pController; // コントローラー管理

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

	bool IsXInputControllerConnected(Index idx);
	bool IsDirectInputControllerConnected(Index idx);
	string_view ControllerName(Index idx);
	size_t Count(void) { return m_apController.size(); }

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
	CInputKeyboard() : m_state{}, m_stateOld{}, m_bPress{}, m_bTrigger{}, m_bRelease{}, m_bRepeat{}, m_PressTime{}, m_RepeatTime{} {};
	virtual ~CInputKeyboard() = default;

	virtual HRESULT Init(HINSTANCE hInstanse, HWND hWnd) = 0;
	virtual void Uninit(void) = 0;
	void Update(void);

	bool IsPress(Index8 key) const { return m_bPress[key]; }
	bool IsTrigger(Index8 key) const { return m_bTrigger[key]; }
	bool IsRelease(Index8 key) const { return m_bRelease[key]; }
	bool IsRepeat(Index8 key) const { return m_bRepeat[key]; }

// 家族公開
protected:
	virtual HRESULT GetState(span<BYTE> state) const = 0;
// 非公開
private:
	array <BYTE, input::MAX_KEY> m_state;	 // 今回のキー状態
	array <BYTE, input::MAX_KEY> m_stateOld; // 前回のキー状態

	array<bool, input::MAX_KEY> m_bPress;	  // キー押下状態
	array<bool, input::MAX_KEY> m_bTrigger;   // キー入力状態
	array<bool, input::MAX_KEY> m_bRelease;   // キー離し状態
	array<bool, input::MAX_KEY> m_bRepeat;    // キー長押し状態

	array<float, input::MAX_KEY> m_PressTime;  // キー押下時間
	array<float, input::MAX_KEY> m_RepeatTime; // キー押下時間
};

//-----------------------------
// マウスクラス
//-----------------------------
class CInputMouse
{
// 公開
public:
	CInputMouse() : m_state{}, m_stateOld{}, m_bPress{}, m_bTrigger{}, m_bRelease{}, m_bRepeat{}, m_PressTime{}, m_RepeatTime{}, m_Move{}, m_WheelMove{} {}
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

// 家族公開
protected:
	virtual HRESULT GetState(span<bool> state) const = 0;
	virtual HRESULT GetMove(input::Axis* pMove) const = 0;
	virtual HRESULT GetWheel(float* pWheel) const = 0;

// 非公開
private:
	array<bool, Index8(input::MOUSE_BUTTON::Max)> m_state;	  // 今回のボタン状態
	array<bool, Index8(input::MOUSE_BUTTON::Max)> m_stateOld; // 前回のボタン状態

	array<bool, Index8(input::MOUSE_BUTTON::Max)> m_bPress;	  // ボタン押下状態
	array<bool, Index8(input::MOUSE_BUTTON::Max)> m_bTrigger; // ボタン入力状態
	array<bool, Index8(input::MOUSE_BUTTON::Max)> m_bRelease; // ボタン離し状態
	array<bool, Index8(input::MOUSE_BUTTON::Max)> m_bRepeat;  // ボタン長押し状態

	input::Axis m_Move; // マウス移動量
	float m_WheelMove;  // ホイール移動量

	array<float, Index8(input::MOUSE_BUTTON::Max)> m_PressTime;  // ボタン押下時間
	array<float, Index8(input::MOUSE_BUTTON::Max)> m_RepeatTime; // ボタン押下時間
};

//-----------------------------
// コントローラークラス
//-----------------------------
class CInputController
{
	// 公開
public:
	CInputController() : m_state{}, m_stateOld{}, m_bPress{}, m_bTrigger{}, m_bRelease{}, m_bRepeat{}, m_Stick{}, m_Trigger{}, m_PressTime{}, m_RepeatTime{} {};
	virtual ~CInputController() = default;

	virtual HRESULT Init(HINSTANCE hInstanse, HWND hWnd) = 0;
	virtual void Uninit(void) = 0;
	void Update(void);

	bool IsPress(input::CONTROLLER_BUTTON button) const { return m_bPress[Index8(button)]; }
	bool IsTrigger(input::CONTROLLER_BUTTON button) const { return m_bTrigger[Index8(button)]; }
	bool IsRelease(input::CONTROLLER_BUTTON button) const { return m_bRelease[Index8(button)]; }
	bool IsRepeat(input::CONTROLLER_BUTTON button) const { return m_bRepeat[Index8(button)]; }
	input::Axis GetAxis(LR lr) const { return m_Stick[lr]; }
	float GetTrigger(LR lr) const { return m_Trigger[lr]; }
	virtual void Vibrate(WORD wLeftMotorSpeed, WORD wRightMotorSpeed) = 0;

// 家族公開
protected:
	virtual HRESULT GetState(span<bool> state) const = 0;
	virtual HRESULT GetStick(span<input::Axis> axis) const = 0;
	virtual HRESULT GetTrigger(span<float> trigger) const = 0;

// 非公開
private:
	array<bool, Index8(input::CONTROLLER_BUTTON::Max)> m_state;    // 今回のボタン状態
	array<bool, Index8(input::CONTROLLER_BUTTON::Max)> m_stateOld; // 前回のボタン状態

	array<bool, Index8(input::CONTROLLER_BUTTON::Max)> m_bPress;   // ボタン押下状態
	array<bool, Index8(input::CONTROLLER_BUTTON::Max)> m_bTrigger; // ボタン入力状態
	array<bool, Index8(input::CONTROLLER_BUTTON::Max)> m_bRelease; // ボタン離し状態
	array<bool, Index8(input::CONTROLLER_BUTTON::Max)> m_bRepeat;  // ボタン長押し状態

	array <input::Axis, 2> m_Stick; // スティック
	array <float, 2> m_Trigger;     // トリガー入力

	array<float, Index8(input::CONTROLLER_BUTTON::Max)> m_PressTime;  // ボタン押下時間
	array<float, Index8(input::CONTROLLER_BUTTON::Max)> m_RepeatTime; // ボタン押下時間
};

//-----------------------------
// rawInput管理クラス
//-----------------------------
class CInputRawInput
{
	// 公開
public:
	CInputRawInput() = delete;

	static size_t GetCount(void) { return m_useCount; }

	static HRESULT AnalysisRawData(RAWINPUT* pRawData);

// 非公開
private:
	static size_t m_useCount;
};

//-----------------------------
// rawInputキーボードクラス
//-----------------------------
class CInputRawInputKeyboard : public CInputKeyboard
{
// 公開
public:
	CInputRawInputKeyboard() = default;
	~CInputRawInputKeyboard() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd) override;
	void Uninit(void) override;
	HRESULT GetState(span<BYTE> state) const override;

// 非公開
private:
};

//-----------------------------
// rawInputマウスクラス
//-----------------------------
class CInputRawInputMouse : public CInputMouse
{
	// 公開
public:
	CInputRawInputMouse() = default;
	~CInputRawInputMouse() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd) override;
	void Uninit(void) override;
	HRESULT GetState(span<bool> state) const override;
	HRESULT GetMove(input::Axis* pMove) const override;
	HRESULT GetWheel(float* pWheel) const override;

	// 非公開
private:
};

//-----------------------------
// XInputコントローラークラス
//-----------------------------
class CInputXInputController : public CInputController
{
// 公開
public:
	CInputXInputController() = default;
	~CInputXInputController() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd) override;
	void Uninit(void) override;
	HRESULT GetState(span<bool> state) const override;
	HRESULT GetStick(span<input::Axis> axis) const override;
	HRESULT GetTrigger(span<float> trigger) const override;
	void Vibrate(WORD wLeftMotorSpeed, WORD wRightMotorSpeed)  override;

// 非公開
private:
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
	CInputDirectInputKeyboard() : m_pDevice{} {}
	~CInputDirectInputKeyboard() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd) override;
	void Uninit(void) override;
	HRESULT GetState(span<BYTE> state) const override;

// 非公開
private:
	LPDIRECTINPUTDEVICE8 m_pDevice;        // インプットデバイス
};

//-----------------------------
// DirectInputマウスクラス
//-----------------------------
class CInputDirectInputMouse : public CInputMouse
{
// 公開
public:
	CInputDirectInputMouse() : m_pDevice{}, m_MouseState{}, m_MouseStateOld{} {}
	~CInputDirectInputMouse() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd) override;
	void Uninit(void) override;
	HRESULT GetState(span<bool> state) const override;
	HRESULT GetMove(input::Axis* pMove) const override;
	HRESULT GetWheel(float* pWheel) const override;

// 非公開
private:
	LPDIRECTINPUTDEVICE8 m_pDevice;        // インプットデバイス

	DIMOUSESTATE2 m_MouseState;	   // 状態
	DIMOUSESTATE2 m_MouseStateOld; // 前回状態
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

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd) override;
	void Uninit(void) override;
	HRESULT GetState(span<bool> state) const override;
	HRESULT GetStick(span<input::Axis> axis) const override;
	HRESULT GetTrigger(span<float> trigger) const override;
	void Vibrate(WORD wLeftMotorSpeed, WORD wRightMotorSpeed)  override;

	// 非公開
private:
	LPDIRECTINPUTDEVICE8 m_pDevice;        // インプットデバイス

	DIJOYSTATE m_state;//dパッド情報
	LPDIRECTINPUTEFFECT m_effect;
	string_view m_sName;
	const DIDEVICEINSTANCE* m_pDiInstance;
};