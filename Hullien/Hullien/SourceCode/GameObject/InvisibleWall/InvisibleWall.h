#ifndef INVISIBLE_WALL_H
#define INVISIBLE_WALL_H

#include "..\GameObject.h"
#include "BoxStruct.h"

class CCollisionManager;	// �����蔻��N���X.
class CInvisibleWallShader;	// �����Ȃ��ǃV�F�[�_�[�N���X.

/**************************************
*	�����Ȃ���.
*/
class CInvisibleWall : public CGameObject
{
	const char* INVISIBLE_WALL_PARAM_FILE_NAME = "Data\\GameParam\\InvisibleWall.bin";
	const float HALF			= 0.5f;		// ����.
	const float POSITION_HEIGHT	= 10.0f;	// ���W�̍���.
public:
	CInvisibleWall();
	virtual ~CInvisibleWall();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

	// �{�b�N�X�̐ݒ�.
	void SetBoxWall( const SBoxWall& boxWall );
	// �{�b�N�X�̎擾.
	inline SBoxWall* GetBoxWall() { return &m_BoxWall; }
	// ���W�ݒ�֐�.
	virtual void SetPosition( const D3DXVECTOR3& vPos ) override;

private:
	// �p�����[�^�̏�����.
	bool InitParam();
	// �����蔻��̏�����.
	bool InitCollision();

private:
	SBoxWall	m_BoxWall;	//�@�ǂ̃p�����[�^.
	std::unique_ptr<CInvisibleWallShader>	m_pInvisibleWallShader;
#ifdef _DEBUG
	std::unique_ptr<CCollisionManager>	m_pCollision;	// �����蔻��N���X(���o�p).
#endif	// #ifdef _DEBUG
	D3DXVECTOR3	m_PlayerPosition;
};

#endif	// #ifndef INVISIBLE_WALL_H.