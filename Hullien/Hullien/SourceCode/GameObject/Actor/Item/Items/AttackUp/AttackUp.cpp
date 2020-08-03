#include "AttackUp.h"

CAttackUpItem::CAttackUpItem()
	: CItemBase			( nullptr )
	, pATTACK_UP_POWER	( nullptr )
	, pATTACK_UP_TIME	( nullptr )
{
}

CAttackUpItem::CAttackUpItem( 
	const float* pAttackUpPower,
	const float* pAttackUpTime,
	const CItemBase::SParameter* pParam )
	: CItemBase			( pParam )
	, pATTACK_UP_POWER	( pAttackUpPower )
	, pATTACK_UP_TIME	( pAttackUpTime )
{
}

CAttackUpItem::~CAttackUpItem()
{
}

// �������֐�.
bool CAttackUpItem::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( ColliderSetting() == false ) return false;
	m_vSclae = { 0.0f, 0.0f, 0.0f };
	return true;
}

// �X�V�֐�.
void CAttackUpItem::Update()
{
	CItemBase::Update();
}

// �`��֐�.
void CAttackUpItem::Render()
{
	CItemBase::Render();
}

// �����蔻��֐�.
void CAttackUpItem::Collision( CActor* pActor )
{
	CItemBase::Collision( pActor );
}

// ������W�̐ݒ�֐�.
void CAttackUpItem::SetTargetPos( CActor& pActor )
{
	m_vPosition = pActor.GetPosition();
}

// ���������ۂ̌��ʂ�^����.
void CAttackUpItem::GiveEffect( CActor* pActor )
{
	if( pATTACK_UP_POWER == nullptr ) return;
	if( pATTACK_UP_TIME == nullptr ) return;

	// �񕜊֐�.
	auto attackUp = [&]( float& attackPower, float& attackUpTime )
	{
		attackPower = *pATTACK_UP_POWER;
		attackUpTime = *pATTACK_UP_TIME;
	};
	// �R�[���o�b�N�֐�.
	pActor->SetAttackEffectTime( attackUp );	// �Ώۂ̗̑͂��񕜂���.
}