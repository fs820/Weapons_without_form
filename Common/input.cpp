//---------------------------------------
//
//入力処理[input.cpp]
//Author fuma sato
//
//---------------------------------------
#include"input.h"
#include "useful.h"
#include "file.h"

#include <cstddef> // offsetof を使うために必要
#include <limits>  // std::numeric_limits を使うために必要
#include <algorithm>

using namespace input; // インプット空間の使用
using namespace common; // 共通空間の使用

//----------------------------------
//
// インプットクラス (呼び出し用窓口)
//
//----------------------------------

//-----------------
// 初期化
//-----------------
HRESULT CInput::Init(HINSTANCE hInstanse, HWND hWnd)
{
    // DirectInput初期化
    if (FAILED(CInputDirectInput::DirectInputSetUp(hInstanse))) return E_FAIL;

	// キーボード
	if (m_pKeyboard == nullptr)
	{
		m_pKeyboard = new CInputKeyboardManager;

		if (m_pKeyboard != nullptr)
		{
			if (FAILED(m_pKeyboard->Init(hInstanse, hWnd))) return E_FAIL;
		}
	}

	// マウス
	if (m_pMouse == nullptr)
	{
		m_pMouse = new CInputMouseManager;

		if (m_pMouse != nullptr)
		{
			if (FAILED(m_pMouse->Init(hInstanse, hWnd))) return E_FAIL;
		}
	}

	// コントローラー
	if (m_pController == nullptr)
	{
		m_pController = new CInputControllerManager;

		if (m_pController != nullptr)
		{
			if (FAILED(m_pController->Init(hInstanse, hWnd))) return E_FAIL;
		}
	}
	return S_OK;
}

//-----------------
// 終了
//-----------------
void CInput::Uninit(void)
{
	// キーボード
	SAFE_UNINIT(m_pKeyboard);

	// マウス
	SAFE_UNINIT(m_pMouse);

	// コントローラー
	SAFE_UNINIT(m_pController);

    // DirectInput解放
    CInputDirectInput::DirectInputRelease();
}

//-----------------
// 更新
//-----------------
void CInput::Update(void)
{
	// キーボード
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Update();
	}

	// マウス
	if (m_pMouse != nullptr)
	{
		m_pMouse->Update();
	}

	// コントローラー
	if (m_pController != nullptr)
	{
		m_pController->Update();
	}
}

//-----------------
// 押している
//-----------------
bool CInput::IsDown(Index8 idx, BUTTON button) const
{
	return m_pKeyboard->IsDown(idx, button) || m_pMouse->IsDown(idx, button) || m_pController->IsDown(idx, button);
}

//-----------------
// 押した瞬間
//-----------------
bool CInput::IsTrigger(Index8 idx, BUTTON button) const
{
	return m_pKeyboard->IsTrigger(idx, button) || m_pMouse->IsTrigger(idx, button) || m_pController->IsTrigger(idx, button);
}

//-----------------
// 離した瞬間
//-----------------
bool CInput::IsRelease(Index8 idx, BUTTON button) const
{
	return m_pKeyboard->IsRelease(idx, button) || m_pMouse->IsRelease(idx, button) || m_pController->IsRelease(idx, button);
}

//-----------------
// 長押し
//-----------------
size_t CInput::Count(void) const
{
	return m_pKeyboard->Count() + m_pMouse->Count() + m_pController->Count();
}

//--------------------------
//
// キーボード
//
//--------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputKeyboardManager::Init(HINSTANCE hInstanse, HWND hWnd)
{
	CInputKeyboard* pKeyboard{}; // キーボードポインタ

	// raw
	do
	{// ループ
        Index8 count = 0;

		if (pKeyboard != nullptr) return E_FAIL;  // null
		pKeyboard = new CInputRawInputKeyboard;   // rawInput生成
		if (pKeyboard == nullptr)  return E_FAIL; // 生成失敗

		// 初期化
		if (SUCCEEDED(pKeyboard->Init(hInstanse, hWnd, count)))
		{// 成功
			m_apKeyboard.push_back(pKeyboard); // キーボード追加
			pKeyboard = nullptr; // null
		}
		else
		{
			SAFE_UNINIT(pKeyboard); // 捨てる
			break;
		}
        ++count;
	} while (true);

	if (m_apKeyboard.size() > 0) { return S_OK; } // キーボード生成済み

	// Direct
	if (pKeyboard != nullptr) return E_FAIL;   // null
	pKeyboard = new CInputDirectInputKeyboard; // DirectInput生成
	if (pKeyboard == nullptr)  return E_FAIL;  // 生成失敗

	// 初期化
	if (SUCCEEDED(pKeyboard->Init(hInstanse, hWnd)))
	{// 成功
		m_apKeyboard.push_back(pKeyboard); // キーボード追加
		return S_OK;
	}
	SAFE_UNINIT(pKeyboard); // 捨てる
	return E_FAIL;
}

//----------------------
//終了処理
//----------------------
void CInputKeyboardManager::Uninit(void)
{
	SAFE_UNINIT_ARRAY(m_apKeyboard); // キーボードをすべて破棄
}

//-------------------------
//更新処理
//-------------------------
void CInputKeyboardManager::Update(void)
{
	for (auto& pKeyboard : m_apKeyboard)
	{// 全てのキーボードの更新
		pKeyboard->Update();
	}
}

//------------------------------
//キー押下処理
//------------------------------
bool CInputKeyboardManager::IsDown(Index8 idx, BUTTON button) const
{
	return m_apKeyboard[idx]->IsDown(0);
}

//------------------------------
//キー入力時の処理
//------------------------------
bool CInputKeyboardManager::IsTrigger(Index8 idx, BUTTON button) const
{
    return m_apKeyboard[idx]->IsTrigger(0);
}

//------------------------------
//キー話離した際の処理
//------------------------------
bool CInputKeyboardManager::IsRelease(Index8 idx, BUTTON button) const
{
    return m_apKeyboard[idx]->IsRelease(0);
}

//------------------------------
// コンフィグ読み込み
//------------------------------
bool CInputKeyboardManager::LoadConfig(const char* pFileName)
{
    CFile file(pFileName);      // ファイルクラス
    auto map = file.ReadJson(); // JSON読み込み
}

//--------------------------
//
// マウス
//
//--------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputMouseManager::Init(HINSTANCE hInstanse, HWND hWnd)
{
	CInputMouse* pMouse{}; // マウスポインタ

	// raw
	do
	{// ループ
        Index8 count = 0;

		if (pMouse != nullptr) return E_FAIL;  // null
		pMouse = new CInputRawInputMouse;      // rawInput生成
		if (pMouse == nullptr)  return E_FAIL; // 生成失敗

		// 初期化
        if (SUCCEEDED(pMouse->Init(hInstanse, hWnd, count)))
        {// 成功
			m_apMouse.push_back(pMouse); // マウス追加
			pMouse = nullptr; // null
		}
		else
		{
			SAFE_UNINIT(pMouse); // 捨てる
			break;
		}
        ++count;
	} while (true);

	if (m_apMouse.size() > 0) { return S_OK; } // マウス生成済み

	// Direct
	if (pMouse != nullptr) return E_FAIL;  // null
	pMouse = new CInputDirectInputMouse;   // DirectInput生成
	if (pMouse == nullptr)  return E_FAIL; // 生成失敗

	// 初期化
	if (SUCCEEDED(pMouse->Init(hInstanse, hWnd)))
	{// 成功
		m_apMouse.push_back(pMouse); // マウス追加
		return S_OK;
	}
	SAFE_UNINIT(pMouse); // 捨てる
	return E_FAIL;
}

