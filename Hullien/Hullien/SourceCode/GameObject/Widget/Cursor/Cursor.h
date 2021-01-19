#ifndef CURSOR_H
#define  CURSOR_H

#include "..\Widget.h"

/********************************************
*	カーソルクラス.
**/
class CCursor : public CWidget
{
private:
	const char* SPRITE_BLACK_NAME	= "selectBlack1";	//スプライトのファイル名.
	const char* SPRITE_BLACK_NAME2	= "selectBlack2";	//スプライトのファイル名.
	const char* SPRITE_BLUE_NAME	= "selectBlue1";	//スプライトのファイル名.
	const char* SPRITE_BLUE_NAME2	= "selectBlue2";	//スプライトのファイル名.
	const float SCALE_MAX	= 1.0f;			//拡大最大値.
	const float SCALE_SPEED = 0.05f;		//拡大速度.
	const float ACC_SPEED   = 0.0011f;		//加速度.
	const float ADJ_POSITOIN_X_MAIN = 20.0f; 
	const float ADJ_POSITOIN_X_SIDE = -10.0f; 

public:
	CCursor();
	CCursor( const bool& isBlue );
	 virtual ~CCursor();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	//描画関数.
	virtual void Render() override;

	// 選択する横幅のサイズ取得.
	void SetWidth( const float& width )
	{
		m_TargetWidth = width; 
		m_IsSetting = true;
	}

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;
	// スケール動き関数.
	void MoveScale();
	// 拡大関数.
	void IncreaseScale();

private:
	std::shared_ptr<CSprite>	m_pSelectSprite;		//スプライトクラス.
	D3DXVECTOR3	m_SlectPosition;
	D3DXVECTOR3	m_vOldPosition;	//移動前の座標.
	float		m_Acceleration;	//加速値.
	float		m_TargetWidth;
	bool		m_IsSetting;
	bool		m_IsBlueSelect;
};

#endif	//#ifndef CURSOR_H.