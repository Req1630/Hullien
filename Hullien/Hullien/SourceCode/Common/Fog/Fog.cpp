#include "Fog.h"

namespace
{
	static const float FOG_TEX_ADD_VALUE	= 0.1f;	// �t�H�O�̃e�N�X�`�����Z�l.
	static const float FOG_TEX_MAX			= 1.0f;
}

CFog::CFog()
	: m_FogTex	{ 0.0f, 0.0f, 0.5f, 0.5f }
{
}

CFog::~CFog()
{
}

// �X�V.
void CFog::Update()
{
	GetInstance()->m_FogTex.x += FOG_TEX_ADD_VALUE;
	if( GetInstance()->m_FogTex.x >= FOG_TEX_MAX ){
		GetInstance()->m_FogTex.x = 0.0f;
	}
	GetInstance()->m_FogTex.w += FOG_TEX_ADD_VALUE;
	if( GetInstance()->m_FogTex.w >= FOG_TEX_MAX ){
		GetInstance()->m_FogTex.w = 0.0f;
	}
}