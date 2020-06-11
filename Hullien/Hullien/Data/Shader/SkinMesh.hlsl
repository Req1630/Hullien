/***************************************************************************************************
*	SkinMeshCode Version 2.00
*	LastUpdate	: 2019/10/09
*	Auther		: an.
**/
//��`.
#define MAX_BONE_MATRICES (255)

//�O���[�o��.
Texture2D		g_Texture	: register( t0 );	//�e�N�X�`���[�� ���W�X�^�[t(n).
SamplerState	g_Sampler	: register( s0 );	//�T���v���[�̓��W�X�^�[s(n).


//�R���X�^���g�o�b�t�@(���b�V������).
cbuffer per_mesh		: register( b0 )
{
	matrix g_mW;		//���[���h�s��.
	matrix g_mWVP;		//���[���h����ˉe�܂ł̕ϊ��s��.
};
//�R���X�^���g�o�b�t�@(�}�e���A������).
cbuffer per_material	: register( b1 )
{
	float4 g_vAmbient	= float4( 0, 0, 0, 0 );	//�A���r�G���g��.
	float4 g_vDiffuse	= float4( 1, 0, 0, 0 );	//�g�U����(�F�j
	float4 g_vSpecular	= float4( 1, 1, 1, 1 );	//���ʔ���.
};
//�R���X�^���g�o�b�t�@(�t���[������).
cbuffer per_frame		: register( b2 )
{
    float4 g_vCamPos; //��׈ʒu.
    float4 g_vLightPos; //ײĈʒu.
    float4 g_vLightDir; //ײĕ���.
    matrix g_mLightRot; //ײĉ�]�s��.
    float4 g_fIntensity; //ײċ��x(���邳). ��x�̂ݎg�p����.
    float4 g_Color;     // �F.
};
//�{�[���̃|�[�Y�s�񂪓���.
cbuffer per_bones		: register( b3 )
{
	matrix g_mConstBoneWorld[MAX_BONE_MATRICES];
};

//�X�L�j���O��̒��_�E�@��������.
struct Skin
{
	float4 Pos;
	float3 Norm;
};
//�o�[�e�b�N�X�o�b�t�@�[�̓���.
struct VSSkinIn
{
	float4 Pos		: POSITION;		//�ʒu.  
	float3 Norm		: NORMAL;		//���_�@��.
	float2 Tex		: TEXCOORD;		//�e�N�X�`���[���W.
	uint4  Bones	: BONE_INDEX;	//�{�[���̃C���f�b�N�X.
	float4 Weights	: BONE_WEIGHT;	//�{�[���̏d��.
};

//�s�N�Z���V�F�[�_�[�̓��́i�o�[�e�b�N�X�o�b�t�@�[�̏o�́j�@
struct VS_OUTPUT
{
	float4	Pos			: SV_Position;
	float4	Color		: COLOR;
	float3	Light		: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	EyeVector	: TEXCOORD2;
	float2	Tex			: TEXCOORD3;
};


//�w�肵���ԍ��̃{�[���̃|�[�Y�s���Ԃ�.
//�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j.
matrix FetchBoneMatrix( uint iBone )
{
	return g_mConstBoneWorld[iBone];
}


//���_���X�L�j���O�i�{�[���ɂ��ړ��j����.
//�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j.
Skin SkinVert( VSSkinIn Input )
{
	Skin Output = (Skin)0;

	float4 Pos = float4(Input.Pos);
	float3 Norm = Input.Norm;
	//�{�[��0.
	uint iBone	=Input.Bones.x;
	float fWeight= Input.Weights.x;
	matrix m	=  FetchBoneMatrix( iBone );
	Output.Pos	+= fWeight * mul( Pos, m );
	Output.Norm	+= fWeight * mul( Norm, (float3x3)m );
	//�{�[��1.
	iBone	= Input.Bones.y;
	fWeight	= Input.Weights.y;
	m		= FetchBoneMatrix( iBone );
	Output.Pos	+= fWeight * mul( Pos, m );
	Output.Norm	+= fWeight * mul( Norm, (float3x3)m );
	//�{�[��2.
	iBone	= Input.Bones.z;
	fWeight	= Input.Weights.z;
	m		= FetchBoneMatrix( iBone );
	Output.Pos	+= fWeight * mul( Pos, m );
	Output.Norm	+= fWeight * mul( Norm, (float3x3)m );
	//�{�[��3.
	iBone	= Input.Bones.w;
	fWeight	= Input.Weights.w;
	m		= FetchBoneMatrix( iBone );
	Output.Pos	+= fWeight * mul( Pos, m );
	Output.Norm	+= fWeight * mul( Norm, (float3x3)m );

	return Output;
}

// �o�[�e�b�N�X�V�F�[�_.
VS_OUTPUT VS_Main(VSSkinIn input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
	Skin vSkinned = SkinVert( input);

	output.Pos	= mul( vSkinned.Pos, g_mWVP );
    output.Normal = normalize(mul(vSkinned.Norm, (float3x3) g_mW));
	output.Tex	= input.Tex;
    output.Light = normalize(g_vLightDir).xyz;
    float3 PosWorld = mul(input.Pos, g_mW).xyz;
    output.EyeVector = normalize(g_vCamPos.xyz - PosWorld);

	return output;
}


// �s�N�Z���V�F�[�_.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	//�����@�@.
    float4 ambient = g_vAmbient;

	//�g�U���ˌ� �A.
    float NL = saturate(dot(input.Normal, input.Light));
    float4 diffuse =
		(g_vDiffuse / 2 + g_Texture.Sample(g_Sampler, input.Tex) / 2) * NL;

	//���ʔ��ˌ� �B.
    float3 reflect = normalize(2 * NL * input.Normal - input.Light);
    float4 specular =
		pow(saturate(dot(reflect, input.EyeVector)), 4) * g_vSpecular;

	//̫����ٍŏI�F�@�@�A�B�̍��v.
    float4 Color = ambient + diffuse + specular;

	//ײċ��x�𔽉f.
    Color *= g_fIntensity.x * g_Color;
    Color.a = g_Color.a;
    
    return Color;
}
