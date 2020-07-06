#ifndef LASER_BEAM_H
#define LASER_BEAM_H

#include "..\Actor.h"

class CLaserBeam : public CActor
{
public:
	CLaserBeam();
	virtual ~CLaserBeam();

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

	// �r�[����ł�.
	void Shot();
	// �p�����[�^�������ɖ߂�.
	void ResetParam();

	// �U������.
	bool IsInAttack() const { return m_IsInAttack; }
	// �U�����I���������ǂ���.
	bool IsEndAttack() const { return m_IsEndAttack; }

private:
	// �����蔻��̐ݒ�.
	bool CollisionSetting();

private:
	D3DXVECTOR3	m_TargetPosition;	// �U���Ώۂ̍��W.
	bool m_IsInAttack;		// �U����.
	bool m_IsEndAttack;		// �U���I��.
};

#endif	// #ifndef LASER_BEAM_H.