#ifndef STG_ENEMY_MANAGER_H
#define STG_ENEMY_MANAGER_H

#include <memory>
#include <vector>
#include <functional>

namespace STG
{
	class CActor;			// �K��N���X.
	class CEnemy;			// �G�N���X.
	class CEnemyFactory;	// �G�쐬�N���X.

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
		std::unique_ptr<STG::CEnemyFactory>			m_pEnemyFactory;	// �G�쐬�N���X.
		std::vector<std::shared_ptr<STG::CEnemy>>	m_Enemys;			// �G���X�g.
	};
};

#endif	// #ifndef STG_ENEMY_MANAGER_H.