//----------------------
//終了処理
//----------------------
void CInputMouseManager::Uninit(void)
{
	SAFE_UNINIT_ARRAY(m_apMouse); // マウスをすべて破棄
}

//-------------------------
//更新処理
//-------------------------
void CInputMouseManager::Update(void)
{
	for (auto& pMouse : m_apMouse)
	{// 全てのマウスの更新
		pMouse->Update();
	}
}

//------------------------------
//キー押下処理
//------------------------------
bool CInputMouseManager::IsDown(Index8 idx, BUTTON button) const
{
    return m_apMouse[idx]->IsDown(MOUSE_BUTTON::Left);
}

//------------------------------
//キー入力時の処理
//------------------------------
bool CInputMouseManager::IsTrigger(Index8 idx, BUTTON button) const
{
    return m_apMouse[idx]->IsTrigger(MOUSE_BUTTON::Left);
}

//------------------------------
//キー話離した際の処理
//------------------------------
bool CInputMouseManager::IsRelease(Index8 idx, BUTTON button) const
{
    return m_apMouse[idx]->IsRelease(MOUSE_BUTTON::Left);
}

//--------------
//マウス移動
//--------------
Axis CInputMouseManager::GetAxis(Index8 idx) const
{
    return m_apMouse[idx]->GetAxis();
}

//----------------
//キー押下処理
//----------------
float CInputMouseManager::GetWheel(Index8 idx) const
{
    return m_apMouse[idx]->GetWheel();
}

//------------------------------------
//
// コントローラークラス
//
//------------------------------------

//-------------------
// 初期化
//-------------------
HRESULT CInputControllerManager::Init(HINSTANCE hInstanse, HWND hWnd)
{
    // X
    XInputEnable(true); // XInput有効化

	CInputController* pController{}; // コントローラーポインタ

	for (Index8 cnt = 0; cnt < 4u; cnt++)
	{// XInput確認
		if (pController != nullptr) return E_FAIL;  // null
		pController = new CInputXInputController;   // XInput生成
		if (pController == nullptr)  return E_FAIL; // 生成失敗

		// 初期化
		if (SUCCEEDED(pController->Init(hInstanse, hWnd, cnt)))
		{// 成功
			m_apController.push_back(pController); // コントローラー追加
			pController = nullptr;                 // null
			continue;                              // 続ける
		}
		SAFE_UNINIT(pController); // 捨てる
	}

	size_t XInputNum = m_apController.size(); // Xinputの数

	// Direct
    
	// デバイス列挙
	if (FAILED(CInputDirectInput::SetEnum(this)))return E_FAIL;

	for (Index cnt = XInputNum; cnt < m_apController.size(); cnt++)
	{// DirectInputを確認
		// 初期化
		if (FAILED(m_apController[cnt]->Init(hInstanse, hWnd)))
		{// 失敗
			SwapRemove(m_apController, cnt); // 削除
		}
	}

	// メモリを合わせる希望
	m_apController.shrink_to_fit();

	return S_OK;
}

//-------------------
// 初期化
//-------------------
void CInputControllerManager::Uninit(void)
{
	SAFE_UNINIT_ARRAY(m_apController);       // コントローラーをすべて破棄
	XInputEnable(false);                     // XInput無効化
}

//-------------------
// 初期化
//-------------------
void CInputControllerManager::Update(void)
{
	for (auto& pController : m_apController)
	{// 全てのコントローラーの更新
		pController->Update();
	}
}

//-------------------
// 初期化
//-------------------
bool CInputControllerManager::IsDown(Index idx, BUTTON button) const
{
	return m_apController[idx]->IsDown(CONTROLLER_BUTTON::Up);
}

//-------------------
// 初期化
//-------------------
bool CInputControllerManager::IsTrigger(Index idx, BUTTON button) const
{
    return m_apController[idx]->IsTrigger(CONTROLLER_BUTTON::Up);
}

//-------------------
// 初期化
//-------------------
bool CInputControllerManager::IsRelease(Index idx, BUTTON button) const
{
    return m_apController[idx]->IsRelease(CONTROLLER_BUTTON::Up);
}

//-------------------
// 初期化
//-------------------
Axis CInputControllerManager::GetAxis(Index idx, Direction lr) const
{
    return m_apController[idx]->GetAxis(lr);
}

//-------------------
// 初期化
//-------------------
float CInputControllerManager::GetTrigger(Index idx, Direction lr) const
{
    return m_apController[idx]->GetTrigger(lr);
}

//-------------------
// 初期化
//-------------------
float CInputControllerManager::GetSlider(Index idx, Direction lr) const
{
    return m_apController[idx]->GetSlider(lr);
}

//-------------------
// 初期化
//-------------------
void CInputControllerManager::SetVibrate(Index idx, float fLeftPower, float fReghtPower)
{
	m_apController[idx]->Vibrate(fLeftPower, fReghtPower);
}

//--------------------------
//
// キーボード基底
//
//--------------------------

//------
// 更新
//------
void CInputKeyboard::Update(void)
{
	std::array <ButtonState, MAX_KEY> keyState{};
	if (SUCCEEDED(GetKey(keyState)))
	{
		for (Index8 cntKey = 0; cntKey < keyState.size(); cntKey++)
		{
			m_keyState[cntKey] = keyState[cntKey]; //一時的な情報を正規情報に渡す
		}
	}
}

//--------------------------
//
// マウス基底
//
//--------------------------

//------
// 更新
//------
void CInputMouse::Update(void)
{
	// ボタンの状態を解析
	std::array<ButtonState, Index8(MOUSE_BUTTON::Max)> buttonState{};
	if (SUCCEEDED(GetButton(buttonState)))
	{
		for (Index8 cntKey = 0; cntKey < buttonState.size(); cntKey++)
		{
			m_buttonState[cntKey] = buttonState[cntKey]; //一時的な情報を正規情報に渡す
		}
	}

	GetMove(&m_Move); 	    // 動きの状態をもらう
	GetWheel(&m_WheelMove); // ホイールの状態をもらう
}

//--------------------------
//
// コントローラー基底
//
//--------------------------

//------
// 更新
//------
void CInputController::Update(void)
{
	// ボタンの状態を解析
	std::array<ButtonState, Index8(CONTROLLER_BUTTON::Max)> buttonState{};
	if (SUCCEEDED(GetButton(buttonState)))
	{
		for (Index8 cntKey = 0; cntKey < buttonState.size(); cntKey++)
		{
			m_buttonState[cntKey] = buttonState[cntKey]; //一時的な情報を正規情報に渡す
		}
	}

	GetStick(m_Stick); 	   // スティックの状態をもらう
	GetTrigger(m_Trigger); // トリガーの状態をもらう
}

