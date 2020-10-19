#ifndef STG_ENEMY_MANAGER_H
#define STG_ENEMY_MANAGER_H

#include <memory>
#include <vector>
#include <functional>

namespace STG
{
	class CActor;
	class CEnemy;	// �G�N���X.

	class CEnemyManager
	{
	public:
		CEnemyManager();
		~CEnemyManager();

		// �������֐�.
		bool Init();
		// �X�V�֐�.
		void Update( std::function<void(STG::CActor*)> updateProc );
		// �`��֐�.
		void Render();

	private:
		std::vector<std::shared_ptr<CEnemy>> m_Enemys;	// �G���X�g.
	};
};

#endif	// #ifndef STG_ENEMY_MANAGER_H.