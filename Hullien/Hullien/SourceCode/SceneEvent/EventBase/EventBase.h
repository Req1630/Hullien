#ifndef EVENT_BASE_H
#define EVENT_BASE_H

/***********************************
*	�C�x���g�V�[�����N���X.
**/
class CEventBase
{
public:
	CEventBase();
	virtual ~CEventBase();

	// �Ǎ��֐�.
	virtual bool Load() = 0;
	// �X�V�֐�.
	virtual void Update() = 0;
	// �`��֐�.
	virtual void Render() = 0;

private:
};

#endif	//#ifndef SCENE_EVENT_BASE_H.