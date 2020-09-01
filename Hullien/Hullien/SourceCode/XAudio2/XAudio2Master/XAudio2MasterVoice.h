#ifndef C_XAUDIO2_MASTERVOICE_H
#define C_XAUDIO2_MASTERVOICE_H

#include "XAudio2.h"
#define COM_SAFE_RELEASE( p ) { if(p) { (p)->Release(); (p) = nullptr; } }


class clsXAudio2MasterVoice
{
public:
	clsXAudio2MasterVoice();
	~clsXAudio2MasterVoice();

	// �C���X�^���X�̎擾.
	static clsXAudio2MasterVoice* GetInstance();

	IXAudio2* GetInterface()const;
	// �}�X�^�[�{�C�X�擾.
	IXAudio2MasteringVoice* GetMasteringVoice()const;
	static clsXAudio2MasterVoice& GetGlobalSystem();
	// �J���֐�.
	static void Release();
protected:
	// �������֐�.
	bool Initialize();

	IXAudio2 *              m_pXAudioInterface;                //XAudio2�C���^�[�t�F�[�X
	IXAudio2MasteringVoice *m_pMasterVoice;                    //�}�X�^�{�C�X
private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	clsXAudio2MasterVoice(const clsXAudio2MasterVoice &) = delete;
	clsXAudio2MasterVoice& operator = (const clsXAudio2MasterVoice &) = delete;
	clsXAudio2MasterVoice(clsXAudio2MasterVoice &&) = delete;
	clsXAudio2MasterVoice& operator = (clsXAudio2MasterVoice &&) = delete;
};

#endif //#ifndef C_XAUDIO2_MASTERVOICE_H