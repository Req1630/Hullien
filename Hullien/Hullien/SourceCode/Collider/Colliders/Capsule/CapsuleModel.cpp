#include "CapsuleModel.h"
#include "..\..\..\Camera\Camera.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\Common\Shader\SimpleShader\SimpleShader.h"
#include "..\..\..\Common\D3DX\D3DX11.h"

CCapsuleModel::CCapsuleModel()
	: m_pSimpleShader	( std::make_unique<CSimpleShader>() )
	, m_pVertexBuffer	( nullptr )
	, m_pIndexBuffer	( nullptr )
	, index_num			( 0 )
{
}

CCapsuleModel::~CCapsuleModel()
{
	SAFE_RELEASE( m_pIndexBuffer );
	SAFE_RELEASE( m_pVertexBuffer );

	m_pSimpleShader->Release();
}

//---------------------.
// ������.
//---------------------.
HRESULT CCapsuleModel::Init( const float& fRadius, const float& fHeight )
{
	InitPram( CDirectX11::GetDevice(), CDirectX11::GetContext() );

	// �V�F�[�_�[�̍쐬.
	if( FAILED( m_pSimpleShader->Init( m_pDevice11, m_pContext11 ))) return E_FAIL;
	// ���f���̍쐬.
	if( FAILED( InitModel( fRadius, fHeight ))) return E_FAIL;

	return S_OK;
}

//---------------------.
// �`��.
//---------------------.
void CCapsuleModel::Render()
{
	D3DXMATRIX mWVP;
	// WVP Matrix�̍쐬.
	CreateWVPMatrix( mWVP );

	// ���݂̐F��ݒ�.
	D3DXVECTOR4 color = m_IsColorChanged == false ? MODEL_COLOR : MODEL_CHANGE_COLOR;

	// �R���X�^���g�o�b�t�@�Ƀf�[�^��n��.
	m_pSimpleShader->SetConstantBufferData( mWVP, color );

	// �e��V�F�[�_�̐ݒ�.
	m_pSimpleShader->ShaderSet( m_pVertexBuffer, m_pIndexBuffer );

	// �v���~�e�B�u�g�|���W�[���Z�b�g.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ );

	SetRasterizerState( enRS_STATE::Wire );
	m_pContext11->DrawIndexed( index_num*2, 0, 0 );
	SetRasterizerState( enRS_STATE::None );
}

//---------------------.
// WVP Matrix�̍쐬.
//---------------------.
D3DXMATRIX CCapsuleModel::CreateWVPMatrix( D3DXMATRIX& mWVP )
{
	// ���[���h�s��.
	D3DXMATRIX mWorld;
	D3DXMATRIX mRot, mTran, mScale;

	// ��].
	D3DXMatrixRotationYawPitchRoll( &mRot, m_vRot.y, m_vRot.x, m_vRot.z );
	// ���s�ړ�.
	D3DXMatrixTranslation( &mTran, m_vPos.x, m_vPos.y, m_vPos.z );
	// �g��k���s��쐬.
	D3DXMatrixScaling( &mScale, m_vScale.x, m_vScale.y, m_vScale.z );
	// ���[���h�s��
	mWorld = mScale * mRot * mTran;
	mWVP = mWorld * CCameraManager::GetViewMatrix() * CCameraManager::GetProjMatrix();
	
	return mWVP;
}

