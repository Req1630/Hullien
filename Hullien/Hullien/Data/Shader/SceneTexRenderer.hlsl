#include "FXAA.hlsl"

//ｸﾞﾛｰﾊﾞﾙ変数.
//ﾃｸｽﾁｬは、ﾚｼﾞｽﾀ t(n).
Texture2D g_TextureColor	: register(t0);	// 色情報.
Texture2D g_TextureNormal	: register(t1);	// 法線情報.
Texture2D g_TextureDepth	: register(t2);	// 深度情報.
Texture2D g_TextureTrans	: register(t3);	// アルファ情報.
Texture2D g_TextureLast		: register(t4);	// 輪郭線などを描画したテクスチャ.
Texture2D g_TextureBloom[6]	: register(t5);	// Bloom用テクスチャ.
//ｻﾝﾌﾟﾗは、ﾚｼﾞｽﾀ s(n).
SamplerState g_SamLinear : register(s0);

//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
cbuffer PerInit : register(b0)
{
	matrix g_mW			: packoffset(c0); // ﾜｰﾙﾄﾞ行列.
	float2 g_vViewPort	: packoffset(c4); // ビューポート幅.
};

cbuffer PerFrame : register(b1)
{
	float4 g_SoftKneePram;
};

//構造体.
struct VS_OUTPUT
{
	float4 Pos			: SV_Position;
	float4 Color		: COLOR;
	float2 Tex			: TEXCOORD0;
	float4 OutLineColor : TEXCOORD1;
};

VS_OUTPUT VS_Main(
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	output.Pos = mul(Pos, g_mW);

	// スクリーン座標に合わせる計算,
	output.Pos.x = (output.Pos.x / g_vViewPort.x) * 2.0f - 1.0f;
	output.Pos.y = 1.0f - (output.Pos.y / g_vViewPort.y) * 2.0f;

	output.Tex = Tex;
//	output.OutLineColor = float4(output.Pos.x + 0.7f, 0.0f, output.Pos.y + 0.7f, 1.0f);
	output.OutLineColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	
	return output;
}

struct PS_OUTPUT
{
	float4 Color : SV_Target0;
	float4 Bloom : SV_Target1;
};

// ピクセルシェーダ.
PS_OUTPUT PS_Main(VS_OUTPUT input) : SV_Target
{
	// 魚眼レンズ.
	/*
	const float f = 1.0f;	// 強さ.
	float2 pos = input.Tex * 2.0f - 1.0f;
	float p = length(pos);
	pos = (1 + f * p * p) / (1 + 2 * f) * pos;
	float4 color = g_TextureColor.Sample(g_SamLinear, pos*0.5+0.5);
	*/
	
	// 描画されたモデルなどの色情報を取得.
	float4 color = g_TextureColor.Sample(g_SamLinear, input.Tex);
	
	//----------------------------------------------------------------.
	// 輪郭線.
	//----------------------------------------------------------------.
	float imageSizeW, imageSizeh, levels;
	// テクスチャのサイズを取得.
	g_TextureNormal.GetDimensions(0, imageSizeW, imageSizeh, levels);
	
	float s = 0.125f;	// サンプリングする強さ.
	const float dx = 1.0f / imageSizeW;
	const float dy = 1.0f / imageSizeh;
	float px = s * dx, py = s * dx;
	
	//----------------------------------------------------------------.
	// 法線の情報を取得.
	float3 normColor = float3(0.0f, 0.0f, 0.0f);
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	-px,	-py)).xyz; // 左上.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( 0 * px,	-py)).xyz; // 上.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	 px,	-py)).xyz; // 右上.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	-px, 0 * py)).xyz; // 左.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( 0 * px, 0 * py)).xyz * -8.0; // 自分.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	 px, 0 * py)).xyz; // 右.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	-px,	 py)).xyz; // 左下.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( 0 * px,	 py)).xyz; // 下.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	 px,	 py)).xyz; // 右下.
	
	s = 1.0f;	// サンプリングする強さ.
	px = s * dx, py = s * dx;
	//----------------------------------------------------------------.
	// 深度値の情報を取得.
	float z = g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(0 * dx, 0 * dy)).x; // 自分.
	float depthColor11		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	 px,	 py)).x; // 左上.
	float depthColor10		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	 px, 0 * py)).x; // 上.
	float depthColor1_1		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	 px,	-py)).x; // 右上.
	float depthColor01		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2( 0 * px,	 py)).x; // 左.
	float depthColor0_1		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2( 0 * px,	-py)).x; // 右.
	float depthColor_11		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	-px,	 py)).x; // 左下.
	float depthColor_10		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	-px, 0 * py)).x; // 下.
	float depthColor_1_1	= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	-px,	-py)).x; // 右下.
	
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
	// エフェクト(パーティクル)を描画したテクスチャの情報を取得.
	float4 transColor = g_TextureTrans.Sample(g_SamLinear, input.Tex);
	// グレースケール化.
	float4 grayScale = transColor.r * 0.299 + transColor.g * 0.587 + transColor.b * 0.114;
	grayScale = 1.0f - saturate(grayScale);
	
	// 法線情報と、深度値の情報が一定以上なら輪郭線を表示.
	if (length(normColor) >= 0.45f || abs(depth) < 0.8f)
	{
		grayScale *= 1.0f;
	}
	else
	{
		grayScale *= 0.0f;
	}
	PS_OUTPUT output = (PS_OUTPUT) 0;
	output.Color = lerp( color, input.OutLineColor, grayScale);
	
	// テクスチャの明度を落とす.
	// --- softknee ---.
	// https://light11.hatenadiary.com/entry/2018/02/15/011155.
	// https://light11.hatenadiary.com/entry/2018/03/15/000022.
	
	if( color.a <= 0.0f ) color.a = 1.0f;
	// 明度を落とした色.
	half source = max(color.r, max(color.g, color.b)) + (1 - color.a);
	
	half soft = clamp(source - g_SoftKneePram.y, 0, g_SoftKneePram.z);
	soft = soft * soft * g_SoftKneePram.w;
	// 計算式.
	//		   (min(knee*2[param.z], max(0, source-threshold+knee[param.y])))2.
	//	soft = ---------------------------------------------------------------.
	//							4 * knee + 0.00001[param.w].
	
	half contribution = max(source-g_SoftKneePram.x, soft);
	contribution /= max(source, 0.0001f);
	// 計算式.
	//				 (max(source - threshold[param.x], soft)).
	// contribution = ---------------------------------------.
	//							max(source, 0.0001f).
	output.Bloom = float4(color.rgb*contribution, 1.0f);
	
	return output;
}

