#ifndef EVENT_PLAYER_H
#define EVENT_PLAYER_H

#include "..\EventCharacter.h"
#include <vector>

class CEffectManager;

/********************************
*	�C�x���g�p�v���C���[�N���X.
**/
class CEventPlayer : public CEventCharacter
{
	// �p�����[�^�̃t�@�C���p�X.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Player\\Player.bin";
	const char*	MODEL_NAME = "kaito_s";			// ���f����.
	const char* MODEL_TEMP_NAME = "kaito";		// �����f����.

		// ���̃{�[����.
	const char* RIGHT_FOOT = "kaito_rifa_2_R_asi_4";
	const char* LEFT_FOOT = "kaito_rifa_2_L_asi_4";

public:
	// �A�j���[�V�����ԍ�.
	enum class enAnimNo
	{
		None = -1,

		Wait,		// �ҋ@.
		Walk,		// ����.
		Attack1,	// �U��1.
		Attack2,	// �U��2.
		Attack3 = Walk,	// �U��3.

		Max = Attack2,
	} typedef EAnimNo;

public:
	enum class enPlayerState
	{
		None = 0,

		Move,				//�ړ�.
		Wait,				//�ҋ@.
		SpecialAbility,	    //����\��.
	} typedef EPlayerState;

public:
	CEventPlayer();
	virtual ~CEventPlayer();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��֐�.
	virtual void Collision(CActor* pActor) override;
	// ������W�̐ݒ�֐�.
	virtual void SetTargetPos(CActor& actor) override;
	// �A�j���[�V�����ݒ�.
	void SetAnimation(const EAnimNo& animNo);

	// ����\�͂��g���Ă��邩.
	bool IsSpecialAbility();

private:
	// ����\�͑���֐�.
	void SPController();
	// �ړ��֐�.
	virtual void Move() override;
	// �G�t�F�N�g�`��֐�.
	void EffectRender();
	// �����蔻��̐ݒ�.
	bool ColliderSetting();
	// �G�t�F�N�g�̐ݒ�.
	bool EffectSetting();


private:
	std::shared_ptr<CCollisionManager>	m_pAttackCollManager;	// �U���p�̓����蔻��.
	EAnimNo										m_NowAnimNo;			// ���̃A�j���[�V�����ԍ�.
	EAnimNo										m_OldAnimNo;				// �O�̃A�j���[�V�����ԍ�.
	D3DXVECTOR3								m_AttackPosition;
	EPlayerState									m_State;						// ���.
	std::vector<std::shared_ptr<CEffectManager>> m_pEffects;		// �G�t�F�N�g.
	float			m_SpecialAbility;			// ����\��.
	bool			m_HasUsableSP;			// ����\�͂��g���邩.
	bool			m_IsAttackSE;				// �U��SE��炷��.

};



#endif // #ifndef EVENT_PLAYER_H.
