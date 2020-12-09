#include "STGEnemyFactory.h"
#include "..\STGEnemy.h"
#include "..\STGEnemyParam.h"
#include "..\..\..\Bullet\BuletManager\BulletManagerParam.h"
#include "..\..\..\..\..\Utility\FileManager\FileManager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

STG::CEnemyFactory::CEnemyFactory()
	: m_BulletParams	()
{
}

STG::CEnemyFactory::~CEnemyFactory()
{
}

// 敵の作成.
bool STG::CEnemyFactory::Create( std::vector<std::shared_ptr<STG::CEnemy>>& enemys )
{
	// 弾リストの作成.
	if( CreateBulletList() == false ) return false;
	// 敵リストの作成.
	if( CreateEnemyList( enemys ) == false ) return false;

	return true;
}

// 弾リストの作成.
bool STG::CEnemyFactory::CreateBulletList()
{
	std::fstream fileStream( BULLET_LIST_FILE_PATH );
	// ファイルが開けない場合.
	if( !fileStream.is_open() ){
		ERROR_MESSAGE( "STGBulletファイル読み込み失敗" );
		return false;
	}

	std::string line;	// 1行分を読み込むための文字.
	while( std::getline( fileStream, line ) ){
		std::string buff = "";		// 文字を一時的に格納する.
		std::istringstream stream( line );

		int no = STG::EBulletManagerParamNo::EBulletManagerParamNo_Start;
		STG::SBulletManagerParam bulletParam;
		// デフォルト ',' 区切りで文字を取得.
		while( std::getline( stream, buff, ',' ) ){
			switch( no )
			{
			case EBulletManagerParamNo_BulletSpeed:
				bulletParam.BulletSpeed = std::stof(buff) * 2.0f;
				break;
			case EBulletManagerParamNo_BulletCountMax:
				bulletParam.BulletCountMax = std::stoi(buff);
				break;
			case EBulletManagerParamNo_AnyBulletCountMax:
				bulletParam.AnyBulletCountMax = std::stoi(buff);
				break;
			case EBulletManagerParamNo_BulletAngle:
				bulletParam.BulletAngle =  static_cast<float>( D3DXToRadian( std::stof(buff) ));
				break;
			case EBulletManagerParamNo_ShotAngle:
				bulletParam.ShotAngle = static_cast<float>( D3DXToRadian( std::stof(buff) ));
				break;
			case EBulletManagerParamNo_BulletAngleAddAccValue:
				bulletParam.BulletAngleAddAccValue = static_cast<float>( D3DXToRadian( std::stof(buff) ));
				break;
			case EBulletManagerParamNo_ShotIntervalFrame:
				bulletParam.ShotIntervalFrame = std::stoi(buff);
				break;
			case EBulletManagerParamNo_BulletCollDisappear:
				bulletParam.BulletCollDisappear = std::stoi(buff);
				break;
			default:
				break;
			}
			no++;
		}
		m_BulletParams.emplace_back( bulletParam );
	}
	return true;
}

// 敵リストの作成.
bool STG::CEnemyFactory::CreateEnemyList( std::vector<std::shared_ptr<STG::CEnemy>>& enemys )
{
	std::fstream fileStream( ENEMY_LIST_FILE_PATH );
	// ファイルが開けない場合.
	if( !fileStream.is_open() ){
		ERROR_MESSAGE( "STG敵ファイル読み込み失敗" );
		return false;
	}

	float posY = 0.0f;
	std::string line;	// 1行分を読み込むための文字.
	while( std::getline( fileStream, line ) ){
		std::string buff = "";		// 文字を一時的に格納する.
		std::istringstream stream( line );

		int no = STG::EEnemyParamNo::EEnemyParamNo_Start;
		STG::SEnemyParam enemyParam;
		std::vector<SBulletManagerParam> bulletList;	// 使用する弾のリスト.
		// デフォルト ',' 区切りで文字を取得.
		while( std::getline( stream, buff, ',' ) ){
			switch( no )
			{
			case EEnemyParamNo_SpawnTime:
				enemyParam.SpawnTime = std::stof(buff) * FPS;
				break;
			case EEnemyParamNo_PositionX:
				enemyParam.PositionX = std::stof(buff);
				break;
			case EEnemyParamNo_Text:
				enemyParam.Text = buff;
				break;
			case EEnemyParamNo_TextSize:
				enemyParam.TextSize = std::stof(buff);
				break;
			case EEnemyParamNo_LifePoint:
				enemyParam.LifePoint = std::stof(buff);
				break;
			case EEnemyParamNo_MoveSpeed:
				enemyParam.MoveSpeed = std::stof(buff);
				break;
			case EEnemyParamNo_BulletIndex:
			{
				// 使用する弾の番号を取得.
				std::istringstream bulletIndexList( buff );
				std::string s_index;
				while( std::getline( bulletIndexList, s_index, '&' ) ){
					const int index = std::stoi(s_index)-1;
					if( index >= static_cast<int>(m_BulletParams.size()) ) break;
					bulletList.emplace_back( m_BulletParams.at(index) );
				}
			}
				break;
			default:
				break;
			}
			no++;
		}
		enemys.emplace_back( std::make_shared<STG::CEnemy>(enemyParam, bulletList ) );
		if( enemys.back()->Init() == false ) return false;

		enemys.back()->SetPositionY( posY );
		// おんなじ座標で重なるとおかしくなるので、
		// 少し座標を足して、調整する.
		posY += POSITION_HEIGHT_ADD_VALUE;
	}

	// 最後の敵なので、フラグを立てる.
	enemys.back()->SetLastSpawn();

	return true;
}
