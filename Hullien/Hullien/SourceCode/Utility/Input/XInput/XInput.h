/**
* @file XInput.h
* @brief This is Use Xinput Controller file.
* @author Fukuta
* @date 2020/12/16
* @details Xinputを使用するためのクラスです.
**/
#ifndef XINPUT_H
#define XINPUT_H

#include <Windows.h>
#include <XInput.h>
#include <vector>
#include <unordered_map>
#include <memory>

#pragma comment( lib, "xinput.lib" )

#include "..\InputStruct.h"

static const SHORT	IDLE_THUMB_MAX		=  10000;	//!< スティックの遊び.
static const SHORT	IDLE_THUMB_MIN		= -10000;	//!< スティックの遊び.
static const BYTE	IDLE_TIGGER_MAX		=  100;		//!< トリガーボタンの遊び.
static const WORD	INPUT_VIBRATION_MIN	=  0;		//!< バイブレーションの最小値.
static const WORD	INPUT_VIBRATION_MAX	=  65535;	//!< バイブレーションの最大値.

/**
* @brief XInput Class(Singleton).
*
* @details バイブレーション機能以外の機能はついてます.
*/
class CXInput
{
	const int FOUR_LIMITED_CONTROLLER = 4;	//!< 接続コントローラの最大数.
public:
	inline static const char* VIBRATION_CONFIG_FILE_PATH = "Data\\Config\\Con_Vib.bin";

public:
	/**
	* @brief コンストラクタ.
	*/
	CXInput();
	/**
	* @brief デストラクタ.
	*/
	~CXInput();

	/**
	* @fn static void StatsUpdate()
	* @brief コントローラーの状態を更新.
	* @details 毎フレーム事に更新すること.
	*/
	static void StatsUpdate();

	/**
	* @fn static int GetConnectCount()
	* @return int 現在のコントローラーの接続数.
	* @brief コントローラーの接続している数を取得.
	*/
	static int GetConnectCount(){ return GetInstance()->m_ConnectedCount; }
	

	// 押されているとき.
	static bool IsPress( const WORD& button_mask, const int& connectNum = 0 );
	// 押された瞬間.
	static bool IsMomentPress( const WORD& button_mask, const int& connectNum = 0 );
	// 長押ししているとき.
	static bool IsHold( const WORD& button_mask, const int& connectNum = 0 );
	// 離した瞬間.
	static bool IsRelease( const WORD& button_mask, const int& connectNum = 0 );
	// 押していない.
	static bool NotPress( const WORD& button_mask, const int& connectNum = 0 );
	// 左スティックのX軸の取得.
	static SHORT LThumbX_Axis( const int& connectNum = 0 );
	// 左スティックのY軸の取得.
	static SHORT LThumbY_Axis( const int& connectNum = 0 );
	// 右スティックのX軸の取得.
	static SHORT RThumbX_Axis( const int& connectNum = 0 );
	// 右スティックのY軸の取得.
	static SHORT RThumbY_Axis( const int& connectNum = 0 );
	// 左トリガーの取得.
	static BYTE LTrigger( const int& connectNum = 0 );
	// 右トリガーの取得.
	static BYTE RTrigger( const int& connectNum = 0 );

	// 振動設定.
	static bool SetVibration( WORD LMotorSpd, WORD RMotorSpd, const int& connectNum = 0 );
	// 振動をするかどうか設定.
	static void SetVibration( const bool& isVib ){ GetInstance()->m_IsVibration = isVib; }
	static bool IsVibration(){ return GetInstance()->m_IsVibration; }

private:
	void ConnectCheck( const int& connectNum );

	bool IsButtonInput( const int& connectNum, const WORD& button_mask );

	void ButtonStateUpdate();

	static CXInput* GetInstance()
	{
		static std::unique_ptr<CXInput> pInstance = std::make_unique<CXInput>();
		return pInstance.get();
	}

private:
	std::vector<XINPUT_STATE>		m_State;		//!< 接続したコントローラーの状態.
	std::vector<XINPUT_VIBRATION>	m_Vibration;	//!< バイブレーションの状態.
	int		m_ConnectedCount;						//!< 接続したコントローラーの数.
	bool	m_IsVibration;							//!< バイブレーションを行うか.
	std::vector<std::unordered_map<WORD, EInputState>> m_ButtonStateList;	//!< 入力されたボタンの状態.

	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CXInput( const CXInput & )				= delete;
	CXInput( CXInput && )					= delete;
	CXInput& operator = ( const CXInput & )	= delete;
	CXInput& operator = ( CXInput && )		= delete;
};


#endif	// #ifndef XINPUT_H.