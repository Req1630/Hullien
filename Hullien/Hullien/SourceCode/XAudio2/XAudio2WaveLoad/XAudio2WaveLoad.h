#ifndef C_XAUDIO2_WAVELOAD_H
#define C_XAUDIO2_WAVELOAD_H

#include "..\XAudio2.h"
#include <string>
class clsXAudio2WaveLoad
{
public:
	clsXAudio2WaveLoad();
	~clsXAudio2WaveLoad();

	// WAVE �t�H�[�}�b�g���擾.
	const WAVEFORMATEX&     GetWaveFmtEx()const;
	// WAVE�f�[�^�I�[�v���֐�.
	bool Open(const char * filename);
	// �t�H�[�}�b�g�����擾.
	bool GetWaveFormat(const char* filename);
	// �T���v�������擾.
	size_t GetSamples(const char* filename);
	// Raw�f�[�^�ǂݍ���.
	size_t ReadRaw(const std::size_t start, const std::size_t samples, void * buffer, const char* filename);
	// ���K���ς݃f�[�^�ǂݍ���.
	size_t ReadNormalized(const std::size_t start, const std::size_t samples, float * left, float * right, const char* filename);

	const char* GetFileName() {
		return Tmp;
	}
	bool Close();
private:
	const char* Tmp;

	// �t�@�C���n���h��.
	FILE* m_pFile;
	// �t�H�[�}�b�g�����擾�ς݂�.
	bool m_hasGotWaveFormat;
	// �t�H�[�}�b�g���.
	WAVEFORMATEX m_WaveFormat;
	// data �`�����N���擪�T���v���ւ̃I�t�Z�b�g.
	long m_firstSampleOffset;
	// data �`�����N�T�C�Y.
	size_t m_dataChunkSize;
	// data �`�����N���T���v����.
	size_t m_dataChunkSamples;
};

#endif //#ifndef C_XAUDIO2_WAVELOAD_H