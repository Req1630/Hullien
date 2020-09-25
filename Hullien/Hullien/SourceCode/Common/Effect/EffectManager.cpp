#include "EffectManager.h"
#include "..\..\Resource\EffectResource\EffectResource.h"

CEffectManager::CEffectManager()
	: m_pEffect	( nullptr )
	, m_Handle	( -1 )
{
}

CEffectManager::~CEffectManager()
{
}

//------------------------------.
// �G�t�F�N�g�̐ݒ�.
//------------------------------.
bool CEffectManager::SetEffect( const char* effectName )
{
	if( m_pEffect != nullptr ) return true;
	m_pEffect = CEffectResource::GetEffect( effectName );
	if( m_pEffect == nullptr ) return false;
	return true;
}

//------------------------------.
// �`��.
//------------------------------.
void CEffectManager::Render()
{
	if( m_pEffect == nullptr ) return;
	if( m_Handle == -1 ) return;
	m_pEffect->Render( m_Handle );
}

//------------------------------.
// �Đ�.
//------------------------------.
void CEffectManager::Play( const D3DXVECTOR3 vPos )
{
	if( m_pEffect == nullptr ) return;
	m_Handle = m_pEffect->Play( vPos );
}

//------------------------------.
// ��~.
//------------------------------.
void CEffectManager::Stop()
{
	m_pEffect->Stop( m_Handle );
}

//------------------------------.
// �S�Ē�~.
//------------------------------.
void CEffectManager::StopAll()
{
	m_pEffect->StopAll();
}

//------------------------------.
// �ꎞ��~.
//------------------------------.
void CEffectManager::Pause( const bool bFlag )
{
	m_pEffect->Pause( m_Handle, bFlag ); 
}

//------------------------------.
// �ʒu���w�肷��.
//------------------------------.
void CEffectManager::SetLocation( const D3DXVECTOR3 vPos )
{
	m_pEffect->SetLocation( m_Handle, vPos );
}

//------------------------------.
// �T�C�Y���w�肷��.
//------------------------------.
void CEffectManager::SetScale( const D3DXVECTOR3& vScale )
{
	m_pEffect->SetScale( m_Handle, vScale );
}
void CEffectManager::SetScale( const float& scale )
{
	m_pEffect->SetScale( m_Handle, { scale, scale, scale } );
}

//------------------------------.
// ��]���w�肷��.
//------------------------------.
void CEffectManager::SetRotation( const D3DXVECTOR3& vRot )
{
	m_pEffect->SetRotation( m_Handle, vRot );
}

//------------------------------.
// �Đ����x��ݒ肷��.
//------------------------------.
void CEffectManager::SetSpeed( const float fSpeed )
{
	m_pEffect->SetSpeed( m_Handle, fSpeed );
}

//------------------------------.
// ���݂��Ă��邩.
//------------------------------.
bool CEffectManager::IsExists()
{
	return m_pEffect->IsExists( m_Handle );
}