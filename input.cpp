//---------------------------------------
//
//入力処理[input.cpp]
//Author fuma sato
//
//---------------------------------------
#include"input.h"

using namespace input; // インプット空間の使用

//#if 0

//----------------------------------
//
// インプットクラス (呼び出し用窓口)
//
//----------------------------------

// 静的メンバ変数の定義
CInput CInput::m_instance{}; // 自身のインスタンス

//-----------------
// 初期化
//-----------------
HRESULT CInput::Init(HINSTANCE hInstanse, HWND hWnd)
{
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
bool CInput::IsPress(Index8 idx, BUTTON button)
{
	return m_pKeyboard->IsPress(idx, button) || m_pMouse->IsPress(idx, button) || m_pController->IsPress(idx, button);
}

//-----------------
// 押した瞬間
//-----------------
bool CInput::IsTrigger(Index8 idx, BUTTON button)
{
	return m_pKeyboard->IsTrigger(idx, button) || m_pMouse->IsTrigger(idx, button) || m_pController->IsTrigger(idx, button);
}

//-----------------
// 離した瞬間
//-----------------
bool CInput::IsRelease(Index8 idx, BUTTON button)
{
	return m_pKeyboard->IsRelease(idx, button) || m_pMouse->IsRelease(idx, button) || m_pController->IsRelease(idx, button);
}

//-----------------
// 長押し
//-----------------
bool CInput::IsRepeat(Index8 idx, BUTTON button)
{
	return m_pKeyboard->IsRepeat(idx, button) || m_pMouse->IsRepeat(idx, button) || m_pController->IsRepeat(idx, button);
}

//-----------------
// 長押し
//-----------------
size_t CInput::Count(void)
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
		if (pKeyboard != nullptr) return E_FAIL;  // null
		pKeyboard = new CInputRawInputKeyboard;   // rawInput生成
		if (pKeyboard == nullptr)  return E_FAIL; // 生成失敗

		// 初期化
		if (SUCCEEDED(pKeyboard->Init(hInstanse, hWnd)))
		{// 成功
			m_apKeyboard.push_back(pKeyboard); // キーボード追加
			pKeyboard = nullptr; // null
		}
		else
		{
			SAFE_UNINIT(pKeyboard); // 捨てる
			break;
		}
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
bool CInputKeyboardManager::IsPress(Index8 idx, BUTTON button)
{
	
}

//------------------------------
//キー入力時の処理
//------------------------------
bool CInputKeyboardManager::IsTrigger(Index8 idx, BUTTON button)
{
	
}

//------------------------------
//キー話離した際の処理
//------------------------------
bool CInputKeyboardManager::IsRelease(Index8 idx, BUTTON button)
{
	
}

//------------------------------
//キー長押し処理
//------------------------------
bool CInputKeyboardManager::IsRepeat(Index8 idx, BUTTON button)
{
	
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
		if (pMouse != nullptr) return E_FAIL;  // null
		pMouse = new CInputRawInputMouse;      // rawInput生成
		if (pMouse == nullptr)  return E_FAIL; // 生成失敗

		// 初期化
		if (SUCCEEDED(pMouse->Init(hInstanse, hWnd)))
		{// 成功
			m_apMouse.push_back(pMouse); // マウス追加
			pMouse = nullptr; // null
		}
		else
		{
			SAFE_UNINIT(pMouse); // 捨てる
			break;
		}
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
bool CInputMouseManager::IsPress(Index8 idx, BUTTON button)
{

}

//------------------------------
//キー入力時の処理
//------------------------------
bool CInputMouseManager::IsTrigger(Index8 idx, BUTTON button)
{

}

//------------------------------
//キー話離した際の処理
//------------------------------
bool CInputMouseManager::IsRelease(Index8 idx, BUTTON button)
{

}

//------------------------------
//キー長押し処理
//------------------------------
bool CInputMouseManager::IsRepeat(Index8 idx, BUTTON button)
{

}

//--------------
//マウス移動
//--------------
Axis CInputMouseManager::GetAxis(Index8 idx)
{

}

//----------------
//キー押下処理
//----------------
float CInputMouseManager::GetWheel(Index8 idx)
{

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
	CInputController* pController{}; // コントローラーポインタ

	// X
	do
	{// ループ
		if (pController != nullptr) return E_FAIL;  // null
		pController = new CInputXInputController;   // XInput生成
		if (pController == nullptr)  return E_FAIL; // 生成失敗

		// 初期化
		if (SUCCEEDED(pController->Init(hInstanse, hWnd)))
		{// 成功
			m_apController.push_back(pController); // コントローラー追加
			pController = nullptr;                 // null
			continue;                              // 続ける
		}
		SAFE_UNINIT(pController); // 捨てる

		// Direct
		if (pController != nullptr) return E_FAIL;     // null
		pController = new CInputDirectInputController; // DirectInput生成
		if (pController == nullptr)  return E_FAIL;    // 生成失敗

		// 初期化
		if (SUCCEEDED(pController->Init(hInstanse, hWnd)))
		{// 成功
			m_apController.push_back(pController); // コントローラー追加
			return S_OK;
		}
		else
		{
			SAFE_UNINIT(pController); // 捨てる
			break;
		}
	} while (true);
	return S_OK;
}

//-------------------
// 初期化
//-------------------
void CInputControllerManager::Uninit(void)
{
	SAFE_UNINIT_ARRAY(m_apController); // コントローラーをすべて破棄
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
bool CInputControllerManager::IsPress(Index idx, BUTTON button)
{

}

//-------------------
// 初期化
//-------------------
bool CInputControllerManager::IsTrigger(Index idx, BUTTON button)
{

}

//-------------------
// 初期化
//-------------------
bool CInputControllerManager::IsRelease(Index idx, BUTTON button)
{

}

//-------------------
// 初期化
//-------------------
bool CInputControllerManager::IsRepeat(Index idx, BUTTON button)
{

}

//-------------------
// 初期化
//-------------------
void CInputControllerManager::SetVibrate(Index idx, float fLeftPower, float fReghtPower)
{
	m_apController[idx]->Vibrate(fLeftPower, fReghtPower);
}

//-------------------
// 初期化
//-------------------
bool CInputControllerManager::IsXInputControllerConnected(Index idx)
{

}

//-------------------
// 初期化
//-------------------
bool CInputControllerManager::IsDirectInputControllerConnected(Index idx)
{

}

//-------------------
// 初期化
//-------------------
string_view CInputControllerManager::ControllerName(Index idx)
{

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
	array <BYTE, input::MAX_KEY> state{};
	if (SUCCEEDED(GetState(state)))
	{
		for (Index8 cntKey = 0; cntKey < state.size(); cntKey++)
		{
			m_state[cntKey] = state[cntKey]; //一時的な情報を正規情報に渡す

			bool now = (m_state[cntKey] & 0x80) != 0;    // 今押しているかどうか
			bool old = (m_stateOld[cntKey] & 0x80) != 0; // 前回押していたかどうか

			m_bPress[cntKey] = now;           // 押
			m_bTrigger[cntKey] = now && !old; // 押下
			m_bRelease[cntKey] = (!now && old && m_PressTime[cntKey] >= RELEASE_TIME) ? true : false; // リリース

			if (m_bTrigger[cntKey] || m_bRelease[cntKey])
			{// キーの変化
				// タイムの初期化
				m_PressTime[cntKey] = 0.0f;
				m_RepeatTime[cntKey] = 0.0f;
			}
			else if (m_bPress[cntKey])
			{// 押している
				// タイムの増加
				m_PressTime[cntKey] += CMain::GetDeltaTimeGameSpeed();
				if (m_PressTime[cntKey] >= REPEAT_START_TIME)
				{// リピート中
					m_RepeatTime[cntKey] += CMain::GetDeltaTimeGameSpeed();
					if (m_RepeatTime[cntKey] >= REPEAT_INTERVAL_TIME)
					{// リピート間隔
						m_RepeatTime[cntKey] = 0.0f;
						m_bRepeat[cntKey] = now ? true : false;
					}
				}
			}

			m_stateOld[cntKey] = state[cntKey]; //一時的な情報を正規情報に渡す
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
	array<bool, Index8(input::MOUSE_BUTTON::Max)> state{};
	if (SUCCEEDED(GetState(state)))
	{
		for (Index8 cntKey = 0; cntKey < state.size(); cntKey++)
		{
			m_state[cntKey] = state[cntKey]; //一時的な情報を正規情報に渡す

			bool now = m_state[cntKey];    // 今押しているかどうか
			bool old = m_stateOld[cntKey]; // 前回押していたかどうか

			m_bPress[cntKey] = now;           // 押
			m_bTrigger[cntKey] = now && !old; // 押下
			m_bRelease[cntKey] = (!now && old && m_PressTime[cntKey] >= RELEASE_TIME) ? true : false; // リリース

			if (m_bTrigger[cntKey] || m_bRelease[cntKey])
			{// キーの変化
				// タイムの初期化
				m_PressTime[cntKey] = 0.0f;
				m_RepeatTime[cntKey] = 0.0f;
			}
			else if (m_bPress[cntKey])
			{// 押している
				// タイムの増加
				m_PressTime[cntKey] += CMain::GetDeltaTimeGameSpeed();
				if (m_PressTime[cntKey] >= REPEAT_START_TIME)
				{// リピート中
					m_RepeatTime[cntKey] += CMain::GetDeltaTimeGameSpeed();
					if (m_RepeatTime[cntKey] >= REPEAT_INTERVAL_TIME)
					{// リピート間隔
						m_RepeatTime[cntKey] = 0.0f;
						m_bRepeat[cntKey] = now ? true : false;
					}
				}
			}

			m_stateOld[cntKey] = state[cntKey]; //一時的な情報を正規情報に渡す
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
	array<bool, Index8(input::CONTROLLER_BUTTON::Max)> state{};
	if (SUCCEEDED(GetState(state)))
	{
		for (Index8 cntKey = 0; cntKey < state.size(); cntKey++)
		{
			m_state[cntKey] = state[cntKey]; //一時的な情報を正規情報に渡す

			bool now = m_state[cntKey];    // 今押しているかどうか
			bool old = m_stateOld[cntKey]; // 前回押していたかどうか

			m_bPress[cntKey] = now;           // 押
			m_bTrigger[cntKey] = now && !old; // 押下
			m_bRelease[cntKey] = (!now && old && m_PressTime[cntKey] >= RELEASE_TIME) ? true : false; // リリース

			if (m_bTrigger[cntKey] || m_bRelease[cntKey])
			{// キーの変化
				// タイムの初期化
				m_PressTime[cntKey] = 0.0f;
				m_RepeatTime[cntKey] = 0.0f;
			}
			else if (m_bPress[cntKey])
			{// 押している
				// タイムの増加
				m_PressTime[cntKey] += CMain::GetDeltaTimeGameSpeed();
				if (m_PressTime[cntKey] >= REPEAT_START_TIME)
				{// リピート中
					m_RepeatTime[cntKey] += CMain::GetDeltaTimeGameSpeed();
					if (m_RepeatTime[cntKey] >= REPEAT_INTERVAL_TIME)
					{// リピート間隔
						m_RepeatTime[cntKey] = 0.0f;
						m_bRepeat[cntKey] = now ? true : false;
					}
				}
			}

			m_stateOld[cntKey] = state[cntKey]; //一時的な情報を正規情報に渡す
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

//---------------------------------------------
// メッセージから貰ったデータをデバイスに送信
//---------------------------------------------
HRESULT CInputRawInput::AnalysisRawData(RAWINPUT* pRawData)
{

}

//--------------------------
//
// rawInputキーボード
//
//--------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputRawInputKeyboard::Init(HINSTANCE hInstanse, HWND hWnd)
{


	return S_OK;
}

//----------------------
//終了処理
//----------------------
void CInputRawInputKeyboard::Uninit(void)
{

}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputRawInputKeyboard::GetState(span<BYTE> keyState) const
{
	for (size_t nCntkey = 0; nCntkey < MAX_KEY; nCntkey++)
	{
		m_KeyStateOld[nCntkey] = m_KeyState[nCntkey]; //前回のキー情報を保存
	}

	BYTE KeyState[MAX_KEY];//一時入力格納
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(KeyState), KeyState)))
	{
		for (size_t nCntkey = 0; nCntkey < MAX_KEY; nCntkey++)
		{
			m_KeyState[nCntkey] = KeyState[nCntkey]; //一時的な情報を正規情報に渡す

			bool now = (m_KeyState[nCntkey] & 0x80) != 0;    // 今押しているかどうか
			bool old = (m_KeyStateOld[nCntkey] & 0x80) != 0; // 前回押していたかどうか

			m_bKeyPress[nCntkey] = now;           // 押
			m_bKeyTrigger[nCntkey] = now && !old; // 押下
			m_bKeyRelease[nCntkey] = (!now && old && m_KeyPressTime[nCntkey] >= CInput::RELEASE_TIME) ? true : false; // リリース

			if (m_bKeyTrigger[nCntkey] || m_bKeyRelease[nCntkey])
			{// キーの変化
				// タイムの初期化
				m_KeyPressTime[nCntkey] = 0.0f;
				m_KeyRepeatTime[nCntkey] = 0.0f;
			}
			else if (m_bKeyPress[nCntkey])
			{// 押している
				// タイムの増加
				m_KeyPressTime[nCntkey] += CMain::GetDeltaTimeGameSpeed();
				if (m_KeyPressTime[nCntkey] >= CInput::REPEAT_START_TIME)
				{// リピート中
					m_KeyRepeatTime[nCntkey] += CMain::GetDeltaTimeGameSpeed();
					if (m_KeyRepeatTime[nCntkey] >= CInput::REPEAT_INTERVAL_TIME)
					{// リピート間隔
						m_KeyRepeatTime[nCntkey] = 0.0f;
						m_bKeyRepeat[nCntkey] = now ? true : false;
					}
				}
			}
		}
	}
	else
	{
		m_pDevice->Acquire();//アクセス権限の取得
	}
}

//--------------------------
//
// rawInputマウス
//
//--------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputRawInputMouse::Init(HINSTANCE hInstanse, HWND hWnd)
{

	return S_OK;
}

//----------------------
//終了処理
//----------------------
void CInputRawInputMouse::Uninit(void)
{

}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputRawInputMouse::GetState(span<bool> buttonState) const
{
	m_MouseStateOld = m_MouseState; //前回のマウス情報を保存

	DIMOUSESTATE2 MouseState;//マウス情報//一時入力格納
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &MouseState)))
	{
		m_MouseState = MouseState;//一時的な情報を正規情報に渡す

		for (size_t nCntButton = 0; nCntButton < MAX; nCntButton++)
		{
			bool now = (m_MouseState.rgbButtons[nCntButton] & 0x80) != 0;    // 今押しているかどうか
			bool old = (m_MouseStateOld.rgbButtons[nCntButton] & 0x80) != 0; // 前回押していたかどうか

			m_bPress[nCntButton] = now;           // 押
			m_bTrigger[nCntButton] = now && !old; // 押下
			m_bRelease[nCntButton] = (!now && old && m_PressTime[nCntButton] >= RELEASE_TIME) ? true : false;

			if (m_bTrigger[nCntButton] || m_bRelease[nCntButton])
			{
				m_PressTime[nCntButton] = 0;
			}
			else if (m_bPress[nCntButton])
			{
				m_PressTime[nCntButton]++;
			}

			m_bRepeat[nCntButton] = (now && m_PressTime[nCntButton] >= REPEAT_TIME) ? true : false;

			m_Move[0] = (float)m_MouseState.lX;
			m_Move[1] = (float)m_MouseState.lY;
			m_Move[0] /= MOUSE_INA;
			m_Move[1] /= MOUSE_INA;

			m_WheelMove = (float)m_MouseState.lZ;
			m_WheelMove /= MOUSE_WHEEL_INA;
		}
	}
	else
	{
		m_pDevice->Acquire();//アクセス権限の取得
	}
}

//--------------------------
//
//パッド
//
//--------------------------

//--------------------------------
//初期化処理
//--------------------------------
HRESULT CInputXInputController::Init(HINSTANCE hInstanse, HWND hWnd)
{
	XInputEnable(true);
	return S_OK;
}

//------------------------
//終了処理
//------------------------
void CInputXInputController::Uninit(void)
{
	XInputEnable(false);
}

//------------------------
//更新処理
//------------------------
HRESULT CInputXInputController::GetState(span<bool> state) const
{
	XINPUT_STATE XState;

	if (XInputGetState(0, &XState) == ERROR_SUCCESS)
	{
		state = XState;
	}
}
//----------------------------
//キー長押し処理
//----------------------------
bool CInputXInputController::IsRepeat(JOYKEY key)
{
	static bool bOldJoy[CONTROLLER_MAX][JOYKEY_MAX] = { false };
	static int KeyPutTime[CONTROLLER_MAX][JOYKEY_MAX] = { 0 };
	BYTE Trigger = 0;

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (key == JOYKEY_LT || key == JOYKEY_RT)
			{
				if (key == JOYKEY_LT)
				{
					Trigger = g_joykeyState[i].Gamepad.bLeftTrigger;
				}
				else
				{
					Trigger = g_joykeyState[i].Gamepad.bRightTrigger;
				}
			}

			if (bOldJoy[i][key] == true && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				return true;
			}
			else if (bOldJoy[i][key] == false && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				KeyPutTime[i][key]++;

				if (KeyPutTime[i][key] >= REPEAT_TIME)
				{
					KeyPutTime[i][key] = 0;
					bOldJoy[i][key] = true;
				}
			}
			else
			{
				bOldJoy[i][key] = false;
				KeyPutTime[i][key] = 0;
			}
		}

		return false;
	}

	if (key == JOYKEY_LT || key == JOYKEY_RT)
	{
		if (key == JOYKEY_LT)
		{
			Trigger = g_joykeyState[Controller].Gamepad.bLeftTrigger;
		}
		else
		{
			Trigger = g_joykeyState[Controller].Gamepad.bRightTrigger;
		}
	}

	if (bOldJoy[Controller][key] == true && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		return true;
	}
	else if (bOldJoy[Controller][key] == false && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		KeyPutTime[Controller][key]++;

		if (KeyPutTime[Controller][key] >= REPEAT_TIME)
		{
			KeyPutTime[Controller][key] = 0;
			bOldJoy[Controller][key] = true;
		}
		return false;
	}
	else
	{
		bOldJoy[Controller][key] = false;
		KeyPutTime[Controller][key] = 0;
		return false;
	}
}

//-----------------
// 左スティック角度
//-----------------
float CInputXInputController::GetLeftStickAngle(void)
{
	static float fStick[2] = { 0.0f };

	switch (Stick)
	{
	case STICK_LEFT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
		break;
	case STICK_RIGHT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
		break;
	}

	fStick[0] /= STICK_NUM;
	fStick[1] /= STICK_NUM;

	return &fStick[0];
}

//-----------------
// 左スティック角度
//-----------------
float CInputXInputController::GetRightStickAngle(void)
{
	static float fStick[2] = { 0.0f };

	switch (Stick)
	{
	case STICK_LEFT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
		break;
	case STICK_RIGHT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
		break;
	}

	fStick[0] /= STICK_NUM;
	fStick[1] /= STICK_NUM;

	return &fStick[0];
}

//-----------------
// 左スティック倒し具合
//-----------------
float CInputXInputController::GetLeftStickLength(void)
{
	static float fStick[2] = { 0.0f };

	switch (Stick)
	{
	case STICK_LEFT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
		break;
	case STICK_RIGHT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
		break;
	}

	fStick[0] /= STICK_NUM;
	fStick[1] /= STICK_NUM;

	return &fStick[0];
}

//-----------------
// 左スティック倒し具合
//-----------------
float CInputXInputController::GetRightStickLength(void)
{
	static float fStick[2] = { 0.0f };

	switch (Stick)
	{
	case STICK_LEFT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
		break;
	case STICK_RIGHT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
		break;
	}

	fStick[0] /= STICK_NUM;
	fStick[1] /= STICK_NUM;

	return &fStick[0];
}

//----------------
//振動
//----------------
void CInputXInputController::Vibrate(WORD wLeftMotorSpeed, WORD wRightMotorSpeed)
{
	XINPUT_VIBRATION vibration{ wLeftMotorSpeed ,wRightMotorSpeed };
	XInputSetState(0, &vibration);
}

//----------------------------------
//
// DirectInput母体
//
//----------------------------------

LPDIRECTINPUT8 CInputDirectInput::m_pInput{};
size_t CInputDirectInput::m_useCount{};

//-----------------
// 初期化
//-----------------
HRESULT CInputDirectInput::Register(HINSTANCE hInstanse, HWND hWnd)
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

	m_useCount++; // DirectInput参照数
}

//-----------------
// 破棄
//-----------------
void CInputDirectInput::Unregister(void)
{
	m_useCount--; // DirectInput参照数

	if (m_useCount <= 0)
	{// 参照数が0以下
		//インプットオブジェクトの破棄
		if (m_pInput != nullptr)
		{
			m_pInput->Release();
			m_pInput = nullptr;
		}
	}
}

//-----------------
// 列挙
//-----------------
HRESULT CInputDirectInput::SetEnum(void)
{
	// DirectInputデバイスを列挙
	m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumDevicesCallback, nullptr, DIEDFL_ATTACHEDONLY);
}

