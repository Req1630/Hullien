#include "XAudio2WaveLoad.h"
#include <crtdbg.h>
#include <stdio.h>
#include <tchar.h>

clsXAudio2WaveLoad::clsXAudio2WaveLoad()
	: m_pFile				( nullptr )
	, m_hasGotWaveFormat	( false )
	, m_firstSampleOffset	( -1 )
	, m_dataChunkSize		( 0 )
	, m_dataChunkSamples	( 0 )
{
	ZeroMemory(&m_WaveFormat, sizeof(m_WaveFormat));
}

clsXAudio2WaveLoad::~clsXAudio2WaveLoad()
{
	Close();
}
// �I�[�v��
bool clsXAudio2WaveLoad::Open(const char* filename)
{
	if (m_pFile) return false;
	std::string Filename = filename;
	// BGM�f�[�^������f�B���N�g���p�X������.
	std::string BGMPass = "Data\\Sound\\BGM\\" + Filename;
	BGMPass += ".wav";
	LPSTR CastLPSTR = const_cast<char*>(BGMPass.c_str());
	
	// CastLPSTR���G���[�l�Ȃ�SE�̃t�@�C���̖��O�̉\��������̂ŁASE�̃f�B���N�g���p�X�Ō���.
	if (_tfopen_s(&m_pFile, CastLPSTR, _T("rb")) != 0) {
		std::string SEPass = "Data\\Sound\\SE\\" + Filename;
		SEPass += ".wav";
		LPSTR CastLPSTR = const_cast<char*>(SEPass.c_str());
		if (_tfopen_s(&m_pFile, CastLPSTR, _T("rb")) != 0) {
			// �f�[�^�����݂��Ȃ�.
			return false;
		}
	}

	Tmp = filename;
	return true;
}


// �t�H�[�}�b�g�����擾.
bool clsXAudio2WaveLoad::GetWaveFormat(const char* filename)
{
	Open(filename);

	// �I�[�v���ς݂�.
	if (!m_pFile) return NULL;

	if (!m_hasGotWaveFormat)
	{
		long offset = 12;

		while (1)
		{
			// �`�����N�擪�ֈړ�.
			if (fseek(m_pFile, offset, SEEK_SET) != 0) break;

			// �`�����N�V�O�l�`����ǂݍ���.
			char chunkSignature[4] = { 0 };
			std::size_t readChars = 0;
			while (readChars < 4)
			{
				std::size_t ret = fread(chunkSignature + readChars, sizeof(char), 4 - readChars, m_pFile);
				if (ret == 0) break;
				readChars += ret;
			}

			// �`�����N�T�C�Y��ǂݍ���.
			uint32_t chunkSize = 0;
			if (fread(&chunkSize, sizeof(uint32_t), 1, m_pFile) == 0) break;

			// fmt �`�����N������������t�H�[�}�b�g����ǂݍ���.
			if (strncmp(chunkSignature, "fmt ", 4) == 0)
			{
				std::size_t readSize = chunkSize < sizeof(WAVEFORMATEX) ? chunkSize : sizeof(WAVEFORMATEX);
				if (fread(&m_WaveFormat, readSize, 1, m_pFile) == 0) break;

				// PCM �̂Ƃ��͈ꉞ cbSize �� 0 �ɂ��Ă���.
				if (m_WaveFormat.wFormatTag == WAVE_FORMAT_PCM) m_WaveFormat.cbSize = 0;
				// �t�H�[�}�b�g���擾�ς�.
				m_hasGotWaveFormat = true;
			}

			// data �`�����N������������I�t�Z�b�g�ƃT�C�Y���L��.
			if (strncmp(chunkSignature, "data", 4) == 0)
			{
				m_firstSampleOffset = offset + 8;	// �V�O�l�`�� 4bytes �{ �T�C�Y 4bytes.
				m_dataChunkSize = chunkSize;
			}

			// ���̃`�����N��
			offset += (static_cast<long>(chunkSize) + 8);
		}

		if (!m_hasGotWaveFormat) return NULL;	// �ǂ����ŃG���[���N���Ă����ƏE���Ȃ�����.

		// �t�H�[�}�b�g��񂪎擾�ł����� data �`�����N���̃T���v�������v�Z.
		m_dataChunkSamples = m_dataChunkSize / m_WaveFormat.nBlockAlign;	// �K������؂��͂�.
	}

	return true;
}
// �T���v�������擾.
size_t clsXAudio2WaveLoad::GetSamples(const char* filename)
{
	// �I�[�v���ς݂�.
	if (!m_pFile) return 0;
	// �t�H�[�}�b�g�����擾���Ă��Ȃ���΂�����.
	if (!m_hasGotWaveFormat) GetWaveFormat(filename);

	return m_dataChunkSamples;
}

