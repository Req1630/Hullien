#include "MoveSpeedUp.h"

CMoveSpeedUpItem::CMoveSpeedUpItem()
	: CItemBase		( nullptr )
	, pMOVE_VALUE	( nullptr )
	, pMOVE_UP_TIME	( nullptr )
{
}

CMoveSpeedUpItem::CMoveSpeedUpItem( 
	const float* pMoveValue,
	const float* pMoveTime,
	const CItemBase::SParameter* pParam )
	: CItemBase		( pParam )
	, pMOVE_VALUE	( pMoveValue )
	, pMOVE_UP_TIME	( pMoveTime )
{
}

CMoveSpeedUpItem::~CMoveSpeedUpItem()
{
}

// �������֐�.
bool CMoveSpeedUpItem::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( ColliderSetting() == false ) return false;
	m_vSclae = { 0.0f, 0.0f, 0.0f };
	return true;
}

// �X�V�֐�.
void CMoveSpeedUpItem::Update()
{
	CItemBase::Update();
}

// �`��֐�.
void CMoveSpeedUpItem::Render()
{
	CItemBase::Render();
}

// �����蔻��֐�.
void CMoveSpeedUpItem::Collision( CActor* pActor )
{
	CItemBase::Collision( pActor );
}

// ������W�̐ݒ�֐�.
void CMoveSpeedUpItem::SetTargetPos( CActor& pActor )
{
	m_vPosition = pActor.GetPosition();
}

// ���������ۂ̌��ʂ�^����.
void CMoveSpeedUpItem::GiveEffect( CActor* pActor )
{
	if( pMOVE_VALUE == nullptr ) return;
	if( pMOVE_UP_TIME == nullptr ) return;

	// �񕜊֐�.
	auto moveSpeedUp = [&]( float& moveValue, float& moveUpTime )
	{
		moveValue = *pMOVE_VALUE;
		moveUpTime = *pMOVE_UP_TIME;
	};
	// �R�[���o�b�N�֐�.
	pActor->SetMoveSpeedEffectTime( moveSpeedUp );	// �Ώۂ̗̑͂��񕜂���.
}