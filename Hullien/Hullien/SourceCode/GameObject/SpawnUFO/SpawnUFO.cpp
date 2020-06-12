#include "SpawnUFO.h"
#include "..\Actor\Character\Alien\AlienList.h"
#include "..\Actor\Character\Alien\Alien_A\Alien_A.h"

CSpawnUFO::CSpawnUFO()
	: m_SpawnParameter		()
	, m_pAbductUFOPosition	( nullptr )
	, m_pAlienParamList		( nullptr )
	, m_FrameCount			( 0 )
	, m_AlienIndex			( 0 )
{
}

CSpawnUFO::~CSpawnUFO()
{
}

// �������֐�.
bool CSpawnUFO::Init()
{
	return true;
}

// �X�V�֐�.
void CSpawnUFO::Update()
{
	m_FrameCount++;	// �J�E���g�̉��Z.
}

// �`��֐�.
void CSpawnUFO::Render()
{
}

// �F���l���X�|�[��������.
void CSpawnUFO::SpawnAlien( std::vector<std::shared_ptr<CAlien>>& alienList )
{
	Update();	// �X�V�֐�.

	if( m_FrameCount != m_SpawnParameter.SpawnTime ) return;
	std::shared_ptr<CAlien> tempAlien = AlienFactory();
	if( tempAlien == nullptr ) return;
	if( m_pAbductUFOPosition == nullptr ) return;

	alienList.emplace_back( tempAlien );	// �F���l�̒ǉ�.
	// �F���l�̃p�����[�^���X�g����Ȃ�I��.
	if( m_pAlienParamList->empty() == true ) return;
	if( m_pAlienParamList->size() <= static_cast<size_t>(m_AlienIndex) ) return;

	// ���X�g�ɂ������p�����[�^�ƃX�|�[�����W��ݒ肵�A�X�|�[��������.
	alienList.back()->Spawn( m_pAlienParamList->at(m_AlienIndex), m_SpawnParameter.Position );
	// �A�ꋎ��UFO�̍��W��ݒ�.
	alienList.back()->SetAbductUFOPosition( m_pAbductUFOPosition );
	m_FrameCount = 0;
}

// �F���l�̃p�����[�^���X�g��ݒ肷��.
void CSpawnUFO::SetAlienParameterList( std::vector<CAlien::SAlienParam>* alienParamList )
{
	m_pAlienParamList = alienParamList;
}

// �G�̍쐬.
std::shared_ptr<CAlien> CSpawnUFO::AlienFactory()
{
	const EAlienList alienNo = EAlienList::A;
	switch( alienNo )
	{
	case EAlienList::A:
		m_AlienIndex = static_cast<int>(EAlienList::A);
		m_AlienIndex--;
		return std::make_shared<CAlienA>();
	default:
		break;
	}
	return nullptr;
}