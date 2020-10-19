#ifndef STG_MANAGER_H
#define STG_MANAGER_H

#include <memory>

namespace STG
{
	class CPlayer;			// �v���C���[�N���X.
	class CEnemyManager;	// �G�Ǘ��N���X.
}
class CCamera;

class CSTGManager
{
public:
	CSTGManager();
	~CSTGManager();

	// �������֐�.
	bool Init();
	// �X�V�֐�.
	void Update();
	// �`��֐�.
	void Render();

private:
	std::shared_ptr<CCamera>			m_pCamera;
	std::shared_ptr<STG::CPlayer>		m_pPlayer;			// �v���C���[�N���X.
	std::unique_ptr<STG::CEnemyManager>	m_pEnemyManager;	// �G�Ǘ��N���X.
};

#endif	// #ifndef STG_MANAGER_H.