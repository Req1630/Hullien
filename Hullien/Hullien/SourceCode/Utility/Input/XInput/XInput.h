/**
* @file XInput.h
* @brief This is Use Xinput Controller file.
* @author Fukuta
* @date 2020/12/16
* @details Xinput���g�p���邽�߂̃N���X�ł�.
**/
#ifndef XINPUT_H
#define XINPUT_H

#include <Windows.h>
#include <XInput.h>
#include <vector>
#include <unordered_map>
#include <memory>

#pragma comment( lib, "xinput.lib" )

#include "..\InputStruct.h"

static const SHORT	IDLE_THUMB_MAX		=  10000;	//!< �X�e�B�b�N�̗V��.
static const SHORT	IDLE_THUMB_MIN		= -10000;	//!< �X�e�B�b�N�̗V��.
static const BYTE	IDLE_TIGGER_MAX		=  100;		//!< �g���K�[�{�^���̗V��.
static const WORD	INPUT_VIBRATION_MIN	=  0;		//!< �o�C�u���[�V�����̍ŏ��l.
static const WORD	INPUT_VIBRATION_MAX	=  65535;	//!< �o�C�u���[�V�����̍ő�l.

/**
* @brief XInput Class(Singleton).
*
* @details �o�C�u���[�V�����@�\�ȊO�̋@�\�͂��Ă܂�.
*/
class CXInput
{
	const int FOUR_LIMITED_CONTROLLER = 4;	//!< �ڑ��R���g���[���̍ő吔.
public:
	inline static const char* VIBRATION_CONFIG_FILE_PATH = "Data\\Config\\Con_Vib.bin";

public:
	/**
	* @brief �R���X�g���N�^.
	*/
	CXInput();
	/**
	* @brief �f�X�g���N�^.
	*/
	~CXInput();

	/**
	* @fn static void StatsUpdate()
	* @brief �R���g���[���[�̏�Ԃ��X�V.
	* @details ���t���[�����ɍX�V���邱��.
	*/
	static void StatsUpdate();

	/**
	* @fn static int GetConnectCount()
	* @return int ���݂̃R���g���[���[�̐ڑ���.
	* @brief �R���g���[���[�̐ڑ����Ă��鐔���擾.
	*/
	static int GetConnectCount(){ return GetInstance()->m_ConnectedCount; }
	

	// ������Ă���Ƃ�.
	static bool IsPress( const WORD& button_mask, const int& connectNum = 0 );
	// �����ꂽ�u��.
	static bool IsMomentPress( const WORD& button_mask, const int& connectNum = 0 );
	// ���������Ă���Ƃ�.
	static bool IsHold( const WORD& button_mask, const int& connectNum = 0 );
	// �������u��.
	static bool IsRelease( const WORD& button_mask, const int& connectNum = 0 );
	// �����Ă��Ȃ�.
	static bool NotPress( const WORD& button_mask, const int& connectNum = 0 );
	// ���X�e�B�b�N��X���̎擾.
	static SHORT LThumbX_Axis( const int& connectNum = 0 );
	// ���X�e�B�b�N��Y���̎擾.
	static SHORT LThumbY_Axis( const int& connectNum = 0 );
	// �E�X�e�B�b�N��X���̎擾.
	static SHORT RThumbX_Axis( const int& connectNum = 0 );
	// �E�X�e�B�b�N��Y���̎擾.
	static SHORT RThumbY_Axis( const int& connectNum = 0 );
	// ���g���K�[�̎擾.
	static BYTE LTrigger( const int& connectNum = 0 );
	// �E�g���K�[�̎擾.
	static BYTE RTrigger( const int& connectNum = 0 );

	// �U���ݒ�.
	static bool SetVibration( WORD LMotorSpd, WORD RMotorSpd, const int& connectNum = 0 );
	// �U�������邩�ǂ����ݒ�.
	static void SetVibration( const bool& isVib ){ GetInstance()->m_IsVibration = isVib; }
	static bool IsVibration(){ return GetInstance()->m_IsVibration; }

private:
	void ConnectCheck( const int& connectNum );

	bool IsButtonInput( const int& connectNum, const WORD& button_mask );

	void ButtonStateUpdate();

	static CXInput* GetInstance()
	{
		static std::unique_ptr<CXInput> pInstance = std::make_unique<CXInput>();
		return pInstance.get();
	}

private:
	std::vector<XINPUT_STATE>		m_State;		//!< �ڑ������R���g���[���[�̏��.
	std::vector<XINPUT_VIBRATION>	m_Vibration;	//!< �o�C�u���[�V�����̏��.
	int		m_ConnectedCount;						//!< �ڑ������R���g���[���[�̐�.
	bool	m_IsVibration;							//!< �o�C�u���[�V�������s����.
	std::vector<std::unordered_map<WORD, EInputState>> m_ButtonStateList;	//!< ���͂��ꂽ�{�^���̏��.

	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CXInput( const CXInput & )				= delete;
	CXInput( CXInput && )					= delete;
	CXInput& operator = ( const CXInput & )	= delete;
	CXInput& operator = ( CXInput && )		= delete;
};


#endif	// #ifndef XINPUT_H.