//��۰��ٕϐ�.
//ø����́Aڼ޽� t(n).
Texture2D g_MaskTexture : register(t0);
Texture2D g_Texture : register(t1);
Texture2D g_DestTexture : register(t2);
//����ׂ́Aڼ޽� s(n).
SamplerState g_SamLinear : register(s0);

cbuffer CBufferPerInit : register(b0)
{
	matrix g_mW : packoffset(c0); // ܰ��ލs��.
	float2 g_fViewPort : packoffset(c4); // UV���W.
}

//�ݽ����ޯ̧.
cbuffer CBufferPerFrame : register(b1)
{
	matrix g_mWorld : packoffset(c0); // ܰ��ލs��.
	float Value : packoffset(c4); // ܰ��ލs��.
};

//�\����.
struct VS_OUTPUT
{
	float4 Pos : SV_Position;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
};

// ���_�V�F�[�_.
VS_OUTPUT VS_Main(
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD )
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	output.Pos = mul(Pos, g_mWorld);

	// �X�N���[�����W�ɍ��킹��v�Z,
	output.Pos.x = (output.Pos.x / g_fViewPort.x) * 2.0f - 1.0f;
	output.Pos.y = 1.0f - (output.Pos.y / g_fViewPort.y) * 2.0f;

	output.Tex = Tex;
	output.Tex.x = (output.Pos.x + 1.0) / 2;
	output.Tex.y = (-output.Pos.y + 1.0) / 2;
	

	return output;
}

// �s�N�Z���V�F�[�_.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	float maskColor = g_MaskTexture.Sample(g_SamLinear, input.Tex).r;
	float4 texColor = g_Texture.Sample(g_SamLinear, input.Tex);
	float4 dest = g_DestTexture.Sample(g_SamLinear, input.Tex); // �K��F.
	
	// �ŏI�F.
	float4 finalColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	// �A���t�@����.
	float maskAlpha = saturate(maskColor + (Value * 2.0f - 1.0f));
	finalColor = texColor * maskAlpha + texColor * (1 - texColor.a);
	clip(finalColor.a - 0.001f);
	
	float4 result;
	
	if (dest.r > 0.5) result.r = dest.r * finalColor.r * 2;
	else result.r = 2 * (dest.r + finalColor.r - dest.r * finalColor.r) - 1.0;
	if (dest.g > 0.5) result.g = dest.g * finalColor.g * 2;
	else result.g = 2 * (dest.g + finalColor.g - dest.g * finalColor.g) - 1.0;
	if (dest.b > 0.5) result.b = dest.b * finalColor.b * 2;
	else result.b = 2 * (dest.b + finalColor.b - dest.b * finalColor.b) - 1.0;
	result.a = texColor.a * finalColor.a;
	
	return result;
	
	/*
	// �J�b�g�A�E�g.
	half maskAlpha = maskColor - (-1 + Value);
	finalColor = float4( texColor.rgb, texColor.a*maskAlpha );
	clip(finalColor.a - 0.9999);
	*/
	return finalColor;
}