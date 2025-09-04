//---------------------------------------
//
// 入力処理の定義 [input.h]
// Author: Fuma Sato
//
//---------------------------------------
#pragma once
#define NOMINMAX
#include <array>
#include <vector>
#include <span>
#include <unordered_map>
#include "common.h"

// dInputとXInputをリンク
#define DIRECTINPUT_VERSION  0x0800
#include <dinput.h>
#include <xinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "xinput.lib")

// トリガーのデジタル入力ようマクロ
#ifndef XINPUT_GAMEPAD_LEFT_TRIGGER
#define XINPUT_GAMEPAD_LEFT_TRIGGER  0x0400
#endif

#ifndef XINPUT_GAMEPAD_RIGHT_TRIGGER
#define XINPUT_GAMEPAD_RIGHT_TRIGGER 0x0800
#endif

// input用定義
namespace input
{
    // MicrosoftのベンダーID
    const WORD MICROSOFT_VENDOR_ID = 0x045E;

    // Keyboard
    constexpr size_t MAX_KEY = 256u; // キー数

    // Mouse
    constexpr float MOUSE_INA = 15.0f;         // マウス移動量
    constexpr float MOUSE_WHEEL_INA = 300.0f;  // マウスホイール移動量

    // Controller
    constexpr WORD VIBRATION_MAX = static_cast<WORD>(65535); // バイブレーション値
    constexpr SHORT STICK_NUM = static_cast<SHORT>(32767);   // スティックの値

    // GUID
    constexpr GUID ELECOM = GUID(0);
    constexpr GUID PlayStation = GUID(0);
    constexpr GUID Nintendo = GUID(0);

    // コントローラーの種類
    enum class DIRECTINPUT_CONTROLLER_TYPE : common::Index8
    {
        ELECOM,
        PlayStation,
        Nintendo,
        Unknown,
        Max
    };

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

    // DirectInputRange
    constexpr LONG DIRECTINPUT_TRIGGER_MIN = 0l;     // トリガーやスライダーの最小値
    constexpr LONG DIRECTINPUT_TRIGGER_MAX = 10000l; // トリガーやスライダーの最大値
    constexpr LONG DIRECTINPUT_AXIS_MIN = -1000l;    // 軸の最小値
    constexpr LONG DIRECTINPUT_AXIS_MAX = 1000l;     // 軸の最大値

    // 抽象ボタンタイプ
    enum class BUTTON : common::Index8
    {
        Interact,
        Attack,
        Jump,
        Max
    };

