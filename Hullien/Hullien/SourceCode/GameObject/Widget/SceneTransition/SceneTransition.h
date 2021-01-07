#ifndef SCENE_TRANSITION_H
#define SCENE_TRANSITION_H

#include "..\Widget.h"

class CTransition;	// トランジションクラス.

class CSceneTransition
{
	const float ALPHA_ADD_VALUE = 0.01f;	// 透過の加算値.
	const float ALPHA_MAX = 1.0f; 
private:
	// フェードの状態.
	enum class enFadeState
	{
		In,			// フェードイン.
		Out,		// フェードアウト.
		Finish,		// フェード終了.
	} typedef EFadeState;
public:
	CSceneTransition();
	~CSceneTransition();

	// インスタンスの生成.
	static CSceneTransition* GetInstance()
	{
		static std::unique_ptr<CSceneTransition> pInstance = std::make_unique<CSceneTransition>();
		return pInstance.get();
	}

	// 初期化関数.
	static bool Init();
	//描画関数.
	static void Render();
	// フェード中か.
	static inline bool GetIsFade() { return GetInstance()->m_FadeState != EFadeState::Finish; }
	// フェード状態取得関数.
	static inline EFadeState GetFadeState() { return GetInstance()->m_FadeState; }
	// フェードイン設定関数.
	static void SetFadeIn();

	static void SetTexture(){ GetInstance()->m_FadeState = EFadeState::Out; }

	// 基準となるテクスチャの設定.
	static void SetDestTexture( ID3D11ShaderResourceView* pDestTexture );
	// 合成するテクスチャの設定.
	static void SetSrcTexture( ID3D11ShaderResourceView* pSrcTexture );

private:
	// 更新関数.
	void Update();
	// スプライト設定関数.
	bool SpriteSetting();

private:
	std::unique_ptr<CTransition>	m_pTransition;	// トランジション.
	ID3D11ShaderResourceView*		m_pDestTexture;	// 基準となるテクスチャ.
	ID3D11ShaderResourceView*		m_pSrcTexture;	// 合成するテクスチャ.

	EFadeState	m_FadeState;	// フェードの状態.
	float		m_NowAlpha;		// 現在の透過値.
	bool		m_IsFade;		// フェード中か.

private:
	//コピー・ムーブコンストラクタ、代入演算子の削除.
	CSceneTransition( const CSceneTransition & )				= delete;
	CSceneTransition( CSceneTransition && )						= delete;
	CSceneTransition& operator = ( const CSceneTransition & )	= delete;
	CSceneTransition& operator = ( CSceneTransition && )		= delete;
};

#endif	// #ifndef SCENE_TRANSITION_H.
