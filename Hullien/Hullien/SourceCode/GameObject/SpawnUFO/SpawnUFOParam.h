#ifndef SPAWN_UFO_PARAM_H
#define SPAWN_UFO_PARAM_H

#include "..\..\Global.h"

// �X�|�[��UFO�̏��.
struct stSpawnUFOParam
{
	D3DXVECTOR3 Position;				// ���W.
	int			SpawnTime;				// �X�|�[�����鎞��.
	int			MaxAlienCount;			// �F���l�̍ő吔.
	int			AlienCWaveTime;			// �F���lC�̃E�F�[�u����.
	int			AlienCWaveIntervalTime;	// �F���lC�̃E�F�[�u�̊Ԋu����.
	int			AlienDWaveTime;			// �F���lD�̃E�F�[�u����.
	int			AlienDWaveIntervalTime;	// �F���lD�̃E�F�[�u�̊Ԋu����.

	stSpawnUFOParam()
		: Position					( 0.0f, 0.0f, 0.0f )
		, SpawnTime					( 0 )
		, MaxAlienCount				( 0 )
		, AlienCWaveTime			( 0 )
		, AlienCWaveIntervalTime	( 0 )
		, AlienDWaveTime			( 0 )
		, AlienDWaveIntervalTime	( 0 )
	{}

} typedef SSpawnUFOParam;

#endif	// #ifndef SPAWN_UFO_PARAM_H.