#ifndef SPAWN_UFO_PARAM_H
#define SPAWN_UFO_PARAM_H

#include "..\..\Global.h"

// �X�|�[��UFO�̏��.
struct stSpawnUFOParam
{
	D3DXVECTOR3 Position;	// ���W.
	int			SpawnTime;	// �X�|�[�����鎞��.

	stSpawnUFOParam()
		: Position	( 0.0f, 0.0f, 0.0f )
		, SpawnTime	( 0 )
	{}

} typedef SSpawnUFOParam;

#endif	// #ifndef SPAWN_UFO_PARAM_H.