#include "CSprite.h"
#include "..\..\Camera\CameraManager\CameraManager.h"
#include <crtdbg.h>
#include <algorithm>

#define STR(ver) #ver

// �R���X�g���N�^.
CSprite::CSprite()
	: m_pSpriteShader	( std::make_unique<CSpriteShader>() )
	, m_pVertexLayout	( nullptr )
	, m_pVertexBuffer	( nullptr )
	, m_pVertexBufferUI	( nullptr )
	, m_pSampleLinear	( nullptr )
	, m_pTexture		( nullptr )
	, m_SState			()
	, m_PatternNo		{ 1, 1 }
	, m_PatternMax		{ 1, 1 }
	, m_UV				( 1.0f, 1.0f )
	, m_AnimNumber		( 0 )
	, m_FrameCount		( 0 )
	, m_FrameTime		( 20 )
	, m_IsAnimation		( true )
	, m_enLocalPosition	( enLocalPosition::None )
	, m_VerticesUI		()
{
}

CSprite::CSprite( ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	const char* fileName, const SPRITE_STATE& pSs )
	: CSprite()
{
	Init(  pDevice11, pContext11, fileName,  pSs );
}

// �f�X�g���N�^.
CSprite::~CSprite()
{
	Release();
}

// ������.
HRESULT CSprite::Init( ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	const char* fileName, const SPRITE_STATE& pSs )
{
	m_SState = pSs;
	m_vPos = m_SState.vPos;
	m_enLocalPosition = static_cast<enLocalPosition>(m_SState.LocalPosNum);
	if( FAILED( InitPram( pDevice11, pContext11 )) ){
		return E_FAIL;
	}
	// �V�F�[�_�[�̍쐬.
	if( FAILED( m_pSpriteShader->Init( m_pDevice11, m_pContext11 ))) return E_FAIL;
	// �|���S���쐬.
	if( FAILED( InitModel() ) ){
		return E_FAIL;
	}
	// �e�N�X�`���쐬.
	if( FAILED( CreateTexture( fileName, &m_pTexture ))){
		return E_FAIL;
	}

	return S_OK;
}

// ���.
void CSprite::Release()
{
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pTexture );
	SAFE_RELEASE( m_pVertexBufferUI );
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pVertexLayout );
}

// ���f���쐬.
HRESULT CSprite::InitModel()
{
	float w = m_SState.Disp.w;	// �\���X�v���C�g��,
	float h = m_SState.Disp.h;	// �\���X�v���C�g����,
	float u = m_SState.Stride.w / m_SState.Base.w;	// 1�R�}������̕�,
	float v = m_SState.Stride.h / m_SState.Base.h;	// 1�R�}������̍���.
	m_PatternMax.x = 
		static_cast<LONG>( m_SState.Base.w / m_SState.Stride.w );// x�̍ő�}�X��.
	m_PatternMax.y = 
		static_cast<LONG>( m_SState.Base.h / m_SState.Stride.h );// y�̍ő�}�X��.
																 //-------------------------------.
																 // UI�p.
																 //-------------------------------.
																 // �o�b�t�@�\����.
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;				// �g�p���@(�f�t�H���g).
	bd.ByteWidth			= sizeof(CSpriteShader::VERTEX)*4;	// ���_�̃T�C�Y.
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;			// ���_�o�b�t�@�Ƃ��Ĉ���.
	bd.CPUAccessFlags		= 0;								// CPU����̓A�N�Z�X���Ȃ�.
	bd.MiscFlags			= 0;								// ���̑��̃t���O(���g�p).
	bd.StructureByteStride	= 0;								// �\���̂̃T�C�Y(���g�p).

																// �T�u���\�[�X�\����.
	D3D11_SUBRESOURCE_DATA InitData;
	CreateVERTEX( w, h, u, v );
	InitData.pSysMem = m_VerticesUI;	// �|���̒��_���Z�b�g.

										// ���_�o�b�t�@�̍쐬.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pVertexBufferUI ) ) ){
		_ASSERT_EXPR( false, L"���_�ޯ̧�쐬���s" );
		return E_FAIL;
	}

	// ���_�o�b�t�@���.
	UINT stride = sizeof(CSpriteShader::VERTEX);	// �f�[�^�Ԋu.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pVertexBufferUI, &stride, &offset );

	//-------------------------------.
	// 3D�p.
	//-------------------------------.
	// �摜�̔䗦�����߂�.
	int as = myGcd( (int)m_SState.Disp.w, (int)m_SState.Disp.h );
	w = m_SState.Disp.w / as;
	h = m_SState.Disp.h / as;
	w *= 0.1f;
	h *= 0.1f;
	// �|��(�l�p�`)�̒��_���쐬.
	CSpriteShader::VERTEX vertices[] =
	{
		// �|���S���̒��S�𒸓_�Ƃ���.
		// ���_���W(x,y,z)				 UV���W(u,v)
		D3DXVECTOR3( -w / 2, -h / 2, 0.0f ), D3DXVECTOR2( 0.0f,    v ),	//���_�P(����).
		D3DXVECTOR3( -w / 2,  h / 2, 0.0f ), D3DXVECTOR2( 0.0f, 0.0f ),	//���_�Q(����).
		D3DXVECTOR3(  w / 2, -h / 2, 0.0f ), D3DXVECTOR2(    u,    v ),	//���_�R(�E��).
		D3DXVECTOR3(  w / 2,  h / 2, 0.0f ), D3DXVECTOR2(    u, 0.0f )	//���_�S(�E��).
	};

	// �T�u���\�[�X�\����.
	InitData.pSysMem = vertices;	// �|���̒��_���Z�b�g.

									// ���_�o�b�t�@�̍쐬.
	if( FAILED( m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pVertexBuffer ) ) ){
		_ASSERT_EXPR( false, L"���_�ޯ̧�쐬���s" );
		return E_FAIL;
	}

	// ���_�o�b�t�@���Z�b�g.
	m_pContext11->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );

	InitSample();

	return S_OK;
}

