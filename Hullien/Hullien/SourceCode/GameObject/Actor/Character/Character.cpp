#include "Character.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"

CCharacter::CCharacter()
	: m_pSkinMesh	( nullptr )
	, m_Life		( 1.0f )
	, m_MoveVector	( 0.0f, 0.0f, 0.0f )
{
	
}

CCharacter::~CCharacter()
{
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