    //マウスボタンの種類
    enum class MOUSE_BUTTON : common::Index8
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
    enum class CONTROLLER_BUTTON : common::Index8
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
        Unmapped,
        Max
    };

    //Xパッドボタンの種類
    enum class JOYKEY : common::Index8
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
    enum class ELEKEY : common::Index8
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
        Up,
        Down,
        Left,
        Right,
        Max
    };

    //PSパッドボタンの種類
    enum class PSKEY : common::Index8
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
        Up,
        Down,
        Left,
        Right,
        Max
    };

    //NINパッドボタンの種類
    enum class NINKEY : common::Index8
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
        Up,
        Down,
        Left,
        Right,
        Max
    };

    //Dパッドボタンの種類
    enum class DKEY : common::Index8
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
        Up,
        Down,
        Left,
        Right,
        Max
    };

    //---------------------------------------------------------------------
    //
    // コントローラーごとのボタンをコントローラー共通ボタン配置にするときの対応表
    //
    //---------------------------------------------------------------------

    // XInput
    constexpr std::array<CONTROLLER_BUTTON, common::Index8(JOYKEY::Max)> XConMap =
    {
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
    };

    // ELECOMコントローラー
    constexpr std::array<CONTROLLER_BUTTON, common::Index8(ELEKEY::Max)> EleConMap =
    {
            CONTROLLER_BUTTON::BottomButton,
            CONTROLLER_BUTTON::RightFaceButton,
            CONTROLLER_BUTTON::LeftFaceButton,
            CONTROLLER_BUTTON::TopButton,
            CONTROLLER_BUTTON::LB,
            CONTROLLER_BUTTON::RB,
            CONTROLLER_BUTTON::LT,
            CONTROLLER_BUTTON::RT,
            CONTROLLER_BUTTON::L3,
            CONTROLLER_BUTTON::R3,
            CONTROLLER_BUTTON::Back,
            CONTROLLER_BUTTON::Start,
            CONTROLLER_BUTTON::Unmapped,
            CONTROLLER_BUTTON::Up,
            CONTROLLER_BUTTON::Down,
            CONTROLLER_BUTTON::Left,
            CONTROLLER_BUTTON::Right
    };

    // PSコントローラー
    constexpr std::array<CONTROLLER_BUTTON, common::Index8(PSKEY::Max)> PsConMap =
    {
            CONTROLLER_BUTTON::LeftFaceButton,
            CONTROLLER_BUTTON::BottomButton,
            CONTROLLER_BUTTON::RightFaceButton,
            CONTROLLER_BUTTON::TopButton,
            CONTROLLER_BUTTON::LB,
            CONTROLLER_BUTTON::RB,
            CONTROLLER_BUTTON::LT,
            CONTROLLER_BUTTON::RT,
            CONTROLLER_BUTTON::Back,
            CONTROLLER_BUTTON::Start,
            CONTROLLER_BUTTON::L3,
            CONTROLLER_BUTTON::R3,
            CONTROLLER_BUTTON::Unmapped,
            CONTROLLER_BUTTON::Unmapped,
            CONTROLLER_BUTTON::Up,
            CONTROLLER_BUTTON::Down,
            CONTROLLER_BUTTON::Left,
            CONTROLLER_BUTTON::Right
    };

    // Ninコントローラー
    constexpr std::array<CONTROLLER_BUTTON, common::Index8(NINKEY::Max)> NinConMap =
    {
            CONTROLLER_BUTTON::BottomButton,
            CONTROLLER_BUTTON::RightFaceButton,
            CONTROLLER_BUTTON::LeftFaceButton,
            CONTROLLER_BUTTON::TopButton,
            CONTROLLER_BUTTON::LB,
            CONTROLLER_BUTTON::RB,
            CONTROLLER_BUTTON::LT,
            CONTROLLER_BUTTON::RT,
            CONTROLLER_BUTTON::Back,
            CONTROLLER_BUTTON::Start,
            CONTROLLER_BUTTON::L3,
            CONTROLLER_BUTTON::R3,
            CONTROLLER_BUTTON::Unmapped,
            CONTROLLER_BUTTON::Unmapped,
            CONTROLLER_BUTTON::Up,
            CONTROLLER_BUTTON::Down,
            CONTROLLER_BUTTON::Left,
            CONTROLLER_BUTTON::Right
    };

    // その他DirectInputコントローラー(個別対応なし)
    constexpr std::array<CONTROLLER_BUTTON, common::Index8(DKEY::Max)> DConMap =
    {
            CONTROLLER_BUTTON::BottomButton,
            CONTROLLER_BUTTON::RightFaceButton,
            CONTROLLER_BUTTON::LeftFaceButton,
            CONTROLLER_BUTTON::TopButton,
            CONTROLLER_BUTTON::LB,
            CONTROLLER_BUTTON::RB,
            CONTROLLER_BUTTON::LT,
            CONTROLLER_BUTTON::RT,
            CONTROLLER_BUTTON::L3,
            CONTROLLER_BUTTON::R3,
            CONTROLLER_BUTTON::Back,
            CONTROLLER_BUTTON::Start,
            CONTROLLER_BUTTON::Up,
            CONTROLLER_BUTTON::Down,
            CONTROLLER_BUTTON::Left,
            CONTROLLER_BUTTON::Right
    };

    // ボタンの状態
    struct ButtonState
    {
        bool isDown;     // 押している
        bool isTrigger;  // 押した瞬間
        bool isRelease;  // 離した瞬間
        ButtonState() : isDown{}, isTrigger{}, isRelease{} {}
        ~ButtonState() = default;
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

// rawInput本体
class CInputRawInput;

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
// インプットクラス
//-------------------------------
class CInput final
{
    // 公開
public:
    CInput() :m_pKeyboard{}, m_pMouse{}, m_pController{} {};
    ~CInput() = default;

    CInput(const CInput&) = delete;
    CInput& operator=(const CInput&) = delete;
    CInput(const CInput&&) = default;
    CInput& operator=(const CInput&&) = default;

    HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
    void Uninit(void);
    void Update(void);

    bool IsDown(common::Index8 idx, input::BUTTON button) const;
    bool IsTrigger(common::Index8 idx, input::BUTTON button) const;
    bool IsRelease(common::Index8 idx, input::BUTTON button) const;

    size_t Count(void) const;

    // 非公開
private:
    CInputKeyboardManager* m_pKeyboard;     // キーボード管理
    CInputMouseManager* m_pMouse;           // マウス管理
    CInputControllerManager* m_pController; // コントローラー管理
};

//-----------------------------
// キーボード管理クラス
//-----------------------------
class CInputKeyboardManager
{
    // 公開
public:
    CInputKeyboardManager() : m_apKeyboard{}, m_keyMap{} {}
    ~CInputKeyboardManager() = default;

    HRESULT Init(HINSTANCE hInstanse, HWND hWnd);
    void Uninit(void);
    void Update(void);

    bool IsDown(common::Index8 idx, input::BUTTON button) const;
    bool IsTrigger(common::Index8 idx, input::BUTTON button) const;
    bool IsRelease(common::Index8 idx, input::BUTTON button) const;

    size_t Count(void) const { return m_apKeyboard.size(); }

    // 非公開
private:
    bool LoadConfig(const char* pFileName); // コンフィグ読み込み

    std::vector<CInputKeyboard*> m_apKeyboard;                  // Keyboardクラスポインタ
    std::unordered_map<input::BUTTON, common::Index8> m_keyMap; // 抽象ボタンとキーの対応表
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

    bool IsDown(common::Index8 idx, input::BUTTON button) const;
    bool IsTrigger(common::Index8 idx, input::BUTTON button) const;
    bool IsRelease(common::Index8 idx, input::BUTTON button) const;
    input::Axis GetAxis(common::Index8 idx) const;
    float GetWheel(common::Index8 idx) const;

    size_t Count(void) const { return m_apMouse.size(); }

    // 非公開
private:
    std::vector<CInputMouse*> m_apMouse;    // Mouseクラスポインタ
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

    bool IsDown(common::Index idx, input::BUTTON button) const;
    bool IsTrigger(common::Index idx, input::BUTTON button) const;
    bool IsRelease(common::Index idx, input::BUTTON button) const;
    input::Axis GetAxis(common::Index idx, common::Direction lr) const;
    float GetTrigger(common::Index idx, common::Direction lr) const;
    float GetSlider(common::Index idx, common::Direction lr) const;

    void SetVibrate(common::Index idx, float fLeftPower, float fReghtPower);

    void Register(CInputController* pController) { m_apController.push_back(pController); }

    size_t Count(void) const { return m_apController.size(); }

    // 非公開
private:
    std::vector<CInputController*> m_apController; // Controllerクラスポインタ
};

//-----------------------------
// キーボードクラス
//-----------------------------
class CInputKeyboard
{
    // 公開
public:
    CInputKeyboard() : m_keyState{} {};
    virtual ~CInputKeyboard() = default;

    virtual HRESULT Init(HINSTANCE hInstanse, HWND hWnd) = 0;
    virtual void Uninit(void) = 0;
    void Update(void);

    bool IsDown(common::Index8 key) const { return m_keyState[key].isDown; }
    bool IsTrigger(common::Index8 key) const { return m_keyState[key].isTrigger; }
    bool IsRelease(common::Index8 key) const { return m_keyState[key].isRelease; }

    // 家族公開
protected:
    virtual HRESULT GetKey(std::span<input::ButtonState> keyState) const = 0;
    // 非公開
private:
    std::array<input::ButtonState, input::MAX_KEY> m_keyState; // キーの状態
};

//-----------------------------
// マウスクラス
//-----------------------------
class CInputMouse
{
    // 公開
public:
    CInputMouse() : m_buttonState{}, m_Move{}, m_WheelMove{} {}
    virtual ~CInputMouse() = default;

    virtual HRESULT Init(HINSTANCE hInstanse, HWND hWnd) = 0;
    virtual void Uninit(void) = 0;
    void Update(void);

    bool IsDown(input::MOUSE_BUTTON button) const { return m_buttonState[common::Index8(button)].isDown; }
    bool IsTrigger(input::MOUSE_BUTTON button) const { return m_buttonState[common::Index8(button)].isTrigger; }
    bool IsRelease(input::MOUSE_BUTTON button) const { return m_buttonState[common::Index8(button)].isRelease; }
    input::Axis GetAxis(void) const { return m_Move; }
    float GetWheel(void) const { return m_WheelMove; }

    // 家族公開
protected:
    virtual HRESULT GetButton(std::span<input::ButtonState> buttonState) const = 0;
    virtual HRESULT GetMove(input::Axis* pMove) const = 0;
    virtual HRESULT GetWheel(float* pWheel) const = 0;

    // 非公開
private:
    std::array<input::ButtonState, common::Index8(input::MOUSE_BUTTON::Max)> m_buttonState; // ボタンの状態

    input::Axis m_Move; // マウス移動量
    float m_WheelMove;  // ホイール移動量
};

//-----------------------------
// コントローラークラス
//-----------------------------
class CInputController
{
    // 公開
public:
    CInputController() : m_buttonState{}, m_Stick{}, m_Trigger{}, m_Slider{} {};
    virtual ~CInputController() = default;

    virtual HRESULT Init(HINSTANCE hInstanse, HWND hWnd, common::Index8 idx = common::INVALID_ID8) = 0;
    virtual void Uninit(void) = 0;
    void Update(void);

    bool IsDown(input::CONTROLLER_BUTTON button) const { return m_buttonState[common::Index8(button)].isDown; }
    bool IsTrigger(input::CONTROLLER_BUTTON button) const { return m_buttonState[common::Index8(button)].isTrigger; }
    bool IsRelease(input::CONTROLLER_BUTTON button) const { return m_buttonState[common::Index8(button)].isRelease; }
    input::Axis GetAxis(common::Direction lr) const { return m_Stick[common::Index8(lr)]; }
    float GetTrigger(common::Direction lr) const { return m_Trigger[common::Index8(lr)]; }
    float GetSlider(common::Direction lr) const { return m_Slider[common::Index8(lr)]; }

    virtual void Vibrate(float leftMotorSpeed = 1.0f, float rightMotorSpeed = 1.0f) = 0;

    // 家族公開
protected:
    virtual HRESULT GetButton(std::span<input::ButtonState> buttonState) = 0;
    virtual HRESULT GetStick(std::span<input::Axis> axis) const = 0;
    virtual HRESULT GetTrigger(std::span<float> trigger) const = 0;
    virtual HRESULT GetSlider(std::span<float> slider) const = 0;

    // 非公開
private:
    std::array<input::ButtonState, common::Index8(input::CONTROLLER_BUTTON::Max)> m_buttonState; // ボタンの状態

    std::array <input::Axis, 2> m_Stick; // スティック
    std::array <float, 2> m_Trigger;     // トリガー入力
    std::array <float, 2>  m_Slider;     // スライダー入力
};

//-----------------------------
// rawInput管理クラス
//-----------------------------
class CInputRawInput
{
    // 公開
public:
    CInputRawInput() = delete;

    static HRESULT SetRawData(RAWINPUT rawData);

    // 非公開
private:
};

//-----------------------------
// rawInputキーボードクラス
//-----------------------------
class CInputRawInputKeyboard : public CInputKeyboard
{
    // 公開
public:
    CInputRawInputKeyboard() : m_key{}, m_keyOld{} {}
    ~CInputRawInputKeyboard() = default;

    HRESULT Init(HINSTANCE hInstanse, HWND hWnd) override;
    void Uninit(void) override;
    HRESULT GetKey(std::span<input::ButtonState> keyState) const override;

    // 非公開
private:
    std::array <bool, input::MAX_KEY> m_key;	// 今回のキー状態
    std::array <bool, input::MAX_KEY> m_keyOld; // 前回のキー状態
};

//-----------------------------
// rawInputマウスクラス
//-----------------------------
class CInputRawInputMouse : public CInputMouse
{
    // 公開
public:
    CInputRawInputMouse() : m_button{}, m_buttonOld{} {}
    ~CInputRawInputMouse() = default;

    HRESULT Init(HINSTANCE hInstanse, HWND hWnd) override;
    void Uninit(void) override;
    HRESULT GetButton(std::span<input::ButtonState> buttonState) const override;
    HRESULT GetMove(input::Axis* pMove) const override;
    HRESULT GetWheel(float* pWheel) const override;

    // 非公開
private:
    std::array<bool, common::Index8(input::MOUSE_BUTTON::Max)> m_button;	// 今回のボタン状態
    std::array<bool, common::Index8(input::MOUSE_BUTTON::Max)> m_buttonOld; // 前回のボタン状態
};

//-----------------------------
// XInputコントローラークラス
//-----------------------------
class CInputXInputController : public CInputController
{
    // 公開
public:
    CInputXInputController() : m_idx{}, m_button{}, m_buttonOld{} {}
    ~CInputXInputController() = default;

    HRESULT Init(HINSTANCE hInstanse, HWND hWnd, common::Index8 idx = common::INVALID_ID8) override;
    void Uninit(void) override {};
    HRESULT GetButton(std::span<input::ButtonState> buttonState) override;
    HRESULT GetStick(std::span<input::Axis> axis) const override;
    HRESULT GetTrigger(std::span<float> trigger) const override;
    HRESULT GetSlider(std::span<float> slider) const override;
    void Vibrate(float leftMotorSpeed = 1.0f, float rightMotorSpeed = 1.0f)  override;

    // 非公開
private:
    void SetTriggerButton(PXINPUT_STATE pState) const;
    WORD GetXInputMask(input::JOYKEY key) const;

    common::Index8 m_idx;

    std::array<bool, common::Index8(input::CONTROLLER_BUTTON::Max)> m_button;    // 今回のボタン状態
    std::array<bool, common::Index8(input::CONTROLLER_BUTTON::Max)> m_buttonOld; // 前回のボタン状態
};

//-----------------------------
// DirectInput管理クラス
//-----------------------------
class CInputDirectInput
{
    // 公開
public:
    CInputDirectInput() = delete;

    static HRESULT DirectInputSetUp(HINSTANCE hInstanse);
    static void DirectInputRelease(void);

    static LPDIRECTINPUT8 GetDirectInput(void) { return m_pInput; }

    static HRESULT SetEnum(CInputControllerManager* pControllerManager);

    // 非公開
private:
    static BOOL CALLBACK EnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

    static LPDIRECTINPUT8 m_pInput;
};

//-----------------------------
// DirectInputキーボードクラス
//-----------------------------
class CInputDirectInputKeyboard : public CInputKeyboard
{
    // 公開
public:
    CInputDirectInputKeyboard() : m_pDevice{}, m_key{}, m_keyOld{} {}
    ~CInputDirectInputKeyboard() = default;

    HRESULT Init(HINSTANCE hInstanse, HWND hWnd) override;
    void Uninit(void) override;
    HRESULT GetKey(std::span<input::ButtonState> keyState) const override;

    // 非公開
private:
    HRESULT SetProperty(void);

    LPDIRECTINPUTDEVICE8 m_pDevice;        // インプットデバイス

    std::array <bool, input::MAX_KEY> m_key;	// 今回のキー状態
    std::array <bool, input::MAX_KEY> m_keyOld; // 前回のキー状態
};

//-----------------------------
// DirectInputマウスクラス
//-----------------------------
class CInputDirectInputMouse : public CInputMouse
{
    // 公開
public:
    CInputDirectInputMouse() : m_pDevice{}, m_button{}, m_buttonOld{} {}
    ~CInputDirectInputMouse() = default;

    HRESULT Init(HINSTANCE hInstanse, HWND hWnd) override;
    void Uninit(void) override;
    HRESULT GetButton(std::span<input::ButtonState> buttonState) const override;
    HRESULT GetMove(input::Axis* pMove) const override;
    HRESULT GetWheel(float* pWheel) const override;

    // 非公開
private:
    HRESULT SetProperty(void);

    LPDIRECTINPUTDEVICE8 m_pDevice;        // インプットデバイス

    std::array<bool, common::Index8(input::MOUSE_BUTTON::Max)> m_button;	// 今回のボタン状態
    std::array<bool, common::Index8(input::MOUSE_BUTTON::Max)> m_buttonOld; // 前回のボタン状態
};

//-----------------------------
// DirectInputコントローラークラス
//-----------------------------
class CInputDirectInputController : public CInputController
{
    // 公開
public:
    CInputDirectInputController() : m_pDevice{}, m_effect{}, m_guidProduct{}, m_type{}, m_bForceFeedback{}, m_effectGuid{}, m_button{}, m_buttonOld{} {}
    ~CInputDirectInputController() = default;

    HRESULT Init(HINSTANCE hInstanse, HWND hWnd, common::Index8 idx = common::INVALID_ID8) override;
    void Uninit(void) override;
    HRESULT GetButton(std::span<input::ButtonState> buttonState) override;
    HRESULT GetStick(std::span<input::Axis> axis) const override;
    HRESULT GetTrigger(std::span<float> trigger) const override;
    HRESULT GetSlider(std::span<float> slider) const override;
    void Vibrate(float leftMotorSpeed = 1.0f, float rightMotorSpeed = 1.0f)  override;

    void SetProductGuid(const GUID& guidProduct) { m_guidProduct = guidProduct; }
    GUID GetProductGuid(void) { return m_guidProduct; }

    // 非公開
private:
    static BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);
    static BOOL CALLBACK EnumEffectsCallback(LPCDIEFFECTINFO pdei, LPVOID pvRef);
    HRESULT SetProperty(void);

    LPDIRECTINPUTDEVICE8 m_pDevice;     // インプットデバイス
    GUID m_guidProduct;                 // プロダクトGUID
    DIRECTINPUT_CONTROLLER_TYPE m_type; // コントローラータイプ

    bool m_bForceFeedback;           // フォースフィードバック対応フラグ
    GUID m_effectGuid;               // 振動エフェクトGUID
    LPDIRECTINPUTEFFECT m_effect;    // 振動

    std::array<bool, common::Index8(input::CONTROLLER_BUTTON::Max) > m_button;    // 今回のボタン状態
    std::array<bool, common::Index8(input::CONTROLLER_BUTTON::Max)> m_buttonOld; // 前回のボタン状態
};
