#include "Character.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"

#include "..\..\..\Utility\FileManager\FileManager.h"

CCharacter::CCharacter()
	: m_pSkinMesh				( nullptr )
	, m_MoveVector				( 0.0f, 0.0f, 0.0f )
	, m_InvincibleCount			( 0 )
	, m_HasFinishedParamSetting	( false )
{
	
}

CCharacter::~CCharacter()
{
}

// ���G���Ԃ��ǂ���.
bool CCharacter::IsInvincibleTime( const int& invincibleTime )
{
	return m_InvincibleCount > invincibleTime * FPS;
}

// ���f���̎擾.
bool CCharacter::GetModel( const char* modelName )
{
	// ���ɓǂݍ��߂Ă�����I��.
	if( m_pSkinMesh != nullptr ) return true;
	// ���f���̎擾.
	CMeshResorce::GetSkin( m_pSkinMesh, modelName );
	// ���f�����ǂݍ��߂ĂȂ���� false.
	if( m_pSkinMesh == nullptr ) return false;
	return true;
}