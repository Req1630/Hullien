#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

class CPlayerManager
{
public:
	CPlayerManager();
	~CPlayerManager();

	// 初期化関数.
	bool Init();
	// 更新関数.
	void Update();
	// 描画関数.
	void Render();
	// UIの描画関数.
	void RenderUI();
};
 
#endif	// #ifndef PLAYER_MANAGER_H.