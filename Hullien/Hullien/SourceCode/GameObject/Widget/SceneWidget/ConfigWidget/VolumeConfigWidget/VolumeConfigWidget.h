#ifndef VOLUME_CONFIG_WIDGET_H
#define VOLUME_CONFIG_WIDGET_H

#include "..\..\..\Widget.h"
#include <vector>

class CSlinder;			// スライダー.
class CConfigCursor;	// カーソル.
class CButtonExp;		// ボタン説明UIクラス.

/********************************************
*	音量設定UIクラス.
**/
class CVolumeConfigWidget : public CWidget
{
	const char* SPRITE_MASTER_NAME	= "MasterVolume";
	const char* SPRITE_BGM_NAME		= "BGMVolume";
	const char* SPRITE_SE_NAME		= "SEVolume";
	const char* SPRITE_MARK_NAME	= "VolumeMark";
	const char* SPRITE_MARK_ICON_NAME	= "volumeMarkIcon";

	const float BAR_POSITION_X = 200.0f;
	const float DEFALUT_VOLUME = 1.0f;
	const float MARK_ICON_POS_X = 10.0f;

	inline static const float INPUT_WAIT_TIME_MAX = 30.0f;	// 入力時の待機フレーム.
	const D3DXVECTOR3 BUTTON_EXP_RENDER_POS = { 900.0f, 650.0f, 0.0f };

	// 設定の状態.
	enum enConfigState
	{
		EConfigState_None,

		EConfigState_Select,	// 選択状態.
		EConfigState_Seting,	// 設定状態.

		EConfigState_Max,
	} typedef EConfigState;

	// 選択の種類.
	enum enSelectType
	{
		ESelectType_None = -1,

		ESelectType_Master,	// マスター.
		ESelectType_BGM,	// BGM.
		ESelectType_SE,		// SE.

		EVolumeType_Mark,
		EVolumeType_MarkIcon,

		EVolumeType_End,

		ESelectType_Max,

		ESelectType_Begin	= ESelectType_Master,	// 始まり.
		ESelectType_End		= EVolumeType_End,		// 終了.
		ESelectType_VolumeMax = ESelectType_SE+1
	} typedef EVolumeType;

	// スプライトの種類.
	enum enSpriteNo
	{
		ESpriteNo_None = -1,

		ESpriteNo_Master,		// マスター.
		ESpriteNo_BGM,			// BGM.
		ESpriteNo_SE,			// SE.

		ESpriteNo_Max,
	} typedef ESpriteNo;

public:
	CVolumeConfigWidget();
	virtual ~CVolumeConfigWidget();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

	// 音量の設定をできるようにする.
	void OnVolumeSeting();
	// 音量の設定をできないようにする.
	void OffVolumeSeting();
	// 保存終了.
	bool IsSaveEnd();

private:
	// 種類の選択.
	void SelectType();
	// 決定.
	void Determination();
	// 音量の設定.
	void VolumeSeting();

	// スプライト設定関数.
	virtual bool SpriteSetting() override;

private:
	std::unique_ptr<CConfigCursor>			m_pCursor;			// カーソルクラス.
	std::unique_ptr<CButtonExp>				m_pButtonExp;		// ボタン説明UI.
	std::vector<std::shared_ptr<CSlinder>>	m_pVolumeSlinders;	// 音量スライダー.
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;			// 画像.
	std::vector<D3DXVECTOR3>				m_SlinderPositions;	// スライダーの座標.
	D3DXVECTOR3	m_CursorPosition;	// カーソルの座標.
	int			m_NowConfigState;	// 現在の設定状態.
	int			m_NowSelectVolume;	// 現在の選択している音量種類.
	int			m_OldSelectVolume;
	float		m_OldVolume;
	float		m_InputWaitTime;	// 入力した際の待機時間.
	bool		m_IsOneStep;		// 一回だけ動作.
};

#endif	// #ifndef VOLUME_CONFIG_WIDGET_H.