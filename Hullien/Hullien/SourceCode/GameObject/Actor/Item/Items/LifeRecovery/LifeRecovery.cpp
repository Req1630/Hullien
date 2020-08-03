#include "LifeRecovery.h"

CLifeRecoveryItem::CLifeRecoveryItem()
	: CItemBase				( nullptr )
	, pLIFE_RECOVERY_VALUE	( nullptr )
{
}

CLifeRecoveryItem::CLifeRecoveryItem( 
	const float* pLifeRecoveryValue, 
	const CItemBase::SParameter* pParam )
	: CItemBase				( pParam )
	, pLIFE_RECOVERY_VALUE	( pLifeRecoveryValue)
{
}

CLifeRecoveryItem::~CLifeRecoveryItem()
{
	pLIFE_RECOVERY_VALUE = nullptr ;
}

// �������֐�.
bool CLifeRecoveryItem::Init()
{
	if( GetModel( MODEL_NAME ) == false ) return false;
	if( ColliderSetting() == false ) return false;
	m_vSclae = { 0.0f, 0.0f, 0.0f };
	return true;
}

// �X�V�֐�.
void CLifeRecoveryItem::Update()
{
	CItemBase::Update();
}

// �`��֐�.
void CLifeRecoveryItem::Render()
{
	CItemBase::Render();
}

// �����蔻��֐�.
void CLifeRecoveryItem::Collision( CActor* pActor )
{
	CItemBase::Collision( pActor );
}

// ������W�̐ݒ�֐�.
void CLifeRecoveryItem::SetTargetPos( CActor& pActor )
{
	m_vPosition = pActor.GetPosition();
}

// ���������ۂ̌��ʂ�^����.
void CLifeRecoveryItem::GiveEffect( CActor* pActor )
{
	if( pLIFE_RECOVERY_VALUE == nullptr ) return;
	// �񕜊֐�.
	auto lifeRecovery = [&]( float& life )
	{
		life += *pLIFE_RECOVERY_VALUE;
	};
	// �R�[���o�b�N�֐�.
	pActor->LifeCalculation( lifeRecovery );	// �Ώۂ̗̑͂��񕜂���.
}