#include "FXAA.hlsl"	// �A���`�G�C���A�X���g�p����ׂɕK�v.

#define BLOOM_SAMPLING_NUM (6)	// �u���[���̃T���v�����O��.

//-----------------------------------------------.
// �e�N�X�`��.
//-----------------------------------------------.
Texture2D g_TextureColor						: register(t0);	// �F���.
Texture2D g_TextureNormal						: register(t1);	// �@�����.
Texture2D g_TextureDepth						: register(t2);	// �[�x���.
Texture2D g_TextureTrans						: register(t3);	// �A���t�@���.
Texture2D g_TextureLast							: register(t4);	// �֊s���Ȃǂ�`�悵���e�N�X�`��.
Texture2D g_TextureBloom[BLOOM_SAMPLING_NUM]	: register(t5);	// Bloom�p�e�N�X�`��.
//-----------------------------------------------.
// �T���v��.
//-----------------------------------------------.
SamplerState g_SamLinear : register(s0);

//-----------------------------------------------.
// �R���X�^���g�o�b�t�@.
//-----------------------------------------------.
// �������p.
cbuffer PerInit		: register(b0)
{
	matrix g_mW			: packoffset(c0); // ܰ��ލs��.
	float2 g_vViewPort	: packoffset(c4); // �r���[�|�[�g��.
};
// �t���[�����ŕς��l.
cbuffer PerFrame	: register(b1)
{
	float4 g_SoftKneePram;
};

//-----------------------------------------------.
// �\����.
//-----------------------------------------------.
// ���_�o�͗p.
struct VS_OUTPUT
{
	float4 Pos			: SV_Position;
	float4 Color		: COLOR;
	float2 Tex			: TEXCOORD0;
	float4 OutLineColor : TEXCOORD1;
};
// �s�N�Z���o�͗p.
struct PS_OUTPUT
{
	float4 Color : SV_Target0;
	float4 Bloom : SV_Target1;
};


VS_OUTPUT VS_Main(
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD )
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	
	output.Pos = mul(Pos, g_mW);
	// �X�N���[�����W�ɍ��킹��v�Z,
	output.Pos.x = (output.Pos.x / g_vViewPort.x) * 2.0f - 1.0f;
	output.Pos.y = 1.0f - (output.Pos.y / g_vViewPort.y) * 2.0f;
	output.Tex = Tex;
	// �A�E�g���C���̐F.
	// ���݂͍��ŌŒ�.
	output.OutLineColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	
	return output;
}

