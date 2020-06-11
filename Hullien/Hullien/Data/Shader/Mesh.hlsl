//��۰��ٕϐ�.

//ø����� ڼ޽� t(n).
Texture2D		g_Texture	: register(t0);
//����ׂ� ڼ޽� s(n).
SamplerState	g_SamLinear	: register(s0);

//�ݽ����ޯ̧.
cbuffer per_mesh : register(b0)
{
	matrix	g_mW;			//ܰ��ލs��.
	matrix	g_mWVP;			//ܰ���,�ޭ�,��ۼު���݂̍����s��.
};
//��رْP��.
cbuffer per_material : register(b1)
{
	float4	g_vAmbient;		//���޴�ĐF(���F).
	float4	g_vDiffuse;		//�ި̭��ސF(�g�U���ːF).
	float4	g_vSpecular;	//��߷�אF(���ʔ��ːF).
};
//�ڰђP��.
cbuffer per_frame : register(b2)
{
    float4 g_vCamPos; //��׈ʒu.
    float4 g_vLightPos; //ײĈʒu.
    float4 g_vLightDir; //ײĕ���.
    matrix g_mLightRot; //ײĉ�]�s��.
    float4 g_fIntensity; //ײċ��x(���邳). ��x�̂ݎg�p����.
    float4 g_Color; // �F.
};

//���_����ނ̏o�����Ұ�.
struct VS_OUTPUT
{
	float4	Pos			: SV_Position;
	float4	Color		: COLOR;
	float3	Light		: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	EyeVector	: TEXCOORD2;
	float2	Tex			: TEXCOORD3;
};

//-------------------------------------------------
//	���_(�ްï��)�����.
//-------------------------------------------------
VS_OUTPUT VS_Main(float4 Pos : POSITION,
				  float4 Norm: NORMAL,
				  float2 Tex : TEXCOORD)
{
    VS_OUTPUT Out = (VS_OUTPUT) 0;

	//��ۼު���ݕϊ�(ܰ��ށ��ޭ�����ۼު����).
    Out.Pos = mul(Pos, g_mWVP);

	//�@�������ق̎p���ɍ��킹��.
	// (���ق���]����Ζ@������]������K�v�����邽��).
    Out.Normal = mul(Norm.xyz, (float3x3) g_mW);

	//ײĕ���:
	// �ިڸ����ײẮA�ǂ��ł���������.�ʒu�͖��֌W.
    Out.Light = normalize(g_vLightDir).xyz;

    float3 PosWorld = mul(Pos, g_mW).xyz;

	//�����޸��:
	// ܰ��ދ�ԏ�ł̒��_���璸�_�֌������޸��.
    Out.EyeVector = normalize(g_vCamPos.xyz - PosWorld);

	//ø������W.
    Out.Tex = Tex;

    return Out;
}

//*************************************************
//	׽�ײ��(���ؑ��Őݒ肷�鍀�ځB�����ł͐G��Ȃ�).
//*************************************************

//-------------------------------------------------
//	�߸�ټ����.
//-------------------------------------------------
float4 PS_Main( VS_OUTPUT In ) : SV_Target
{
	//�����@�@.
    float4 ambient = g_vAmbient;

	//�g�U���ˌ� �A.
    float NL = saturate(dot(In.Normal, In.Light));
    float4 diffuse =
		(g_vDiffuse / 2 + g_Texture.Sample(g_SamLinear, In.Tex) / 2) * NL;

	//���ʔ��ˌ� �B.
    float3 reflect = normalize(2 * NL * In.Normal - In.Light);
    float4 specular =
		pow(saturate(dot(reflect, In.EyeVector)), 4) * g_vSpecular;

	//̫����ٍŏI�F�@�@�A�B�̍��v.
    float4 Color = ambient + diffuse + specular;

	//ײċ��x�𔽉f.
    Color *= g_fIntensity.x * g_Color;
    Color.a = g_Color.a;

    return Color;
}

//========= ø��������p ========================================
//-------------------------------------------------
//	���_(�ްï��)�����.
//-------------------------------------------------
VS_OUTPUT VS_NoTex(float4 Pos : POSITION,
				   float4 Norm: NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//�@����ܰ��ދ�Ԃ�.
	Norm.w = 0;	//w=0�ňړ������𔽉f�����Ȃ�.
	output.Normal = mul(Norm, g_mW).xyz;
	output.Pos = mul(Pos, g_mWVP);
	//ײĕ���.
	output.Light = g_vLightDir.xyz;
	//�����޸��.
	float3 PosWorld = mul(Pos, g_mW).xyz;
	output.EyeVector = g_vCamPos.xyz - PosWorld;

	float3 Normal = normalize(output.Normal);
	float3 LightDir = normalize(output.Light);
	float3 ViewDir = normalize(output.EyeVector);
	float NL = saturate(dot(Normal, LightDir));

	float3 Reflect = normalize(2 * NL*Normal - LightDir);
	float4 Specular = pow(saturate(dot(Reflect, ViewDir)), 4);

	output.Color = g_vDiffuse * NL + Specular * g_vSpecular;

	return output;
}

//-------------------------------------------------
//	�߸�ټ����.
//-------------------------------------------------
float4 PS_NoTex(VS_OUTPUT input) : SV_Target
{
	float4 Color = input.Color;
	return Color;
}

//=====================================================================
//	�ȍ~�A����غ�ݗp����.
//=====================================================================
//�ݽ����ޯ̧.
cbuffer Ita_Global : register(b4)	//ڼ޽��𐔕��ǉ�.
{
	matrix g_WVP;	//ܰ���,�ޭ�,��ۼު���݂܂ł̕ϊ��s��.
	float4 g_Color2;	//�F.
	float4 g_UV;	//UV���W.
};

//�\����.
struct VS_ITAOUT
{
	float4	Pos : SV_Position;
	float2	Tex	: TEXCOORD;
};

//���_�����.
VS_ITAOUT VS_Ita(float4 Pos : POSITION,
				 float2 Tex : TEXCOORD)
{
	VS_ITAOUT output = (VS_ITAOUT)0;
	output.Pos = mul(Pos, g_WVP);
	output.Tex = Tex;

	//ø�����UV���W�𑀍삷��.
	output.Tex.x += g_UV.x;

	return output;
}

//�߸�ټ����.
float4 PS_Ita( VS_ITAOUT input ) : SV_Target
{
	return g_Texture.Sample(g_SamLinear, input.Tex);//�F��Ԃ�.
}