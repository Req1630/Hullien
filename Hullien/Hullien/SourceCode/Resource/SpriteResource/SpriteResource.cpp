#include "SpriteResource.h"
#include "..\..\Utility\FileManager\FileManager.h"

CSpriteResource::CSpriteResource()
	: m_SpriteList			()
	, m_HasFinishedLoading	( false )
{
}

CSpriteResource::~CSpriteResource()
{
	m_SpriteList.clear();
}

//-------------------------------.
// インスタンスの取得.
//-------------------------------.
CSpriteResource* CSpriteResource::GetInstance()
{
	static std::unique_ptr<CSpriteResource> pInstance = 
		std::make_unique<CSpriteResource>();
	return pInstance.get();
}

//-------------------------------.
// スプライトの読み込み(ラッパー).
//-------------------------------.
HRESULT CSpriteResource::Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	if( FAILED( GetInstance()->SpriteLoad( pDevice11, pContext11 ) )) return E_FAIL;
	return S_OK;
}

//-------------------------------.
// スプライト取得関数.
//-------------------------------.
std::shared_ptr<CSprite> CSpriteResource::GetSprite( const std::string& spriteName )
{
	// 指定したスプライト名を取得する.
	for( auto& s : GetInstance()->m_SpriteList ){
		if( s.first == spriteName ) return s.second;
	}

	// スプライトの読込が終了してないので null を返す.
	if( GetInstance()->m_HasFinishedLoading == false ) return nullptr;

	ERROR_MESSAGE( spriteName + "sprite not found" );
	return nullptr;
}

//-------------------------------.
// スプライト読み込み.
//-------------------------------.
HRESULT CSpriteResource::SpriteLoad( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	auto eachLoad = [&]( const fs::directory_entry& entry )
	{
		const std::string exe		= entry.path().extension().string();	// 拡張子.
		const std::string filePath	= entry.path().string();				// ファイルパス.
		const std::string fileName	= entry.path().stem().string();			// ファイル名.
		const std::string errorMsg	= fileName+" : スプライト読み込み失敗";	// あらかじめエラーメッセージを設定する.

		// "_"が入ったものは無視される.
		if( filePath.find("_") != std::string::npos ) return;
		// 指定した拡張子以外は読み込い.
		if( exe != ".png" && exe != ".PNG" && 
			exe != ".bmp" && exe != ".BMP" &&
			exe != ".jpg" && exe != ".JPG" ) return;

		// スプライト情報の取得.
		SSpriteState ss;
		if( FAILED( SpriteStateRead( filePath, &ss ))) throw errorMsg;

		// スプライトの作成.
		m_SpriteList[fileName] = std::make_shared<CSprite>();
		if( FAILED( m_SpriteList[fileName]->Init( pDevice11, pContext11, filePath.c_str(), ss ) ))
			throw errorMsg;
	};

	try {
		fs::recursive_directory_iterator dir_itr(FILE_PATH), end_itr;
		std::for_each( dir_itr, end_itr, eachLoad );
	} catch ( const fs::filesystem_error& e ){
		// エラーメッセージを表示.
		ERROR_MESSAGE( e.path1().string().c_str() );
		return E_FAIL;
	} catch( const std::string& e ){
		// エラーメッセージを表示.
		ERROR_MESSAGE( e );
		return E_FAIL;
	}

	// 読込が終わったので true にする.
	m_HasFinishedLoading = true;
	return S_OK;
}

//-------------------------------.
// スプライト情報の読み込み.
//-------------------------------.
SSpriteState CSpriteResource::SpriteStateRead( const std::string& path )
{
	SSpriteState ss;

	std::string filePath = path;
	// 拡張子の"."の位置を取得.
	size_t i = filePath.find(".");
	// 拡張子取り除いて".txt"に変更.
	filePath.erase( i, filePath.size() ) += ".txt";

	std::vector<std::string> pramList = CFileManager::TextLoading( filePath );

	if( pramList.empty() == true ){
		ERROR_MESSAGE( "The list of " + filePath + " was empty" );
		return ss;
	}

	ss.LocalPosNum	= std::stoi(pramList[SSpriteState::enLocalPosNum]);
	ss.Disp.w		= std::stof(pramList[SSpriteState::enDisp_w]);
	ss.Disp.h		= std::stof(pramList[SSpriteState::enDisp_h]);
	ss.Base.w		= std::stof(pramList[SSpriteState::enBase_w]);
	ss.Base.h		= std::stof(pramList[SSpriteState::enBase_h]);
	ss.Stride.w		= std::stof(pramList[SSpriteState::enStride_w]);
	ss.Stride.h		= std::stof(pramList[SSpriteState::enStride_h]);
	ss.vPos.x		= std::stof(pramList[SSpriteState::envPos_x]);
	ss.vPos.y		= std::stof(pramList[SSpriteState::envPos_y]);
	ss.vPos.z		= std::stof(pramList[SSpriteState::envPos_z]);
	ss.AnimNum		= std::stoi(pramList[SSpriteState::enAnimNum]);

	return ss;
}

// スプライト情報の読み込み.
HRESULT CSpriteResource::SpriteStateRead( const std::string& path, SSpriteState* ss )
{
	std::string filePath = path;
	// 拡張子の"."の位置を取得.
	size_t i = filePath.find(".");
	// 拡張子取り除いて".txt"に変更.
	filePath.erase( i, filePath.size() ) += ".txt";

	std::vector<std::string> pramList = CFileManager::TextLoading( filePath );

	if( pramList.empty() == true ){
		ERROR_MESSAGE( "The list of " + filePath + " was empty" );
		return E_FAIL;
	}

	ss->LocalPosNum	= std::stoi(pramList[SSpriteState::enLocalPosNum]);
	ss->Disp.w		= std::stof(pramList[SSpriteState::enDisp_w]);
	ss->Disp.h		= std::stof(pramList[SSpriteState::enDisp_h]);
	ss->Base.w		= std::stof(pramList[SSpriteState::enBase_w]);
	ss->Base.h		= std::stof(pramList[SSpriteState::enBase_h]);
	ss->Stride.w	= std::stof(pramList[SSpriteState::enStride_w]);
	ss->Stride.h	= std::stof(pramList[SSpriteState::enStride_h]);
	ss->vPos.x		= std::stof(pramList[SSpriteState::envPos_x]);
	ss->vPos.y		= std::stof(pramList[SSpriteState::envPos_y]);
	ss->vPos.z		= std::stof(pramList[SSpriteState::envPos_z]);
	ss->AnimNum		= std::stoi(pramList[SSpriteState::enAnimNum]);

	return S_OK;
}