// �s�N�Z���V�F�[�_.
PS_OUTPUT PS_Main( VS_OUTPUT input ) : SV_Target
{
	// ���჌���Y.
	/*
	const float f = 1.0f;	// ����.
	float2 pos = input.Tex * 2.0f - 1.0f;
	float p = length(pos);
	pos = (1 + f * p * p) / (1 + 2 * f) * pos;
	float4 color = g_TextureColor.Sample(g_SamLinear, pos*0.5+0.5);
	*/
	
	// �`�悳�ꂽ���f���Ȃǂ̐F�����擾.
	float4 color = g_TextureColor.Sample(g_SamLinear, input.Tex);
	
	//----------------------------------------------------------------.
	// �֊s��.
	//----------------------------------------------------------------.
	float imageSizeW, imageSizeh, levels;
	// �e�N�X�`���̃T�C�Y���擾.
	g_TextureNormal.GetDimensions(0, imageSizeW, imageSizeh, levels);
	
	float s = 0.125f;	// �T���v�����O���鋭��.
	const float dx = 1.0f / imageSizeW;
	const float dy = 1.0f / imageSizeh;
	float px = s * dx, py = s * dx;
	
	//----------------------------------------------------------------.
	// �@���̏����擾.
	float3 normColor = float3(0.0f, 0.0f, 0.0f);
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	-px,	-py)).xyz; // ����.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( 0 * px,	-py)).xyz; // ��.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	 px,	-py)).xyz; // �E��.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	-px, 0 * py)).xyz; // ��.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( 0 * px, 0 * py)).xyz * -8.0; // ����.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	 px, 0 * py)).xyz; // �E.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	-px,	 py)).xyz; // ����.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( 0 * px,	 py)).xyz; // ��.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	 px,	 py)).xyz; // �E��.
	
	s = 1.0f;	// �T���v�����O���鋭��.
	px = s * dx, py = s * dx;
	//----------------------------------------------------------------.
	// �[�x�l�̏����擾.
	float z = g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(0 * dx, 0 * dy)).x; // ����.
	float depthColor11		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	 px,	 py)).x; // ����.
	float depthColor10		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	 px, 0 * py)).x; // ��.
	float depthColor1_1		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	 px,	-py)).x; // �E��.
	float depthColor01		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2( 0 * px,	 py)).x; // ��.
	float depthColor0_1		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2( 0 * px,	-py)).x; // �E.
	float depthColor_11		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	-px,	 py)).x; // ����.
	float depthColor_10		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	-px, 0 * py)).x; // ��.
	float depthColor_1_1	= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	-px,	-py)).x; // �E��.
	
	// Horizontal.
	float h = (depthColor11	 *  1.0f + depthColor10	  *  2.0f)
			+ (depthColor1_1 *  1.0f + depthColor_11  * -1.0f)
			+ (depthColor_10 * -2.0f + depthColor_1_1 * -1.0f);
	// vertical.
	float v = (depthColor11	 *  1.0f + depthColor01	  *  2.0f)
			+ (depthColor_11 *  1.0f + depthColor1_1  * -1.0f)
			+ (depthColor0_1 * -2.0f + depthColor_1_1 * -1.0f);
	float depth = 1.0f - clamp(abs(h + v), 0.0f, 1.0f);
	
	//----------------------------------------------------------------.
	// �G�t�F�N�g(�p�[�e�B�N��)��`�悵���e�N�X�`���̏����擾.
	float4 transColor = g_TextureTrans.Sample(g_SamLinear, input.Tex);
	// �O���[�X�P�[����.
	float4 grayScale = transColor.r * 0.299 + transColor.g * 0.587 + transColor.b * 0.114;
	grayScale = 1.0f - saturate(grayScale);
	
	// �@�����ƁA�[�x�l�̏�񂪈��ȏ�Ȃ�֊s����\��.
	if (length(normColor) >= 0.572f || abs(depth) < 0.99f)
	{
		grayScale *= 1.0f;
	}
	else
	{
		grayScale *= 0.0f;
	}
	PS_OUTPUT output = (PS_OUTPUT) 0;
	output.Color = lerp( color, input.OutLineColor, grayScale);
	
	// �e�N�X�`���̖��x�𗎂Ƃ�.
	// --- softknee ---.
	// https://light11.hatenadiary.com/entry/2018/02/15/011155.
	// https://light11.hatenadiary.com/entry/2018/03/15/000022.
	
	if( color.a <= 0.0f ) color.a = 1.0f;
	// ���x�𗎂Ƃ����F.
	half source = max(color.r, max(color.g, color.b)) + (1 - color.a);
	
	half soft = clamp(source - g_SoftKneePram.y, 0, g_SoftKneePram.z);
	soft = soft * soft * g_SoftKneePram.w;
	// �v�Z��.
	//		   (min(knee*2[param.z], max(0, source-threshold+knee[param.y])))2.
	//	soft = ---------------------------------------------------------------.
	//							4 * knee + 0.00001[param.w].
	
	half contribution = max(source-g_SoftKneePram.x, soft);
	contribution /= max(source, 0.0001f);
	// �v�Z��.
	//				 (max(source - threshold[param.x], soft)).
	// contribution = ---------------------------------------.
	//							max(source, 0.0001f).
	output.Bloom = float4(color.rgb*contribution, 1.0f);
	
	return output;
}