//--------------------------
//
// DirectInputキーボード
//
//--------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputDirectInputKeyboard::Init(HINSTANCE hInstanse, HWND hWnd)
{
	// DirectInput母体への登録
	CInputDirectInput::Register(hInstanse, hWnd);

	//インプットデバイスの作成
	if (FAILED(CInputDirectInput::GetDirectInput()->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr)))
	{
		// DirectInput母体への登録解除
		CInputDirectInput::Unregister();
		return E_FAIL;
	}

	//フォーマット作成
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		// DirectInput母体への登録解除
		CInputDirectInput::Unregister();
		return E_FAIL;
	}

	// バッファリングを有効にする
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		// DirectInput母体への登録解除
		CInputDirectInput::Unregister();
		return E_FAIL;
	}

	//アクセス権限の取得
	if (FAILED(m_pDevice->Acquire()))
	{
		// DirectInput母体への登録解除
		CInputDirectInput::Unregister();
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

	// DirectInput母体への登録解除
	CInputDirectInput::Unregister();
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputDirectInputKeyboard::GetState(span<BYTE> state) const
{
	for (size_t nCntkey = 0; nCntkey < MAX_KEY; nCntkey++)
	{
		m_KeyStateOld[nCntkey] = m_KeyState[nCntkey]; //前回のキー情報を保存
	}

	BYTE KeyState[MAX_KEY];//一時入力格納
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(KeyState), KeyState)))
	{
		for (size_t nCntkey = 0; nCntkey < MAX_KEY; nCntkey++)
		{
			m_KeyState[nCntkey] = KeyState[nCntkey]; //一時的な情報を正規情報に渡す

			bool now = (m_KeyState[nCntkey] & 0x80) != 0;    // 今押しているかどうか
			bool old = (m_KeyStateOld[nCntkey] & 0x80) != 0; // 前回押していたかどうか

			m_bKeyPress[nCntkey] = now;           // 押
			m_bKeyTrigger[nCntkey] = now && !old; // 押下
			m_bKeyRelease[nCntkey] = (!now && old && m_KeyPressTime[nCntkey] >= CInput::RELEASE_TIME) ? true : false; // リリース

			if (m_bKeyTrigger[nCntkey] || m_bKeyRelease[nCntkey])
			{// キーの変化
				// タイムの初期化
				m_KeyPressTime[nCntkey] = 0.0f;
				m_KeyRepeatTime[nCntkey] = 0.0f;
			}
			else if (m_bKeyPress[nCntkey])
			{// 押している
				// タイムの増加
				m_KeyPressTime[nCntkey] += CMain::GetDeltaTimeGameSpeed();
				if (m_KeyPressTime[nCntkey] >= CInput::REPEAT_START_TIME)
				{// リピート中
					m_KeyRepeatTime[nCntkey] += CMain::GetDeltaTimeGameSpeed();
					if (m_KeyRepeatTime[nCntkey] >= CInput::REPEAT_INTERVAL_TIME)
					{// リピート間隔
						m_KeyRepeatTime[nCntkey] = 0.0f;
						m_bKeyRepeat[nCntkey]= now ? true : false;
					}
				}
			}
		}
	}
	else
	{
		m_pDevice->Acquire();//アクセス権限の取得
	}
}

