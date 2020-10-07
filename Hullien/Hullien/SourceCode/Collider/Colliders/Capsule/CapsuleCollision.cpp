#include "CapsuleCollision.h"
#include "CapsuleModel.h"


#ifdef _DEBUG
bool CCapsuleCollision::m_IsRender = false;
#endif	// #ifdef _DEBUG.

CCapsuleCollision::CCapsuleCollision()
	: m_Segment			()
	, m_Radius			( 0.0f )
	, m_AdjRadius		( 0.0f )
	, m_Height			( 0.0f )
	, m_AdjHeight		( 0.0f )
#ifdef _DEBUG
	, m_pDebugCapsule	( nullptr )
#endif	// #ifdef _DEBUG.
{
	m_pDebugCapsule = std::make_unique<CCapsuleModel>();
}

CCapsuleCollision::~CCapsuleCollision()
{
}

//----------------------------------.
// ���f���̔��a�̍쐬.
//----------------------------------.
HRESULT CCapsuleCollision::InitModelCapsule( LPD3DXMESH pMesh )
{
	GetBoundingSphere( pMesh, m_Radius );
	D3DXVECTOR3 Max, Min;
	GetBoundingBox( pMesh, Max, Min );

	m_Radius = (m_Radius * (*m_pScale)) + m_AdjRadius;
	m_Height = ((fabsf( Max.y - Min.y ) * (*m_pScale) * 0.5f )) + m_AdjHeight;
	// ���a��0.0f��菬�����Ƃ��߂Ȃ̂ŁA0.0��菭���傫������.
	m_Radius = m_Radius <= 0.0f ? 0.001f : m_Radius;
	// ������蔼�a�̂ق����傫���ƃJ�v�Z�����Ԃ��̂ŁA
	// �����ɔ��a+1.0f�����Ē�������.
	m_Height = m_Radius >= m_Height ? m_Radius + 0.001f : m_Height;
#ifdef _DEBUG
	if( FAILED( m_pDebugCapsule->Init( m_Radius, m_Height ))){
		ERROR_MESSAGE("Model creation failed");
		return E_FAIL;
	}
#endif	// #ifdef _DEBUG.
	return S_OK;
}

//----------------------------------.
// ���f���̔��a�̍쐬.
//----------------------------------.
HRESULT CCapsuleCollision::InitModelCapsule( const float& height, const float& radius )
{
	m_Radius = radius;
	m_Height = height;
	m_Radius = m_Radius <= 0.0f ? 0.001f : m_Radius;
	// ������蔼�a�̂ق����傫���ƃJ�v�Z�����Ԃ��̂ŁA
	// �����ɔ��a+1.0f�����Ē�������.
	m_Height = m_Radius >= m_Height ? m_Radius + 0.001f : m_Height;
#ifdef _DEBUG
	if( FAILED( m_pDebugCapsule->Init( m_Radius, m_Height ))){
		ERROR_MESSAGE("Model creation failed");
		return E_FAIL;
	}
#endif	// #ifdef _DEBUG.
	return S_OK;
}

//----------------------------------.
// �����蔻��̕\��.
//----------------------------------.
void CCapsuleCollision::DebugRender()
{
#ifdef _DEBUG
	if( GetAsyncKeyState(VK_F3) & 0x0001 ) m_IsRender = !m_IsRender;
	if( m_IsRender == false ) return;
	m_pDebugCapsule->SetRotation( GetRotation() );
	m_pDebugCapsule->SetPosition( GetPosition() );
	m_pDebugCapsule->Render();
#endif	// #ifdef _DEBUG.
}

//----------------------------------.
// �F��ς��邩�ǂ���.
//----------------------------------.
void CCapsuleCollision::SetChangeColor( const bool& changed )
{
#ifdef _DEBUG
	m_pDebugCapsule->ChangeColor( changed );
#endif	// #ifdef _DEBUG.
}

//----------------------------------.
// �_�ƒ����̍ŒZ����.
//----------------------------------.
float CCapsuleCollision::calcPointLineDist( const Point& p, const Line& l, Point& h, float& t )
{
	float lenSqV = l.v.lengthSq();
	t = 0.0f;
	if ( lenSqV > 0.0f )
		t = l.v.dot( p - l.p ) / lenSqV;

	h = l.p + t * l.v;
	return ( h - p ).length();
}

//----------------------------------.
// ��p1p2p3�͉s�p�H.
//----------------------------------.
bool CCapsuleCollision::isSharpAngle( const Point& p1, const Point& p2, const Point& p3 )
{
	return Vec3( p1 - p2 ).isSharpAngle( p3 - p2 );
}

//----------------------------------.
// �_�Ɛ����̍ŒZ����.
//----------------------------------.
float CCapsuleCollision::calcPointSegmentDist( const Point& p, const Segment& seg, Point& h, float& t )
{
	const Point e = seg.getEndPoint();

	// �����̒����A�����̑��̍��W�y��t���Z�o
	float len = calcPointLineDist( p, Line( seg.p, e - seg.p ), h, t );

	if ( isSharpAngle( p, seg.p, e ) == false ) {
		// �n�_���̊O��
		h = seg.p;
		return ( seg.p - p ).length();
	}
	else if ( isSharpAngle( p, e, seg.p ) == false ) {
		// �I�_���̊O��
		h = e;
		return ( e - p ).length();
	}

	return len;
}

