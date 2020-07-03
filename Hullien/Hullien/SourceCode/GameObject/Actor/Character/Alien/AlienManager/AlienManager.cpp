#include "AlienManager.h"
#include "..\..\..\..\SpawnUFO\SpawnUFO.h"
#include "..\AlienList.h"
#include "..\..\..\Explosion\Explosion.h"

#include "..\..\..\..\..\Common\DebugText\DebugText.h"
#include "..\..\..\..\..\Utility\FileManager\FileManager.h"

#include <algorithm>

CAlienManager::CAlienManager()
	: m_AilenList			()
	, m_SpawnUFOList		()
	, m_AlienParamList		()
	, m_AbductUFOPosition	( 10.0f, 0.0f, 0.0f )
	, m_IsAlienAbduct		( false )
	, m_SortCount			( 0 )
{
}

CAlienManager::~CAlienManager()
{
}

// �������֐�.
bool CAlienManager::Init()
{
	if( SpawnUFOInit() == false ) return false;
	if( ReadAlienParamList() == false ) return false;
	return true;
}

// �X�V�֐�.
void CAlienManager::Update( CActor* pPlayer, CActor* pGirl, std::function<void(CActor*)> collProc )
{
	Spawn();	// �X�|�[��.
	bool isAbduct = false;
	// �F���l�B�̍X�V.
	for( size_t i = 0; i < m_AilenList.size(); i++ ){
		m_AilenList[i]->SetOtherAbduct( &m_IsAlienAbduct );

		// ���W�̐ݒ�.
		m_AilenList[i]->SetTargetPos( *pPlayer );
		m_AilenList[i]->SetTargetPos( *pGirl );
		// �X�V����.
		m_AilenList[i]->Update();
		// �v���C���[�A���̎q�̓����蔻��.
		collProc( m_AilenList[i].get() );
		// �F���l�̓����蔻��.
		m_AilenList[i]->Collision( pPlayer );
		m_AilenList[i]->Collision( pGirl );

		// ���̎q��A�ꋎ���Ă��邩�m�F.
		if( isAbduct == false ){
			if( m_AilenList[i]->IsAbduct() == true ){
				isAbduct = m_IsAlienAbduct = true;
			}
		}

		// �����ł��邩�m�F.
		ExplosionConfirming( m_AilenList[i] );
		// �����ƉF���l�̓����蔻��.
		for( auto& e : m_ExplosionList ) e.Collision( m_AilenList[i].get() );

		// ���X�g����w��̉F���l��������.
		if( m_AilenList[i]->IsDelete() == false ) continue;
		m_AilenList[i] = m_AilenList.back();
		m_AilenList.pop_back();
		i--;
	}
	if( isAbduct == false ) m_IsAlienAbduct = false;
	// ���f���̃A���t�@�l�ł̃\�[�g.
	ModelAlphaSort();
}

// �`��֐�.
void CAlienManager::Render()
{
	// �F���l�B�̕`��.
	for( auto& a : m_AilenList ) a->Render();
	// �X�|�[��UFO�̕`��.
	for( auto& s : m_SpawnUFOList ) s.Render();
	// �����̕`��.
	for( auto& e : m_ExplosionList ) e.Render();
#if _DEBUG
	DebugRender();
#endif	// #if _DEBUG.
}

// �X�|�[��.
void CAlienManager::Spawn()
{
	for( auto& s : m_SpawnUFOList ){
		s.SpawnAlien( m_AilenList );
	}
}

// �����ł��邩�ǂ����m�F.
void CAlienManager::ExplosionConfirming( const std::shared_ptr<CAlien>& ailen  )
{
	// �F���lC����Ȃ���ΏI��.
	if( ailen->GetObjectTag() != EObjectTag::Alien_C ) return;
	// �������Ȃ���ΏI��.
	if( ailen->IsExplosion() == false ) return;

	// ������ǉ�.
	m_ExplosionList.emplace_back();
	m_ExplosionList.back().Init();
}

