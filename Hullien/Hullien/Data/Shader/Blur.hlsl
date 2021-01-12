//-------------------------------------------------.
// テクスチャ.
//-------------------------------------------------.
Texture2D g_Texture : register(t0);
//-------------------------------------------------.
// サンプラ.
//-------------------------------------------------.
SamplerState g_SamLinear : register(s0);

//-------------------------------------------------.
// コンスタントバッファ.
//-------------------------------------------------.
cbuffer cBuffer : register(b0)
{
	matrix g_mW			: packoffset(c0); // ワールド行列.
	float2 g_vViewPort	: packoffset(c4); // ビューポート幅.
	float2 g_vPixelSize	: packoffset(c5); // ピクセルのサイズ.
	float2 g_Ratio		: packoffset(c6); // 画面の比率.
};

//-------------------------------------------------.
// 構造体.
//-------------------------------------------------.
struct VS_OUTPUT
{
	float4 Pos : SV_Position;
	float2 Tex : TEXCOORD0;
};

//-------------------------------------------------.
// 頂点シェーダー.
//-------------------------------------------------.
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
	
	return output;
}

//-------------------------------------------------.
// ピクセルシェーダ.
//-------------------------------------------------.
// 横方向にぼかす.
float4 PS_HorizontalBlur(VS_OUTPUT input) : SV_Target
{
	float4 color = float4( 0.0f, 0.0f, 0.0f, 1.0f );
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.x + g_vPixelSize.x*float2(-3.0f,0.0f)) * 0.053;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.x + g_vPixelSize.x*float2(-2.0f,0.0f)) * 0.123;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.x + g_vPixelSize.x*float2(-1.0f,0.0f)) * 0.203;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.x + g_vPixelSize.x*float2( 0.0f,0.0f)) * 0.240;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.x + g_vPixelSize.x*float2( 1.0f,0.0f)) * 0.203;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.x + g_vPixelSize.x*float2( 2.0f,0.0f)) * 0.123;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.x + g_vPixelSize.x*float2( 3.0f,0.0f)) * 0.053;
	
	return color;
}

// 縦方向にぼかす.
float4 PS_VerticalBlur(VS_OUTPUT input) : SV_Target
{
	float4 color = float4( 0.0f, 0.0f, 0.0f, 1.0f );
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.y + g_vPixelSize.y*float2(0.0f,-3.0f)) * 0.053;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.y + g_vPixelSize.y*float2(0.0f,-2.0f)) * 0.123;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.y + g_vPixelSize.y*float2(0.0f,-1.0f)) * 0.203;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.y + g_vPixelSize.y*float2(0.0f, 0.0f)) * 0.240;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.y + g_vPixelSize.y*float2(0.0f, 1.0f)) * 0.203;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.y + g_vPixelSize.y*float2(0.0f, 2.0f)) * 0.123;
	color += g_Texture.Sample(g_SamLinear, input.Tex*g_Ratio.y + g_vPixelSize.y*float2(0.0f, 3.0f)) * 0.053;
	
	return color;
}
