// DllManager.cpp: implementation of the DllManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DllManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//char pPCM[1024 * 1024*20];

DllManager::DllManager()
{
	m_phReader = NULL;
	m_phStreamVR = NULL;
	m_phStreamAR = NULL;

	m_phWriter = NULL;
	m_phStreamVW = NULL;

	m_SampleNOV = 0;
	m_SampleNOA = 0;

	m_uiFileLen = 0;
	m_uiReadLen = 0;

	m_bRun = FALSE;
    m_szFlag = FALSE;

	CR_AUDIODEC_INIT_PARAM audioDecInitParam;
	audioDecInitParam.MaxPCMLen = 4096;
	m_hCRAudioDec = CR_AudioDec_Init(&audioDecInitParam);
	Start();
}

DllManager::~DllManager()
{
	Stop();
	//Sleep(1000);
	CloseFile();
}

int DllManager::CAVI2AVI(const char *pCAVIPath, const char *pAVIPath ,float ReadLen,int SampleV,int SampleA,HWND hwnd)
{
	m_uiReadLen = ReadLen;
	m_SampleNOV = SampleV;
	m_SampleNOA = SampleA;
	m_szHwnd = hwnd;
	FILE *fp = fopen(pCAVIPath, ("rb+"));
	fseek(fp, 0, SEEK_END);
	m_uiFileLen = ftell(fp);
	fclose(fp);

	int rv = CAVIReader_OpenFile(&m_phReader, pCAVIPath);
	if (rv != 0)
	{
		return -1;
	}

	CAVIWriter_Init(&m_phWriter, 0);
	rv = CAVIWriter_CreateFile(m_phWriter, pAVIPath);
	if (rv != 0)
	{
		return -1;
	}

	rv = CAVIReader_GetStream(m_phReader, &m_phStreamVR, CAVIREADER_streamtypeVIDEO, 0);
	if (rv == 0)
	{
		TCAVIReaderStreamHeader sinfoV;
		TCAVIReaderBMPInfoHeader formatV;
		rv = CAVIReader_GetStreamInfo(m_phStreamVR, &sinfoV);
		rv = CAVIReader_GetStreamFormat(m_phStreamVR, &formatV, sizeof(TCAVIReaderBMPInfoHeader));

		TCAVIWriterStreamInfo Info;
		Info.dwScale = 1;
		Info.dwRate = sinfoV.dwRate*Info.dwScale;
		Info.fccHandler = 0x34363248;//lc VIDEO_FOURCC_CREARO_R264;
		Info.fccType = CAVIWRITER_streamtypeVIDEO;
		strcpy(Info.szName, "Video");

		TAVIWriterBMPInfoHeader Format;
		Format.biBitCount = formatV.biBitCount;
		Format.biSize = formatV.biSize;
		Format.biWidth = formatV.biWidth;
		Format.biHeight = formatV.biHeight;
		Format.biCompression = formatV.biCompression; //压缩算法,跟TCAVIWriterStreamInfo中的fccHandler一样,可不填
		Format.biPlanes = formatV.biPlanes;
		Format.biXPelsPerMeter = formatV.biXPelsPerMeter;
		Format.biYPelsPerMeter = formatV.biYPelsPerMeter;
		Format.biClrImportant = formatV.biClrImportant;
		Format.biClrUsed = formatV.biClrUsed;
		Format.biSizeImage = formatV.biSizeImage;

		rv = CAVIWriter_CreateStream(m_phWriter, &m_phStreamVW, &Info, &Format, sizeof(TAVIWriterBMPInfoHeader));
	}

	rv = CAVIReader_GetStream(m_phReader, &m_phStreamAR, CAVIREADER_streamtypeAUDIO, 0);
	if (rv == 0)
	{
		TCAVIReaderStreamHeader sinfoA;
		TCAVIReaderWaveFormatEx formatA;
		rv = CAVIReader_GetStreamInfo(m_phStreamAR, &sinfoA);
		int ss = sizeof(sinfoA);
		rv = CAVIReader_GetStreamFormat(m_phStreamAR, &formatA, sizeof(TCAVIReaderWaveFormatEx));

		TCAVIWriterStreamInfo Info;
		Info.dwScale = 1;
		Info.dwRate = 8000;
		Info.fccHandler = 1;
		Info.fccType = sinfoA.fccType;
		strcpy(Info.szName, "Audio");

		TAVIWriterWaveFormatEx Format;
		Format.cbSize = 2;//formatA.cbSize; 
		Format.rsv = formatA.rsv;
		Format.wFormatTag = 1;
		Format.nChannels = 1;
		Format.nSamplesPerSec = formatA.nSamplesPerSec;
		Format.nAvgBytesPerSec = CAVIReader_GetLength(m_phStreamAR) * 2;//0;//16000;
		Format.nBlockAlign = 2;
		Format.wBitsPerSample = formatA.wBitsPerSample;

		rv = CAVIWriter_CreateStream(m_phWriter, &m_phStreamAW, &Info, &Format, sizeof(TAVIWriterWaveFormatEx));
	}
	m_bRun = TRUE;
	return 0;
}