// �X�|�[��UFO�̏�����.
bool CAlienManager::SpawnUFOInit()
{
	std::vector<SSpawnUFOParam> spawnPramList;
	if( CFileManager::BinaryVectorReading( SPAWN_PARAM_FILE_PATH, spawnPramList ) == false ) return false;
	for( const auto& s : spawnPramList ){
		m_SpawnUFOList.emplace_back();
		m_SpawnUFOList.back().SetSpawnParameter( s );
		m_SpawnUFOList.back().SetAbductUFOPosition( &m_AbductUFOPosition );
	}
	return true;
}

// �F���l�p�����[�^���X�g�̓ǂݍ���.
bool CAlienManager::ReadAlienParamList()
{
	// �e�F���l�̃p�����[�^�p�X���X�g�̎擾.
	std::vector<std::string> readList = 
		CFileManager::TextLoading( ALIEN_PARAM_LIST_FILE_PATH );
	// �e�L�X�g���ǂݍ��߂ĂȂ�������I��.
	if( readList.empty() == true ) return false;

	for( const auto& l : readList ){
		// �e�F���l�̃p�����[�^�ǂݍ���.
		CAlien::SAlienParam param = {};
		CFileManager::BinaryReading( l.c_str(), param );
		m_AlienParamList.emplace_back( param );
	}
	// �X�|�[��UFO�Ɋe�F���l�̃p�����[�^�ݒ�.
	for( auto& s : m_SpawnUFOList ){
		s.SetAlienParameterList( &m_AlienParamList );
	}

	return true;
}

// �F���l�̃��f���̒l�ł̃\�[�g�֐�.
void CAlienManager::ModelAlphaSort()
{
	m_SortCount++;
	if( m_SortCount < FPS ) return;

	auto comp = []( auto& a, auto b )
	{ return a->GetModelAplha() > b->GetModelAplha();};

	std::sort( m_AilenList.begin(), m_AilenList.end(), comp );
	m_SortCount = 0;
}

// �f�o�b�O�p�̕`��֐�.
void CAlienManager::DebugRender()
{
	const float pos_y = 40.0f;
	const float pos_x = 880.0f;
	CDebugText::SetPosition( { pos_x, pos_y+CDebugText::GetScale()*-1, 0.0f } );
	CDebugText::Render( "- Alien Parameter -" );

	CDebugText::SetPosition( { pos_x, pos_y+CDebugText::GetScale()*1, 0.0f } );
	CDebugText::Render( "AlienCount : ", m_AilenList.size() );
	CDebugText::SetPosition( { pos_x, pos_y+CDebugText::GetScale()*2, 0.0f } );
	CDebugText::Render( "SpawnCount : ", m_SpawnUFOList.size() );

	auto alienA_ADashPramRender = [&]()
	{
		size_t a_index = static_cast<size_t>(EAlienList::A);
		if( m_AlienParamList.size() < a_index ) return;
		CDebugText::SetPosition( { pos_x, pos_y+CDebugText::GetScale()*4, 0.0f } );
		CDebugText::Render( "-- Alien A Param --" );

		CDebugText::SetPosition( { pos_x, pos_y+CDebugText::GetScale()*5, 0.0f } );
		CDebugText::Render( "Life : ", m_AlienParamList[a_index-1].Life );
		CDebugText::SetPosition( { pos_x, pos_y+CDebugText::GetScale()*6, 0.0f } );
		CDebugText::Render( "MoveSpeed : ", m_AlienParamList[a_index-1].MoveSpeed );
		CDebugText::SetPosition( { pos_x, pos_y+CDebugText::GetScale()*7, 0.0f } );
		CDebugText::Render( "RotationalSpeed : ", m_AlienParamList[a_index-1].RotationalSpeed );
		CDebugText::SetPosition( { pos_x, pos_y+CDebugText::GetScale()*8, 0.0f } );
		CDebugText::Render( "ModelAlphaAddValue : ", m_AlienParamList[a_index-1].ModelAlphaAddValue );
	};
	alienA_ADashPramRender();
}