//--------------------------
//
// DirectInputマウス
//
//--------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputDirectInputMouse::Init(HINSTANCE hInstanse, HWND hWnd)
{
	// DirectInput母体への登録
	CInputDirectInput::Register(hInstanse, hWnd);

	//インプットデバイスの作成
	if (FAILED(CInputDirectInput::GetDirectInput()->CreateDevice(GUID_SysMouse, &m_pDevice, nullptr)))
	{
		// DirectInput母体への登録解除
		CInputDirectInput::Unregister();
		return E_FAIL;
	}

	//フォーマット作成
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		// DirectInput母体への登録解除
		CInputDirectInput::Unregister();
		return E_FAIL;
	}

	// バッファリングを有効にする
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		// DirectInput母体への登録解除
		CInputDirectInput::Unregister();
		return E_FAIL;
	}

	m_pDevice->Acquire();//アクセス権限の取得

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

	// インプットの破棄
	CInputDirectInput::Unregister();
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputDirectInputMouse::GetState(span<bool> state) const
{
	m_MouseStateOld = m_MouseState; //前回のマウス情報を保存

	DIMOUSESTATE2 MouseState;//マウス情報//一時入力格納
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &MouseState)))
	{
		m_MouseState = MouseState;//一時的な情報を正規情報に渡す

		for (size_t nCntButton = 0; nCntButton < MAX; nCntButton++)
		{
			bool now = (m_MouseState.rgbButtons[nCntButton] & 0x80) != 0;    // 今押しているかどうか
			bool old = (m_MouseStateOld.rgbButtons[nCntButton] & 0x80) != 0; // 前回押していたかどうか

			m_bPress[nCntButton] = now;           // 押
			m_bTrigger[nCntButton] = now && !old; // 押下
			m_bRelease[nCntButton] = (!now && old && m_PressTime[nCntButton] >= RELEASE_TIME) ? true : false;

			if (m_bTrigger[nCntButton] || m_bRelease[nCntButton])
			{
				m_PressTime[nCntButton] = 0;
			}
			else if (m_bPress[nCntButton])
			{
				m_PressTime[nCntButton]++;
			}

			m_bRepeat[nCntButton] = (now && m_PressTime[nCntButton] >= REPEAT_TIME) ? true : false;

			m_Move[0] = (float)m_MouseState.lX;
			m_Move[1] = (float)m_MouseState.lY;
			m_Move[0] /= MOUSE_INA;
			m_Move[1] /= MOUSE_INA;

			m_WheelMove = (float)m_MouseState.lZ;
			m_WheelMove /= MOUSE_WHEEL_INA;
		}
	}
	else
	{
		m_pDevice->Acquire();//アクセス権限の取得
	}
}

