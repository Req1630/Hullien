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
	enum enConfigState
	{
		EConfigState_None,

		EConfigState_Volume,	// ���ʂ̐ݒ�.
		EConfigState_Key,		// �L�[�̑���.

		EConfigState_Max,
	} typedef EConfigState;

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
	int	m_NowConfigState;	// ���݂̐ݒ�̏��.
};

#endif	// #ifndef CONFIG_WIDGET_H.