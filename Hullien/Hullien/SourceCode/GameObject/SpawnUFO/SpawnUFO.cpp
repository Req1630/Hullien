#include "SpawnUFO.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\Actor\Character\Alien\Alien_A\Alien_A.h"
#include "..\Actor\Character\Alien\Alien_B\Alien_B.h"
#include "..\Actor\Character\Alien\Alien_C\Alien_C.h"
#include "..\Actor\Character\Alien\Alien_D\Alien_D.h"

#include "..\..\Collider\CollsionManager\CollsionManager.h"

CSpawnUFO::CSpawnUFO()
	: m_pStaticMesh				( nullptr )
	, m_pCollManager			( nullptr )
	, m_SpawnParameter			()
	, m_SpawnPoint				{ 0.0f, 0.0f, 0.0f }
	, m_pAbductUFOPosition		( nullptr )
	, m_pAlienParamList			( nullptr )
	, m_FrameCount				( 0 )
	, m_SpawnCount				( 0 )
	, m_AlienIndex				( 0 )
	, m_IsDisp					( true ) 
	, m_RandomSeed				()
{
	// �����_���V�[�h�̏�����.
	std::random_device rd;
	m_RandomSeed = std::mt19937( rd() );
}

CSpawnUFO::~CSpawnUFO()
{
}

// �������֐�.
bool CSpawnUFO::Init()
{
	if( GetModel() == false ) return false;
	if( CollisionSetting() == false ) return false;
	return true;
}

// �����蔻��(�C�x���g�Ŏg�p).
D3DXVECTOR3 CSpawnUFO::Collision(CActor * pActor)
{
	// �ΏۃI�u�W�F�N�g����Ȃ���ΏI��.
	if ((pActor->GetObjectTag() != EObjectTag::Player) &&
		(pActor->GetObjectTag() != EObjectTag::Girl)) return pActor->GetPosition();

	if (m_pCollManager->IsShereToShere(pActor->GetCollManager()) == false) return pActor->GetPosition();

	D3DXVECTOR3 pos = pActor->GetPosition();
	pos.y += ADD_POS_POWER;	// ���W����ɂ�����.

	return pos;
}

// �X�V�֐�.
void CSpawnUFO::Update()
{
	m_SpawnCount++;
}

// �`��֐�.
void CSpawnUFO::Render()
{
	// ��ʊO�Ȃ�I��.
	if( IsDisplayOut() == true ) return;
	if( m_pStaticMesh == nullptr ) return;
	if( m_IsDisp == false ) return;

	m_pStaticMesh->SetPosition( m_vPosition );
	m_pStaticMesh->SetScale( m_vSclae );
	m_pStaticMesh->SetRasterizerState( CCommon::enRS_STATE::Back );
	m_pStaticMesh->Render();
	m_pStaticMesh->SetRasterizerState( CCommon::enRS_STATE::None );
}

// �F���l���X�|�[��������.
void CSpawnUFO::SpawnAlien( std::vector<std::shared_ptr<CAlien>>& alienList )
{
	// �F���l���X�g�̃T�C�Y���ő吔��葽����ΏI��.
	if( (int)alienList.size() >= m_SpawnParameter.MaxAlienCount ) return;

	Update();	// �X�V�֐�.

	if( m_SpawnCount < m_SpawnParameter.SpawnTime*FPS ) return;
	if( m_pAbductUFOPosition == nullptr ) return;

	alienList.emplace_back( AlienFactory() );	// �F���l�̒ǉ�.
	// �ǉ������G�C���A����null�Ȃ�폜���āA�I��.
	if( alienList.back() == nullptr ){
		alienList.pop_back();
		return;
	}

	// �F���l�̃p�����[�^���X�g����Ȃ�I��.
	if( m_pAlienParamList->empty() == true ) return;
	if( m_pAlienParamList->size() <= static_cast<size_t>(m_AlienIndex) ) return;

	// ���X�g�ɂ������p�����[�^�ƃX�|�[�����W��ݒ肵�A�X�|�[��������.
	alienList.back()->Spawn( m_pAlienParamList->at(m_AlienIndex), m_SpawnPoint );
	// �A�ꋎ��UFO�̍��W��ݒ�.
	alienList.back()->SetAbductUFOPosition( m_pAbductUFOPosition );
	// �A�C�e���̐ݒ�.
	alienList.back()->SetItem( ProbabilityGetItem( static_cast<EAlienList>(m_AlienIndex) == EAlienList::D ) );
	m_SpawnCount = 0;
}

// �F���l�̃p�����[�^���X�g��ݒ肷��.
void CSpawnUFO::SetAlienParameterList( std::vector<CAlien::SAlienParam>* alienParamList )
{
	m_pAlienParamList = alienParamList;
}

