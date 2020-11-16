#include "EditAlien_A.h"
#include "..\..\..\..\..\Arm\Arm.h"
#include "..\..\..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"

CEditAlienA::CEditAlienA()
{}

CEditAlienA::~CEditAlienA()
{}

// 初期化関数.
bool CEditAlienA::Init()
{
	if( GetModel( MODEL_NAME )		== false ) return false;
	if( GetAnimationController()	== false ) return false;
	if( SetAnimFrameList()			== false ) return false;
	if( EffectSetting()				== false ) return false;
	m_pSkinMesh->ChangeAnimSet_StartPos( alien::EAnimNo_Move, 0.0f, m_pAC );
	return true;
}

// 更新関数.
void CEditAlienA::Update()
{}

// 描画関数.
void CEditAlienA::Render()
{}

// 当たり判定関数.
void CEditAlienA::Collision( CActor* pActor )
{}