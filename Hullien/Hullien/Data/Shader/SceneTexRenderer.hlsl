#include "FXAA.hlsl"

//��۰��ٕϐ�.
//ø����́Aڼ޽� t(n).
Texture2D g_TextureColor	: register(t0);
Texture2D g_TextureNormal	: register(t1);
Texture2D g_TextureDepth	: register(t2);
Texture2D g_TextureTrans	: register(t3);
Texture2D g_TextureLast		: register(t4);
//����ׂ́Aڼ޽� s(n).
SamplerState g_SamLinear : register(s0);

//�ݽ����ޯ̧.
cbuffer cBuffer : register(b0)
{
	matrix g_mW			: packoffset(c0); // ܰ��ލs��.
	float2 g_vViewPort	: packoffset(c4); // �r���[�|�[�g��.
};

//�\����.
struct VS_OUTPUT
{
	float4 Pos		: SV_Position;
	float4 Color	: COLOR;
	float2 Tex		: TEXCOORD;
};

VS_OUTPUT VS_Main(
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	output.Pos = mul(Pos, g_mW);

	// �X�N���[�����W�ɍ��킹��v�Z,
	output.Pos.x = (output.Pos.x / g_vViewPort.x) * 2.0f - 1.0f;
	output.Pos.y = 1.0f - (output.Pos.y / g_vViewPort.y) * 2.0f;

	output.Tex = Tex;
	
	return output;
}

// �s�N�Z���V�F�[�_.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	// ���჌���Y.
	/*
	const float f = 1.0f;	// ����.
	float2 pos = input.Tex * 2.0f - 1.0f;
	float p = length(pos);
	pos = (1 + f * p * p) / (1 + 2 * f) * pos;
	float4 color = g_TextureColor.Sample(g_samLinear, pos*0.5+0.5);
	*/
	
	// �`�悳�ꂽ���f���Ȃǂ̐F�����擾.
	float4 color = g_TextureColor.Sample(g_SamLinear, input.Tex);
	
	//----------------------------------------------------------------.
	// �֊s��.
	//----------------------------------------------------------------.
	float imageSizeW, imageSizeh, levels;
	// �e�N�X�`���̃T�C�Y���擾.
	g_TextureNormal.GetDimensions(0, imageSizeW, imageSizeh, levels);
	
	const float s = 0.2f;	// �T���v�����O���鋭��.
	const float dx = 1.0f / imageSizeW;
	const float dy = 1.0f / imageSizeh;
	
	//----------------------------------------------------------------.
	// �@���̏����擾.
	float3 normColor = float3(0.0f, 0.0f, 0.0f);
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(-s * dx, -s * dy)).xyz;			// ����.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( 0 * dx, -s * dy)).xyz;			// ��.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( s * dx, -s * dy)).xyz;			// �E��.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(-s * dx,  0 * dy)).xyz;			// ��.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( 0 * dx,  0 * dy)).xyz * -8.0;	// ����.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( s * dx,  0 * dy)).xyz;			// �E.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(-s * dx,  s * dy)).xyz;			// ����.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( 0 * dx,  s * dy)).xyz;			// ��.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( s * dx,  s * dy)).xyz;			// �E��.
	
	//----------------------------------------------------------------.
	// �[�x�l�̏����擾.
	float4 zColor = g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(0 * dx, 0 * dy)); // ����.
	float z = zColor.r + (zColor.g + (zColor.b + zColor.a / 256.0f) / 256.0f) / 256.0f;
	
	float4 depthColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	depthColor	 = g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(-s * dx, -s * dy)); // ����.
	depthColor	+= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2( 0 * dx, -s * dy)); // ��.
	depthColor	+= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2( s * dx, -s * dy)); // �E��.
	depthColor	+= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(-s * dx,  0 * dy)); // ��.
	depthColor	+= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2( s * dx,  0 * dy)); // �E.
	depthColor	+= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(-s * dx,  s * dy)); // ����.
	depthColor	+= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2( 0 * dx,  s * dy)); // ��.
	depthColor	+= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2( s * dx,  s * dy)); // �E��.
	float depth = 0.0f;
	depth = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
	depth /= 8.0f;	// �[�x�l�̕��ς��v�Z.
	
	//----------------------------------------------------------------.
	// �G�t�F�N�g(�p�[�e�B�N��)��`�悵���e�N�X�`���̏����擾.
	float4 transColor = g_TextureTrans.Sample(g_SamLinear, input.Tex);
	// �O���[�X�P�[����.
	float grayScale = transColor.r * 0.299 + transColor.g * 0.587 + transColor.b * 0.114;
	grayScale = 1.0f - saturate(grayScale);
	
	float4 outLineColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	// �@�����ƁA�[�x�l�̏�񂪈��ȏ�Ȃ�֊s����\��.
	if (length(normColor) >= 0.62f || abs(depth-z) > 0.00097f)
	{
		outLineColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
		grayScale *= 0.0f;
	}
	else
	{
		outLineColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
		grayScale *= 1.0f;
	}
	color *= lerp(color, outLineColor, grayScale);
	
	return color;
}

// �s�N�Z���V�F�[�_.
float4 PS_LastMain(VS_OUTPUT input) : SV_Target
{
	FxaaTex tex = { g_SamLinear, g_TextureLast };
	return float4( FxaaPixelShader( input.Tex, tex, float2( 1.0f/g_vViewPort.x, 1.0f/g_vViewPort.y ) ), 1.0f );
}