// �X�|�[���p�����[�^�̐ݒ�.
void CSpawnUFO::SetSpawnParameter( const SSpawnUFOParam& param )
{ 
	m_SpawnParameter = param;
	m_SpawnPoint	= m_vPosition = m_SpawnParameter.Position;
//	m_SpawnPoint.y	= m_SpawnParameter.SpawnPointHight;
}

// �F���l�̍쐬.
std::shared_ptr<CAlien> CSpawnUFO::AlienFactory()
{
	// �F���l�ԍ��̍쐬.
	const EAlienList alienNo = static_cast<EAlienList>(GetAlienNo());

	switch( alienNo )
	{
	case EAlienList::A:
		m_AlienIndex = static_cast<int>(alienNo);
		return std::make_shared<CAlienA>();

	case EAlienList::Ada:
		m_AlienIndex = static_cast<int>(alienNo);
		return std::make_shared<CAlienA>();

	case EAlienList::B:
		m_AlienIndex = static_cast<int>(alienNo);
		return std::make_shared<CAlienB>();

	case EAlienList::Bda:
		m_AlienIndex = static_cast<int>(alienNo);
		return std::make_shared<CAlienB>();

	case EAlienList::C:
		m_AlienIndex = static_cast<int>(alienNo);
		return std::make_shared<CAlienC>();

	case EAlienList::D:
		m_AlienIndex = static_cast<int>(alienNo);
		return std::make_shared<CAlienD>();
	default:
		break;
	}
	return nullptr;
}

// �F���l�ԍ��̍쐬.
int CSpawnUFO::GetAlienNo()
{
	if( (rand()%100) < m_SpawnParameter.ProbabilityD ){
		// �F���lD���܂߂�.
		return CreateAlienNo( 
			static_cast<int>(EAlienList::First),	// �ŏ��l.
			static_cast<int>(EAlienList::D),		// �ő�l.
			static_cast<int>(EAlienList::C) );		// ���O����l.
	} else if( (rand()%100) < m_SpawnParameter.ProbabilityC ){
		// �F���lC���܂߂�.
		return CreateAlienNo( 
			static_cast<int>(EAlienList::First),	// �ŏ��l.
			static_cast<int>(EAlienList::C),		// �ő�l.
			static_cast<int>(EAlienList::D) );		// ���O����l.
	} else {
		// �F���lC�AD���܂߂Ȃ�.
		return CreateAlienNo( 
			static_cast<int>(EAlienList::First),	// �ŏ��l.
			static_cast<int>(EAlienList::Bda),		// �ő�l.
			static_cast<int>(EAlienList::C) );		// ���O����l.
	}
}

int CSpawnUFO::CreateAlienNo( const int& min, const int& max, const int& outVal )
{
	// �����_���l�̍ő�ƍŏ��̒l��ݒ�.
	std::uniform_int_distribution<int> alienNoMaxMin( min, max );
	int outAlienNo = alienNoMaxMin( m_RandomSeed );
	if( outVal == outAlienNo ){
		// ���O����l�ɂȂ����ꍇ�A�ēx��蒼��.
		outAlienNo = CreateAlienNo( min, max, outVal );
	}
	return outAlienNo;
}

// �A�C�e�����擾����.
EItemList CSpawnUFO::ProbabilityGetItem( const bool& isAlienD )
{
	// �������m���l��菭�Ȃ��ꍇ�@�܂��́@�F���lD�̏ꍇ.
	if(( (rand()%100) < m_SpawnParameter.ProbabilityItem ) || ( isAlienD == true )){
		// �����_���l�̍ő�ƍŏ��̒l��ݒ�.
		std::uniform_int_distribution<int> itemNoMaxMin( 
			static_cast<int>(EItemList::First),
			static_cast<int>(EItemList::Last) );
		// �A�C�e���ԍ����擾����.
		return static_cast<EItemList>(itemNoMaxMin( m_RandomSeed ));
	} else {
		return EItemList::None;	// �A�C�e���Ȃ�.
	}
}

// ���f���̎擾.
bool CSpawnUFO::GetModel()
{
	if( m_pStaticMesh != nullptr ) return false;
	CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME );
	if( m_pStaticMesh == nullptr ) return false;
	return true;
}

// �����蔻��̐ݒ�.
bool CSpawnUFO::CollisionSetting()
{
	if (m_pCollManager == nullptr) {
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if (FAILED(m_pCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_vSclae.x,
		{ 0.0f, 0.0f, 0.0f },
		COLLISION_RADIUS))) {
		return false;
	}
	return true;
}