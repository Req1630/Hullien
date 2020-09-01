#ifndef XAUDIO2_PLAYSE_H
#define XAUDIO2_PLAYSE_H


#include <vector>
#include <mutex>
#include "..\XAudio2.h"
#include "..\XAudio2Master\XAudio2MasterVoice.h"
#include "..\OggLoad\OggLoad.h"
#include "..\XAudio2CallBack\VoiceCallBack.h"

struct IXAudio2SourceVoice;

class CXAudio2PlaySE
{
private:
	static const int SE_VOICE_SIZE = 20;
public:
	// �R���X�g���N�^.
	CXAudio2PlaySE();

	// �f�X�g���N�^.
	virtual ~CXAudio2PlaySE();

	const bool IsPlayingSE(const size_t ArrayNum);

	// �T�E���h�Đ��A�O�Ɉꎞ��~����Ă����ꍇ�͋L���ʒu����ĊJ.
	virtual bool Play(
		std::shared_ptr<COggLoad> pWavData);

	bool NoMultiplePlay(std::shared_ptr<COggLoad> pWavData);

	// SE�T�E���h��~.
	bool SeStop(const size_t ArrayNum);
	// �쐬���Ă�\������SE����~.
	bool AllSeStop();

	HRESULT CreateOggSound(std::shared_ptr<COggLoad> pOggData, const char* filename);
	//=======================================================================
	// ��{���ʑ���n.
	//====

	// SE��Voice�\�[�X���ő剹�ʂ�ݒ�.
	void SetMaxSEVolume(const float& fMaxVolume) { m_fSEVolume = fMaxVolume; }
	// SE��Voice�\�[�X���ő剹�ʂ��擾.
	const float GetMaxSEVolume() { return m_fSEVolume; }
	// SE�̉��ʂ��擾���܂�.
	const float GetSEVolume(size_t Array);
	// SE�̉��ʂ�ݒ肵�܂�.
	bool SetSEVolume(float value, size_t Array);

	// SoundSource��j�󂷂�֐�.
	void DestoroySource();

	// �I�v�V������ʂ�SE�̉��ʂ������āASE�\�[�X���̉��ʂ�����ɍ��킹�ĉ����邪.
	// ���̌�SE���グ�Ă��������ɔ��f�ł��Ȃ��̂ŁA�����������t���O.
	void SetOperationSEVolumeFlag(const bool& bFlag) { m_bCanChangeSE = bFlag; }
	// �S��SE���ʂƂ͕ʂ̃\�[�X���̉��ʂ��Z�b�g.
	void SetAnotherSEVolume(const float& fVolume) { m_fAnotherVolume = fVolume; }
	// �\�[�X���Őݒ肳��Ă��鉹�ʂ�K�p���邩�ǂ���.
	void SetUseAnotherSEVolume(const float& bFlag) { m_bisAnotherVolume = bFlag; }
protected:
	void	InitSEFlag();
	int		CheckPlaySeState();
	int		CheckPlaySeBuffer();
protected:
	COggLoad*				m_pOggData; // Ogg�f�[�^���[�h�N���X.

	IXAudio2SourceVoice*	m_pSourceVoice[SE_VOICE_SIZE];		// ����SE�𓯎��ɂȂ点��悤�ɔz��ɂ���.
	bool				m_isSePlayVoice[SE_VOICE_SIZE];			// SE�Ǘ��p.
	bool				m_bisAnotherVolume;
	float				m_fAnotherVolume;
	int					m_CheckNonePlaySeVoice;					// �Đ����Ă��Ȃ�SE��􂢏o���p.
	int					m_RepeatPlaySe;							// �S�����Đ����̎��Ɏ��Ɋ����߂��čĐ�����z��ԍ���Ԃ�.		

	float m_fSEVolume;		// SE����.
	bool  m_bCanChangeSE;	// SE���ʂ�������ɂ��グ���邩�ǂ���.
};

#endif // #ifndef XAUDIO2_PLAYSE_H