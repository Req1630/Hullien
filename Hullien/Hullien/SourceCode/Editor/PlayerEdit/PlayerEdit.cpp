#include "PlayerEdit.h"

CPlayerEdit::CPlayerEdit()	
	: m_pPlayer	( nullptr )
{
	m_pPlayer = std::make_unique<CPlayer>();
}

CPlayerEdit::~CPlayerEdit()
{
}

// �������֐�.
bool CPlayerEdit::Init() 
{
	if( m_pPlayer->Init() == false ) return false;
	return true;
}

// �X�V�֐�.
void CPlayerEdit::Update()
{
	m_pPlayer->Update();
}

// �`��֐�.
void CPlayerEdit::Render() 
{
}

// ���f���̕`��.
void CPlayerEdit::ModelRender() 
{
	m_pPlayer->Render();
}