//--------------------------
//
// rawInput母体
//
//--------------------------

std::vector<DeviceInfo> CInputRawInput::m_aDevice{};                               // rawInputデバイス
std::unordered_map<HANDLE, std::vector<ButtonState>> CInputRawInput::m_state{};    // デバイスの生データ保存用
std::unordered_map<HANDLE, std::vector<ButtonState>> CInputRawInput::m_stateOld{}; // デバイスの生データ保存用(前回)
std::unordered_map<HANDLE, Axis> CInputRawInput::m_move{};                         // マウス移動量保存用
std::unordered_map<HANDLE, float> CInputRawInput::m_wheel{};                       // マウスホイール移動量保存用

//-----------------------------
// rawInputデバイス登録
//-----------------------------
HRESULT CInputRawInput::SetDevice(HWND hWnd, bool allDevice)
{
    return allDevice ? RegisterAllInputDevices(hWnd) : RegisterStandardInputDevices(hWnd);
}

//---------------------------------------------
// デバイスハンドルを取得 (タイプとインデックスで指定)
//---------------------------------------------
HANDLE CInputRawInput::GetDeviceHandle(DWORD dwType, common::Index8 idx)
{
    Index8 count = 0;
    for (const auto& device : m_aDevice)
    {
        if (device.dwType == dwType)
        {
            if (count == idx)
            {
                return device.hDevice; // デバイスハンドルを返す
            }
            ++count;
        }
    }
    return nullptr; // 見つからなかった場合はnullptrを返す
}