//-----------------------------
//
//dinputパッド
//
//-----------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputDirectInputController::Init(HINSTANCE hInstanse, HWND hWnd)
{
	// DirectInput母体への登録
	CInputDirectInput::Register(hInstanse, hWnd);

	//フォーマット作成
	m_pDevice->SetDataFormat(&c_dfDIJoystick);

	m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_EXCLUSIVE));

	m_pDevice->Acquire();//アクセス権限の取得

	return S_OK;
}

//----------------------
//終了処理
//----------------------
void CInputDirectInputController::Uninit(void)
{
	//エフェクトデバイスの破棄
	Vibrate(0, 0);

	// デバイスの破棄
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	// インプットの破棄
	CInputDirectInput::Unregister();
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputDirectInputController::GetState(span<bool> state) const
{
	DIJOYSTATE  djoykeyState[CONTROLLER_MAX];//一時入力格納
	int ControllerNum = 0;

	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		if (g_DevdJoypad[i] != nullptr)
		{
			if (SUCCEEDED(g_DevdJoypad[i]->GetDeviceState(sizeof(DIJOYSTATE), &djoykeyState[i])))
			{
				g_djoykeyState[i] = djoykeyState[i];//一時的な情報を正規情報に渡す
			}
			else
			{
				ControllerNum++;
			}
		}
		else
		{
			ControllerNum++;
		}
	}

	if (g_DcontrollerNum!=(CONTROLLER_MAX-ControllerNum))
	{
		SetUnCon(true);
		InputReSet();
		// DirectInputデバイスを列挙
		g_pinput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumDevicesCallback, nullptr, DIEDFL_ATTACHEDONLY);
	}
}

