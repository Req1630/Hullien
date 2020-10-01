#include "GameObject.h"
#include "..\Camera\CameraManager\CameraManager.h"
#include "..\Common\SceneTexRenderer\SceneTexRenderer.h"

// �E�B���h�E�O�̒����l.
static const float WND_OUT_ADJ_SIZE = 300.0f;

/****************************************
*	�Q�[���I�u�W�F�N�g���N���X.
**/
CGameObject::CGameObject()
	: m_ObjectTag	( EObjectTag::None )
	, m_vPosition	( 0.0f, 0.0f, 0.0f )
	, m_vRotation	( 0.0f, 0.0f, 0.0f )
	, m_vSclae		( 1.0f, 1.0f, 1.0f )
{
}

CGameObject::~CGameObject()
{
}

//-----------------------------.
// �I�u�W�F�N�g�^�O�擾�֐�.
//-----------------------------.
EObjectTag CGameObject::GetObjectTag() const
{
	return m_ObjectTag;
}

//-----------------------------.
// ���W�擾�֐�.
//-----------------------------.
D3DXVECTOR3 CGameObject::GetPosition() const
{
	return m_vPosition;
}

//-----------------------------.
// ���W�ݒ�֐�.
//-----------------------------.
void CGameObject::SetPosition( const D3DXVECTOR3& vPos )
{
	m_vPosition = vPos;
}

//-----------------------------.
// ��ʂ̊O�ɏo�Ă��邩.
//-----------------------------.
bool CGameObject::IsDisplayOut()
{
	// �r���[�|�[�g�s��.
	D3DXMATRIX mViewport;
	D3DXMatrixIdentity( &mViewport );	// �P�ʍs��쐬.
	mViewport._11 = static_cast<float>( WND_W ) *  0.5f;
	mViewport._22 = static_cast<float>( WND_H ) * -0.5f;
	mViewport._41 = static_cast<float>( WND_W ) *  0.5f;
	mViewport._42 = static_cast<float>( WND_H ) *  0.5f;

	// ���[���h, �r���[, �v���W�F, �r���[�|�[�g�̌v�Z.
	D3DXMATRIX m = CCameraManager::GetViewMatrix() * CCameraManager::GetProjMatrix() * mViewport;
	D3DXVECTOR3 screenPos = { 0.0f, 0.0f, 0.0f };	// �X�N���[�����W.
	// D3DXVECTOR3�Ɗ|�����킹.
	D3DXVec3TransformCoord( &screenPos, &m_vPosition, &m );

	if( -WND_OUT_ADJ_SIZE < screenPos.x && screenPos.x < WND_W + WND_OUT_ADJ_SIZE &&
		-WND_OUT_ADJ_SIZE < screenPos.y && screenPos.y < WND_H + WND_OUT_ADJ_SIZE ) return false;	// ��ʓ�.
	// ��ʊO�ł��V���h�E�͕`�悷��.
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::Shadow ) return false;
	
	return true;	// ��ʊO.
}