#ifndef CONFIG_WIDGET_H
#define CONFIG_WIDGET_H

#include "..\SceneWidget.h"
#include <vector>

class CVolumeConfigWidget;	// ���ʐݒ�UI�N���X.

/********************************************
*	�ݒ�UI�N���X.
**/
class CConfigWidget : public CSceneWidget
{
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
	std::unique_ptr<CVolumeConfigWidget> m_pVolumeConfig;	// ���ʐݒ�UI.
};

#endif	// #ifndef CONFIG_WIDGET_H.