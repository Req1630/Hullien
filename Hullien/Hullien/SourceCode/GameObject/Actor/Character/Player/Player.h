#ifndef PLAYER_H
#define PLAYER_H

#include "..\Character.h"
#include <queue>

class CRotLookAtCenter;
class CCharacterWidget;

class CPlayer : public CCharacter
{
	// �p�����[�^�̃t�@�C���p�X.
	const char* PARAMETER_FILE_PATH = "Data\\GamePram\\Player\\Player.bin";
	const char*	MODEL_NAME	= "Sayaka_s";		// ���f����.

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
	};

	// �v���C���[�p�����[�^.
	struct stPlayerParam : public stParameter
	{
		int			AttackComboMax;		// �U���̍ő吔.
		int			AttackQueueMax;		// �U���L���[�̍ő吔.
		float		AvoidMoveDistance;	// ����̈ړ�����.
		float		AvoidMoveSpeed;		// ���p�̈ړ����x.
		float		CameraMoveSpeed;	// �J�����̈ړ����x.
		float		CameraDistance;		// �J�����̋���.
		float		CameraHeight;		// �J�����̍���.
		D3DXVECTOR3 SphereAdjPos;		// �X�t�B�A�̒������W.
		float		SphereAdjRadius;	// �X�t�B�A�̒������a.

		stPlayerParam()
			: AttackComboMax	( 0 )
			, AttackQueueMax	( 0 )
			, AvoidMoveDistance	( 0.0f )
			, AvoidMoveSpeed	( 0.0f )
			, CameraMoveSpeed	( 0.01f )
			, CameraDistance	( 7.0f )
			, CameraHeight		( 4.0f )
			, SphereAdjPos		( 0.0f, 0.0f, 0.0f )
			, SphereAdjRadius	( 0.0f )
		{}
	} typedef SPlayerParam;

	// �U���p�f�[�^.
	struct stAttackData
	{
		enAnimNo	AnimNo;				// �A�j���[�V�����ԍ�.
		double		Frame;				// �o�߃t���[��.
		double		EnabledEndFrame;	// �L���I���t���[��.
		double		EndFrame;			// �I���t���[��.
		stAttackData()
			: AnimNo			( enAnimNo::None )
			, Frame				( 0.0 )
			, EnabledEndFrame	( 0.0 )
			, EndFrame			( 0.0 )
		{}
	} typedef SAttackData;

public:
	CPlayer();
	virtual ~CPlayer();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;
	// �����蔻��֐�.
	virtual void Collision( CActor* pActor ) override;
	// ������W�̐ݒ�֐�.
	virtual void SetTargetPos( CActor& actor ) override;

private:
	// ����֐�.
	void Controller();
	// �U������֐�.
	void AttackController();
	// ��𑀍�֐�.
	void AvoidController();

	// �ړ��֐�.
	virtual void Move() override;
	// ��𓮍�֐�.
	void AvoidMove();

	// �U���A�j���[�V����.
	void AttackAnimation();
	// �A�j���[�V�����ݒ�.
	void SetAnimation( const enAnimNo& animNo );

	// �U���A�j���[�V�����t���[�����X�g�̐ݒ�.
	void SetAttackFrameList();
	// �U���̒ǉ����ł�����.
	bool IsPushAttack();

	// ���C�t�v�Z�֐�.
	virtual void LifeCalculation( const std::function<void(float&)>& ) override;

	// �����蔻��̐ݒ�.
	bool ColliderSetting();

	// �G�f�B�b�g�p�̕`��֐�.
	void EditRender();
	// �f�o�b�O�p�̕`��.
	void DebugRender();

	// �E�B�W�F�b�g�ݒ�.
	bool WidgetSetting();

private:
	std::shared_ptr<CRotLookAtCenter>				m_pCamera;		// �J�����N���X.
	std::vector<std::shared_ptr<CCharacterWidget>>	m_pWidget;		// Widget�N���X.
	D3DXVECTOR3	m_OldPosition;			// �O��̍��W.
	enAnimNo	m_NowAnimNo;			// ���̃A�j���[�V�����ԍ�.
	enAnimNo	m_OldAnimNo;			// �O�̃A�j���[�V�����ԍ�.
	int			m_AttackComboCount;					// �U���R���{�J�E���g.
	std::vector<double>	m_AttackEnabledFrameList;	// �U���L���t���[���̃��X�g.
	std::queue<SAttackData>	m_AttackDataQueue;		// �U���f�[�^�̃L���[.
	bool			m_IsDuringAvoid;	// ��𒆂��ǂ���.
	D3DXVECTOR3		m_AvoidVector;		// ����x�N�g��.
	float			m_AvoidMoveSpeed;	// ���p�ړ����x.
	SPlayerParam	m_Parameter;		// �p�����[�^�[.
};

#endif	// #ifndef PLAYER_H.