//------------------------------
//キー長押し処理
//------------------------------
bool CInputDirectInputController::IsRepeat(int key)
{
	static bool bOldkey[CONTROLLER_MAX][NUM_DIJOYKEY_MAX] = { false };
	static int KeyPutTime[CONTROLLER_MAX][NUM_DIJOYKEY_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (bOldkey[i][key] == true && ((g_djoykeyState[i].rgbButtons[key] & 0x80) ? true : false) == true)
			{
				return true;
			}
			else if (bOldkey[i][key] == false && ((g_djoykeyState[i].rgbButtons[key] & 0x80) ? true : false) == true)
			{
				KeyPutTime[i][key]++;

				if (KeyPutTime[i][key] >= REPEAT_TIME)
				{
					KeyPutTime[i][key] = 0;
					bOldkey[i][key] = true;
				}
			}
			else
			{
				bOldkey[i][key] = false;
				KeyPutTime[i][key] = 0;
			}
		}

		return false;
	}

	if (bOldkey[Controller][key] == true && ((g_djoykeyState[Controller].rgbButtons[key] & 0x80) ? true : false) == true)
	{
		return true;
	}
	else if (bOldkey[Controller][key] == false && ((g_djoykeyState[Controller].rgbButtons[key] & 0x80) ? true : false) == true)
	{
		KeyPutTime[Controller][key]++;

		if (KeyPutTime[Controller][key] >= REPEAT_TIME)
		{
			KeyPutTime[Controller][key] = 0;
			bOldkey[Controller][key] = true;
		}
		return false;
	}
	else
	{
		bOldkey[Controller][key] = false;
		KeyPutTime[Controller][key] = 0;
		return false;
	}
}

