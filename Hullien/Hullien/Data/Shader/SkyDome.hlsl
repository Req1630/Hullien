Texture2D g_Texture			: register(t0);
SamplerState g_SamLinear	: register(s0);

// �O���[�o��.
cbuffer global : register(b0)
{
	matrix g_mWVP;
};

struct VS_INPUT
{
	float4 Pos	: POSITION;
	float4 Norm : NORMAL; // �@��.
	float2 Tex	: TEXCOORD; // �e�N�X�`�����W.
};
struct VS_OUTPUT
{
	float4 Pos	: SV_Position;	// WVP�ł̍��W.
	float2 Tex	: TEXCOORD0;	// ���[���h���W.
};

// ���_�V�F�[�_�[.
VS_OUTPUT VS_Main( VS_INPUT input )
{
	VS_OUTPUT Out	= (VS_OUTPUT) 0;
	input.Pos.w		= 1.0f;
	Out.Pos			= mul(input.Pos, g_mWVP);
	Out.Tex			= input.Tex;
	return Out;
}

// �s�N�Z���V�F�[�_�[.
float4 PS_Main( VS_OUTPUT input ) : SV_Target
{
	return g_Texture.Sample(g_SamLinear, input.Tex);
}