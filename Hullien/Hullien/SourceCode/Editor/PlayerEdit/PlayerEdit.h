/**
* @file PlayerEdit.h.
* @brief プレイヤーのパラメータエディタクラス.
* @author 福田玲也.
*/
#ifndef PLAYER_EDIT_H
#define PLAYER_EDIT_H

#include "..\EditBase.h"
#include "..\..\GameObject\Actor\Character\Player\Player.h"

class CEditAlienA;

/**********************************************
*	プレイヤーのパラメータエディタクラス.
*/
class CPlayerEdit : public CEditBase
{
	// パラメータのファイルパス.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Player\\Player.bin";
public:
	CPlayerEdit();
	virtual ~CPlayerEdit();

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
	// パラメータの設定.
	void PlayerParamEdit();
	// アニメーションの引きずり調整の設定.
	void AnimAdjDrggingParamEdit();
	// 攻撃調整の設定.
	void AttackAdjParamEdit();

private:
	std::unique_ptr<CPlayer>			m_pPlayer;
	std::shared_ptr<CEditAlienA>		m_pAlien;
	player::SPlayerParam				m_pPlayerParam;
	player::SAnimDraggingAdjParam		m_AnimAdjDrggingParam;
	player::SAttackAdjParam				m_AttackAdjParam;
};

#endif	// #ifndef PLAYER_EDIT_H.