// �s�N�Z���V�F�[�_.
float4 PS_EffectMain( VS_OUTPUT input ) : SV_Target
{
	FxaaTex tex = { g_SamLinear, g_TextureLast };
	float4 color = float4( FxaaPixelShader( input.Tex, tex, float2( 1.0f/g_vViewPort.x, 1.0f/g_vViewPort.y ) ), 1.0f );

	// �T���v�����O�����u���[�̃e�N�X�`���𑫂��Ă���.
	for( int i = 0; i < BLOOM_SAMPLING_NUM; i++ ) {
		color += g_TextureBloom[i].Sample(g_SamLinear, input.Tex);
	}
	
	return color;
}

float rand( float2 co )
{
	return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453);
}
 
float2 mod( float2 a, float2 b )
{
	return a - floor(a / b) * b;
}

// �s�N�Z���V�F�[�_.
float4 PS_FinalMain( VS_OUTPUT input ) : SV_Target
{
	float4 color = g_TextureColor.Sample(g_SamLinear, input.Tex);
	
	return color;
}

// �h�b�g��.
float4 PS_DotRenderEffect( VS_OUTPUT input ) : SV_Target
{
	float2 n = float2(70.0f, 70.0f);	// ��ʂ̃h�b�g��.
	float2 pixelSize = float2(1.0f/n.x, 1.0f/n.y);
	float2 ratio = float2(0, 0);
	ratio.x = (int)(input.Tex.x/pixelSize.x) * pixelSize.x;
	ratio.y = (int)(input.Tex.y/pixelSize.y) * pixelSize.y;
	
	float4 color = g_TextureColor.Sample(g_SamLinear, ratio);
	
	return color;
};

// �Q�[���{�[�C��.
float4 PS_GameBoyRenderEffect( VS_OUTPUT input ) : SV_Target
{
	float2 n = float2(70.0f, 70.0f);	// ��ʂ̃h�b�g��.
	float2 pixelSize = float2(1.0f/n.x, 1.0f/n.y);
	float2 ratio = float2(0, 0);
	ratio.x = (int)(input.Tex.x/pixelSize.x) * pixelSize.x;
	ratio.y = (int)(input.Tex.y/pixelSize.y) * pixelSize.y;
	
	float4 color = g_TextureColor.Sample(g_SamLinear, ratio);
	
	color.rgb = dot(color.rgb, float3(0.3, 0.59, 0.11));
	if (color.r <= 0.25) {
		color.rgb = float3(0.06, 0.22, 0.06);
	} else if (color.r > 0.75) {
		color.rgb = float3(0.6, 0.74, 0.06);
	} else if (color.r > 0.25 && color.r <= 0.5) {
		color.rgb = float3(0.19, 0.38, 0.19);
	} else {
		color.rgb = float3(0.54, 0.67, 0.06);
	}
	
	return color;
};

// �h�b�g���~�`.
float4 PS_DotCircleRenderEffect( VS_OUTPUT input ) : SV_Target
{
	float n = 70;
	float2 pixelSize = float2(1.0f/n, 1.0f/n);
	float2 ratio = float2(0, 0);
	ratio.x = (int)(input.Tex.x/pixelSize.x) * pixelSize.x;
	ratio.y = (int)(input.Tex.y/pixelSize.y) * pixelSize.y;
	
	float4 color = g_TextureColor.Sample(g_SamLinear, ratio);
	
	float2 st = (floor(input.Tex*n)+0.5)/n;
	
	float a = input.Tex.x - st.x;
	float b = input.Tex.y - st.y;
	float c = sqrt( a*a + b*b );
	if (c <= length(color.rgb*1.2f)*pixelSize.x*0.4f) {
		color.rgb *= 1.5;
	} else {
		color.rgb *= 0.1;
	}
	
	return color;
};

// RGB�����炷.
float4 PS_ChromaticAberration( VS_OUTPUT input ) : SV_Target
{
	float2 redUV	= input.Tex + float2( 0.0f, 0.0f );
	float2 greenUV	= input.Tex + float2( -0.01f, 0.0f );
	float2 blueUV	= input.Tex + float2( 0.015f, 0.0f );
	
	float4 color = float4( 0.0f, 0.0f, 0.0f, 1.0f );
	color.r = g_TextureColor.Sample(g_SamLinear, redUV).r;
	color.g = g_TextureColor.Sample(g_SamLinear, greenUV).g;
	color.b = g_TextureColor.Sample(g_SamLinear, blueUV).b;
	
	return color;
}

