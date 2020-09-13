//-------------------------------------------------.
// �O���[�o���ϐ�.
//-------------------------------------------------.
// �e�N�X�`��.
Texture2D		g_Texture	: register(t0);	// ���f���̃e�N�X�`��.
Texture2D		g_ToonMap	: register(t1);	// toon�V�F�[�_�[�p�̃e�N�X�`��.
// �T���v��.
SamplerState	g_SamLinear	: register(s0);

//-------------------------------------------------.
// �R���X�^���g�o�b�t�@.
//-------------------------------------------------.
// ���b�V���P��.
cbuffer per_mesh : register(b0)
{
	matrix	g_mW;	// ���[���h�s��.
	matrix	g_mWVP;	// ���[���h,�r���[,�v���W�F�N�V�����̍����s��.
};
// �}�e���A���P��.
cbuffer per_material : register(b1)
{
	float4	g_vAmbient;		// �A���r�G���g�F(���F).
	float4	g_vDiffuse;		// �f�B�q���[�Y�F(�g�U���ːF).
	float4	g_vSpecular;	// �X�y�L�����F(���ʔ��ːF).
};
// �t���[���P��.
cbuffer per_frame : register(b2)
{
    float4 g_vCamPos;		// �J�����ʒu.
    float4 g_vLightPos;		// ���C�g�ʒu.
    float4 g_vLightDir;		// ���C�g����.
    matrix g_mLightRot;		// ���C�g��]�s��.
    float4 g_fIntensity;	// ���C�g���x(���邳). ��x�̂ݎg�p����.
    float4 g_Color;			// �F.
};

//-------------------------------------------------.
// �\����.
//-------------------------------------------------.
// ���_�C���v�b�g.
struct VS_INPUT
{
	float4 Pos : POSITION;	// ���W.
	float4 Norm: NORMAL;	// �@��.
	float2 Tex : TEXCOORD;	// �e�N�X�`�����W.
};
// ���_�A�E�g�v�b�g.
struct VS_OUTPUT
{
	float4	Pos			: SV_Position;	// WVP�ł̍��W.
	float4	PosW		: Position;		// W�ł̍��W.
	float3	LightDir	: TEXCOORD0;	// ���C�g����.
	float3	Normal		: TEXCOORD1;	// �@��.
	float3	EyeVector	: TEXCOORD2;	// ���_�x�N�g��.
	float2	Tex			: TEXCOORD3;	// �e�N�X�`�����W.
};

//-------------------------------------------------.
//	���_�V�F�[�_�[.
//-------------------------------------------------.
VS_OUTPUT VS_Main( VS_INPUT input )
{
    VS_OUTPUT Out = (VS_OUTPUT) 0;

    Out.Pos		= mul( input.Pos, g_mWVP );	// WVP���W.
	Out.PosW	= mul( input.Pos, g_mW );	// world���W.
    Out.Normal	= mul( input.Norm.xyz, (float3x3) g_mW );		// �@��.
    Out.LightDir	= normalize( g_vLightDir ).xyz;				// ���C�g����.
    Out.EyeVector	= normalize( g_vCamPos - Out.PosW ).xyz;	// ���_�x�N�g��.
    Out.Tex			= input.Tex;	//�@�e�N�X�`�����W.

    return Out;
}

//-------------------------------------------------.
//	�s�N�Z���V�F�[�_�[.
//-------------------------------------------------.
float4 PS_Main( VS_OUTPUT input ) : SV_Target
{
	// ���f���̃e�N�X�`���F���擾.
    float4 color = g_Texture.Sample( g_SamLinear, input.Tex );
	
	//-----�g�D�[������------.
	// �n�[�t�����o�[�g�g�U�Ɩ��ɂ�郉�C�e�B���O�v�Z
    float p = dot( input.Normal, input.LightDir );
	p = p * 0.5f + 0.5f;
	p = p * p;
	// �v�Z���ʂ��g�D�[���V�F�[�_�[�p�̃e�N�X�`������F���t�F�b�`����
	float4 toonColor = g_ToonMap.Sample( g_SamLinear, float2( p, 0.0f ) );
    color *= toonColor * g_fIntensity.x;
	
	//-----�����t�H�O����------.
	const float4 fogColor = float4( 0.5f, 0.5f, 0.5f, 1.0f );
	const float4 fogTColor = float4( 0.5f, 0.5f, 0.5f, 1.0f );
	const float minHeight = -5.0f;
	const float maxHeight = 20.0f;
	float alpha = clamp((input.PosW.y - minHeight) / (maxHeight - minHeight), 0.0f, 1.0f );
	float4 alphas = 1.0f - ( 1.0f - alpha ) * fogTColor;

	color = color * alphas + fogColor * ( 1.0f - alpha );
	color *= g_Color;
	
	return color;
}

//========= �e�N�X�`�������p ========================================
//-------------------------------------------------.
//	���_�V�F�[�_�[.
//-------------------------------------------------.
VS_OUTPUT VS_NoTex(float4 Pos : POSITION,
				   float4 Norm: NORMAL)
{
	VS_OUTPUT Out = (VS_OUTPUT) 0;

    Out.Pos		= mul( Pos, g_mWVP );	// WVP���W.
	Out.PosW	= mul( Pos, g_mW );		// world���W.
    Out.Normal	= mul( Norm.xyz, (float3x3) g_mW );				// �@��.
    Out.LightDir	= normalize( g_vLightDir ).xyz;				// ���C�g����.
    Out.EyeVector	= normalize( g_vCamPos - Out.PosW ).xyz;	// ���_�x�N�g��.

    return Out;
}

//-------------------------------------------------.
//	�s�N�Z���V�F�[�_�[.
//-------------------------------------------------.
float4 PS_NoTex(VS_OUTPUT input) : SV_Target
{
	// ���f���̃e�N�X�`���F���擾.
    float4 color = float4( 1.0f, 1.0f, 1.0f, 1.0f );
	
	//-----�g�D�[������------.
	// �n�[�t�����o�[�g�g�U�Ɩ��ɂ�郉�C�e�B���O�v�Z
    float p = dot( input.Normal, input.LightDir );
	p = p * 0.5f + 0.5f;
	p = p * p;
	// �v�Z���ʂ��g�D�[���V�F�[�_�[�p�̃e�N�X�`������F���t�F�b�`����
	float4 toonColor = g_ToonMap.Sample( g_SamLinear, float2( p, 0.0f ) );
    color *= toonColor * g_fIntensity.x;
	
	//-----�����t�H�O����------.
	const float4 fogColor = float4( 0.5f, 0.5f, 0.5f, 1.0f );
	const float4 fogTColor = float4( 0.5f, 0.5f, 0.5f, 1.0f );
	const float minHeight = -5.0f;
	const float maxHeight = 20.0f;
	float alpha = clamp((input.PosW.y - minHeight) / (maxHeight - minHeight), 0.0f, 1.0f );
	float4 alphas = 1.0f - ( 1.0f - alpha ) * fogTColor;

	color = color * alphas + fogColor * ( 1.0f - alpha );
	color *= g_Color;
	
	return color;
}