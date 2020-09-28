#include "SpawnUFO.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\Actor\Character\Alien\Alien_A\Alien_A.h"
#include "..\Actor\Character\Alien\Alien_B\Alien_B.h"
#include "..\Actor\Character\Alien\Alien_C\Alien_C.h"
#include "..\Actor\Character\Alien\Alien_D\Alien_D.h"

CSpawnUFO::CSpawnUFO()
	: m_pStaticMesh				( nullptr )
	, m_SpawnParameter			()
	, m_pAbductUFOPosition		( nullptr )
	, m_pAlienParamList			( nullptr )
	, m_FrameCount				( 0 )
	, m_SpawnCount				( 0 )
	, m_AlienIndex				( 0 )
	, m_AlienCWaveCount			( 0 )
	, m_AlienCWaveIntervalCount	( 0 )
	, m_AlienDWaveCount			( 0 )
	, m_AlienDWaveIntervalCount	( 0 )
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
	return true;
}

// �X�V�֐�.
void CSpawnUFO::Update()
{
	m_FrameCount++;	// �J�E���g�̉��Z.
	m_SpawnCount++;	// �X�|�[���J�E���g�̉��Z.
	m_AlienCWaveCount++;	// �F���lC�̃E�F�[�u�J�E���g�̉��Z.
	m_AlienDWaveCount++;	// �F���lD�̃E�F�[�u�J�E���g�̉��Z.
	WaveCountUpdate(
		m_AlienCWaveCount, m_SpawnParameter.AlienCWaveTime,
		m_AlienCWaveIntervalCount, m_SpawnParameter.AlienCWaveIntervalTime );
	WaveCountUpdate(
		m_AlienDWaveCount, m_SpawnParameter.AlienDWaveTime,
		m_AlienDWaveIntervalCount, m_SpawnParameter.AlienDWaveIntervalTime );
}

// �`��֐�.
void CSpawnUFO::Render()
{
	if( m_pStaticMesh == nullptr ) return;

	m_pStaticMesh->SetPosition( m_vPosition );
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

	if( m_SpawnCount != m_SpawnParameter.SpawnTime*FPS ) return;
//	std::shared_ptr<CAlien> tempAlien = AlienFactory();
//	if( tempAlien == nullptr ) return;
	if( m_pAbductUFOPosition == nullptr ) return;

	alienList.emplace_back( AlienFactory() );	// �F���l�̒ǉ�.
	if( alienList.back() == nullptr ){
		alienList.pop_back();
		return;
	}
	// �F���l�̃p�����[�^���X�g����Ȃ�I��.
	if( m_pAlienParamList->empty() == true ) return;
	if( m_pAlienParamList->size() <= static_cast<size_t>(m_AlienIndex) ) return;

	// ���X�g�ɂ������p�����[�^�ƃX�|�[�����W��ݒ肵�A�X�|�[��������.
	alienList.back()->Spawn( m_pAlienParamList->at(m_AlienIndex), m_SpawnParameter.Position );
	// �A�ꋎ��UFO�̍��W��ݒ�.
	alienList.back()->SetAbductUFOPosition( m_pAbductUFOPosition );
//	m_SpawnCount = 0;
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
	m_vPosition = m_SpawnParameter.Position;
	m_vPosition.y = POS_HEIGHT;
}

// �E�F�[�u�J�E���g�̍X�V.
void CSpawnUFO::WaveCountUpdate( 
	int& waveCount, const int& waveTime, 
	int& intervalcount, const int& intervalTime )
{
	if( waveCount < waveTime*FPS ) return;
	intervalcount++;	// �Ԋu�J�E���g�̉��Z.

	if( intervalcount < intervalTime*FPS ) return;
	intervalcount = 0;	// ������.
	waveCount = 0;		// ������.
}

// �F���l�̍쐬.
std::shared_ptr<CAlien> CSpawnUFO::AlienFactory()
{
	// �F���l�ԍ��̍쐬.
	const EAlienList alienNo = EAlienList::D;//static_cast<EAlienList>(GetAlienNo());

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
	if( m_AlienCWaveCount >= m_SpawnParameter.AlienCWaveTime*FPS &&
		m_AlienDWaveCount >= m_SpawnParameter.AlienDWaveTime*FPS ){
		// �F���lC�AD���X�|�[�����܂߂�.
		return CreateAlienNo( 
			static_cast<int>(EAlienList::First),	// �ŏ��l.
			static_cast<int>(EAlienList::Last),		// �ő�l.
			static_cast<int>(EAlienList::Max) );	// ���O����l.
	} else if( m_AlienCWaveCount >= m_SpawnParameter.AlienCWaveTime*FPS ){
		// �F���lC���܂߂�.
		return CreateAlienNo( 
			static_cast<int>(EAlienList::First),	// �ŏ��l.
			static_cast<int>(EAlienList::C),		// �ő�l.
			static_cast<int>(EAlienList::D) );		// ���O����l.
	} else if( m_AlienDWaveCount >= m_SpawnParameter.AlienDWaveTime*FPS ){
		// �F���lD���܂߂�.
		return CreateAlienNo( 
			static_cast<int>(EAlienList::First),	// �ŏ��l.
			static_cast<int>(EAlienList::D),		// �ő�l.
			static_cast<int>(EAlienList::C) );		// ���O����l.
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

// ���f���̎擾.
bool CSpawnUFO::GetModel()
{
	if( m_pStaticMesh != nullptr ) return false;
	CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME );
	if( m_pStaticMesh == nullptr ) return false;
	return true;
}