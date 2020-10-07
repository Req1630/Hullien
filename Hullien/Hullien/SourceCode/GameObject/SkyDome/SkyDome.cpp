#include "SkyDome.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Common\SceneTexRenderer\SceneTexRenderer.h"

CSkyDome::CSkyDome()
	: m_StaticMesh	( nullptr )
{
	m_vPosition.y = -10.0f;
}

CSkyDome::~CSkyDome()
{
}

// �������֐�.
bool CSkyDome::Init()
{
	if( GetModel() == false ) return false;
	return true;
}

// �X�V�֐�.
void CSkyDome::Update()
{
}

// �`��֐�.
void CSkyDome::Render()
{
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::Shadow ) return;
	if( m_StaticMesh == nullptr ) return;

	m_StaticMesh->SetPosition( m_vPosition );
	m_StaticMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_StaticMesh->Render();
	m_StaticMesh->SetRasterizerState( CCommon::enRS_STATE::None );

}

// ���f���̎擾.
bool CSkyDome::GetModel()
{
	if( m_StaticMesh != nullptr ) return false;
	CMeshResorce::GetStatic( m_StaticMesh, MODEL_NAME );
	if( m_StaticMesh == nullptr ) return false;

	return true;
}