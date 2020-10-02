#ifndef SPAWN_UFO_PARAM_H
#define SPAWN_UFO_PARAM_H

#include "..\..\Global.h"

// �X�|�[��UFO�̏��.
struct stSpawnUFOParam
{
	D3DXVECTOR3 Position;				// ���W.
	float		SpawnPointHight;		// �X�|�[���|�C���g�̍���.
	int			SpawnTime;				// �X�|�[�����鎞��.
	int			MaxAlienCount;			// �F���l�̍ő吔.
	int			ProbabilityC;			// �F���lC�̏o����.
	int			ProbabilityD;			// �F���lD�̏o����.
	int			ProbabilityItem;		// �A�C�e���𗎂Ƃ��m��.

	stSpawnUFOParam()
		: Position					( 0.0f, 0.0f, 0.0f )
		, SpawnPointHight			( 0.0f )
		, SpawnTime					( 0 )
		, MaxAlienCount				( 0 )
		, ProbabilityC				( 0 )
		, ProbabilityD				( 0 )
		, ProbabilityItem			( 0 )
	{}

} typedef SSpawnUFOParam;

#endif	// #ifndef SPAWN_UFO_PARAM_H.