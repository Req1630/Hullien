#ifndef STG_CHARACTER_H
#define STG_CHARACTER_H

#include "..\STGActor.h"
#include <vector>

namespace STG
{
	class CBullet;	// �e�N���X.

	class CCharacter : public STG::CActor
	{
	public:
		CCharacter();
		virtual ~CCharacter();

	protected:
		// �ړ��֐�.
		virtual void Move() = 0;
		// �e�̏�����.
		virtual bool BulletInit( const char* modelName );
		// �e�̍X�V.
		virtual void BulletUpdate();
		// �e�̕`��.
		virtual void BulletRender();
		// �e������.
		virtual void BulletShot( const float& rot, const float& moveSpeed );

	protected:
		std::vector<std::shared_ptr<CBullet>> m_pBullets;
		int m_ShotCount;
	};

};

#endif	// #ifndef STG_CHARACTER_H.