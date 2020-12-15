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
		// ゲーム系.
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

// インスタンスの取得.
CInput* CInput::GetInstance()
{
	static std::unique_ptr<CInput> pInstance = std::make_unique<CInput>();
	return pInstance.get();
}