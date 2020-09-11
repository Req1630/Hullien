#include "EventUFO.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"

/**************************************
*	�C�x���g�pUFO�N���X.
**/
CEventUFO::CEventUFO()
	: m_pStaticMesh( nullptr )
{
}

CEventUFO::~CEventUFO()
{
}


// �������֐�.
bool CEventUFO::Init()
{
	if (GetModel() == false) return false;
	m_vPosition.y = POS_HEIGHT;
	return true;
}

// �X�V�֐�.
void CEventUFO::Update()
{
}

// �`��֐�.
void CEventUFO::Render()
{
	if ( m_pStaticMesh == nullptr ) return;

	m_pStaticMesh->SetPosition( m_vPosition );
	m_pStaticMesh->SetScale( m_vSclae );
	m_pStaticMesh->Render();
}

// ���f���̎擾.
bool CEventUFO::GetModel()
{
	if ( m_pStaticMesh != nullptr ) return false;
	CMeshResorce::GetStatic(m_pStaticMesh, MODEL_NAME);
	if ( m_pStaticMesh == nullptr ) return false;
	return true;
}