//---------------------------------------------
// メッセージから貰ったデータをデバイスに送信
//---------------------------------------------
HRESULT CInputRawInput::SetRawData(const RAWINPUT& rawData)
{
    switch (rawData.header.dwType)
    {
    case RIM_TYPEKEYBOARD:
        m_state.try_emplace(rawData.header.hDevice, std::vector<bool>(MAX_KEY, false));    // デバイスがなければ追加
        m_stateOld.try_emplace(rawData.header.hDevice, std::vector<bool>(MAX_KEY, false)); // デバイスがなければ追加

        if ((rawData.data.keyboard.Flags & RI_KEY_BREAK) == 0)
        {// 押された
            m_state[rawData.header.hDevice][rawData.data.keyboard.VKey].isDown = true;
            if (!m_stateOld[rawData.header.hDevice][rawData.data.keyboard.VKey].isDown)
            {// 前回押されていなかった
                m_state[rawData.header.hDevice][rawData.data.keyboard.VKey].isTrigger = true; // 押した瞬間
            }
        }
        else
        {
            m_state[rawData.header.hDevice][rawData.data.keyboard.VKey].isDown = false;
            if (m_stateOld[rawData.header.hDevice][rawData.data.keyboard.VKey].isDown)
            {// 前回押されていた
                m_state[rawData.header.hDevice][rawData.data.keyboard.VKey].isRelease = true; // 離した瞬間
            }
        }
        break;
    case RIM_TYPEMOUSE:
        m_state.try_emplace(rawData.header.hDevice, std::vector<bool>(Index8(MOUSE_BUTTON::Max), false));    // デバイスがなければ追加
        m_stateOld.try_emplace(rawData.header.hDevice, std::vector<bool>(Index8(MOUSE_BUTTON::Max), false)); // デバイスがなければ追加
        m_move.try_emplace(rawData.header.hDevice, Axis::Null);                                              // デバイスがなければ追加
        m_wheel.try_emplace(rawData.header.hDevice, 0.0f);                                                   // デバイスがなければ追加

        const RAWMOUSE& mouseData = rawData.data.mouse;

        // MOUSE_MOVE_ABSOLUTE でない限り、lLastX/Yは相対移動量
        if ((mouseData.usFlags & MOUSE_MOVE_ABSOLUTE) == 0)
        {
            m_move[rawData.header.hDevice].x += mouseData.lLastX;
            m_move[rawData.header.hDevice].y += mouseData.lLastY;
        }

        if (mouseData.usButtonFlags & RI_MOUSE_WHEEL)
        {
            // usButtonData は符号付き16bit整数
            short wheelDelta = (short)mouseData.usButtonData;
            m_wheel[rawData.header.hDevice] += wheelDelta;
        }
        if (mouseData.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
        {// 右ボタン押下
            m_state[rawData.header.hDevice][Index8(MOUSE_BUTTON::Left)].isDown = true;
            if (!m_stateOld[rawData.header.hDevice][Index8(MOUSE_BUTTON::Left)].isDown)
            {// 前回押されていなかった
                m_state[rawData.header.hDevice][Index8(MOUSE_BUTTON::Left)].isTrigger = true; // 押した瞬間
            }
        }
        if (mouseData.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
        {// 右ボタン離上
            m_state[rawData.header.hDevice][Index8(MOUSE_BUTTON::Left)].isDown = false;
            if (m_stateOld[rawData.header.hDevice][Index8(MOUSE_BUTTON::Left)].isDown)
            {// 前回押されていた
                m_state[rawData.header.hDevice][Index8(MOUSE_BUTTON::Left)].isRelease = true; // 離した瞬間
            }
        }
        if (mouseData.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
        {// 左ボタン押下
            m_state[rawData.header.hDevice][Index8(MOUSE_BUTTON::Right)].isDown = true;
            if (!m_stateOld[rawData.header.hDevice][Index8(MOUSE_BUTTON::Right)].isDown)
            {// 前回押されていなかった
                m_state[rawData.header.hDevice][Index8(MOUSE_BUTTON::Right)].isTrigger = true; // 押した瞬間
            }
        }
        if (mouseData.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
        {// 左ボタン離上
            m_state[rawData.header.hDevice][Index8(MOUSE_BUTTON::Right)].isDown = false;
            if (m_stateOld[rawData.header.hDevice][Index8(MOUSE_BUTTON::Right)].isDown)
            {// 前回押されていた
                m_state[rawData.header.hDevice][Index8(MOUSE_BUTTON::Right)].isRelease = true; // 離した瞬間
            }
        }
        if (mouseData.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN)
        {// 中ボタン押下
            m_state[rawData.header.hDevice][Index8(MOUSE_BUTTON::Senter)].isDown = true;
            if (!m_stateOld[rawData.header.hDevice][Index8(MOUSE_BUTTON::Senter)].isDown)
            {// 前回押されていなかった
                m_state[rawData.header.hDevice][Index8(MOUSE_BUTTON::Senter)].isTrigger = true; // 押した瞬間
            }
        }
        if (mouseData.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP)
        {// 中ボタン離上
            m_state[rawData.header.hDevice][Index8(MOUSE_BUTTON::Senter)].isDown = false;
            if (m_stateOld[rawData.header.hDevice][Index8(MOUSE_BUTTON::Senter)].isDown)
            {// 前回押されていた
                m_state[rawData.header.hDevice][Index8(MOUSE_BUTTON::Senter)].isRelease = true; // 離した瞬間
            }
        }
        if (mouseData.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN)
        {// 4ボタン押下
            m_state[rawData.header.hDevice][Index8(MOUSE_BUTTON::B1)].isDown = true;
            if (!m_stateOld[rawData.header.hDevice][Index8(MOUSE_BUTTON::B1)].isDown)
            {// 前回押されていなかった
                m_state[rawData.header.hDevice][Index8(MOUSE_BUTTON::B1)].isTrigger = true; // 押した瞬間
            }
        }
        if (mouseData.usButtonFlags & RI_MOUSE_BUTTON_4_UP)
        {// 4ボタン離上
            m_state[rawData.header.hDevice][Index8(MOUSE_BUTTON::B1)].isDown = false;
            if (m_stateOld[rawData.header.hDevice][Index8(MOUSE_BUTTON::B1)].isDown)
            {// 前回押されていた
                m_state[rawData.header.hDevice][Index8(MOUSE_BUTTON::B1)].isRelease = true; // 離した瞬間
            }
        }
        if (mouseData.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN)
        {// 5ボタン押下
            m_state[rawData.header.hDevice][Index8(MOUSE_BUTTON::B2)].isDown = true;
            if (!m_stateOld[rawData.header.hDevice][Index8(MOUSE_BUTTON::B2)].isDown)
            {// 前回押されていなかった
                m_state[rawData.header.hDevice][Index8(MOUSE_BUTTON::B2)].isTrigger = true; // 押した瞬間
            }
        }
        if (mouseData.usButtonFlags & RI_MOUSE_BUTTON_5_UP)
        {// 5ボタン離上
            m_state[rawData.header.hDevice][Index8(MOUSE_BUTTON::B2)].isDown = false;
            if (m_stateOld[rawData.header.hDevice][Index8(MOUSE_BUTTON::B2)].isDown)
            {// 前回押されていた
                m_state[rawData.header.hDevice][Index8(MOUSE_BUTTON::B2)].isRelease = true; // 離した瞬間
            }
        }
        break;
    }
	return S_OK;
}

//---------------------------------------------
// デバイスの生データを取得 (デバイスハンドルで指定)
//---------------------------------------------
void CInputRawInput::GetRawData(HANDLE hDevice, std::span<input::ButtonState> state, Axis* pAxis, float* pWheel)
{
    if (!state.empty() && m_state.find(hDevice) != m_state.end())
    {// デバイスが存在する場合
        for (Index8 cnt = 0; cnt < std::min(state.size(), m_state[hDevice].size()); ++cnt)
        {
            m_stateOld[hDevice] = m_state[hDevice];  // 前回の状態を保存

            state[cnt] = m_state[hDevice][cnt];      // 生データをコピー
            m_state[hDevice][cnt].isTrigger = false; // 押した瞬間をリセット
            m_state[hDevice][cnt].isRelease = false; // 離した瞬間をリセット
        }
    }
    if (pAxis != nullptr && m_move.find(hDevice) != m_move.end())
    {// マウス移動量が欲しい場合
        *pAxis = m_move[hDevice];     // 移動量をコピー
        m_move[hDevice] = Axis::Null; // 移動量をリセット
    }
    if (pWheel != nullptr && m_wheel.find(hDevice) != m_wheel.end())
    {// ホイール移動量が欲しい場合
        *pWheel = m_wheel[hDevice];   // 移動量をコピー
        m_wheel[hDevice] = 0.0f;      // 移動量をリセット
    }
}

//----------------------------------------------------
// インプットの登録 (スタンダード)
//----------------------------------------------------
HRESULT CInputRawInput::RegisterStandardInputDevices(HWND hWnd)
{
    RAWINPUTDEVICE RawDevice[] =
    {
        { 0x01, 0x06, RIDEV_INPUTSINK | RIDEV_NOLEGACY, hWnd }, // Standard Keyboard
        { 0x01, 0x02, RIDEV_INPUTSINK | RIDEV_NOLEGACY, hWnd }, // Standard Mouse
    };

    if (RegisterRawInputDevices(RawDevice, 2, sizeof(RAWINPUTDEVICE)))
    {
        return S_OK;
    }
    else
    {
        return E_FAIL;
    }

    UINT nDevices = 0;
    if (GetRawInputDeviceList(nullptr, &nDevices, sizeof(RAWINPUTDEVICELIST)) != 0)
    {
        return E_FAIL;
    }

    if (nDevices == 0)
    {
        return E_FAIL;
    }

    std::vector<RAWINPUTDEVICELIST> devices(nDevices);
    if (GetRawInputDeviceList(devices.data(), &nDevices, sizeof(RAWINPUTDEVICELIST)) == (UINT)-1)
    {
        return E_FAIL;
    }

    for (const auto& dev : devices)
    {
        m_aDevice.push_back(DeviceInfo(dev.dwType, dev.hDevice)); // デバイスを保存
    }
}

//----------------------------------------------------
// インプットの登録 (ALL)
//----------------------------------------------------
HRESULT CInputRawInput::RegisterAllInputDevices(HWND hWnd)
{
    try
    {
        UINT nDevices = 0;
        if (GetRawInputDeviceList(nullptr, &nDevices, sizeof(RAWINPUTDEVICELIST)) != 0)
        {
            return E_FAIL;
        }

        if (nDevices == 0)
        {
            return E_FAIL;
        }

        std::vector<RAWINPUTDEVICELIST> devices(nDevices);
        if (GetRawInputDeviceList(devices.data(), &nDevices, sizeof(RAWINPUTDEVICELIST)) == (UINT)-1)
        {
            return E_FAIL;
        }

        std::vector<RAWINPUTDEVICE> inputDevices;
        std::set<std::pair<USHORT, USHORT>> registeredPairs;

        for (const auto& dev : devices)
        {
            RID_DEVICE_INFO info = {};
            UINT size = sizeof(info);
            info.cbSize = sizeof(info);

            if (GetRawInputDeviceInfo(dev.hDevice, RIDI_DEVICEINFO, &info, &size) > 0)
            {
                RAWINPUTDEVICE rawDevice = {};
                rawDevice.dwFlags = RIDEV_INPUTSINK;
                rawDevice.hwndTarget = hWnd;

                if (info.dwType == RIM_TYPEKEYBOARD)
                {
                    // キーボードの場合は標準的なUsagePage/Usageを使用
                    USHORT usagePage = 0x01;             // HID_USAGE_PAGE_GENERIC
                    USHORT usage = 0x06;                 // HID_USAGE_GENERIC_KEYBOARD
                    rawDevice.dwFlags |= RIDEV_NOLEGACY | RIDEV_NOHOTKEYS; // NOLEGACY_NOHOTKEYS

                    auto pair = std::make_pair(usagePage, usage);
                    if (registeredPairs.find(pair) == registeredPairs.end())
                    {
                        registeredPairs.insert(pair);
                        rawDevice.usUsagePage = usagePage;
                        rawDevice.usUsage = usage;
                        inputDevices.push_back(rawDevice);
                    }
                }
                else if (info.dwType == RIM_TYPEMOUSE)
                {
                    // マウスの場合は標準的なUsagePage/Usageを使用
                    USHORT usagePage = 0x01;             // HID_USAGE_PAGE_GENERIC
                    USHORT usage = 0x02;                 // HID_USAGE_GENERIC_MOUSE

                    auto pair = std::make_pair(usagePage, usage);
                    if (registeredPairs.find(pair) == registeredPairs.end())
                    {
                        registeredPairs.insert(pair);
                        rawDevice.usUsagePage = usagePage;
                        rawDevice.usUsage = usage;
                        inputDevices.push_back(rawDevice);
                    }
                }
                else if (info.dwType == RIM_TYPEHID)
                {
                    // HIDデバイスの場合のみ info.hid を使用
                    USHORT usagePage = info.hid.usUsagePage;
                    USHORT usage = info.hid.usUsage;

                    // 有効なUsagePage/Usageかチェック
                    if (usagePage != 0)
                    {
                        auto pair = std::make_pair(usagePage, usage);
                        if (registeredPairs.find(pair) == registeredPairs.end())
                        {
                            registeredPairs.insert(pair);
                            rawDevice.usUsagePage = usagePage;
                            rawDevice.usUsage = usage;
                            inputDevices.push_back(rawDevice);
                        }
                    }
                }
            }
            m_aDevice.push_back(DeviceInfo(dev.dwType, dev.hDevice)); // デバイスを保存
        }

        if (!inputDevices.empty())
        {
            if (RegisterRawInputDevices(inputDevices.data(), UINT(inputDevices.size()), sizeof(RAWINPUTDEVICE)))
            {
                return S_OK;
            }
            else
            {
                return E_FAIL;
            }
        }
    }
    catch (...)
    {
        return E_FAIL;
    }
}

//--------------------------
//
// rawInputキーボード
//
//--------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputRawInputKeyboard::Init(HINSTANCE hInstanse, HWND hWnd, Index8 idx)
{
    m_hDevice = CInputRawInput::GetDeviceHandle(RIM_TYPEKEYBOARD, idx); // デバイスハンドルを取得
    if (m_hDevice = nullptr)
    {
        return E_FAIL;
    }

	return S_OK;
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputRawInputKeyboard::GetKey(std::span<input::ButtonState> keyState) const
{
    CInputRawInput::GetRawData(m_hDevice, keyState);
    return S_OK;
}

//--------------------------
//
// rawInputマウス
//
//--------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputRawInputMouse::Init(HINSTANCE hInstanse, HWND hWnd, Index8 idx)
{
    m_hDevice = CInputRawInput::GetDeviceHandle(RIM_TYPEMOUSE, idx); // デバイスハンドルを取得
    if (m_hDevice = nullptr)
    {
        return E_FAIL;
    }

	return S_OK;
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputRawInputMouse::GetButton(std::span<input::ButtonState> buttonState) const
{
    CInputRawInput::GetRawData(m_hDevice, buttonState);
    return S_OK;
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputRawInputMouse::GetMove(input::Axis* pMove) const
{
    CInputRawInput::GetRawData(m_hDevice, std::span<ButtonState>(), pMove);
    return S_OK;
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputRawInputMouse::GetWheel(float* pWheel) const
{
    CInputRawInput::GetRawData(m_hDevice, std::span<ButtonState>(), nullptr, pWheel);
    return S_OK;
}

//--------------------------
//
//パッド
//
//--------------------------

//--------------------------------
//初期化処理
//--------------------------------
HRESULT CInputXInputController::Init(HINSTANCE hInstanse, HWND hWnd, Index8 idx)
{
	m_idx = idx; // インデックスを保存

	XINPUT_STATE state;
	if (XInputGetState(m_idx, &state) == ERROR_SUCCESS)
	{
		return S_OK;
	}
	return E_FAIL;
}

//------------------------
//更新処理
//------------------------
HRESULT CInputXInputController::GetButton(std::span<input::ButtonState> buttonState)
{
	XINPUT_STATE state;
	if (XInputGetState(m_idx, &state) == ERROR_SUCCESS)
	{
		SetTriggerButton(&state);
		for (Index8 cntButton = 0; cntButton < Index8(JOYKEY::Max); cntButton++)
		{
			// 変換表からXInput番号からコントローラー共通の番号を取得する
			Index8 controllerButton = Index8(XConMap[cntButton]);

			// JOYKEYからXInputようのMaskに変換
			WORD mask = GetXInputMask(JOYKEY(cntButton));

            // 前回のボタンの状態を保存する
            m_buttonOld[controllerButton] = m_button[controllerButton];

            // ボタンの状態を取得する
            m_button[controllerButton] = (state.Gamepad.wButtons & mask) != 0;

            // ボタンの状態を保存する
            buttonState[controllerButton].isDown = m_button[controllerButton];
            buttonState[controllerButton].isTrigger = m_button[controllerButton] && !m_buttonOld[controllerButton];
            buttonState[controllerButton].isRelease = !m_button[controllerButton] && m_buttonOld[controllerButton];
		}
		return S_OK;
	}
	return E_FAIL;
}

//------------------------
//更新処理
//------------------------
HRESULT CInputXInputController::GetStick(std::span<input::Axis> axis) const
{
	XINPUT_STATE state;
	if (XInputGetState(m_idx, &state) == ERROR_SUCCESS)
	{
		// 左スティック
		axis[Index8(Direction::Left)].x = float(state.Gamepad.sThumbLX);
		axis[Index8(Direction::Left)].y = float(state.Gamepad.sThumbLY);

		// 右スティック
		axis[Index8(Direction::Right)].x = float(state.Gamepad.sThumbRX);
		axis[Index8(Direction::Right)].y = float(state.Gamepad.sThumbRY);

		return S_OK;
	}

	return E_FAIL;
}

//------------------------
//更新処理
//------------------------
HRESULT CInputXInputController::GetTrigger(std::span<float> trigger) const
{
	constexpr float TRIGGER_MAX = float(std::numeric_limits<BYTE>::max());

	XINPUT_STATE XState;
	if (XInputGetState(m_idx, &XState) == ERROR_SUCCESS)
	{
		// 左スティック
		trigger[Index8(Direction::Left)] = float(XState.Gamepad.bLeftTrigger) / TRIGGER_MAX;

		// 右スティック
		trigger[Index8(Direction::Right)] = float(XState.Gamepad.bRightTrigger) / TRIGGER_MAX;

		return S_OK;
	}

	return E_FAIL;
}

//------------------------
// スライダー
//------------------------
HRESULT CInputXInputController::GetSlider(std::span<float> slider) const
{
	XINPUT_STATE state;
	if (XInputGetState(m_idx, &state) == ERROR_SUCCESS)
	{
		// XInputのスライダーはスティックのY軸を入れておく
		slider[Index8(Direction::Left)] = float(state.Gamepad.sThumbLY) / float(STICK_NUM);
		slider[Index8(Direction::Right)] = float(state.Gamepad.sThumbRY) / float(STICK_NUM);

		return S_OK;
	}

	return E_FAIL;
}

//----------------
//振動
//----------------
void CInputXInputController::Vibrate(float leftMotorSpeed, float rightMotorSpeed)
{
	constexpr WORD VIBRATION_MAX = std::numeric_limits<WORD>::max();
	leftMotorSpeed = std::clamp(leftMotorSpeed, 0.0f, 1.0f);
	rightMotorSpeed = std::clamp(rightMotorSpeed, 0.0f, 1.0f);
	XINPUT_VIBRATION vibration{ WORD(float(VIBRATION_MAX) * leftMotorSpeed) ,WORD(float(VIBRATION_MAX) * rightMotorSpeed) };
	XInputSetState(0, &vibration);
}

//-----------------------------------------------
// トリガーの値が閾値を超えたらボタンに入力に送る
//-----------------------------------------------
void CInputXInputController::SetTriggerButton(PXINPUT_STATE pState) const
{
	// トリガーの閾値を確認
	if (pState->Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{// 超えていたら
		// wButtonsのビットに格納
		pState->Gamepad.wButtons |= XINPUT_GAMEPAD_LEFT_TRIGGER;
	}
	if (pState->Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{// 超えていたら
		// wButtonsのビットに格納
		pState->Gamepad.wButtons |= XINPUT_GAMEPAD_RIGHT_TRIGGER;
	}
}

//-----------------------------------------------
// 列挙型をXInput本来のマクロに置き換える
//-----------------------------------------------
WORD CInputXInputController::GetXInputMask(input::JOYKEY key) const
{
	switch (key)
	{
	case JOYKEY::Up:    return XINPUT_GAMEPAD_DPAD_UP;
	case JOYKEY::Down:  return XINPUT_GAMEPAD_DPAD_DOWN;
	case JOYKEY::Left:  return XINPUT_GAMEPAD_DPAD_LEFT;
	case JOYKEY::Right: return XINPUT_GAMEPAD_DPAD_RIGHT;
	case JOYKEY::Start: return XINPUT_GAMEPAD_START;
	case JOYKEY::Back:  return XINPUT_GAMEPAD_BACK;
	case JOYKEY::L3:    return XINPUT_GAMEPAD_LEFT_THUMB;
	case JOYKEY::R3:    return XINPUT_GAMEPAD_RIGHT_THUMB;
	case JOYKEY::LB:    return XINPUT_GAMEPAD_LEFT_SHOULDER;
	case JOYKEY::RB:    return XINPUT_GAMEPAD_RIGHT_SHOULDER;
	case JOYKEY::LT:    return XINPUT_GAMEPAD_LEFT_TRIGGER;
	case JOYKEY::RT:    return XINPUT_GAMEPAD_RIGHT_TRIGGER;
	case JOYKEY::A:     return XINPUT_GAMEPAD_A;
	case JOYKEY::B:     return XINPUT_GAMEPAD_B;
	case JOYKEY::X:     return XINPUT_GAMEPAD_X;
	case JOYKEY::Y:     return XINPUT_GAMEPAD_Y;
	default:            return 0;
	}
}

//----------------------------------
//
// DirectInput母体
//
//----------------------------------

LPDIRECTINPUT8 CInputDirectInput::m_pInput{};

//-----------------
// 初期化
//-----------------
HRESULT CInputDirectInput::DirectInputSetUp(HINSTANCE hInstanse)
{
	//インプットオブジェクトの作成
	if (m_pInput == nullptr)
	{
		if (FAILED(DirectInput8Create
		(
			hInstanse,
			DIRECTINPUT_VERSION, IID_IDirectInput8,
			(void**)&m_pInput,
			nullptr
		)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//-----------------
// 破棄
//-----------------
void CInputDirectInput::DirectInputRelease(void)
{
    //インプットオブジェクトの破棄
    SAFE_RELEASE(m_pInput);
}

//-----------------
// 列挙
//-----------------
HRESULT CInputDirectInput::SetEnum(CInputControllerManager* pControllerManager)
{
	//インプットオブジェクトの作成
	if (m_pInput != nullptr)
	{
		// DirectInputデバイスを列挙
		return m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumDevicesCallback, pControllerManager, DIEDFL_ATTACHEDONLY);
	}
	return E_FAIL;
}

//---------------------------
//デバイス情報取得
//---------------------------
BOOL CALLBACK CInputDirectInput::EnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
    // guidProductからベンダーIDを抽出
    WORD vendorId = LOWORD(lpddi->guidProduct.Data1);

    // ベンダーIDがMicrosoftのものであれば、XInputデバイスの可能性が高い
    if (vendorId == MICROSOFT_VENDOR_ID)
    {
        return DIENUM_CONTINUE; // XInputデバイスなので無視して列挙を続行
    }

	// CInputControllerManagerを抽出する
	CInputControllerManager* pControllerManager = static_cast<CInputControllerManager*>(pvRef);

	// DirectInputコントローラーの生成
	CInputDirectInputController* pdController = new CInputDirectInputController;

	// デバイス情報を渡す
	pdController->SetProductGuid(lpddi->guidProduct);

	// 基底クラスポインタにアップ (安全)
	CInputController* pController = pdController;

	// Managerに渡す
	pControllerManager->Register(pController);

	// 列挙を続行
	return DIENUM_CONTINUE;
}

//--------------------------
//
// DirectInputキーボード
//
//--------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputDirectInputKeyboard::Init(HINSTANCE hInstanse, HWND hWnd, Index8 idx)
{
	//インプットデバイスの作成 
	if (FAILED(CInputDirectInput::GetDirectInput()->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr))) return E_FAIL;

	//フォーマット作成
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	// バッファリングを有効にする
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	// デバイス設定
	if (FAILED(SetProperty()))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	//アクセス権限の取得
	if (FAILED(m_pDevice->Acquire()))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	return S_OK;
}

//----------------------
//終了処理
//----------------------
void CInputDirectInputKeyboard::Uninit(void)
{
	// デバイスの破棄
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputDirectInputKeyboard::GetKey(std::span<input::ButtonState> keyState) const
{
	if (m_pDevice != nullptr)
	{
		BYTE state[MAX_KEY];//一時入力格納
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(state), state)))
		{
			for (Index8 cntKey = 0; cntKey < keyState.size(); cntKey++)
			{
                keyState[cntKey].isDown = (state[cntKey] & 0x80) != 0;
			}

            DIDEVICEOBJECTDATA events[DIRECT_BUFFER_SIZE];
            DWORD numEvents = DIRECT_BUFFER_SIZE;
            if (SUCCEEDED(m_pDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), events, &numEvents, 0)))
            {
                for (Index8 cntEvent = 0; cntEvent < numEvents; ++cntEvent)
                {
                    if (events[cntEvent].dwData & 0x80)
                    {// 押された
                        keyState[Index8(events[cntEvent].dwOfs)].isTrigger = true;
                    }
                    else
                    {// 離された
                        keyState[Index8(events[cntEvent].dwOfs)].isRelease = true;
                    }
                }
                return S_OK;
            }
		}
		else
		{
			m_pDevice->Acquire();//アクセス権限の取得
		}
	}
	return E_FAIL;
}

//------------------------
// デバイス設定
//------------------------
HRESULT CInputDirectInputKeyboard::SetProperty(void)
{
	if (m_pDevice == nullptr)return E_FAIL;

	// デバイス全体に設定
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.diph.dwObj = 0;

	// 16個のイベントを格納できるバッファを作成
	dipdw.dwData = DIRECT_BUFFER_SIZE;
	return m_pDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
}

//--------------------------
//
// DirectInputマウス
//
//--------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputDirectInputMouse::Init(HINSTANCE hInstanse, HWND hWnd, Index8 idx)
{
	//インプットデバイスの作成
	if (FAILED(CInputDirectInput::GetDirectInput()->CreateDevice(GUID_SysMouse, &m_pDevice, nullptr)))return E_FAIL;

	//フォーマット作成
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	// バッファリングを有効にする
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	// デバイス設定
	if (FAILED(SetProperty()))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	//アクセス権限の取得
	if (FAILED(m_pDevice->Acquire()))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	return S_OK;
}

//----------------------
//終了処理
//----------------------
void CInputDirectInputMouse::Uninit(void)
{
	// デバイスの破棄
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputDirectInputMouse::GetButton(std::span<input::ButtonState> buttonState) const
{
	if (m_pDevice != nullptr)
	{
		DIMOUSESTATE2 state{};//マウス情報//一時入力格納
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &state)))
		{
			for (Index8 cntButton = 0; cntButton < buttonState.size(); cntButton++)
			{
                buttonState[cntButton].isDown = (state.rgbButtons[cntButton] & 0x80) != 0;
			}

            DIDEVICEOBJECTDATA events[DIRECT_BUFFER_SIZE];
            DWORD numEvents = DIRECT_BUFFER_SIZE;
            if (SUCCEEDED(m_pDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), events, &numEvents, 0)))
            {
                for (Index8 cntEvent = 0; cntEvent < numEvents; ++cntEvent)
                {
                    if (events[cntEvent].dwOfs >= DIMOFS_BUTTON0 && events[cntEvent].dwOfs <= DIMOFS_BUTTON7)
                    {// ボタンのイベント
                        Index8 index = Index8(events[cntEvent].dwOfs - DIMOFS_BUTTON0);
                        if (events[cntEvent].dwData & 0x80)
                        {// 押された
                            buttonState[index].isTrigger = true;
                        }
                        else
                        {// 離された
                            buttonState[index].isRelease = true;
                        }
                    }
                }
                return S_OK;
            }
		}
		else
		{
			m_pDevice->Acquire();//アクセス権限の取得
		}
	}
	return E_FAIL;
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputDirectInputMouse::GetMove(input::Axis* pMove) const
{
	if (m_pDevice != nullptr)
	{
		DIMOUSESTATE2 state{};//マウス情報//一時入力格納
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &state)))
		{
			pMove->x = float(state.lX);
			pMove->y = float(state.lY);
			return S_OK;
		}
		else
		{
			m_pDevice->Acquire();//アクセス権限の取得
		}
	}
	return E_FAIL;
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputDirectInputMouse::GetWheel(float* pWheel) const
{
	if (m_pDevice!=nullptr)
	{
		DIMOUSESTATE2 state{};//マウス情報//一時入力格納
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &state)))
		{
			*pWheel = float(state.lZ);
			return S_OK;
		}
		else
		{
			m_pDevice->Acquire();//アクセス権限の取得
		}
	}
	return E_FAIL;
}

