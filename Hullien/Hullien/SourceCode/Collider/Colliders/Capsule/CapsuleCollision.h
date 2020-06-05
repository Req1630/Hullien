#ifndef CAPSULE_COLLISION_H
#define CAPSULE_COLLISION_H

#include "..\..\ColliderBase\ColliderBase.h"
#include "..\..\..\Utility\MyVector\MyVector.h"

class CCapsuleModel;

class CCapsuleCollision : public CColliderBase
{
public:
	CCapsuleCollision();
	virtual ~CCapsuleCollision();

	// ���f���̔��a�̍쐬.
	HRESULT InitModelCapsule( LPD3DXMESH pMesh );
	// �����蔻��̕\��.
	virtual void DebugRender() override;

	// �F��ς��邩�ǂ���.
	void SetChangeColor( const bool& changed );

	// ���a�̎擾.
	float GetRadius() const { return m_Radius + m_AdjRadius; }

	// �Z�O�����g�擾�֐�.
	Segment GetSegment();

	// �����p�̋��̔��a�̐ݒ�.
	void SetAdjRadius( const float& radius ){ m_Radius = radius; }
	// �����p�̋��̔��a�̐ݒ�.
	void SetAdjHeight( const float& height ){ m_AdjHeight = height; }

	// 2�����̍ŒZ����.
	// s1 : S1(����1).
	// s2 : S2(����2).
	// p1 : S1���̐����̑��i�߂�l�j.
	// p2 : S2���̐����̑��i�߂�l�j.
	// t1 : S1���̃x�N�g���W���i�߂�l�j.
	// t2 : S2���̃x�N�g���W���i�߂�l�j.
	// �߂�l: �ŒZ����.
	float calcSegmentSegmentDist( 
		const Segment &s1, const Segment &s2, 
		Point &p1, Point &p2, 
		float &t1, float &t2 );

private:
	// �_�ƒ����̍ŒZ����.
	// p : �_.
	// l : ����.
	// h : �_���炨�낵�������̑�(�Ԃ�l).
	// t : �x�N�g���W��(�߂�l).
	// �߂�l : �ŒZ����.
	float calcPointLineDist( const Point& p, const Line& l, Point& h, float& t );

	// ��p1p2p3�͉s�p�H.
	bool isSharpAngle( const Point& p1, const Point& p2, const Point& p3 );

	// �_�Ɛ����̍ŒZ����.
	// p : �_.
	// seg : ����.
	// h : �ŒZ�����ƂȂ�[�_(�߂�l).
	// t : �[�_�ʒu( t < 0 : �n�_�̊O, 0 <= t <= 1 : ������, t > 1 : �����̊O ).
	float calcPointSegmentDist( const Point& p, const Segment& seg, Point& h, float& t ); 

	// 2�����̍ŒZ����.
	// l1 : L1.
	// l2 : L2.
	// p1 : L1���̐����̑�(�߂�l).
	// p2 : L2���̐����̑�(�߂�l).
	// t1 : L1���̃x�N�g���W��(�߂�l).
	// t2 : L2���̃x�N�g���W��(�߂�l).
	// �߂�l : �ŒZ����.
	float calcLineLineDist( 
		const Line& l1, const Line& l2, 
		Point& p1, Point& p2, 
		float& t1, float& t2 );


	// 0�`1�̊ԂɃN�����v.
	void clamp01( float &v ) {
		if ( v < 0.0f )
			v = 0.0f;
		else if ( v > 1.0f )
			v = 1.0f;
	}

private:
	Segment m_Segment;	// �Z�O�����g.
	float m_Radius;		// �J�v�Z���̔��a.
	float m_AdjRadius;	// �����p�̃J�v�Z���̔��a.
	float m_Height;		// �J�v�Z���̍���.
	float m_AdjHeight;	// �����p�̃J�v�Z���̍���.

#ifdef _DEBUG
	std::unique_ptr<CCapsuleModel> m_pDebugCapsule;	// �f�o�b�O�p�̋���.
	static bool m_IsRender;	// �`�悷�邩�ǂ���.
#endif	// #ifdef _DEBUG.
};

#endif	// #ifndef CAPSULE_COLLISION_H.