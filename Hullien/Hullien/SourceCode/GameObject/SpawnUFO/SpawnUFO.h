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
	const float POS_HEIGHT = 10.0f;			// UFO�̍���.
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
	// �`��t���O�ݒ�֐�.
	void SetDisp(const bool& disp) { m_IsDisp = disp; }

private:
	// �X�V�֐�.�O���Ŏg�p���Ȃ��̂ŉB��.
	virtual void Update() override;
	// �E�F�[�u�J�E���g�̍X�V.
	void WaveCountUpdate(
		int& waveCount, const int& waveTime, 
		int& intervalcount, const int& intervalTime );

	// �F���l�̍쐬.
	std::shared_ptr<CAlien> AlienFactory();
	// �F���l�ԍ��̎擾.
	int GetAlienNo();
	// �F���l�ԍ��̍쐬.
	int CreateAlienNo( const int& min, const int& max, const int& outVal );

	// ���f���̎擾.
	bool GetModel();

private:
	std::shared_ptr<CDX9StaticMesh>	m_pStaticMesh;	// ���b�V��.
	SSpawnUFOParam	m_SpawnParameter;		// �X�|�[���p�����[�^.
	D3DXVECTOR3*	m_pAbductUFOPosition;	// �A�ꋎ��UFO�̍��W.
	std::vector<CAlien::SAlienParam>* m_pAlienParamList;	// �F���l�p�����[�^���X�g.
	int	m_FrameCount;	// �o�߃t���[���J�E���g.
	int m_SpawnCount;	// �X�|�[���J�E���g.
	int m_AlienIndex;	// �F���l.

	int m_AlienCWaveCount;			// �F���lC�̃E�F�[�u�J�E���g.
	int m_AlienCWaveIntervalCount;	// �F���lC�̃E�F�[�u�Ԋu�J�E���g.
	int m_AlienDWaveCount;			// �F���lD�̃E�F�[�u�J�E���g.
	int m_AlienDWaveIntervalCount;	// �F���lD�̃E�F�[�u�Ԋu�J�E���g,

	bool m_IsDisp;		//�`��t���O.

	std::mt19937 m_RandomSeed;	// �����_���V�[�h.
};

#endif	// #ifndef SPAWN_UFO_H.