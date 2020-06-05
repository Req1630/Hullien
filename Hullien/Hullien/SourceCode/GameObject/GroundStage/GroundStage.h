#ifndef GROUND_STAGE_H
#define GROUND_STAGE_H

#include "..\GameObject.h"

/**********************************
*	�n�ʃN���X.
*/
class CGroundStage : public CGameObject
{
	const char* MODEL_NAME = "ground";	// ���f���̖��O.
public:
	CGroundStage();
	virtual ~CGroundStage();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

private:
	std::shared_ptr<CDX9StaticMesh>	m_pStaticMesh;
};

#endif	// #ifndef GROUND_STAGE_H.