//ｸﾞﾛｰﾊﾞﾙ変数.
//ﾃｸｽﾁｬは、ﾚｼﾞｽﾀ t(n).
Texture2D		g_Texture : register( t0 );
//ｻﾝﾌﾟﾗは、ﾚｼﾞｽﾀ s(n).
SamplerState	g_samLinear		: register( s0 );

//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
cbuffer global : register(b0)
{
	matrix	g_mW			        : packoffset(c0);	// ﾜｰﾙﾄﾞ行列.
	float4	g_Color			        : packoffset(c4);	// カラー.
	float2	g_fViewPort		        : packoffset(c5);	// ビューポート幅.
    float   g_fImageWidth           : packoffset(c6);	// 画像幅.
    float   g_fDispXPos             : packoffset(c7);	// 表示X位置.
    float   g_fMaxGaugeValue        : packoffset(c8);	// ゲージ最大値.
    float   g_fNowGaugeRangeValue   : packoffset(c9);	// ゲージ現在の値.
};

//構造体.
struct VS_OUTPUT
{
	float4 Pos			: SV_Position;
	float4 Color		: COLOR;
	float2 Tex			: TEXCOORD;
};

// 頂点シェーダ.
VS_OUTPUT VS_Main( 
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD )
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul( Pos, g_mW );

	// スクリーン座標に合わせる計算,
	output.Pos.x = ( output.Pos.x / g_fViewPort.x ) * 2.0f - 1.0f;
	output.Pos.y = 1.0f - ( output.Pos.y / g_fViewPort.y ) * 2.0f;

	output.Tex = Tex;

	return output;
}

// ピクセルシェーダ.
float4 PS_Main( VS_OUTPUT input ) : SV_Target
{
	float4 maskColor = g_Texture.Sample(g_samLinear, input.Tex);
	float4 texColor = float4(0.5f, 0.f, 0.f, 1.0f);
	
	if( maskColor.a <= 0.0f ) discard;
	
	float maskAlpha = saturate(length(maskColor) + (g_fNowGaugeRangeValue * 2.0f - 1.0f));
	float4 outColor = texColor * maskAlpha + texColor * (1 - texColor.a);

	return outColor;
}