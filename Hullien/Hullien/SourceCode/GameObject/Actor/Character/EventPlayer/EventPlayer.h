#ifndef EVENT_PLAYER_H
#define EVENT_PLAYER_H

#include "..\Character.h"

class CEffectManager;

/********************************
*	�C�x���g�p�v���C���[�N���X.
**/
class CEventPlayer : public CCharacter
{
	// �p�����[�^�̃t�@�C���p�X.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Player\\Player.bin";
	const char*	MODEL_NAME = "Kaito_s";	// ���f����.
	const char* MODEL_TEMP_NAME = "kaito";	// �����f����.

	// �A�j���[�V�����ԍ�.
	enum class enAnimNo
	{
		None = -1,

		Dead = 0,
		Wait,
		Walk,
		Wait1,
		Happy,

		Max = Happy,
	} typedef EAnimNo;

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
	// �X�v���C�g�̕`��.
	virtual void SpriteRender() override;
	// �A�j���[�V�����ݒ�.
	void SetAnimation(const EAnimNo& animNo);

	// ����\�͂��g���Ă��邩.
	bool IsSpecialAbility();

private:
	// �U������֐�.
	void AttackController();
	// ����\�͑���֐�.
	void SPController();
	// �ړ��֐�.
	virtual void Move() override;
	// �G�t�F�N�g�`��֐�.
	void EffectRender();
	// �U���̓����蔻��.
	void AttackCollision(CActor* pActor);
	// �����蔻��̐ݒ�.
	bool ColliderSetting();
	// �G�t�F�N�g�̐ݒ�.
	bool EffectSetting();

private:
	std::shared_ptr<CCollisionManager>	m_pAttackCollManager;	// �U���p�̓����蔻��.
	EAnimNo	m_NowAnimNo;			// ���̃A�j���[�V�����ԍ�.
	EAnimNo	m_OldAnimNo;			// �O�̃A�j���[�V�����ԍ�.
	D3DXVECTOR3		m_AttackPosition;
	std::vector<std::shared_ptr<CEffectManager>> m_pEffects;	// �G�t�F�N�g.
	float			m_SpecialAbility;		// ����\��.
	bool			m_HasUsableSP;			// ����\�͂��g���邩.
	bool			m_IsAttackSE;				//�U��SE��炷��.

};



#endif // #ifndef EVENT_PLAYER_H.