//-----------------
//スティック処理
//-----------------
float CInputDirectInputController::GetLeftStickAngle(void)
{
	static float fStick[2] = { 0.0f };

	switch (Stick)
	{
	case STICK_LEFT:
		fStick[0] = (float)g_djoykeyState[Controller].lX;
		fStick[1] = (float)g_djoykeyState[Controller].lY;
		break;
	case STICK_RIGHT:
		fStick[0] = (float)g_djoykeyState[Controller].lZ;
		fStick[1] = (float)g_djoykeyState[Controller].lRz;
		break;
	}

	fStick[0] /= STICK_NUM;
	fStick[1] /= STICK_NUM;

	fStick[0] += DSTICK_NUM;
	fStick[1] += DSTICK_NUM;

	return &fStick[0];
}

//----------------------
//スライダー軸
//----------------------
float CInputDirectInputController::GetSlider(int nSlider)
{
	float fSlider = 0.0f;

	fSlider = (float)g_djoykeyState[Controller].rglSlider[nSlider];

	fSlider /= STICK_NUM;

	fSlider += DSTICK_NUM;

	return fSlider;
}

//------------------------
//ハットスイッチ
//------------------------
bool CInputDirectInputController::IsPressUp(int nPov)
{
	float fPov = 0.0f;

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			fPov = (float)g_djoykeyState[i].rgdwPOV[nPov];

			if (fPov==fData)
			{
				return true;
			}
		}

		return false;
	}

	fPov = (float)g_djoykeyState[Controller].rgdwPOV[nPov];

	return (fPov == fData) ? true : false;
}

