#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

class CPlayerManager
{
public:
	CPlayerManager();
	~CPlayerManager();

	// �������֐�.
	bool Init();
	// �X�V�֐�.
	void Update();
	// �`��֐�.
	void Render();
	// UI�̕`��֐�.
	void RenderUI();
};
 
#endif	// #ifndef PLAYER_MANAGER_H.