//---------------------.
// ���f���̍쐬.
//---------------------.
HRESULT CCapsuleModel::InitModel( const float& fRadius, const float& fHeight )
{
	int divideH = 20, divideV = 20;
	divideH = divideH < 4 ? 4 : divideH;
	divideV = divideV < 4 ? 4 : divideV;
	float radius = fRadius;
	float height = fHeight;

	// �����̂ݗL��.
	if( divideV % 2 != 0 ) divideV++;

	int cnt = 0;

	// =============================
	// ���_���W�쐬
	// =============================
	int vertCount = (divideH * divideV + 2);
	D3DXVECTOR3* vertices = new D3DXVECTOR3[vertCount];

	// ���S�p
	float centerEulerRadianH = 2.0f * 3.14f / (float) divideH;
	float centerEulerRadianV = 2.0f * 3.14f / (float) divideV;

	float offsetHeight = height * 0.5f;

	// �V��
	vertices[cnt++] = D3DXVECTOR3( 0, radius+offsetHeight, 0 );

	// �J�v�Z���㕔
	for( int vv = 0; vv < divideV / 2; vv++ ){
		float vRadian = (float)(vv + 1) * centerEulerRadianV / 2.0f;
		// 1�ӂ̒���
		float tmpLen = fabsf( sinf(vRadian) * radius );
		float y = cosf(vRadian) * radius;

		for( int vh = 0; vh < divideH; vh++ ){
			D3DXVECTOR3 pos = 
			{
				tmpLen * sinf( (float)vh * centerEulerRadianH ),
				y + offsetHeight,
				tmpLen * cosf( (float) vh * centerEulerRadianH )
			};
			// �T�C�Y���f
			vertices[cnt++] = pos;
		}
	}

	// �J�v�Z������
	int offset = divideV / 2;
	for( int vv = 0; vv < divideV / 2; vv++ ){
		float yRadian = (float)(vv + offset) * centerEulerRadianV / 2.0f;
		// 1�ӂ̒���
		float tmpLen = fabsf( sinf(yRadian) * radius );
		float y = cosf(yRadian) * radius;

		for( int vh = 0; vh < divideH; vh++ ){

			D3DXVECTOR3 pos = 
			{
				tmpLen * sinf( (float)vh * centerEulerRadianH ),
				y - offsetHeight,
				tmpLen * cosf( (float)vh * centerEulerRadianH )
			};
			// �T�C�Y���f
			vertices[cnt++] = pos;
		}
	}
	// ���
	vertices[cnt] = D3DXVECTOR3( 0, -radius-offsetHeight, 0 );

	// =============================
	// ���_�C���f�b�N�X�쐬
	// =============================
	int topAndBottomTriCount = divideH * 2;
	// ���ʎO�p�`�̐�
	int aspectTriCount = divideH * (divideV - 2 + 1) * 2;

	int* indices = new int[(topAndBottomTriCount + aspectTriCount) * 3];

	//�V��
	int offsetIndex = 0;
	cnt = 0;
	for (int i = 0; i < divideH * 3; i++)
	{
		if( i % 3 == 0 ){
			indices[cnt++] = 0;
		} else if ( i % 3 == 1 ){
			indices[cnt++] = 1 + offsetIndex;
		} else if ( i % 3 == 2 ){
			int index = 2 + offsetIndex++;
			// �W������
			index = index > divideH ? indices[1] : index;
			indices[cnt++] = index;
		}
	}

	// ����Index
	/* ���_���q���C���[�W
	* 1 - 2
	* |   |
	* 0 - 3
	* 
	* 0, 1, 2
	* 0, 2, 3
	*
	* ���� : 1����������Clamp����̂�Y��Ȃ��悤�ɁB
	*/
	// �J�nIndex�ԍ�
	int startIndex = indices[1];

	// �V�ʁA��ʂ��������J�v�Z��Index�v�f��
	int sideIndexLen = aspectTriCount * 3;

	int lap1stIndex = 0;
	int lap2ndIndex = 0;

	// ��������Ƃ���index��
	int lapDiv = divideH * 2 * 3;

	int createSquareFaceCount = 0;

	for( int i = 0; i < sideIndexLen; i++ ){
		// ����̒��_���𒴂�����X�V(������܂�)
		if( i % lapDiv == 0 ){
			lap1stIndex = startIndex;
			lap2ndIndex = startIndex + divideH;
			createSquareFaceCount++;
		}

		if( i % 6 == 0 || i % 6 == 3 ){
			indices[cnt++] = startIndex;
		} else if ( i % 6 == 1 ){
			indices[cnt++] = startIndex + divideH;
		} else if ( i % 6 == 2 || i % 6 == 4 ){
			if( i > 0 &&
				( i % (lapDiv * createSquareFaceCount - 2) == 0 || 
					i % (lapDiv * createSquareFaceCount - 4) == 0 )){
				// 1�������Ƃ���Clamp����
				// ����|���S���̍Ōォ��2�Ԗڂ�Index
				indices[cnt++] = lap2ndIndex;
			} else {
				indices[cnt++] = startIndex + divideH + 1;
			}
		} else if ( i % 6 == 5 ){
			if(( i > 0 && i % (lapDiv * createSquareFaceCount - 1 )) == 0 ){
				// 1�������Ƃ���Clamp����
				// ����|���S���̍Ō��Index
				indices[cnt++] = lap1stIndex;
			} else {
				indices[cnt++] = startIndex + 1;
			}

			// �J�nIndex�̍X�V
			startIndex++;
		} else {
		}
	}

	// ���Index
	offsetIndex = vertCount - 1 - divideH;
	lap1stIndex = offsetIndex;
	int finalIndex = vertCount - 1;
	int len = divideH * 3;

	for( int i = len - 1; i >= 0; i-- ){
		if( i % 3 == 0 ){
			// ��ʂ̐撸�_
			indices[cnt++] = finalIndex;
			offsetIndex++;
		} else if ( i % 3 == 1 ){
			indices[cnt++] = offsetIndex;
		} else if ( i % 3 == 2 ){
			int value = 1 + offsetIndex;
			if( value >= vertCount - 1 ){
				value = lap1stIndex;
			}
			indices[cnt++] = value;
		}
	}
	
	// �ő�v�f�����Z�o����.
	int vertex_num = vertCount;
	index_num = (topAndBottomTriCount + aspectTriCount) * 3;

	// �o�b�t�@�\����.
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;		// �g�p���@(�f�t�H���g).
	bd.ByteWidth			= sizeof(CSimpleShader::VERTEX)*vertex_num;// ���_�̃T�C�Y.
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;	// ���_�o�b�t�@�Ƃ��Ĉ���.
	bd.CPUAccessFlags		= 0;						// CPU����̓A�N�Z�X���Ȃ�.
	bd.MiscFlags			= 0;						// ���̑��̃t���O(���g�p).
	bd.StructureByteStride	= 0;						// �\���̂̃T�C�Y(���g�p).

	//���ؿ���ް��\����.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;

	//���_�ޯ̧�̍쐬.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pVertexBuffer ) ) ){
		_ASSERT_EXPR( false, L"���_�ޯ̧�쐬���s" );
		return E_FAIL;
	}

	D3D11_BUFFER_DESC id;
	id.Usage				= D3D11_USAGE_DEFAULT;	   // �g�p���@(�f�t�H���g).
	id.ByteWidth			= sizeof(WORD)*index_num*2;  // ���_�̃T�C�Y.
	id.BindFlags			= D3D11_BIND_INDEX_BUFFER; // ���_�o�b�t�@�Ƃ��Ĉ���.
	id.CPUAccessFlags		= 0;					   // CPU����̓A�N�Z�X���Ȃ�.
	id.MiscFlags			= 0;					   // ���̑��̃t���O(���g�p).
	id.StructureByteStride	= 0;					   // �\���̂̃T�C�Y(���g�p).

	D3D11_SUBRESOURCE_DATA IndexData;
	IndexData.pSysMem			= indices;
	IndexData.SysMemPitch		= 0;
	IndexData.SysMemSlicePitch	= 0;

	if( FAILED( m_pDevice11->CreateBuffer(
		&id, &IndexData, &m_pIndexBuffer ))){
		_ASSERT_EXPR( false, L"�C���f�b�N�X�ޯ̧�쐬���s" );
		return E_FAIL;
	}

	return S_OK;
}