//-------------------------
//
//-------------------------
bool GetdJoyPovTrigger(float fData, int nPov, CONTROLLER Controller)
{
	float fPov = 0.0f;
	static bool bOldPov[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { false };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			fPov = (float)g_djoykeyState[i].rgdwPOV[nPov];

			if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = true;
				return true;
			}
			else if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == false)
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = false;
				return false;
			}
		}

		return false;
	}

	fPov = (float)g_djoykeyState[Controller].rgdwPOV[nPov];

	if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = true;
		return true;
	}
	else if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == false)
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = false;
		return false;
	}
	else
	{
		return false;
	}
}

//-----------------------
//
//-----------------------
bool GetdJoyPovRelease(float fData, int nPov, CONTROLLER Controller)
{
	float fPov = 0.0f;
	static bool bOldPov[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { false };
	static int PovPutTime[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			fPov = (float)g_djoykeyState[i].rgdwPOV[nPov];

			if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == false)
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = false;
				if (PovPutTime[i][nPov][(int)(fData / POV_NUM)] >= RELEASE_TIME)
				{
					PovPutTime[i][nPov][(int)(fData / POV_NUM)] = 0;
					return true;
				}
				else
				{
					PovPutTime[i][nPov][(int)(fData / POV_NUM)] = 0;
					return false;
				}
			}
			else if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = true;
				return false;
			}
			else if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == true)
			{
				PovPutTime[i][nPov][(int)(fData / POV_NUM)]++;
				return false;
			}
		}

		return false;
	}

	fPov = (float)g_djoykeyState[Controller].rgdwPOV[nPov];

	if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == false)
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = false;
		if (PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] >= RELEASE_TIME)
		{
			PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] = 0;
			return true;
		}
		else
		{
			PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] = 0;
			return false;
		}
	}
	else if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = true;
		return false;
	}
	else if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == true)
	{
		PovPutTime[Controller][nPov][(int)(fData / POV_NUM)]++;
		return false;
	}
	else
	{
		return false;
	}
}

//---------------------
//
//---------------------
bool GetdJoyPovRepeat(float fData, int nPov, CONTROLLER Controller)
{
	float fPov = 0.0f;
	static bool bOldPov[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { false };
	static int PovPutTime[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			fPov = (float)g_djoykeyState[i].rgdwPOV[nPov];

			if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == true)
			{
				return true;
			}
			else if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
			{
				PovPutTime[i][nPov][(int)(fData / POV_NUM)]++;

				if (PovPutTime[i][nPov][(int)(fData / POV_NUM)] >= REPEAT_TIME)
				{
					PovPutTime[i][nPov][(int)(fData / POV_NUM)] = 0;
					bOldPov[i][nPov][(int)(fData / POV_NUM)] = true;
				}
				return false;
			}
			else
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = false;
				PovPutTime[i][nPov][(int)(fData / POV_NUM)] = 0;
			}


		}

		return false;
	}

	fPov = (float)g_djoykeyState[Controller].rgdwPOV[nPov];

	if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == true)
	{
		return true;
	}
	else if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
	{
		PovPutTime[Controller][nPov][(int)(fData / POV_NUM)]++;

		if (PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] >= REPEAT_TIME)
		{
			PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] = 0;
			bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = true;
		}
		return false;
	}
	else
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = false;
		PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] = 0;
		return false;
	}
}

//-------------
//振動
//-------------
void CInputDirectInputController::Vibrate(WORD wLeftMotorSpeed, WORD wRightMotorSpeed)
{
	if (m_effect != nullptr)
	{
		m_effect->Stop();
		m_effect->Release();
		m_effect = nullptr;
	}

	// フォースフィードバックのエフェクトを作成する
	DIEFFECT effect;
	ZeroMemory(&effect, sizeof(DIEFFECT));
	effect.dwSize = sizeof(DIEFFECT);
	effect.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	effect.dwDuration = INFINITE;  // 無限に続く振動
	effect.dwGain = DI_FFNOMINALMAX;
	effect.dwTriggerButton = DIEB_NOTRIGGER;

	// 軸と方向の設定
	DWORD rgdwAxes[1] = { DIJOFS_X };  // X軸の振動
	LONG rglDirection[1] = { 0 };      // 方向は正方向（0）
	effect.cAxes = 1;                  // 1軸
	effect.rgdwAxes = rgdwAxes;        // 使用する軸
	effect.rglDirection = rglDirection;// 振動の方向

	DICONSTANTFORCE cf;
	cf.lMagnitude = lMotorPower;  // 振動の強さ (0 - DI_FFNOMINALMAX)

	effect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
	effect.lpvTypeSpecificParams = &cf;

	if (SUCCEEDED(m_pDevice->CreateEffect(GUID_ConstantForce, &effect, &m_effect, nullptr)))
	{// 作成できた
		if (FAILED(m_effect->Start(1, 0)))
		{// 失敗
			m_effect->Release();
			m_effect = nullptr;
		}
	}
}

//--------------------------------
//接続確認
//--------------------------------

