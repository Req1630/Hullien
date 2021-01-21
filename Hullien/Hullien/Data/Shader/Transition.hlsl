//-----------------------------------------------.
// �e�N�X�`��.
//-----------------------------------------------.
Texture2D g_MaskTexture	: register(t0);	// �}�X�N�摜.
Texture2D g_SrcTexture	: register(t1);	// �t�F�[�h�摜.
Texture2D g_DestTexture	: register(t2);	// �K��摜.
//-----------------------------------------------.
// �T���v��.
//-----------------------------------------------.
SamplerState g_SamLinear : register(s0);

//-----------------------------------------------.
// �R���X�^���g�o�b�t�@.
//-----------------------------------------------.
// �������p.
cbuffer CBufferPerInit : register(b0)
{
	matrix g_mW			: packoffset(c0); // ���[���h�s��.
	float2 g_ViewPort	: packoffset(c4); // �r���[�|�[�g�̃T�C�Y..
}
// �t���[�����ŕς��l.
cbuffer CBufferPerFrame : register(b1)
{
	matrix	g_mWorld		: packoffset(c0); // ���[���h�s��.
	float	g_Value			: packoffset(c4); // �t�F�[�h�l.
	float	g_IsScreenSize	: packoffset(c5); // screen�T�C�Y�ɍ��킹�邩�ǂ���.
};

//-----------------------------------------------.
// �\����.
//-----------------------------------------------.
// ���_�o�͗p.
struct VS_OUTPUT
{
	float4 Pos		: SV_Position;
	float4 Color	: COLOR;
	float2 Tex		: TEXCOORD;
};

//-----------------------------------------------.
// ���_�V�F�[�_.
//-----------------------------------------------.
VS_OUTPUT VS_Main(
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD )
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	output.Pos = mul(Pos, g_mWorld);

	// �X�N���[�����W�ɍ��킹��v�Z,
	output.Pos.x = (output.Pos.x / g_ViewPort.x) * 2.0f - 1.0f;
	output.Pos.y = 1.0f - (output.Pos.y / g_ViewPort.y) * 2.0f;

	if( g_IsScreenSize >= 1.0f ) {
		output.Tex.x = (output.Pos.x + 1.0) / 2;
		output.Tex.y = (-output.Pos.y + 1.0) / 2;
	} else {
		output.Tex = Tex;
	}
	
	

	return output;
}

//-----------------------------------------------.
// �A���t�@����.
//-----------------------------------------------.
float4 PS_AlphaOut(VS_OUTPUT input) : SV_Target
{
	float maskColor = g_MaskTexture.Sample(g_SamLinear, input.Tex).r;
	float4 texColor = g_SrcTexture.Sample(g_SamLinear, input.Tex);
	
	// �ŏI�F.
	float4 finalColor = float4(0.4f, 0.0f, 0.0f, 1.0f);
	
	// �A���t�@����.
	float maskAlpha = saturate(maskColor+(g_Value*2.0f-1.0f));
	finalColor = texColor * maskAlpha + texColor * (1 - texColor.a);
	clip(finalColor.a - 0.001f);
	
	return finalColor;
}

//-----------------------------------------------.
// �J�b�g�A�E�g.
//-----------------------------------------------.
float4 PS_CutOut(VS_OUTPUT input) : SV_Target
{
	float maskColor = g_MaskTexture.Sample(g_SamLinear, input.Tex).r;
	float4 texColor = g_SrcTexture.Sample(g_SamLinear, input.Tex);
	
	// �ŏI�F.
	float4 finalColor = float4(0.4f, 0.0f, 0.0f, 1.0f);
	
	// �J�b�g�A�E�g.
	half maskAlpha = maskColor-(-1+g_Value);
	clip(maskAlpha - 0.9999);
	finalColor = texColor;
	
	return finalColor;
}

//-----------------------------------------------.
// �A���t�@����(�n�[�h���C�g�u�����h���g�p).
//-----------------------------------------------.
float4 PS_HardBlenAlphaOut(VS_OUTPUT input) : SV_Target
{
	float maskColor = g_MaskTexture.Sample(g_SamLinear, input.Tex).r;
	float4 texColor = g_SrcTexture.Sample(g_SamLinear, input.Tex);
	float4 dest = g_DestTexture.Sample(g_SamLinear, input.Tex); // �K��F.
	
	// �ŏI�F.
	float4 finalColor = float4(0.4f, 0.0f, 0.0f, 1.0f);
	
	// �A���t�@����.
	float maskAlpha = saturate(maskColor+(g_Value*2.0f-1.0f));
	finalColor = texColor * maskAlpha + texColor * (1 - texColor.a);
	clip(finalColor.a - 0.001f);
	
	float4 color = 0.0f;
	
	// �n�[�h���C�g�u�����h����.
	if (dest.r > 0.5) color.r = dest.r * finalColor.r * 2;
	else color.r = 2 * (dest.r + finalColor.r - dest.r * finalColor.r) - 1.0;
	if (dest.g > 0.5) color.g = dest.g * finalColor.g * 2;
	else color.g = 2 * (dest.g + finalColor.g - dest.g * finalColor.g) - 1.0;
	if (dest.b > 0.5) color.b = dest.b * finalColor.b * 2;
	else color.b = 2 * (dest.b + finalColor.b - dest.b * finalColor.b) - 1.0;
	color.a = texColor.a * finalColor.a;
	
	return color;
}