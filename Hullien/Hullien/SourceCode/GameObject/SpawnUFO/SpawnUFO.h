#ifndef SPAWN_UFO_H
#define SPAWN_UFO_H

#include "..\GameObject.h"
#include "..\Actor\Character\Alien\AlienList.h"
#include "..\Actor\Character\Alien\Alien.h"

#include "SpawnUFOParam.h"

#include <vector>
#include <random>

class CSpawnUFO : public CGameObject
{
	const char* MODEL_NAME = "ufo_mini";	// ���f����.
public:
	CSpawnUFO();
	virtual ~CSpawnUFO();

	// �������֐�.
	virtual bool Init() override;
	// �`��֐�.
	virtual void Render() override;

	// �F���l���X�|�[��������.
	void SpawnAlien( std::vector<std::shared_ptr<CAlien>>& );
	// �F���l�̃p�����[�^���X�g��ݒ肷��.
	void SetAlienParameterList( std::vector<CAlien::SAlienParam>* );
	// �X�|�[���p�����[�^�̐ݒ�.
	void SetSpawnParameter( const SSpawnUFOParam& param );
	// �A�ꋎ��UFO�̍��W�ݒ�.
	void SetAbductUFOPosition( D3DXVECTOR3* pPos ){ m_pAbductUFOPosition = pPos; }

private:
	// �X�V�֐�.�O���Ŏg�p���Ȃ��̂ŉB��.
	virtual void Update() override;

	// �F���l�̍쐬.
	std::shared_ptr<CAlien> AlienFactory();
	// �F���l�ԍ��̎擾.
	int GetAlienNo();
	// �F���l�ԍ��̍쐬.
	int CreateAlienNo( const int& min, const int& max, const int& outVal );
	// �A�C�e�����擾����.
	EItemList ProbabilityGetItem( const bool& isAlienD );
	// ���f���̎擾.
	bool GetModel();

private:
	std::shared_ptr<CDX9StaticMesh>		m_pStaticMesh;			// ���b�V��.
	SSpawnUFOParam						m_SpawnParameter;		// �X�|�[���p�����[�^.
	D3DXVECTOR3							m_SpawnPoint;			// �X�|�[���|�C���g.
	D3DXVECTOR3*						m_pAbductUFOPosition;	// �A�ꋎ��UFO�̍��W.
	std::vector<CAlien::SAlienParam>*	m_pAlienParamList;		// �F���l�p�����[�^���X�g.
	int	m_FrameCount;	// �o�߃t���[���J�E���g.
	int m_SpawnCount;	// �X�|�[���J�E���g.
	int m_AlienIndex;	// �F���l.

	std::mt19937 m_RandomSeed;	// �����_���V�[�h.
};

#endif	// #ifndef SPAWN_UFO_H.