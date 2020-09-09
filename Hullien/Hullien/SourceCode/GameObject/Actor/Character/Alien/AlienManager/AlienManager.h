#ifndef ALIEN_MANAGER_H
#define ALIEN_MANAGER_H

#include "..\Alien.h"
#include "..\..\..\Explosion\Explosion.h"

#include <vector>
#include <memory>
#include <functional>

class CMotherShipUFO;	// �}�U�[�V�b�vUFO.
class CSpawnUFO;		// �X�|�[��UFO.
class CExplosion;		// �����N���X.

class CAlienManager
{
	const char* SPAWN_PARAM_FILE_PATH = "Data\\GameParam\\Alien\\SpaenParam\\SpaenParam.bin";
	const char* EXPLOSION_PARAM_FILE_PATH = "Data\\GameParam\\Alien\\Explosion\\Explosion.bin";
	const char* ALIEN_PARAM_LIST_FILE_PATH = "Data\\GameParam\\Alien\\AlienParamPathList.txt";
public:
	CAlienManager();
	~CAlienManager();

	// �������֐�.
	bool Init();
	// �X�V�֐�.
	void Update( std::function<void(CActor*)> updateProc );
	// �`��֐�.
	void Render();
	// �X�v���C�g�`��֐�.
	void SpriteRender();
	// ���Ƃ��A�C�e�����X�g�̎擾.
	DropItemList GetDropItemList(){ return m_DropItemList; }
	// ���̎q��A�ꋎ���Ă��邩.
	bool IsGirlAbduct();

private:
	// �X�|�[��.
	void Spawn();

	// �����ł��邩�ǂ����m�F.
	void ExplosionConfirming( CAlien* ailen );
	// ���Ƃ��A�C�e���̐ݒ�.
	void SetDropItemList( CAlien* ailen );
	// �X�|�[��UFO�̏�����.
	bool SpawnUFOInit();
	// �F���l�p�����[�^���X�g�̓ǂݍ���.
	bool ReadAlienParamList();
	// �����p�����[�^�̓ǂݍ���.
	bool ReadExplosionParam();

	// �F���l�̃��f���̒l�ł̃\�[�g�֐�.
	void ModelAlphaSort();

	// �f�o�b�O�p�̕`��֐�.
	void DebugRender();

private:
	std::unique_ptr<CMotherShipUFO>			m_pMotherShipUFO;	// �}�U�[�V�b�vUFO.
	std::vector<std::shared_ptr<CAlien>>	m_AilenList;		// �F���l���X�g.
	std::vector<CSpawnUFO>					m_SpawnUFOList;		// �X�|�[��UFO���X�g.
	std::vector<CAlien::SAlienParam>		m_AlienParamList;	// �F���l�p�����[�^���X�g.
	std::vector<CExplosion>		m_ExplosionList;	// �������X�g.
	CExplosion::SExplosionParam	m_ExplosionParam;	// �����p�����[�^�[.
	DropItemList m_DropItemList;		// ���Ƃ��A�C�e���̃��X�g.
	D3DXVECTOR3	m_AbductUFOPosition;	// �A�ꋎ��UFO�̍��W.
	bool		m_IsAlienAbduct;		// �F���l���A�ꋎ���Ă��邩�ǂ���.
	int			m_SortCount;			// �\�[�g���鎞�̃J�E���g.
	bool		m_IsRisingMotherShip;	// �}�U�[�V�b�v�ɏ����Ă��邩.
};

#endif	// #ifdef ALIEN_MANAGER_H.