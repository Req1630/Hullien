#ifndef EDIT_RENDERER_H
#define EDIT_RENDERER_H

#include <memory>
#include <vector>
#include <functional>

class CEditRenderer
{
public:
	CEditRenderer();
	~CEditRenderer();
	// �C���X�^���X�̎擾.
	static CEditRenderer* GetInstance();

	// �`��֐���ǉ�����.
	static void PushRenderProc( std::function<void()> proc );
	// �`�悷��.
	static void Render();

private:
	std::vector<std::function<void()>> m_RenderList;
	bool m_IsRender;

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CEditRenderer( const CEditRenderer & )				= delete;
	CEditRenderer( CEditRenderer && )					= delete;
	CEditRenderer& operator = ( const CEditRenderer & )	= delete;
	CEditRenderer& operator = ( CEditRenderer && )		= delete;
};

#endif	// #ifndef EDIT_RENDERER_H.