// �e���r��.
float4 PS_TelevisionRenderEffect( VS_OUTPUT input ) : SV_Target
{
	// �u���E���ǃe���r�̃m�C�Y�̏���.
	float2 inUV = input.Tex;
	float2 uv = input.Tex - 0.5;
				
	// UV���W���Čv�Z���A��ʂ�c�܂���
	float vignet = length(uv);
	uv /= 1 - vignet * 0.2;
	float2 texUV = uv + 0.5;
 
	// ��ʊO�Ȃ�`�悵�Ȃ�
	if (max(abs(uv.y) - 0.5, abs(uv.x) - 0.5) > 0)
	{
		return float4(0, 0, 0, 1);
	}
	
	float	_NoiseX = 0.0f;
	float2	_Offset = float2(0.0f, 0.0f);
	float	_RGBNoise = 0.04f;
	float	_SinNoiseWidth = 1.0f;
	float	_SinNoiseScale = 0.0f;
	float	_SinNoiseOffset = 1.0f;
	float	_ScanLineTail = 1.0f;
	float	_ScanLineSpeed = 160.0f;
 
	// �F���v�Z
	float3 col;
	float2 _Time = float2( input.Tex.x+0.005f, 0.01f );
	float2 _ScreenParams;
	float imageSizeW, imageSizeh, levels;
	// �e�N�X�`���̃T�C�Y���擾.
	g_TextureLast.GetDimensions(0, imageSizeW, imageSizeh, levels);
	_ScreenParams.x = 1.0f / imageSizeW;
	_ScreenParams.y = 1.0f / imageSizeh;
 
	// �m�C�Y�A�I�t�Z�b�g��K�p
	texUV.x += sin(texUV.y * _SinNoiseWidth + _SinNoiseOffset) * _SinNoiseScale;
	texUV += _Offset;
	texUV.x += (rand(floor(texUV.y * 100) + _Time.y) - 0) * _NoiseX;
	texUV = mod(texUV, 1);
	texUV.x = clamp(texUV.x, 0.0f, 1.0f);
 
	// �F���擾�ARGB�����������炷
	col.r = g_TextureLast.Sample(g_SamLinear, texUV).r;
	col.g = g_TextureLast.Sample(g_SamLinear, texUV).g;
	col.b = g_TextureLast.Sample(g_SamLinear, texUV).b;
 
	
	// RGB�m�C�Y
	if (rand((rand(floor(texUV.y * 500) + _Time.y) - 0.5) + _Time.y) < _RGBNoise)
	{
		col.r = rand(uv + float2(123 + _Time.y, 0));
		col.g = rand(uv + float2(123 + _Time.y, 1));
		col.b = rand(uv + float2(123 + _Time.y, 2));
	}
 
	// �s�N�Z�����Ƃɕ`�悷��RGB�����߂�
	float floorX = fmod(inUV.x * _ScreenParams.x * 0.25f, 1);
	col.r *= floorX > 0.3333;
	col.g *= floorX < 0.3333 || floorX > 0.3333;
	col.b *= floorX < 0.3333;
 
	// �X�L�������C����`��
	float scanLineColor = sin(_Time.y * 10 + uv.y * 500) / 2 + 0.2;
	col *= 0.5 + clamp(scanLineColor + 0.5, 0, 1) * 0.5;
 
	// �X�L�������C���̎c����`��
	float tail = clamp((frac(uv.y + _Time.y * _ScanLineSpeed) - 1 + _ScanLineTail) / min(_ScanLineTail, 1), 1, 1);
	col *= tail;
 
	// ��ʒ[���Â�����
	col *= 1 - vignet * 1.3;
				
	return float4(col, 1);
}