//------------------------
// デバイス設定
//------------------------
HRESULT CInputDirectInputMouse::SetProperty(void)
{
	if (m_pDevice == nullptr)return E_FAIL;

	// デバイス全体に設定
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.diph.dwObj = 0;

	// 16個のイベントを格納できるバッファを作成
	dipdw.dwData = DIRECT_BUFFER_SIZE;
	return m_pDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
}

//-----------------------------
//
//dinputパッド
//
//-----------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputDirectInputController::Init(HINSTANCE hInstanse, HWND hWnd, Index8 idx)
{
	//インプットデバイスの作成
	if (FAILED(CInputDirectInput::GetDirectInput()->CreateDevice(m_guidProduct, &m_pDevice, nullptr)))return E_FAIL;

	//フォーマット作成
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIJoystick)))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_EXCLUSIVE))))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	// デバイス設定
	if (FAILED(SetProperty()))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	//アクセス権限の取得
	if (FAILED(m_pDevice->Acquire()))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

    // コントローラーの種類を判別
    if (IsEqualGUID(m_guidProduct, ELECOM))
    {// ELECOM
        m_pConMap = EleConMap;
    }
    else if (IsEqualGUID(m_guidProduct, PlayStation))
    {// PlayStation
        m_pConMap = PsConMap;
    }
    else if (IsEqualGUID(m_guidProduct, Nintendo))
    {// Nintendo
        m_pConMap = NinConMap;
    }
    else
    {// 不明
        m_pConMap = DConMap;
    }

	return S_OK;
}

