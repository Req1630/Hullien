#include "SPEffectTime.h"

CSPEffectTimeItem::CSPEffectTimeItem()
	: CItemBase				( nullptr )
	, pSP_RECOVERY_VALUE	( nullptr )
	, pSP_RECOVERY_TIME		( nullptr )
{
}

CSPEffectTimeItem::CSPEffectTimeItem( 
	const float* pSPRecoveryValue,
	const float* pSPRecoveryTime,
	const CItemBase::SParameter* pParam )
	: CItemBase				( pParam )
	, pSP_RECOVERY_VALUE	( pSPRecoveryValue )
	, pSP_RECOVERY_TIME		( pSPRecoveryTime )
{
}

CSPEffectTimeItem::~CSPEffectTimeItem()
{
	pSP_RECOVERY_VALUE = nullptr;
	pSP_RECOVERY_TIME = nullptr;
}

// �������֐�.
bool CSPEffectTimeItem::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( ColliderSetting() == false ) return false;
	m_vSclae = { 0.0f, 0.0f, 0.0f };
	return true;
}

// �X�V�֐�.
void CSPEffectTimeItem::Update()
{
	CItemBase::Update();
}

// �`��֐�.
void CSPEffectTimeItem::Render()
{
	CItemBase::Render();
}

// �����蔻��֐�.
void CSPEffectTimeItem::Collision( CActor* pActor )
{
	CItemBase::Collision( pActor );
}

// ������W�̐ݒ�֐�.
void CSPEffectTimeItem::SetTargetPos( CActor& pActor )
{
	m_vPosition = pActor.GetPosition();
}

// ���������ۂ̌��ʂ�^����.
void CSPEffectTimeItem::GiveEffect( CActor* pActor )
{
	if( pSP_RECOVERY_VALUE == nullptr ) return;
	if( pSP_RECOVERY_TIME == nullptr ) return;

	// �񕜊֐�.
	auto spEffectTime = [&]( float& recoveryValue, float& recoveryTime )
	{
		recoveryValue = *pSP_RECOVERY_VALUE;
		recoveryTime = *pSP_RECOVERY_TIME;
	};
	// �R�[���o�b�N�֐�.
	pActor->SetSPEffectTime( spEffectTime );	// �Ώۂ̗̑͂��񕜂���.
}