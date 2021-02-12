/**
* @file ExplosionEdit.h.
* @brief 爆発の当たり判定サイズの編集クラス.
* @author 福田玲也.
*/
#ifndef EXPLOSION_EDIT_H
#define EXPLOSION_EDIT_H

#include "..\EditBase.h"
#include "..\..\GameObject\Actor\Explosion\Explosion.h"
#include <vector>

/******************************************
*	爆発の当たり判定サイズの編集クラス.
**/
class CExplosionEdit : public CEditBase
{
	const char* FILE_PATH = "Data\\GameParam\\Alien\\Explosion\\Explosion.bin";

public:
	CExplosionEdit();
	~CExplosionEdit();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// モデルの描画.
	virtual void ModelRender() override;
	// エフェクトの描画.
	virtual void EffectRender() override;

private:
	// ファイルの読み込み.
	bool FileReading();
	// ファイルの書き込み.
	bool FileWriting();

private:
	std::unique_ptr<CExplosion>	m_pExplosion;		// 爆発.
	CExplosion::SExplosionParam m_ExplosionParam;	// 爆発パラメーター.
};

#endif	// #ifndef EXPLOSION_EDIT_H.