//----------------------
//終了処理
//----------------------
void CInputDirectInputController::Uninit(void)
{
	//エフェクトデバイスの破棄
	Vibrate(0);

	// デバイスの破棄
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputDirectInputController::GetButton(std::span<input::ButtonState> buttonState)
{
	if (m_pDevice != nullptr)
	{
		DIJOYSTATE  state;// 入力格納
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIJOYSTATE), &state)))
		{
            for (Index8 cntButton = 0; cntButton < m_pConMap.size(); cntButton++)
            {
                CONTROLLER_BUTTON button = m_pConMap[cntButton];
                buttonState[Index8(button)].isDown = (state.rgbButtons[cntButton] & 0x80) != 0;
            }

            DIDEVICEOBJECTDATA events[DIRECT_BUFFER_SIZE];
            DWORD numEvents = DIRECT_BUFFER_SIZE;
            if (SUCCEEDED(m_pDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), events, &numEvents, 0)))
            {
                for (Index8 cntEvent = 0; cntEvent < numEvents; ++cntEvent)
                {
                    if (events[cntEvent].dwOfs >= DIJOFS_BUTTON0 && events[cntEvent].dwOfs <= DIJOFS_BUTTON31)
                    {// ボタンのイベント
                        Index8 index = Index8(events[cntEvent].dwOfs - DIJOFS_BUTTON0);
                        CONTROLLER_BUTTON button = m_pConMap[index];
                        if (events[cntEvent].dwData & 0x80)
                        {// 押された
                            buttonState[Index8(button)].isTrigger = true;
                        }
                        else
                        {// 離された
                            buttonState[Index8(button)].isRelease = true;
                        }
                    }
                }
                return S_OK;
            }
        }
		else
		{
			m_pDevice->Acquire();//アクセス権限の取得
		}
	}
	return E_FAIL;
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputDirectInputController::GetStick(std::span<input::Axis> axis) const
{
	if (m_pDevice != nullptr)
	{
		DIJOYSTATE  state;// 入力格納
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIJOYSTATE), &state)))
		{
			axis[Index8(Direction::Left)].x = float(state.lX) / float(DIRECTINPUT_TRIGGER_MAX);
			axis[Index8(Direction::Left)].y = float(state.lY) / float(DIRECTINPUT_TRIGGER_MAX);
			axis[Index8(Direction::Right)].x = float(state.lRx) / float(DIRECTINPUT_TRIGGER_MAX);
			axis[Index8(Direction::Right)].y = float(state.lRy) / float(DIRECTINPUT_TRIGGER_MAX);
			return S_OK;
		}
		else
		{
			m_pDevice->Acquire();//アクセス権限の取得
		}
	}
	return E_FAIL;
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputDirectInputController::GetTrigger(std::span<float> trigger) const
{
	if (m_pDevice != nullptr)
	{
		DIJOYSTATE state{};//一時入力格納
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIJOYSTATE), &state)))
		{
			trigger[Index8(Direction::Left)] = float(state.lZ) / float(DIRECTINPUT_TRIGGER_MAX);
			trigger[Index8(Direction::Left)] = float(state.lRz) / float(DIRECTINPUT_TRIGGER_MAX);
			return S_OK;
		}
		else
		{
			m_pDevice->Acquire();//アクセス権限の取得
		}
	}
	return E_FAIL;
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputDirectInputController::GetSlider(std::span<float> slider) const
{
	if (m_pDevice != nullptr)
	{
		DIJOYSTATE state{};//一時入力格納
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIJOYSTATE), &state)))
		{
			// スライダー入力を取る
			slider[Index8(Direction::Left)] = float(state.rglSlider[Index8(Direction::Left)]) / float(DIRECTINPUT_TRIGGER_MAX);
			slider[Index8(Direction::Right)] = float(state.rglSlider[Index8(Direction::Right)]) / float(DIRECTINPUT_TRIGGER_MAX);
			return S_OK;
		}
		else
		{
			m_pDevice->Acquire();//アクセス権限の取得
		}
	}
	return E_FAIL;
}

