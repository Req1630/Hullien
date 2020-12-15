#include "Input.h"

CInput::CInput()
	: m_KeyBindList		()
{
}

CInput::~CInput()
{
}

// キーバインドの初期化.
void CInput::InitKeyBind()
{
	GetInstance()->m_KeyBindList =
	{

		{ EKeyBind::Decision,		SKeyBindPair( VK_RETURN,	XINPUT_GAMEPAD_B )				},
		{ EKeyBind::Cancel,			SKeyBindPair( VK_BACK,		XINPUT_GAMEPAD_A )				},
		{ EKeyBind::Skip,			SKeyBindPair( VK_RETURN,	XINPUT_GAMEPAD_B )				},
		
		{ EKeyBind::Attack,			SKeyBindPair( 'F',			XINPUT_GAMEPAD_X )				},
		{ EKeyBind::SpecialAbility,	SKeyBindPair( 'Y',			XINPUT_GAMEPAD_Y )				},
		{ EKeyBind::Avoidance,		SKeyBindPair( 'R',			XINPUT_GAMEPAD_A )				},
		{ EKeyBind::STGAttack,		SKeyBindPair( 'Z',			XINPUT_GAMEPAD_RIGHT_SHOULDER )	},

		{ EKeyBind::Up,				SKeyBindPair( VK_UP,		XINPUT_GAMEPAD_DPAD_UP )		},
		{ EKeyBind::Down,			SKeyBindPair( VK_DOWN,		XINPUT_GAMEPAD_DPAD_DOWN )		},
		{ EKeyBind::Right,			SKeyBindPair( VK_RIGHT,		XINPUT_GAMEPAD_DPAD_RIGHT )		},
		{ EKeyBind::Left,			SKeyBindPair( VK_LEFT,		XINPUT_GAMEPAD_DPAD_LEFT )		},

		{ EKeyBind::Start,			SKeyBindPair( VK_ESCAPE,	XINPUT_GAMEPAD_START )			},
		{ EKeyBind::Back,			SKeyBindPair( VK_BACK,		XINPUT_GAMEPAD_BACK )			},

	};

	GetInstance()->m_AxisBindList = 
	{
		{ EAxisBind::L_Forward,	SAxisBind( 'W', 'S', [](){ return CXInput::LThumbY_Axis(); }, -1.0f, 1.0f ) },
		{ EAxisBind::L_Right,	SAxisBind( 'D', 'A', [](){ return CXInput::LThumbX_Axis(); }, -1.0f, 1.0f ) },
	};
}

// キーバインドの初期化.
void CInput::InitKeyBind( std::function<void(std::unordered_map<EKeyBind, SKeyBindPair>&)> func )
{
	func( GetInstance()->m_KeyBindList );
}

// 入力状態の更新.
void CInput::StateUpdate()
{
	CXInput::StatsUpdate();
	CKeyInput::Update();
}

// 押されているとき.
bool CInput::IsPress( const EKeyBind& key )
{
	return 
		CKeyInput::IsPress( GetInstance()->m_KeyBindList[key].Key ) ||
		CXInput::IsPress( GetInstance()->m_KeyBindList[key].Button );
}

// 押された瞬間.
bool CInput::IsMomentPress( const EKeyBind& key )
{
	return 
		CKeyInput::IsMomentPress( GetInstance()->m_KeyBindList[key].Key ) || 
		CXInput::IsMomentPress( GetInstance()->m_KeyBindList[key].Button );
}

// 長押ししているとき.
bool CInput::IsHold( const EKeyBind& key )
{
	return 
		CKeyInput::IsHold( GetInstance()->m_KeyBindList[key].Key ) ||
		CXInput::IsHold( GetInstance()->m_KeyBindList[key].Button );
}

// 離した瞬間.
bool CInput::IsRelease( const EKeyBind& key )
{
	return 
		CKeyInput::IsRelease( GetInstance()->m_KeyBindList[key].Key ) ||
		CXInput::IsRelease( GetInstance()->m_KeyBindList[key].Button );
}

// 押していない.
bool CInput::NotPress( const EKeyBind& key )
{
	// どっちかが押されていたら falseを返す.
	if( CKeyInput::NotPress( GetInstance()->m_KeyBindList[key].Key ) == false ||
		CXInput::NotPress( GetInstance()->m_KeyBindList[key].Button ) == false ){
		return false;
	}
	return true;
}

// 軸値の取得.
float CInput::GetAxisValue( const EAxisBind& key )
{
	float value = 0.0f;
	if( CKeyInput::IsPress( GetInstance()->m_AxisBindList[key].PlusKey ) == true ){
		value += GetInstance()->m_AxisBindList[key].MaxValue;
	}
	if( CKeyInput::IsPress( GetInstance()->m_AxisBindList[key].MinusKey ) == true ){
		value += GetInstance()->m_AxisBindList[key].MinValue;
	}
	if( value != 0.0f ) return value;

	value = static_cast<float>(GetInstance()->m_AxisBindList[key].GetValue())/static_cast<float>(SHRT_MAX);
	value = pow( value, 3 );
	if( fabsf(value) < 0.01f ) value = 0.0f;

	return value;
}

// インスタンスの取得.
CInput* CInput::GetInstance()
{
	static std::unique_ptr<CInput> pInstance = std::make_unique<CInput>();
	return pInstance.get();
}