// Raw�f�[�^�ǂݍ���.
size_t clsXAudio2WaveLoad::ReadRaw(const std::size_t start, const std::size_t samples, void * buffer, const char* filename)
{
	// �o�b�t�@�A�h���X���s���ł͂Ȃ���.
	if (!buffer) {
		return 0;
		_ASSERT_EXPR(false, L"Raw�f�[�^�̃o�b�t�@�A�h���X���s���l�ł�");
	}
	// �I�[�v���ς݂�.
	if (!m_pFile) return 0;
	// �t�H�[�}�b�g�����擾���Ă��Ȃ���΂�����.
	if (!m_hasGotWaveFormat)
	{
		if (!GetWaveFormat(filename)) return 0;
	}
	// �J�n�ʒu���I�[�o�[���Ă��Ȃ���.
	if (start >= m_dataChunkSamples) return 0;

	// ���ۂɓǂݍ��ރT���v�������v�Z.
	std::size_t actualSamples = start + samples > m_dataChunkSamples ? m_dataChunkSamples - start : samples;

	// �ǂݍ��݊J�n�ʒu�ֈړ�.
	if (fseek(m_pFile, m_firstSampleOffset + start * m_WaveFormat.nBlockAlign, SEEK_SET) != 0) return 0;
	// �ǂݍ���.
	std::size_t readSamples = 0;
	while (readSamples < actualSamples)
	{
		std::size_t ret = fread(reinterpret_cast<uint8_t *>(buffer) + readSamples * m_WaveFormat.nBlockAlign,
			m_WaveFormat.nBlockAlign,
			actualSamples - readSamples,
			m_pFile);
		if (ret == 0) break;
		readSamples += ret;
	}

	return readSamples;
}

// ���K���ς݃f�[�^�ǂݍ���.
size_t clsXAudio2WaveLoad::ReadNormalized(const std::size_t start,
	const std::size_t samples, float * left, float * right, const char* filename)
{
	// ���Ȃ��Ƃ� 1ch �Ԃ�͎w�肳��Ă��邩.
	if (!left) {
		_ASSERT_EXPR(false, L"���K���ς݃f�[�^�ǂݍ��݂Ɏ��s���܂���");
	}
	// �I�[�v���ς݂�.
	if (!m_pFile) return 0;
	// �t�H�[�}�b�g�����擾���Ă��Ȃ���΂�����.
	if (!m_hasGotWaveFormat)
	{
		if (!GetWaveFormat(filename)) return 0;
	}
	// �J�n�ʒu���I�[�o�[���Ă��Ȃ���.
	if (start >= m_dataChunkSamples) return 0;

	// ���ۂɓǂݍ��ރT���v�������v�Z.
	std::size_t actualSamples = start + samples > m_dataChunkSamples ? m_dataChunkSamples - start : samples;

	return 0;
}

// �N���[�Y.
bool clsXAudio2WaveLoad::Close()
{
	if (m_pFile)
	{	
		fclose(m_pFile);
		m_pFile = NULL;
		m_hasGotWaveFormat = false;
		m_firstSampleOffset = -1;
		m_dataChunkSize = 0;
		m_dataChunkSamples = 0;
	}
	return true;
}

const WAVEFORMATEX& clsXAudio2WaveLoad::GetWaveFmtEx()const
{
	return m_WaveFormat;
}
