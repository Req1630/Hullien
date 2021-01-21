//-----------------------------------------------.
// テクスチャ.
//-----------------------------------------------.
Texture2D g_MaskTexture	: register(t0);	// マスク画像.
Texture2D g_SrcTexture	: register(t1);	// フェード画像.
Texture2D g_DestTexture	: register(t2);	// 規定画像.
//-----------------------------------------------.
// サンプラ.
//-----------------------------------------------.
SamplerState g_SamLinear : register(s0);

//-----------------------------------------------.
// コンスタントバッファ.
//-----------------------------------------------.
// 初期化用.
cbuffer CBufferPerInit : register(b0)
{
	matrix g_mW			: packoffset(c0); // ワールド行列.
	float2 g_ViewPort	: packoffset(c4); // ビューポートのサイズ..
}
// フレーム毎で変わる値.
cbuffer CBufferPerFrame : register(b1)
{
	matrix	g_mWorld		: packoffset(c0); // ワールド行列.
	float	g_Value			: packoffset(c4); // フェード値.
	float	g_IsScreenSize	: packoffset(c5); // screenサイズに合わせるかどうか.
};

//-----------------------------------------------.
// 構造体.
//-----------------------------------------------.
// 頂点出力用.
struct VS_OUTPUT
{
	float4 Pos		: SV_Position;
	float4 Color	: COLOR;
	float2 Tex		: TEXCOORD;
};

//-----------------------------------------------.
// 頂点シェーダ.
//-----------------------------------------------.
VS_OUTPUT VS_Main(
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD )
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	output.Pos = mul(Pos, g_mWorld);

	// スクリーン座標に合わせる計算,
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
// アルファ抜き.
//-----------------------------------------------.
float4 PS_AlphaOut(VS_OUTPUT input) : SV_Target
{
	float maskColor = g_MaskTexture.Sample(g_SamLinear, input.Tex).r;
	float4 texColor = g_SrcTexture.Sample(g_SamLinear, input.Tex);
	
	// 最終色.
	float4 finalColor = float4(0.4f, 0.0f, 0.0f, 1.0f);
	
	// アルファ抜け.
	float maskAlpha = saturate(maskColor+(g_Value*2.0f-1.0f));
	finalColor = texColor * maskAlpha + texColor * (1 - texColor.a);
	clip(finalColor.a - 0.001f);
	
	return finalColor;
}

//-----------------------------------------------.
// カットアウト.
//-----------------------------------------------.
float4 PS_CutOut(VS_OUTPUT input) : SV_Target
{
	float maskColor = g_MaskTexture.Sample(g_SamLinear, input.Tex).r;
	float4 texColor = g_SrcTexture.Sample(g_SamLinear, input.Tex);
	
	// 最終色.
	float4 finalColor = float4(0.4f, 0.0f, 0.0f, 1.0f);
	
	// カットアウト.
	half maskAlpha = maskColor-(-1+g_Value);
	clip(maskAlpha - 0.9999);
	finalColor = texColor;
	
	return finalColor;
}

//-----------------------------------------------.
// アルファ抜き(ハードライトブレンドを使用).
//-----------------------------------------------.
float4 PS_HardBlenAlphaOut(VS_OUTPUT input) : SV_Target
{
	float maskColor = g_MaskTexture.Sample(g_SamLinear, input.Tex).r;
	float4 texColor = g_SrcTexture.Sample(g_SamLinear, input.Tex);
	float4 dest = g_DestTexture.Sample(g_SamLinear, input.Tex); // 規定色.
	
	// 最終色.
	float4 finalColor = float4(0.4f, 0.0f, 0.0f, 1.0f);
	
	// アルファ抜け.
	float maskAlpha = saturate(maskColor+(g_Value*2.0f-1.0f));
	finalColor = texColor * maskAlpha + texColor * (1 - texColor.a);
	clip(finalColor.a - 0.001f);
	
	float4 color = 0.0f;
	
	// ハードライトブレンド処理.
	if (dest.r > 0.5) color.r = dest.r * finalColor.r * 2;
	else color.r = 2 * (dest.r + finalColor.r - dest.r * finalColor.r) - 1.0;
	if (dest.g > 0.5) color.g = dest.g * finalColor.g * 2;
	else color.g = 2 * (dest.g + finalColor.g - dest.g * finalColor.g) - 1.0;
	if (dest.b > 0.5) color.b = dest.b * finalColor.b * 2;
	else color.b = 2 * (dest.b + finalColor.b - dest.b * finalColor.b) - 1.0;
	color.a = texColor.a * finalColor.a;
	
	return color;
}