//-----------
//Xinput
//-----------
bool IsXInputControllerConnected(CONTROLLER Controller)
{
	XINPUT_STATE joykeyState[CONTROLLER_MAX];

	if (Controller==CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (XInputGetState(i, &joykeyState[i]) == ERROR_SUCCESS)
			{
				return true;
			}
		}

		return false;
	}

	return XInputGetState(Controller, &joykeyState[Controller]) == ERROR_SUCCESS;
}

//------------
//dinput
//------------
bool IsDirectInputControllerConnected(CONTROLLER Controller)
{
	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (g_DevdJoypad[i] != nullptr)
			{
				return true;
			}
		}

		return false;
	}

	return g_DevdJoypad[Controller] != nullptr;
}

//------------------
//コントローラー名
//------------------
char *ControllerName(CONTROLLER Controller)
{
	return &g_ConName[Controller][0];
}

//------------------
//コントローラー数
//------------------
int ControllerNum(CONTYPE Contype)
{
	if (Contype==CONTYPE_X)
	{
		return g_XcontrollerNum;
	}
	else
	{
		return g_DcontrollerNum;
	}
}

//---------------------------
//デバイス情報取得
//---------------------------
BOOL CALLBACK CInputDirectInput::EnumDevicesCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	static int nControllerNum = 0;
	static int XcontrollerNum = 0;

	if (g_bUnCon)
	{
		nControllerNum = 0;
		XcontrollerNum = 0;
		g_DcontrollerNum = 0;
		g_XcontrollerNum = 0;
		g_bX = false;
		g_bUnCon = false;
		//エフェクトデバイスの破棄
		VibratedControllerStop(CONTROLLER_MAX);
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			//キーボードデバイスの破棄
			if (g_DevdJoypad[i] != nullptr)
			{
				g_DevdJoypad[i]->Unacquire();
				g_DevdJoypad[i]->Release();
				g_DevdJoypad[i] = nullptr;
			}
		}
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			g_ConName[i][0] = '\0';
		}
	}

	if ((nControllerNum+XcontrollerNum)<=CONTROLLER_MAX)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		g_pDiInstance[nControllerNum] = pdidInstance;

		if (XInputGetState(XcontrollerNum, &state) == ERROR_SUCCESS)
		{
			if (SUCCEEDED(g_pinput->CreateDevice(pdidInstance->guidInstance, &g_DevdJoypad[XcontrollerNum], nullptr)))
			{//インプットデバイスの作成
				HINSTANCE hInstanse;
				HWND hWnd;

				hInstanse = GethInstanse();
				hWnd = GethWnd();

				//フォーマット作成
				g_DevdJoypad[nControllerNum]->SetDataFormat(&c_dfDIJoystick);

				g_DevdJoypad[nControllerNum]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_EXCLUSIVE));

				g_DevdJoypad[nControllerNum]->Acquire();//アクセス権限の取得

				strcpy(&g_ConName[nControllerNum][0], pdidInstance->tszProductName);
				XcontrollerNum++;
			}
		}
		else if (SUCCEEDED(g_pinput->CreateDevice(pdidInstance->guidInstance, &g_DevdJoypad[nControllerNum + XNum()], nullptr)))
		{//インプットデバイスの作成
			HINSTANCE hInstanse;
			HWND hWnd;

			hInstanse = GethInstanse();
			hWnd = GethWnd();

			//フォーマット作成
			g_DevdJoypad[nControllerNum + XNum()]->SetDataFormat(&c_dfDIJoystick);

			g_DevdJoypad[nControllerNum + XNum()]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_EXCLUSIVE));

			g_DevdJoypad[nControllerNum + XNum()]->Acquire();//アクセス権限の取得

			strcpy(&g_ConName[nControllerNum + XNum()][0], pdidInstance->tszProductName);
			nControllerNum++;
		}
	}
	g_DcontrollerNum = nControllerNum+XcontrollerNum;

	// 列挙を続行
	return DIENUM_CONTINUE;
}

//----------
//
//----------
int XNum(void)
{
	int XNum = 0;
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		if (XInputGetState(i, &state) == ERROR_SUCCESS)
		{
			XNum++;
		}
	}
	return XNum;
}

LPDIRECTINPUT8 GetInput(void)
{
	return g_pinput;
}

void SetUnCon(bool bUnCon)
{
	g_bUnCon = bUnCon;
}

//-----------------------
//コントローラー管理初期化
//-----------------------
void InputReSet(void)
{
	g_bUnCon = false;
	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		g_ConName[i][0] = '\0';
	}
	g_XcontrollerNum = 0;
	g_DcontrollerNum = 0;
    g_bX = false;
	//エフェクトデバイスの破棄
	VibratedControllerStop(CONTROLLER_MAX);
	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		//キーボードデバイスの破棄
		if (g_DevdJoypad[i] != nullptr)
		{
			g_DevdJoypad[i]->Unacquire();
			g_DevdJoypad[i]->Release();
			g_DevdJoypad[i] = nullptr;
		}
	}
}

//------------------------------------------------
//コントローラー情報取得
//------------------------------------------------
const DIDEVICEINSTANCE* GetDiInstance(CONTROLLER Controller)
{
	return g_pDiInstance[Controller];
}

//----------------------------------------------
//Xinputコントローラー情報取得
//----------------------------------------------
XINPUT_STATE GetXstate(CONTROLLER Controller)
{
	return g_joykeyState[Controller];
}

//----------------------------------------------
//dinputコントローラー情報取得
//----------------------------------------------
DIJOYSTATE Getdstate(CONTROLLER Controller)
{
	return g_djoykeyState[Controller];
}

//#endif