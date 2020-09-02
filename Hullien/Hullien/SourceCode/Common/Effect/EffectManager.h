#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "Effect.h"

#include <memory>

class CEffectManager
{
public:
	CEffectManager();
	~CEffectManager();

	void Play( const D3DXVECTOR3 vPos );

private:
	std::shared_ptr<CEffect>	m_pEffect;	// �G�t�F�N�g�N���X.
	CEffect::Handle				m_Handle;	// �G�t�F�N�g�n���h��.
};

#endif	// #ifndef EFFECT_MANAGER_H.