#ifndef STG_ENEMY_FACTORY_H
#define STG_ENEMY_FACTORY_H

#include <memory>
#include <vector>

namespace STG
{
	class CEnemy;
}

namespace STG
{
	class CEnemyFactory
	{
		const char* FILE_NAME = "Data\\GameParam\\STG\\EnemyData.csv";	// �t�@�C����.
		const float POSITION_HEIGHT_ADD_VALUE = 0.0001f;				// ���������p�l.
	public:
		CEnemyFactory();
		~CEnemyFactory();

		// �G�̍쐬.
		bool Create( std::vector<std::shared_ptr<STG::CEnemy>>& enemys );
	};
}

#endif	// #ifndef STG_ENEMY_FACTORY_H.