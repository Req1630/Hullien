#ifndef SPAWN_UFO_H
#define SPAWN_UFO_H

#include "..\GameObject.h"
#include "..\Actor\Character\Alien\Alien.h"

#include "SpawnUFOParam.h"

#include <vector>

class CSpawnUFO : public CGameObject
{
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
	void SetSpawnParameter( const SSpawnUFOParam& param ){ m_SpawnParameter = param; }

private:
	// �X�V�֐�.�O���Ŏg�p���Ȃ��̂ŉB��.
	virtual void Update() override;

	// �G�̍쐬.
	std::shared_ptr<CAlien> AlienFactory();

private:
	SSpawnUFOParam m_SpawnParameter;	// �X�|�[���p�����[�^.
	std::vector<CAlien::SAlienParam>* m_pAlienParamList;	// �F���l�p�����[�^���X�g.
	int	m_FrameCount;	// �o�߃t���[���J�E���g.
	int m_AlienIndex;	// �F���l.
};

#endif	// #ifndef SPAWN_UFO_H.