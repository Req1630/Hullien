/**
* @file EditRender.h.
* @brief エディタ描画クラス.
* @author 福田玲也.
*/
#ifndef EDIT_RENDERER_H
#define EDIT_RENDERER_H

#include <memory>
#include <vector>
#include <functional>

/*****************************************
*	エディタ描画クラス.
*	　PushRenderProc( std::function<void()> proc )に、
*		ImGuiで編集で使用する関数を入れる.
**/
class CEditRenderer
{
public:
	CEditRenderer();
	~CEditRenderer();
	// インスタンスの取得.
	static CEditRenderer* GetInstance();

	// 描画関数を追加する.
	static void PushRenderProc( std::function<void()> proc );
	// 描画する.
	static void Render();

private:
	std::vector<std::function<void()>> m_RenderList;
	bool m_IsRender;

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CEditRenderer( const CEditRenderer & )				= delete;
	CEditRenderer( CEditRenderer && )					= delete;
	CEditRenderer& operator = ( const CEditRenderer & )	= delete;
	CEditRenderer& operator = ( CEditRenderer && )		= delete;
};

#endif	// #ifndef EDIT_RENDERER_H.