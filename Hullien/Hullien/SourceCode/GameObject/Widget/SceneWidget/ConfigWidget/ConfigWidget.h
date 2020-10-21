#ifndef CONFIG_WIDGET_H
#define CONFIG_WIDGET_H

#include "..\SceneWidget.h"
#include <vector>

class CVolumeBar;

/********************************************
*	�ݒ�UI�N���X.
**/
class CConfigWidget : public CSceneWidget
{
	const char* SPRITE_MASTER_NAME = "MasterVolume";
	const char* SPRITE_BGM_NAME = "BGMVolume";
	const char* SPRITE_SE_NAME = "SEVolume";
	const char* SPRITE_CURSOR_NAME = "VolumeCursor";

	// �X�v���C�g�ԍ�.
	enum enSpriteNo
	{
		ESpriteNo_None = -1,

		ESpriteNo_Master,
		ESpriteNo_BGM,
		ESpriteNo_SE,

		ESpriteNo_Max,
	} typedef ESpriteNo;

public:
	CConfigWidget();
	virtual ~CConfigWidget();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

private:
	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CVolumeBar>>	m_pVolumeBars;
	std::vector<std::shared_ptr<CSprite>>		m_pSprites;
	std::shared_ptr<CSprite>					m_pCursor;
	D3DXVECTOR3 m_CursorPosition;
	int	m_NowSelectVolume;
};

#endif	// #ifndef CONFIG_WIDGET_H.