/*
float rand(float2 co) {
	return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453);
}
 
float2 mod(float2 a, float2 b)
{
	return a - floor(a / b) * b;
}
*/

// ピクセルシェーダ.
float4 PS_LastMain(VS_OUTPUT input) : SV_Target
{
	FxaaTex tex = { g_SamLinear, g_TextureLast };
	float4 color = float4( FxaaPixelShader( input.Tex, tex, float2( 1.0f/g_vViewPort.x, 1.0f/g_vViewPort.y ) ), 1.0f );

	
	for( int i = 0; i < 6; i++ ){
		color += g_TextureBloom[i].Sample(g_SamLinear, input.Tex);
	}
	
/*
	// ブラウン管テレビのノイズの処理.
	float2 inUV = input.Tex;
	float2 uv = input.Tex - 0.5;
				
	// UV座標を再計算し、画面を歪ませる
	float vignet = length(uv);
	uv /= 1 - vignet * 0.2;
	float2 texUV = uv + 0.5;
 
	// 画面外なら描画しない
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
 
	// 色を計算
	float3 col;
	float2 _Time = float2( input.Tex.x+0.005f, 0.01f );
	float2 _ScreenParams;
	float imageSizeW, imageSizeh, levels;
	// テクスチャのサイズを取得.
	g_TextureLast.GetDimensions(0, imageSizeW, imageSizeh, levels);
	_ScreenParams.x = 1.0f / imageSizeW;
	_ScreenParams.y = 1.0f / imageSizeh;
 
	// ノイズ、オフセットを適用
	texUV.x += sin(texUV.y * _SinNoiseWidth + _SinNoiseOffset) * _SinNoiseScale;
	texUV += _Offset;
	texUV.x += (rand(floor(texUV.y * 100) + _Time.y) - 0) * _NoiseX;
	texUV = mod(texUV, 1);
	texUV.x = clamp(texUV.x, 0.0f, 1.0f);
 
	// 色を取得、RGBを少しずつずらす
	col.r = g_TextureLast.Sample(g_SamLinear, texUV).r;
	col.g = g_TextureLast.Sample(g_SamLinear, texUV).g;
	col.b = g_TextureLast.Sample(g_SamLinear, texUV).b;
 
	
	// RGBノイズ
	if (rand((rand(floor(texUV.y * 500) + _Time.y) - 0.5) + _Time.y) < _RGBNoise)
	{
		col.r = rand(uv + float2(123 + _Time.y, 0));
		col.g = rand(uv + float2(123 + _Time.y, 1));
		col.b = rand(uv + float2(123 + _Time.y, 2));
	}
 
	// ピクセルごとに描画するRGBを決める
	float floorX = fmod(inUV.x * _ScreenParams.x * 0.25f, 1);
	col.r *= floorX > 0.3333;
	col.g *= floorX < 0.3333 || floorX > 0.3333;
	col.b *= floorX < 0.3333;
 
	// スキャンラインを描画
	float scanLineColor = sin(_Time.y * 10 + uv.y * 500) / 2 + 0.2;
	col *= 0.5 + clamp(scanLineColor + 0.5, 0, 1) * 0.5;
 
	// スキャンラインの残像を描画
	float tail = clamp((frac(uv.y + _Time.y * _ScanLineSpeed) - 1 + _ScanLineTail) / min(_ScanLineTail, 1), 1, 1);
	col *= tail;
 
	// 画面端を暗くする
	col *= 1 - vignet * 1.3;
				
	return float4(col, 1);
*/	
/*	
	//--------------------------.
	// ブラウン管の湾曲の部分.
	const float dipsEdgeWidth = 0.1f;
	float2 inUV = input.Tex;
	float2 uv = input.Tex-0.5f;
	
	float vignet = length(uv);
	uv /= 1 - vignet * dipsEdgeWidth;
	
	if(max(abs(uv.y)-0.5f, abs(uv.x)-0.5f) > 0.0f){
		return float4(0.05, 0.05, 0.05f, 1.0f);
	}
	//--------------------------.
*/
	return color;
}