/**
* @file SceneBase.h.
* @brief �V�[�����N���X.
* @author ���c���.
*/
#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include <Windows.h>
#include <string>
#include <stack>
#include <memory>

#include "..\..\Global.h"

class CSceneManager;

/************************
*	�V�[�����N���X.
**/
class CSceneBase
{
public:
	CSceneBase( CSceneManager* pSceneManager )
		: m_pSceneManager	( pSceneManager )
	{}

	virtual ~CSceneBase(){};
	
	// �Ǎ��֐�.
	virtual bool Load() = 0;
	// �X�V�֐�.
	virtual void Update() = 0;
	// �`��֐�.
	virtual void Render() = 0;

protected:
	CSceneManager* m_pSceneManager;	// �V�[�����.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CSceneBase( const CSceneBase & )				= delete;
	CSceneBase& operator = ( const CSceneBase & )	= delete;
	CSceneBase( CSceneBase && )						= delete;
	CSceneBase& operator = ( CSceneBase && )		= delete;
};

#endif // #ifndef SCENE_BASE_H.