//----------------------------------.
// 2�����̍ŒZ����.
//----------------------------------.
float CCapsuleCollision::calcLineLineDist( 
	const Line& l1, const Line& l2, 
	Point& p1, Point& p2, 
	float& t1, float& t2 )
{
	// 2���������s�H
	if ( l1.v.isParallel( l2.v ) == true ) {

		// �_P11�ƒ���L2�̍ŒZ�����̖��ɋA��
		float len = calcPointLineDist( l1.p, l2, p2, t2 );
		p1 = l1.p;
		t1 = 0.0f;

		return len;
	}

	// 2�����͂˂���֌W
	float DV1V2 = l1.v.dot( l2.v );
	float DV1V1 = l1.v.lengthSq();
	float DV2V2 = l2.v.lengthSq();
	Vec3 P21P11 = l1.p - l2.p;
	t1 = ( DV1V2 * l2.v.dot( P21P11 ) - DV2V2 * l1.v.dot( P21P11 ) ) / ( DV1V1 * DV2V2 - DV1V2 * DV1V2 );
	p1 = l1.getPoint( t1 );
	t2 = l2.v.dot( p1 - l2.p ) / DV2V2;
	p2 = l2.getPoint( t2 );

	return ( p2 - p1 ).length();
}

float CCapsuleCollision::calcSegmentSegmentDist( 
	const Segment &s1, const Segment &s2, 
	Point &p1, Point &p2, 
	float &t1, float &t2 )
{
	// S1���k�ނ��Ă���H.
	if( s1.v.lengthSq() < _OX_EPSILON_ ){
		// S2���k�ށH.
		if( s2.v.lengthSq() < _OX_EPSILON_ ){
			// �_�Ɠ_�̋����̖��ɋA��.
			float len = ( s2.p - s1.p ).length();
			p1 = s1.p;
			p2 = s2.p;
			t1 = t2 = 0.0f;
			return len;
		} else {
			// S1�̎n�_��S2�̍ŒZ���ɋA��.
			float len = calcPointSegmentDist( s1.p, s2, p2, t2 );
			p1 = s1.p;
			t1 = 0.0f;
			clamp01( t2 );
			return len;
		}
	}

	// S2���k�ނ��Ă���H.
	else if ( s2.v.lengthSq() < _OX_EPSILON_ ) {
		// S2�̎n�_��S1�̍ŒZ���ɋA��.
		float len = calcPointSegmentDist( s2.p, s1, p1, t1 );
		p2 = s2.p;
		clamp01( t1 );
		t2 = 0.0f;
		return len;
	}

	/* �������m */

	// 2���������s�������琂���̒[�_�̈��P1�ɉ�����.
	if ( s1.v.isParallel( s2.v ) == true ) {
		t1 = 0.0f;
		p1 = s1.p;
		float len = calcPointSegmentDist( s1.p, s2, p2, t2 );
		if ( 0.0f <= t2 && t2 <= 1.0f )
			return len;
	}
	else {
		// �����͂˂���̊֌W.
		// 2�����Ԃ̍ŒZ���������߂ĉ���t1,t2�����߂�.
		float len = calcLineLineDist( s1, s2, p1, p2, t1, t2 );
		if (
			0.0f <= t1 && t1 <= 1.0f &&
			0.0f <= t2 && t2 <= 1.0f
			) {
			return len;
		}
	}

	// �����̑����O�ɂ��鎖������.
	// S1����t1��0�`1�̊ԂɃN�����v���Đ������~�낷.
	clamp01( t1 );
	p1 = s1.getPoint( t1 );
	float len = calcPointSegmentDist( p1, s2, p2, t2 );
	if ( 0.0f <= t2 && t2 <= 1.0f )
		return len;

	// S2�����O�������̂�S2�����N�����v�AS1�ɐ������~�낷.
	clamp01( t2 );
	p2 = s2.getPoint( t2 );
	len = calcPointSegmentDist( p2, s1, p1, t1 );
	if ( 0.0f <= t1 && t1 <= 1.0f )
		return len;

	// �o���̒[�_���ŒZ�Ɣ���.
	clamp01( t1 );
	p1 = s1.getPoint( t1 );
	return ( p2 - p1 ).length();
}

Segment CCapsuleCollision::GetSegment() 
{
	D3DXMATRIX mRot;
	D3DXVECTOR3 vRot = *m_pvRotation;
	D3DXMATRIX mYaw, mPitch, mRoll;
	D3DXMatrixRotationX( &mPitch, vRot.x );
	D3DXMatrixRotationY( &mYaw, vRot.y );
	D3DXMatrixRotationZ( &mRoll, vRot.z );
	mRot = mYaw * mPitch * mRoll;

	// ���x�N�g����p��.
	D3DXVECTOR3 vAxis = { 0.0, m_Height/2.0f, 0.0f };
	D3DXVECTOR3 vecAxisY;
	// Y�x�N�g�����̂��̂����݂̉�]���ɂ��ϊ�����.
	D3DXVec3TransformCoord( &vecAxisY, &vAxis, &mRot );

	D3DXVECTOR3 startPos = *m_pvPosition - vecAxisY * 1.0f;
	D3DXVECTOR3 endPos = *m_pvPosition + vecAxisY * 1.0f;

	m_Segment = Segment(
		Point( startPos.x, startPos.y, startPos.z ),	// �n�_.
		Point( endPos.x, endPos.y, endPos.z ) );		// �I�_.

	return m_Segment;
}