HRESULT CSprite::InitSample()
{
	// �e�N�X�`���p�̃T���v���\����.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory( &samDesc, sizeof( samDesc ) );
	samDesc.Filter		= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU	= D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressV	= D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW	= D3D11_TEXTURE_ADDRESS_WRAP;
	// �T���v���쐬.
	if( FAILED( m_pDevice11->CreateSamplerState(
		&samDesc, &m_pSampleLinear ) ) )
	{
		ERROR_MESSAGE( "SamplerState creation failed" );
		return E_FAIL;
	}

	return S_OK;
}

// �e�N�X�`���쐬.
HRESULT CSprite::CreateTexture( const char* fileName,
	ID3D11ShaderResourceView** pTexture )
{
	//ø����쐬.
	if( FAILED( D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11,	//ؿ�����g�p�������޲����߲��.
		fileName,		//̧�ٖ�.
		nullptr, nullptr,
		pTexture,		//(out)ø���.
		nullptr ) ) ){
		_ASSERT_EXPR( false, L"ø����쐬���s" );
		return E_FAIL;
	}

	return S_OK;
}

//�����ݸޗp.
void CSprite::Render( const bool& isBillboard )
{
	//�@���[���h�s��.
	D3DXMATRIX mWorld;

	// ���[���h�s��쐬.
	mWorld = CreateWorldMatrix();
	if( isBillboard == true ){
		// �r���{�[�h�p.
		D3DXMATRIX CancelRotation = CCameraManager::GetViewMatrix();
		CancelRotation._41 = CancelRotation._42 = CancelRotation._43 = 0.0f; // xyz��0�ɂ���.
																			 // CancelRotation�̋t�s������߂�.
		D3DXMatrixInverse( &CancelRotation, nullptr, &CancelRotation );
		mWorld = CancelRotation * mWorld;
	}
	// WVP�̍쐬.
	D3DXMATRIX mWVP = mWorld * CCameraManager::GetViewMatrix() * CCameraManager::GetProjMatrix();

	AnimUpdate();
	// �R���X�^���g�o�b�t�@�̐ݒ�.
	m_pSpriteShader->SetConstantBufferData( mWVP, m_vColor.w, { m_UV.x, m_UV.y } );

	// 3D�p�̃V�F�[�_�[��ݒ�.
	m_pSpriteShader->ShaderSet( m_pVertexBuffer );

	//���_���߯�ڲ��Ă��.
	m_pContext11->IASetInputLayout( m_pVertexLayout );
	//����è�ށE���ۼް���.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	//ø����𼪰�ނɓn��.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );
	m_pContext11->PSSetShaderResources( 0, 1, &m_pTexture );

	//����è�ނ������ݸ�.
	m_pContext11->Draw( 4, 0 );//�����(���_4��).
}

