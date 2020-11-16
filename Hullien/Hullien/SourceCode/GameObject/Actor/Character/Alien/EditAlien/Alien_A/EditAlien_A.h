#ifndef EDIT_ALIEN_A_H
#define EDIT_ALIEN_A_H

#include "..\EditAlien.h"

class CEditAlienA : public CEditAlien
{
	const char* MODEL_NAME = "a_s";
public:
	CEditAlienA();
	virtual	~CEditAlienA();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;
};

#endif	// #ifndef EDIT_ALIEN_A_H.