//-------------
//振動
//-------------
void CInputDirectInputController::Vibrate(float leftMotorSpeed, float rightMotorSpeed)
{
    // 以前に作成したエフェクトがあれば破棄
    if (m_effect != nullptr)
    {
        m_effect->Stop();
        m_effect->Release();
        m_effect = nullptr;
    }

    // 振動非対応、またはモーター強度が0なら処理しない
    if (!m_bForceFeedback) return;
    float motorSpeed = std::max(leftMotorSpeed, rightMotorSpeed);
    if (motorSpeed < EPSILON) return;

    // フォースフィードバックのエフェクトを作成する
    DIEFFECT effect;
    ZeroMemory(&effect, sizeof(DIEFFECT));
    effect.dwSize = sizeof(DIEFFECT);
    effect.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
    effect.dwDuration = INFINITE;  // 無限に続く振動
    effect.dwGain = DI_FFNOMINALMAX;
    effect.dwTriggerButton = DIEB_NOTRIGGER;

    // RumbleやConstantForceでは軸の設定は不要なことが多いですが、
    // ドライバによっては必要な場合があるため設定しておくとより安全です。
    DWORD rgdwAxes[2] = { DIJOFS_X, DIJOFS_Y }; // 2軸指定がより一般的
    LONG rglDirection[2] = { 0, 0 };
    effect.cAxes = 2;
    effect.rgdwAxes = rgdwAxes;
    effect.rglDirection = rglDirection;

    // --- ここからがGUIDに応じた分岐 ---
    DICONSTANTFORCE cf;

    // 保存しておいたGUIDに応じて、使用するパラメータ構造体を変える
    if (IsEqualGUID(m_effectGuid, GUID_ConstantForce))
    {
        // ConstantForceの場合
        cf.lMagnitude = LONG(motorSpeed * DI_FFNOMINALMAX);
        effect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
        effect.lpvTypeSpecificParams = &cf;
    }
    else
    {
        return; // 対応していない
    }

    // 保存しておいたGUIDを使ってエフェクトを作成
    if (SUCCEEDED(m_pDevice->CreateEffect(m_effectGuid, &effect, &m_effect, nullptr)))
    {
        if (FAILED(m_effect->Start(1, 0)))
        {
            m_effect->Release();
            m_effect = nullptr;
        }
    }
}

