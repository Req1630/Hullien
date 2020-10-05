#ifndef EVENT_WIDGET_H
#define EVENT_WIDGET_H

#include "..\Widget.h"

/*************************************
*	�C�x���gUI���N���X.
**/
class CEventWidget : public CWidget
{
public:
	CEventWidget();
	virtual ~CEventWidget();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �X�L�b�v�������ݒ�֐�.
	void SetSkip(const bool& skip) { m_IsSkip = skip; }


private:
	std::shared_ptr<CSprite> m_pSprite;	// �X�v���C�g�N���X.
	bool					 m_IsSkip;	// �X�L�b�v������.
	float					 m_Alpha;	// ���ߒl.
};

#endif	//#ifndef EVENT_WIDGET_H.
