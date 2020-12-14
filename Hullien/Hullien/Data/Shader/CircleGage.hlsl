//��۰��ٕϐ�.
//ø����́Aڼ޽� t(n).
Texture2D		g_Texture : register( t0 );
Texture2D		g_MaskTexture : register( t1 );
//����ׂ́Aڼ޽� s(n).
SamplerState	g_samLinear	: register( s0 );

// �~����.
#define PI	(3.14159265)

//�ݽ����ޯ̧.
cbuffer global : register(b0)
{
	matrix	g_mW			: packoffset(c0);	// ���[���h�s��.
	float2	g_ViewPort		: packoffset(c4);	// �r���[�|�[�g��.
	float2	g_CenterPos		: packoffset(c5);	// ���S���W.
	float2	g_StartVector	: packoffset(c6);	// �~�̊J�n�x�N�g��.
    float   g_Value			: packoffset(c7);	// �Q�[�W���݂̒l.
};

//�\����.
struct VS_OUTPUT
{
	float4 Pos			: SV_Position;
	float4 Color		: COLOR;
	float2 Tex			: TEXCOORD;
};

// ���_�V�F�[�_.
VS_OUTPUT VS_Main( 
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD )
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul( Pos, g_mW );

	// �X�N���[�����W�ɍ��킹��v�Z,
	output.Pos.x = ( output.Pos.x / g_ViewPort.x ) * 2.0f - 1.0f;
	output.Pos.y = 1.0f - ( output.Pos.y / g_ViewPort.y ) * 2.0f;

	output.Tex = Tex;

	return output;
}

// �s�N�Z���V�F�[�_.
float4 PS_Main( VS_OUTPUT input ) : SV_Target
{
	float maskColor = g_MaskTexture.Sample(g_samLinear, input.Tex).r;
	float4 texColor = g_Texture.Sample(g_samLinear, input.Tex);
	
	// �Ώۃs�N�Z�����W�ƒ��S�̍��W�̃x�N�g�������߂�.
    float2 endVector = normalize(input.Tex - g_CenterPos);
	// ��̃x�N�g���̓��ς����߂�.
	// ���ς̋t�R�T�C�������߂�.
	// ��Βl�����߂�.
    float Deg = abs(acos(dot(g_StartVector, endVector)));
	// 0 ~ 1 ���@���v����	<=.
	// 0 ~ 1 ���@�����v��� >=.
    if (endVector.x <= g_StartVector.x){
		Deg = PI + (PI - Deg);
	}
    if (Deg >= g_Value) discard;
	
	return texColor*maskColor;
}