#include "PlayerEdit.h"

CPlayerEdit::CPlayerEdit()	
	: m_pPlayer	( nullptr )
{
	m_pPlayer = std::make_unique<CPlayer>();
}

CPlayerEdit::~CPlayerEdit()
{
}

// 初期化関数.
bool CPlayerEdit::Init() 
{
	if( m_pPlayer->Init() == false ) return false;
	return true;
}

// 更新関数.
void CPlayerEdit::Update()
{
	m_pPlayer->Update();
}

// 描画関数.
void CPlayerEdit::Render() 
{
}

// モデルの描画.
void CPlayerEdit::ModelRender() 
{
	m_pPlayer->Render();
}