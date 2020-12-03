#ifndef STG_ENEMY_FACTORY_H
#define STG_ENEMY_FACTORY_H

#include <memory>
#include <vector>

namespace STG
{
	class CEnemy;
	struct stBulletManagerParam typedef SBulletManagerParam;
}

namespace STG
{
	class CEnemyFactory
	{
		const char* ENEMY_LIST_FILE_PATH	= "Data\\GameParam\\STG\\EnemyData.csv";	// �t�@�C����.
		const char* BULLET_LIST_FILE_PATH	= "Data\\GameParam\\STG\\BulletData.csv";	// �t�@�C����.
		const float POSITION_HEIGHT_ADD_VALUE = 0.0001f;				// ���������p�l.
	public:
		CEnemyFactory();
		~CEnemyFactory();

		// �G�̍쐬.
		bool Create( std::vector<std::shared_ptr<STG::CEnemy>>& enemys );

	private:
		// �e���X�g�̍쐬.
		bool CreateBulletList();
		// �G���X�g�̍쐬.
		bool CreateEnemyList( std::vector<std::shared_ptr<STG::CEnemy>>& enemys );

	private:
		std::vector<SBulletManagerParam>	m_BulletParams;
	};
}

#endif	// #ifndef STG_ENEMY_FACTORY_H.