#ifndef CHARACTER_H
#define CHARACTER_H

#include "..\Actor.h"

class CCharacter : public CActor
{
protected:
	// キャラクターパラメータ.
	struct stParameter
	{
		float	Life;		// 体力.
		float	MoveSpeed;	// 移動速度.

		stParameter()
			: Life		( 1.0f )
			, MoveSpeed	( 0.5f )
		{}
	} typedef SParameter;

public:
	CCharacter();
	virtual ~CCharacter();

	// 移動関数.
	virtual void Move() = 0;

protected:
	// モデルの取得.
	bool GetModel( const char* modelName );
	// パラメータの設定.
	template<class inParam>
	bool ParameterSetting( const char* fileName, inParam& param )
	{
		// 既にパラメータが設定済みなら終了.
		if( m_HasFinishedParamSetting == true ) return true;

		// 読み込みが正常に行えてなければ終了.
		if( CFileManager::BinaryReading( fileName, param ) == false ) return false;

		m_HasFinishedParamSetting = true;
		return true;
	}

protected:
	std::shared_ptr<CDX9SkinMesh>	m_pSkinMesh;	// スキンメッシュ.
	D3DXVECTOR3	m_MoveVector;	// 移動ベクトル.

	bool m_HasFinishedParamSetting;	// パラメーターの設定が終わったか.
};

#endif	// #ifndef CHARACTER_H.