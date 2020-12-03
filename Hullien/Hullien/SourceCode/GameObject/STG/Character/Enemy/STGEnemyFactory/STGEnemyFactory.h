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
		const char* ENEMY_LIST_FILE_PATH	= "Data\\GameParam\\STG\\EnemyData.csv";	// ファイル名.
		const char* BULLET_LIST_FILE_PATH	= "Data\\GameParam\\STG\\BulletData.csv";	// ファイル名.
		const float POSITION_HEIGHT_ADD_VALUE = 0.0001f;				// 高さ調整用値.
	public:
		CEnemyFactory();
		~CEnemyFactory();

		// 敵の作成.
		bool Create( std::vector<std::shared_ptr<STG::CEnemy>>& enemys );

	private:
		// 弾リストの作成.
		bool CreateBulletList();
		// 敵リストの作成.
		bool CreateEnemyList( std::vector<std::shared_ptr<STG::CEnemy>>& enemys );

	private:
		std::vector<SBulletManagerParam>	m_BulletParams;
	};
}

#endif	// #ifndef STG_ENEMY_FACTORY_H.