void CSprite::RenderUI()
{
	// ���W��z�l��������.
	m_vPos.z = 0.0f;
	AnimUpdate();
	// �R���X�^���g�o�b�t�@�̐ݒ�.
	m_pSpriteShader->SetConstantBufferData( 
		CreateWorldMatrix(), m_vColor.w, { m_UV.x, m_UV.y } );

	// UI�p�̃V�F�[�_�[��ݒ�.
	m_pSpriteShader->ShaderUISet( m_pVertexBufferUI );

	//���_���߯�ڲ��Ă��.
	m_pContext11->IASetInputLayout( m_pVertexLayout );
	//����è�ށE���ۼް���.
	m_pContext11->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	//ø����𼪰�ނɓn��.
	m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );
	m_pContext11->PSSetShaderResources( 0, 1, &m_pTexture );

	//����è�ނ������ݸ�.
	m_pContext11->Draw( 4, 0 );//�����(���_4��).
}

D3DXMATRIX CSprite::CreateWorldMatrix()
{
	//�@���[���h�s��, �X�P�[���s��, ��]�s��, ���s�ړ��s��.
	D3DXMATRIX mScale, mRot, mTran;

	// �g��k���s��쐬.
//	D3DXMatrixScaling( &mScale, m_vScale.x, m_vScale.y, 1.0f );
	// ��]�s����쐬.
	D3DXMatrixRotationYawPitchRoll( &mRot, m_vRot.x, m_vRot.y, m_vRot.z );
	// ���s�ړ��s��.
	D3DXMatrixTranslation( &mTran, m_vPos.x, m_vPos.y, m_vPos.z );

	// ���[���h�s��쐬.
	return mScale * mRot * mTran;
}

// �A�j���[�V�����ԍ��̐ݒ�.
void CSprite::SetAnimNumber( const int& animNumber )
{
	if( m_AnimNumber >= m_SState.AnimNum ) return;
	m_IsAnimation = false;	// �A�j���[�V���������Ȃ�.
							// �A�j���[�V�����ԍ����Z�o.
	m_PatternNo.x = animNumber % m_PatternMax.x;
	m_PatternNo.y = animNumber / m_PatternMax.x;
	// UV���W�ɕϊ�.
	m_UV = ConvertIntoUV();
}

// UV���W�ɕϊ�.
D3DXVECTOR2 CSprite::ConvertIntoUV()
{
	return {
		// x���W.
		static_cast<float>(m_PatternNo.x) / static_cast<float>( m_PatternMax.x ),
		// y���W.
		static_cast<float>(m_PatternNo.y) / static_cast<float>( m_PatternMax.y )
	};
}

// �A�j���[�V�����̍X�V.
void CSprite::AnimUpdate()
{
	if( m_IsAnimation == false ) return;

	if( m_FrameCount % m_FrameTime == 0 ){
		// �A�j���[�V�����ԍ����Z�o.
		m_PatternNo.x = m_AnimNumber % m_PatternMax.x;
		m_PatternNo.y = m_AnimNumber / m_PatternMax.x;
		m_AnimNumber++;	// �A�j���[�V�����ԍ������Z.
		if( m_AnimNumber >= m_SState.AnimNum ){
			// �A�j���[�V�����ԍ����ő�A�j���[�V��������葽�����.
			// ����������.
			m_FrameCount = 0;
			m_AnimNumber = 0;
		}
	}
	m_FrameCount++;
	// UV���W�ɕϊ�.
	m_UV = ConvertIntoUV();
}

