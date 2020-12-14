#include "Command.h"
#include "..\Input\XInput\XInput.h"
#include "..\Input\KeyInput\KeyInput.h"

CCommand::CCommand()
	: m_KeyQueue			()
	, m_TimeCount			( 0 )
	, m_isSuccessCommand	( false )
{
}

CCommand::~CCommand()
{
}

void CCommand::Update()
{
	m_isSuccessCommand = false;
	InputKey();	// キーの入力.
	Check();	// 確認.
	m_TimeCount++;
	if( m_TimeCount >= COMMAND_RESET_FRAME ){
		m_TimeCount = 0;
		for( int i = 0; i < static_cast<int>(m_KeyQueue.size()); i++ ) m_KeyQueue.pop();
	}
}

// キー(buttonの入力).
void CCommand::InputKey()
{
	if( CKeyInput::IsMomentPress(VK_UP) == true || CXInput::DPadUp()	== CXInput::enPRESSED_MOMENT )
		PushKey( EKey_Up );
	if( CKeyInput::IsMomentPress(VK_DOWN) == true || CXInput::DPadDown()	== CXInput::enPRESSED_MOMENT )
		PushKey( EKey_Down );
	if( CKeyInput::IsMomentPress(VK_RIGHT) == true || CXInput::DPadRIGHT()== CXInput::enPRESSED_MOMENT )
		PushKey( EKey_Right );
	if( CKeyInput::IsMomentPress(VK_LEFT) == true || CXInput::DPadLEFT()	== CXInput::enPRESSED_MOMENT )
		PushKey( EKey_Left );
	if( CKeyInput::IsMomentPress('A') == true || CXInput::A_Button() == CXInput::enPRESSED_MOMENT )
		PushKey( EKey_A );
	if( CKeyInput::IsMomentPress('B') == true || CXInput::B_Button() == CXInput::enPRESSED_MOMENT )
		PushKey( EKey_B );
	if( CKeyInput::IsMomentPress('X') == true || CXInput::X_Button() == CXInput::enPRESSED_MOMENT )
		PushKey( EKey_X );
	if( CKeyInput::IsMomentPress('Y') == true || CXInput::Y_Button() == CXInput::enPRESSED_MOMENT )
		PushKey( EKey_Y );
	if( CKeyInput::IsMomentPress('R') == true || CXInput::R_Button() == CXInput::enPRESSED_MOMENT )
		PushKey( EKey_RB );
	if( CKeyInput::IsMomentPress('L') == true || CXInput::L_Button() == CXInput::enPRESSED_MOMENT )
		PushKey( EKey_LB );
}

// キーの追加.
void CCommand::PushKey( const EKey& key )
{
	m_KeyQueue.push( key );
	m_TimeCount = 0;
}

// キューの確認.
void CCommand::Check()
{
	// キューのサイズをリストのサイズを比較.
	if( COMMAND_LIST.size() != m_KeyQueue.size() ) return;

	for( int i = 0; i < static_cast<int>(COMMAND_LIST.size()); i++ ){
		if( COMMAND_LIST[i] == m_KeyQueue.front() ){
			m_isSuccessCommand = true;
		} else {
			m_isSuccessCommand = false;
		}
		m_KeyQueue.pop();
	}
}