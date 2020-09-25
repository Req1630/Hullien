#ifndef LASER_BEAM_H
#define LASER_BEAM_H

#include "..\Actor.h"
#include <queue>

class CLaserBeam : public CActor
{
	const float DEFAULT_MOVE_SPEED		= 0.1f;	// �ړ����x.
	const float DEFAULT_PARALYSIS_TIME	= 3.0f;	// ��Ⴢ̎���.
	const char* MODEL_NAME = "Barrier";
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
	void Shot( const D3DXVECTOR3& pos );
	// �p�����[�^�������ɖ߂�.
	void ResetParam();
	// �x�W�F�Ȑ��̐���_�̐ݒ�.
	void SetControlPointList( std::vector<D3DXVECTOR3> pointList );

	// �ړ����x�̐ݒ�.
	void SetMoveSpped( const float& moveSpeed ){ m_MoveSpeed = moveSpeed; }
	// ��჎��Ԃ̐ݒ�.
	void SetParalysisTime( const float& time ){ m_ParalysisTime = time; }

	// �U������.
	bool IsInAttack() const { return m_IsInAttack; }
	// �U�����I���������ǂ���.
	bool IsEndAttack() const { return m_IsEndAttack; }

private:
	// �񎟃x�W�F�Ȑ�.
	void SecondaryBeziercurve();
	// �O���x�W�F�Ȑ�.
	void ThirdBezierCurve();

	// ���f���̎擾.
	bool GetModel();
	// �����蔻��̐ݒ�.
	bool CollisionSetting();

	void CreateVertex();

private:
	std::shared_ptr<CDX9StaticMesh>	m_pStaticMesh;	// ���f��.
	float m_MoveSpeed;
	float m_ParalysisTime;
	D3DXVECTOR3	m_TargetPosition;	// �U���Ώۂ̍��W.
	bool m_IsInAttack;		// �U����.
	bool m_IsEndAttack;		// �U���I��.

	float m_FrameCount;		// �t���[���J�E���g.
	float m_FrameTime;		// �t���[������.
	D3DXVECTOR3 m_InitPosition;	// �������W.
	std::vector<D3DXVECTOR3> m_ControlPointList;	// ������W.

	std::queue<std::pair<D3DXVECTOR3,D3DXVECTOR3>> m_VertexPoint;
	int m_VertexCount;
};

#endif	// #ifndef LASER_BEAM_H.