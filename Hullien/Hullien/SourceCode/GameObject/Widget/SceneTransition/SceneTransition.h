#ifndef SCENE_TRANSITION_H
#define SCENE_TRANSITION_H

#include "..\Widget.h"

class CTransition;	// �g�����W�V�����N���X.

class CSceneTransition
{
	const float ALPHA_ADD_VALUE = 0.01f;	// ���߂̉��Z�l.
	const float ALPHA_MAX = 1.0f; 
private:
	// �t�F�[�h�̏��.
	enum class enFadeState
	{
		In,			// �t�F�[�h�C��.
		Out,		// �t�F�[�h�A�E�g.
		Finish,		// �t�F�[�h�I��.
	} typedef EFadeState;
public:
	CSceneTransition();
	~CSceneTransition();

	// �C���X�^���X�̐���.
	static CSceneTransition* GetInstance()
	{
		static std::unique_ptr<CSceneTransition> pInstance = std::make_unique<CSceneTransition>();
		return pInstance.get();
	}

	// �������֐�.
	static bool Init();
	//�`��֐�.
	static void Render();
	// �t�F�[�h����.
	static inline bool GetIsFade() { return GetInstance()->m_FadeState != EFadeState::Finish; }
	// �t�F�[�h��Ԏ擾�֐�.
	static inline EFadeState GetFadeState() { return GetInstance()->m_FadeState; }
	// �t�F�[�h�C���ݒ�֐�.
	static void SetFadeIn();

	static void SetTexture(){ GetInstance()->m_FadeState = EFadeState::Out; }

	// ��ƂȂ�e�N�X�`���̐ݒ�.
	static void SetDestTexture( ID3D11ShaderResourceView* pDestTexture );
	// ��������e�N�X�`���̐ݒ�.
	static void SetSrcTexture( ID3D11ShaderResourceView* pSrcTexture );

private:
	// �X�V�֐�.
	void Update();
	// �X�v���C�g�ݒ�֐�.
	bool SpriteSetting();

private:
	std::unique_ptr<CTransition>	m_pTransition;	// �g�����W�V����.
	ID3D11ShaderResourceView*		m_pDestTexture;	// ��ƂȂ�e�N�X�`��.
	ID3D11ShaderResourceView*		m_pSrcTexture;	// ��������e�N�X�`��.

	EFadeState	m_FadeState;	// �t�F�[�h�̏��.
	float		m_NowAlpha;		// ���݂̓��ߒl.
	bool		m_IsFade;		// �t�F�[�h����.

private:
	//�R�s�[�E���[�u�R���X�g���N�^�A������Z�q�̍폜.
	CSceneTransition( const CSceneTransition & )				= delete;
	CSceneTransition( CSceneTransition && )						= delete;
	CSceneTransition& operator = ( const CSceneTransition & )	= delete;
	CSceneTransition& operator = ( CSceneTransition && )		= delete;
};

#endif	// #ifndef SCENE_TRANSITION_H.
