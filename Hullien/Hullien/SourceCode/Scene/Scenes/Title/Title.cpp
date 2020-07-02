#include "..\SceneList.h"
#include "..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\..\Camera\RotLookAtCenter\RotLookAtCenter.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\Common\DebugText\DebugText.h"

CTitle::CTitle( CSceneManager* pSceneManager )
	: CSceneBase	( pSceneManager )
{
}

CTitle::~CTitle()
{
}

//============================.
//	�Ǎ��֐�.
//============================.
bool CTitle::Load()
{
	return true;
}

//============================.
//	�X�V�֐�.
//============================.
void CTitle::Update()
{
	if( GetAsyncKeyState(VK_RETURN) & 0x0001 ){
		m_pSceneManager->NextSceneMove();
	}
}

//============================.
//	�`��֐�.
//============================.
void CTitle::Render()
{
}