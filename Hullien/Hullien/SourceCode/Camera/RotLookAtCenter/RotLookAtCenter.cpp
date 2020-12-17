#include "RotLookAtCenter.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\Utility\MyVector\MyVector.h"

CRotLookAtCenter::CRotLookAtCenter()
	: m_vDegree			{ 0.0f, 0.0f }
	, m_Length			( DEFAULT_LENGTH )
	, m_HorizontalMax	( DEFAULT_HORIZONTAL_MAX )
	, m_HorizontalMin	( DEFAULT_HORIZONTAL_MIN )
	, m_VerticalMax		( DEFAULT_VERTICAL_MAX )
	, m_VerticalMin		( DEFAULT_VERTICAL_MIN )
	, m_RightMoveSpeed	( DEFAULT_MOVE_SPEED )
	, m_LeftMoveSpeed	(-DEFAULT_MOVE_SPEED )
	, m_AddValue			( 0.0f, 0.0f, 0.0f )
{
}

CRotLookAtCenter::CRotLookAtCenter( const bool& isLoad )
	: CRotLookAtCenter	()
{
	// �p�����[�^�[�̓ǂݍ���.
	if( LoadParameter( isLoad ) == false ){}
}

CRotLookAtCenter::~CRotLookAtCenter()
{
}

// �p�����[�^�[�̓ǂݍ���.
bool CRotLookAtCenter::LoadParameter( const bool& isLoad )
{
	if( isLoad == false ) return false;
	SConfigState tmpConfigState;
	if( CFileManager::BinaryReading( CONFIG_FILE_PATH, tmpConfigState ) == false ) return false;
	// �J�����𔻒肷�邩.
	if( tmpConfigState.IsReverse == true ){
		// ���].
		m_RightMoveSpeed	= -tmpConfigState.MoveSpeed;
		m_LeftMoveSpeed		=  tmpConfigState.MoveSpeed;
	} else {
		// �ʏ�.
		m_RightMoveSpeed	=  tmpConfigState.MoveSpeed;
		m_LeftMoveSpeed		= -tmpConfigState.MoveSpeed;
	}
	return true;
}

// �X�V.
void CRotLookAtCenter::Update()
{
}

//-------------------------------.
// �I�u�W�F�N�g���𒍎��ɉ�].
//-------------------------------.
void CRotLookAtCenter::RotationLookAtObject( const D3DXVECTOR3& vLookPos, const float& attenRate )
{
	// �����ʒu���擾.
	m_vLookPosition = vLookPos;

	D3DXVECTOR3 pos;	// �{���̍��W.
	// �J�����ʒu���Z�o.
	pos.x = m_vLookPosition.x + ( sinf(m_vDegree.x) * m_Length );
	pos.y = m_vPosition.y;
	pos.z = m_vLookPosition.z + ( cosf(m_vDegree.x) * m_Length );
	// lerp����. ( out, ���݂̍��W, �{�����B���Ă�����W, �␳�l ).
	D3DXVec3Lerp( &m_vPosition, &m_vPosition, &pos, attenRate );
	m_vPosition += m_AddValue;
	m_AddValue = { 0.0f, 0.0f, 0.0f };
}

//-------------------------------.
// �������̉�].
//-------------------------------.
void CRotLookAtCenter::DegreeHorizontalMove( const float& movePower )
{
	// �������ɑ������킹��.
	m_vDegree.x += movePower;

	// �I�[�o�[�t���[�΍�.
	CVector::OverflowDegree( m_vDegree.x, 
		 m_HorizontalMax, m_HorizontalMin, 
		-m_HorizontalMax, m_HorizontalMin );
}

//-------------------------------.
// �������̉E��].
//-------------------------------.
void CRotLookAtCenter::DegreeHorizontalRightMove()
{
	DegreeHorizontalMove( m_RightMoveSpeed );
}

//-------------------------------.
// �������̍���].
//-------------------------------.
void CRotLookAtCenter::DegreeHorizontalLeftMove()
{
	DegreeHorizontalMove( m_LeftMoveSpeed );
}

//-------------------------------.
// �c�����̉�].
//-------------------------------.
void CRotLookAtCenter::DegreeVerticalMove( const float& movePower )
{
	// �c�����ɑ������킹��.
	m_vDegree.y += movePower;

	// �I�[�o�[�t���[�΍�.
	CVector::OverflowDegree( m_vDegree.y, 
		m_VerticalMax, m_VerticalMax, 
		m_VerticalMin, m_VerticalMin );
}

//-------------------------------.
// �J�����ƒ����_�̒����ݒ�.
//-------------------------------.
void CRotLookAtCenter::SetLength( const float& length )
{
	m_Length = length;
}

//-------------------------------.
// �����̐ݒ�.
//-------------------------------.
void CRotLookAtCenter::SetHeight( const float& height )
{
	m_vPosition.y = height;
}

//-------------------------------.
// �������̍ő�A�ŏ��̐ݒ�.
//-------------------------------.
void CRotLookAtCenter::SetHorizontalDegree( const float& max, const float& min )
{
	m_HorizontalMax	= max;
	m_HorizontalMin	= min;
}

//-------------------------------.
// �c�����̍ő�A�ŏ��̐ݒ�.
//-------------------------------.
void CRotLookAtCenter::SetVerticalDegree( const float& max, const float& min )
{	
	m_VerticalMax = max;
	m_VerticalMin = min;
}

//-------------------------------.
// ���W�ɑ������킹��l�̐ݒ�.
//-------------------------------.
void CRotLookAtCenter::SetAddPositionValue( const D3DXVECTOR3& addValue )
{
	m_AddValue = addValue;
}