//------------------------
// デバイス設定
//------------------------
HRESULT CInputDirectInputController::SetProperty(void)
{
	if (m_pDevice == nullptr)return E_FAIL;

	// デバイス全体に設定
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.diph.dwObj = 0;

	// 16個のイベントを格納できるバッファを作成
	dipdw.dwData = DIRECT_BUFFER_SIZE;
	if (FAILED(m_pDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))return E_FAIL;

	// デッドゾーンを20%に設定
	dipdw.dwData = 2000;
	if (FAILED(m_pDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph)))return E_FAIL;

	// 軸オブジェクト設定
    if (FAILED(m_pDevice->EnumObjects(EnumAxesCallback, this, DIDFT_AXIS)))return E_FAIL;

    // 振動エフェクトの列挙
    return m_pDevice->EnumEffects(EnumEffectsCallback, this, DIEFT_ALL);
}

//------------------------
// 範囲設定コールバック
//------------------------
BOOL CALLBACK CInputDirectInputController::EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	CInputDirectInputController* pThis = static_cast<CInputDirectInputController*>(pvRef);

	DIPROPRANGE dipr;
	dipr.diph.dwSize = sizeof(DIPROPRANGE);
	dipr.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipr.diph.dwHow = DIPH_BYOFFSET;
	dipr.diph.dwObj = lpddoi->dwOfs;

	// dwOfsはDIJOYSTATE2構造体内のオフセット値なので、
	// C++標準のoffsetofを使って直接比較する
	switch (lpddoi->dwOfs)
	{
		// スティック系の軸 (中央がニュートラル)
	case offsetof(DIJOYSTATE2, lX):
	case offsetof(DIJOYSTATE2, lY):
	case offsetof(DIJOYSTATE2, lRx):
	case offsetof(DIJOYSTATE2, lRy):
		dipr.lMin = DIRECTINPUT_AXIS_MIN;
		dipr.lMax = DIRECTINPUT_AXIS_MAX;
		break;

		// トリガーやスライダー系の軸 (最小～最大)
	case offsetof(DIJOYSTATE2, lZ):
	case offsetof(DIJOYSTATE2, lRz):
	case offsetof(DIJOYSTATE2, rglSlider[0]): // DIJOFS_SLIDER(0) はこれに相当
	case offsetof(DIJOYSTATE2, rglSlider[1]): // DIJOFS_SLIDER(1) はこれに相当
	default: // 不明な軸もこちらで処理
		dipr.lMin = DIRECTINPUT_TRIGGER_MIN;
		dipr.lMax = DIRECTINPUT_TRIGGER_MAX;
		break;
	}

	HRESULT hr = pThis->m_pDevice->SetProperty(DIPROP_RANGE, &dipr.diph);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}

//------------------------
// 振動用コールバック
//------------------------
BOOL CALLBACK CInputDirectInputController::EnumEffectsCallback(LPCDIEFFECTINFO pdei, LPVOID pvRef)
{
    CInputDirectInputController* pThis = static_cast<CInputDirectInputController*>(pvRef);

    if (IsEqualGUID(pdei->guid, GUID_ConstantForce))
    {// 定数力対応フラグ
        pThis->m_bForceFeedback = true;
        pThis->m_effectGuid = pdei->guid;
    }
    else
    {// 対応していない
        pThis->m_bForceFeedback = false;
        pThis->m_effectGuid = GUID_NULL;
    }

    return DIENUM_CONTINUE;
}