void CSprite::CreateVERTEX( const float& w, const float& h, const float& u, const float& v )
{
	switch( m_enLocalPosition )
	{
	case CSprite::enLocalPosition::LeftUp:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(  0.0f,    h, 0.0f ), D3DXVECTOR2( 0.0f,    v ) };	//���_�P(����).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(  0.0f, 0.0f, 0.0f ), D3DXVECTOR2( 0.0f, 0.0f ) };	//���_�Q(����).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(     w,    h, 0.0f ), D3DXVECTOR2(    u,    v ) };	//���_�R(�E��).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(     w, 0.0f, 0.0f ), D3DXVECTOR2(    u, 0.0f ) };	//���_�S(�E��).
		break;
	case CSprite::enLocalPosition::Left:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(  0.0f, -h/2, 0.0f ), D3DXVECTOR2( 0.0f, 0.0f ) };	//���_�P(����).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(  0.0f,  h/2, 0.0f ), D3DXVECTOR2( 0.0f,    v ) };	//���_�Q(����).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(     w, -h/2, 0.0f ), D3DXVECTOR2(    u, 0.0f ) };	//���_�R(�E��).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(     w,  h/2, 0.0f ), D3DXVECTOR2(    u,    v ) };	//���_�S(�E��).
		break;
	case CSprite::enLocalPosition::LeftDown:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(  0.0f,    -h, 0.0f ), D3DXVECTOR2( 0.0f, 0.0f ) };	//���_�P(����).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(  0.0f,  0.0f, 0.0f ), D3DXVECTOR2( 0.0f,    v ) };	//���_�Q(����).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(     w,    -h, 0.0f ), D3DXVECTOR2(    u, 0.0f ) };	//���_�R(�E��).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(     w,  0.0f, 0.0f ), D3DXVECTOR2(    u,    v ) };	//���_�S(�E��).
		break;
	case CSprite::enLocalPosition::Down:
		m_VerticesUI[0] =
		{ D3DXVECTOR3( -w / 2,    -h, 0.0f ), D3DXVECTOR2( 0.0f, 0.0f ) };	//���_�P(����).
		m_VerticesUI[1] =
		{ D3DXVECTOR3( -w / 2,  0.0f, 0.0f ), D3DXVECTOR2( 0.0f,    v ) };	//���_�Q(����).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(  w / 2,    -h, 0.0f ), D3DXVECTOR2(    u, 0.0f ) };	//���_�R(�E��).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(  w / 2,  0.0f, 0.0f ), D3DXVECTOR2(    u,    v ) };	//���_�S(�E��).
		break;
	case CSprite::enLocalPosition::RightDown:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(   -w,   -h, 0.0f ), D3DXVECTOR2( 0.0f, 0.0f ) };	//���_�P(����).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(   -w, 0.0f, 0.0f ), D3DXVECTOR2( 0.0f,    v ) };	//���_�Q(����).
		m_VerticesUI[2] =
		{ D3DXVECTOR3( 0.0f,   -h, 0.0f ), D3DXVECTOR2(    u, 0.0f ) };	//���_�R(�E��).
		m_VerticesUI[3] =
		{ D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXVECTOR2(    u,    v ) };	//���_�S(�E��).
		break;
	case CSprite::enLocalPosition::Right:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(    -w, -h / 2, 0.0f ), D3DXVECTOR2( 0.0f, 0.0f ) };	//���_�P(����).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(    -w,  h / 2, 0.0f ), D3DXVECTOR2( 0.0f,    v ) };	//���_�Q(����).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(  0.0f, -h / 2, 0.0f ), D3DXVECTOR2(    u, 0.0f ) };	//���_�R(�E��).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(  0.0f,  h / 2, 0.0f ), D3DXVECTOR2(    u,    v ) };	//���_�S(�E��).
		break;
	case CSprite::enLocalPosition::RightUp:
		m_VerticesUI[0] =
		{ D3DXVECTOR3(    -w,    h, 0.0f ), D3DXVECTOR2( 0.0f,    v ) };	//���_�P(����).
		m_VerticesUI[1] =
		{ D3DXVECTOR3(    -w, 0.0f, 0.0f ), D3DXVECTOR2( 0.0f, 0.0f ) };	//���_�Q(����).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(  0.0f,    h, 0.0f ), D3DXVECTOR2(    u,    v ) };	//���_�R(�E��).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(  0.0f, 0.0f, 0.0f ), D3DXVECTOR2(    u, 0.0f ) };	//���_�S(�E��).
		break;
	case CSprite::enLocalPosition::Up:
		m_VerticesUI[0] =
		{ D3DXVECTOR3( -w / 2,    h, 0.0f ), D3DXVECTOR2( 0.0f,    v ) };	//���_�P(����).
		m_VerticesUI[1] =
		{ D3DXVECTOR3( -w / 2, 0.0f, 0.0f ), D3DXVECTOR2( 0.0f, 0.0f ) };	//���_�Q(����).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(  w / 2,    h, 0.0f ), D3DXVECTOR2(    u,    v ) };	//���_�R(�E��).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(  w / 2, 0.0f, 0.0f ), D3DXVECTOR2(    u, 0.0f ) };	//���_�S(�E��).
		break;
	case CSprite::enLocalPosition::Center:
		m_VerticesUI[0] =
		{ D3DXVECTOR3( -w / 2, -h / 2, 0.0f ), D3DXVECTOR2( 0.0f, 0.0f ) };	//���_�P(����).
		m_VerticesUI[1] =
		{ D3DXVECTOR3( -w / 2,  h / 2, 0.0f ), D3DXVECTOR2( 0.0f,    v ) };	//���_�Q(����).
		m_VerticesUI[2] =
		{ D3DXVECTOR3(  w / 2, -h / 2, 0.0f ), D3DXVECTOR2(    u, 0.0f ) };	//���_�R(�E��).
		m_VerticesUI[3] =
		{ D3DXVECTOR3(  w / 2,  h / 2, 0.0f ), D3DXVECTOR2(    u,    v ) };	//���_�S(�E��).
		break;
	default:
		break;
	}
}