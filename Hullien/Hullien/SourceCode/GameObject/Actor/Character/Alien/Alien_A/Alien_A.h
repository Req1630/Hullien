#ifndef ALIEN_A_H
#define ALIEN_A_H

#include "..\Alien.h"

class CAlienA : public CAlien
{
	const char* MODEL_NAME = "GhostB_s";

public:
	CAlienA();
	virtual ~CAlienA();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

private:
	// 移動関数.
	virtual void Move() override;
	
};

#endif	// #ifndef ALIEN_A_H.