//ｸﾞﾛｰﾊﾞﾙ変数.
//ﾃｸｽﾁｬは、ﾚｼﾞｽﾀ t(n).
Texture2D		g_Texture : register( t0 );
Texture2D		g_MaskTexture : register( t1 );
//ｻﾝﾌﾟﾗは、ﾚｼﾞｽﾀ s(n).
SamplerState	g_samLinear	: register( s0 );

// 円周率.
#define PI	(3.14159265)

//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
cbuffer global : register(b0)
{
	matrix	g_mW			: packoffset(c0);	// ワールド行列.
	float2	g_ViewPort		: packoffset(c4);	// ビューポート幅.
	float2	g_CenterPos		: packoffset(c5);	// 中心座標.
	float2	g_StartVector	: packoffset(c6);	// 円の開始ベクトル.
    float   g_Value			: packoffset(c7);	// ゲージ現在の値.
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
	output.Pos.x = ( output.Pos.x / g_ViewPort.x ) * 2.0f - 1.0f;
	output.Pos.y = 1.0f - ( output.Pos.y / g_ViewPort.y ) * 2.0f;

	output.Tex = Tex;

	return output;
}

// ピクセルシェーダ.
float4 PS_Main( VS_OUTPUT input ) : SV_Target
{
	float maskColor = g_MaskTexture.Sample(g_samLinear, input.Tex).r;
	float4 texColor = g_Texture.Sample(g_samLinear, input.Tex);
	
	// 対象ピクセル座標と中心の座標のベクトルを求める.
    float2 endVector = normalize(input.Tex - g_CenterPos);
	// 二つのベクトルの内積を求める.
	// 内積の逆コサインを求める.
	// 絶対値を求める.
    float Deg = abs(acos(dot(g_StartVector, endVector)));
	// 0 ~ 1 →　時計周り	<=.
	// 0 ~ 1 →　反時計回り >=.
    if (endVector.x <= g_StartVector.x){
		Deg = PI + (PI - Deg);
	}
    if (Deg >= g_Value) discard;
	
	return texColor*maskColor;
}