void DllManager::Run()
{
	while (!m_bStop)
	{
		if (!m_szFlag)
		{
			Sleep(100);
			continue;
		}
		int nDataLen = 1024 * 1024 * 2;
		long lBytes;
		long lSamples;
		long lFlag;
		int rv = 0;
		if (m_phStreamVR != NULL)
		{
			rv = CAVIReader_ReadSample(m_phStreamVR, m_SampleNOV, 1, pFrmRead, nDataLen, &lBytes, &lSamples, &lFlag);
			if (rv == 0)
			{
				m_uiReadLen += lBytes;
				m_SampleNOV ++;
				if (lBytes == 28)
				{
					char pEmptyH264[4] = { 0 };
					rv = CAVIWriter_WriteSample(m_phStreamVW, pEmptyH264, 4, lFlag);
				}
				else
				{
					rv = CAVIWriter_WriteSample(m_phStreamVW, (const char *)pFrmRead + 40, lBytes - 40, lFlag);
				}
			}
			else if (rv == CAVIREADER_E_START)
			{
				m_uiReadLen = m_uiFileLen ;
				CloseFile();
				continue;
			}
		}

		if (m_phStreamAR != NULL)
		{
			rv = CAVIReader_ReadSample(m_phStreamAR, m_SampleNOA, 1, pFrmRead, nDataLen, &lBytes, &lSamples, &lFlag);
			if (rv == 0)
			{
				m_SampleNOA++;
				m_uiReadLen += lBytes;
				int j;
				for (j = 32; j != lBytes; j++)
				{
					if (pFrmRead[j] == 0xFF)
					{
						continue;
					}
					else
					{
						break;
					}
				}

				if (j == lBytes)
				{
					char pEmptyPCM[160] = { 0 };
					for (int k = 0; k != 9; k++)
					{
						rv = CAVIWriter_WriteSample(m_phStreamAW, pEmptyPCM, 160, lFlag);
					}
					continue;
				}

			    static CR_AUDIODEC_FRAME_PARAM audioDecParam;
				audioDecParam.Alg = pFrmRead[28];
				short sAlgFrmNum = *(short *)(pFrmRead + 18);

				int nOffset = 12 + 16;
				int nFrmLen = (lBytes - nOffset) / sAlgFrmNum;
				for (int i = 0; i != sAlgFrmNum; i++)
				{
					audioDecParam.pBuffer = pFrmRead + nOffset + 4;
					audioDecParam.Length = nFrmLen - 4;
					nOffset += nFrmLen;
					rv = CR_AudioDec_DecFrm(m_hCRAudioDec, &audioDecParam);
					if (rv == 0)
					{
						rv = CAVIWriter_WriteSample(m_phStreamAW, (char *)audioDecParam.pPCM, audioDecParam.PCMLen, lFlag);
					}
				}	
			}
		}
		Sleep(1);
	}
	//Unlock();
	//return 0;
}

int DllManager::GetProgress(int *pnProgress)
{
	if (m_uiFileLen == 0)
	{
		*pnProgress = 0;
		return 0;
	}
	*pnProgress = m_uiReadLen / m_uiFileLen * 100;
	return 0;
}

void DllManager::CloseFile()
{
	CR_AudioDec_Close(m_hCRAudioDec);
	m_hCRAudioDec = NULL;
	m_bStop = TRUE;
	if (m_phReader != NULL)
	{
		CAVIReader_Close(m_phReader);
		m_phReader = NULL;
		m_phStreamVR = NULL;
		m_phStreamAR = NULL;
		m_SampleNOV = 0;
		m_SampleNOA = 0;
	}
	if (m_phWriter != NULL)
	{
		CAVIWriter_CloseFile(m_phWriter);
		CAVIWriter_Close(m_phWriter);
		m_phWriter = NULL;
		m_phStreamVW = NULL;
	}
	m_bRun = FALSE;
}