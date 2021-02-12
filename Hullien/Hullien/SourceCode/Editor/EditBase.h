/**
* @file EditBase.h.
* @brief エディット基底クラス.
* @author 福田玲也.
*/
#ifndef EDIT_BASE_H
#define EDIT_BASE_H

#include "..\Utility\ImGuiManager\ImGuiManager.h"
#include <vector>

/***************************************
*	エディット基底クラス.
**/
class CEditBase
{
protected:
	inline static const ImVec2 WINDOW_SIZE		= { 510.0f, WND_H-70.0f };	// ウィンドウサイズ.
	inline static const ImVec2 RENDER_POSITION	= { 0.0f, 70.0f };		// 描画座標.

public:
	CEditBase();
	virtual ~CEditBase();

	// 初期化関数.
 	virtual bool Init() = 0;
	// 更新関数.
	virtual void Update() = 0;
	// 描画関数.
	virtual void Render() = 0;
	// モデルの描画.
	virtual void ModelRender() = 0;
	// エフェクトの描画.
	virtual void EffectRender() = 0;

	// カメラをセットしているかどうか.
	inline bool IsSetCamera(){ return m_IsSetCamera; }

protected:
	bool m_IsSetCamera;
};

#endif	// #ifndef EDIT_BASE_H.