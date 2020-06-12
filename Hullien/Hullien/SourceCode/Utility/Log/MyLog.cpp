#include "MyLog.h"
#include "..\..\Global.h"

#include <iostream>
#include <fstream>
#include <sstream>

CLog::CLog()
{
}

CLog::~CLog()
{
}

//---------------------------------.
// �C���X�^���X�̎擾.
//---------------------------------.
CLog* CLog::GetInstance()
{
	static std::unique_ptr<CLog> pInstance = std::make_unique<CLog>();
	return pInstance.get();
}

//---------------------------------.
// ���O�e�L�X�g�t�@�C�����쐬.
//---------------------------------.
HRESULT CLog::OpenLogTextFile()
{
	std::ofstream logText;
	logText.open( GetInstance()->LOG_TEXT_FILE_PATH, std::ios::trunc );

	// �t�@�C�����ǂݍ��߂ĂȂ�������I��.
	if( !logText.is_open() ){
		ERROR_MESSAGE( "log text read failed" );
		return E_FAIL;
	}

	time_t		nowTime		= time( nullptr );	// ���݂̎��Ԃ��l��.
	TIME_DATA	timeData;	
	localtime_s( &timeData, &nowTime );// ���[�J�����Ԃɕϊ�.

	// ���݂̎��Ԃ��e�L�X�g�ɓ���.
	logText << "[";
	logText << timeData.tm_year+1900	<< "/";	// 1900 �������ƂŌ��݂̔N�ɂȂ�.
	logText << timeData.tm_mon+1		<< "/";	// 1 �������ƂŌ��݂̔N�ɂȂ�.
	logText << timeData.tm_mday			<< " ";
	logText << timeData.tm_hour			<< ":";
	logText << timeData.tm_min;
	logText << "]" << " >> App launch"	<< std::endl << std::endl;

	// �t�@�C�������.
	logText.close();
	return S_OK;
}

//---------------------------------.
// ���O�e�L�X�g�t�@�C�������.
//---------------------------------.
HRESULT CLog::CloseLogTextFile()
{
	std::ofstream logText( GetInstance()->LOG_TEXT_FILE_PATH, std::ios::app );

	// �t�@�C�����ǂݍ��߂ĂȂ�������I��.
	if( !logText.is_open() ){
		ERROR_MESSAGE( "log text read failed" );
		return E_FAIL;
	}

	time_t		nowTime = time( nullptr );	// ���݂̎��Ԃ��l��.
	TIME_DATA	timeData;
	localtime_s( &timeData, &nowTime );// ���[�J�����Ԃɕϊ�.

	// ���݂̎��Ԃ��e�L�X�g�ɓ���.
	logText << std::endl		<<  "[";
	logText << timeData.tm_hour	<< ":";
	logText << timeData.tm_min	<< ":";
	logText << timeData.tm_sec;
	logText << "]" << " >> App End" << std::endl;

	// �t�@�C�������.
	logText.close();

	return S_OK;
}

//---------------------------------.
// ���O�̓���.
//---------------------------------.
HRESULT CLog::Print( const std::string& message )
{
	std::ofstream logText( GetInstance()->LOG_TEXT_FILE_PATH, std::ios::app );

	// �t�@�C�����ǂݍ��߂ĂȂ�������I��.
	if( !logText.is_open() ){
		ERROR_MESSAGE( "log text read failed" );
		return E_FAIL;
	}

	time_t		nowTime = time( nullptr );	// ���݂̎��Ԃ��l��.
	TIME_DATA	timeData;
	localtime_s( &timeData, &nowTime );// ���[�J�����Ԃɕϊ�.

	// ���݂̎��Ԃ��e�L�X�g�ɓ���.
	logText << "[";
	logText << timeData.tm_hour << ":";
	logText << timeData.tm_min	<< ":";
	logText << timeData.tm_sec;
	logText << "]" << " >> " << message << std::endl;	// message�̓���.

	// �t�@�C�������.
	logText.close();

	return S_OK;
}