/*  
 *  Copyright (c) 2005, ���մ����Ƽ����޹�˾
 *  All rights reserved.
 *  
 *  �ļ�����: CAVIReader.h
 *  �ļ���ʶ: 
 *  ժ    Ҫ: ��AVI�ļ�ģ��,֧�ִ󲿷ֵ�AVI�ļ�
 *  
 *  ��ǰ�汾: 
 *  ��    ��: Tommy
 *  �������: 2009-11-6 10:15:59
 *  ������ע: 
 *		1.֧�ֶ�ȡ˽������
 *		2.֧����ǰ/���Ѱ��I֡
 */

#ifndef __CAVIREADER_H__
#define __CAVIREADER_H__

//�����붨��
#define CAVIREADER_E_OK					0
#define CAVIREADER_E_UNKNOWN			-1			//δ�������
#define CAVIREADER_E_FILE_OPEN			-2			//���ļ�����
#define CAVIREADER_E_MEMORY				-3			//�ڴ治��
#define CAVIREADER_E_FILE_READ			-4			//��ȡ�ļ�����
#define CAVIREADER_E_FILE_FORMAT		-5			//�ļ���ʽ����
#define CAVIREADER_E_FILE_SEEK			-6			//��λ�ļ�ʧ��
#define CAVIREADER_E_STREAMCOUNT		 -7			//����Ŀ����,
													//���ܹ�����CAVIREADER_MAX_STREAM_NUM
#define CAVIREADER_E_START				-8			//��ʼλ�ô���
#define CAVIREADER_E_OVERFLOW			-9			//���岻��,���߲���
#define CAVIREADER_E_STREAMTYPE			-10			//�����ʹ���

//֧�ֵ��������Ŀ
#define	CAVIREADER_MAX_STREAM_NUM		4		
//֧�ֵ�˽�����ݳ���
#define CAVIREADER_PRIVATEDATA_LEN		1024

//ý�����ͺ궨��
#define CAVIREADER_mmioFOURCC( ch0, ch1, ch2, ch3 ) \
	( (unsigned char)(ch0) | ( (unsigned char)(ch1) << 8 ) |	\
	( (unsigned char)(ch2) << 16 ) | ( (unsigned char)(ch3) << 24 ) )
#define CAVIREADER_streamtypeVIDEO	CAVIREADER_mmioFOURCC('v', 'i', 'd', 's')
#define CAVIREADER_streamtypeAUDIO	CAVIREADER_mmioFOURCC('a', 'u', 'd', 's')
#define CAVIREADER_streamtypeMIDI	CAVIREADER_mmioFOURCC('m', 'i', 'd', 's')
#define CAVIREADER_streamtypeTEXT	CAVIREADER_mmioFOURCC('t', 'x', 't', 's')

//�������
typedef void* CAVIREADER_HANDLE;
typedef void* CAVIREADER_STREAMHANDLE;

//�������Ͷ���
#define CAVIREADER_AVIINDEX_FLAG_NULL			0x00	//��I֡
#define CAVIREADER_AVIINDEX_FLAG_KEYFRAME		0x10	//I֡

//AVIͷ����,Ӧ����һ�㲻��Ҫ�������
typedef struct _TCAVIReaderMainHeader {
    unsigned long dwMicroSecPerFrame;
    unsigned long dwMaxBytesPerSec;
    unsigned long dwReserved1;
    unsigned long dwFlags;
    unsigned long dwTotalFrames;
    unsigned long dwInitialFrames;
    unsigned long dwStreams;
    unsigned long dwSuggestedBufferSize;
    unsigned long dwWidth;
    unsigned long dwHeight;
    unsigned long dwReserved2;
    unsigned long dwReserved3;
    unsigned long dwReserved4;
    unsigned long dwReserved5;
} TCAVIReaderMainHeader;

//����Ϣͷ����
typedef struct _TCAVIReaderStreamHeader {
	unsigned long fccType;				//������,��ǰ��궨��CAVIREADER_streamtypeVIDEO��
    unsigned long fccHandler;			//ѹ���㷨
    unsigned long dwFlags;				//���岻��,һ��Ϊ0
    unsigned long dwPriority;			//���岻��,һ��Ϊ0
    unsigned long dwInitialFrames;		//���岻��,����д����0
    unsigned long dwScale;				//֡�ʵĳ�����,dwRate/dwScale����ʵ�ʵ�֡��
    unsigned long dwRate;				//֡��,dwRate/dwScale����ʵ�ʵ�֡��,��Ƶ����Ƶ����Ч
    unsigned long dwStart;				//���岻��,һ��Ϊ0
    unsigned long dwLength;				//֡������
    unsigned long dwSuggestedBufferSize;//���֡����
    unsigned long dwQuality;			//���岻��,���������0
    unsigned long dwSampleSize;			//��ƵΪ0,��ƵΪ֡����
	unsigned long dwReserved2;			//����,һ��Ϊ0
	unsigned long dwReserved3;			//����,һ��Ϊ0
//	RECT   rcFrame;
} TCAVIReaderStreamHeader;

//��Ƶ����ʽ����
typedef struct _TCAVIReaderBMPInfoHeader {
	unsigned int   biSize;			//����ṹ�Ĵ�С,sizeof(TCAVIReaderBMPInfoHeader)
	unsigned int   biWidth;			//ͼ���ʵ�����ؿ��
	unsigned int   biHeight;		//ͼ���ʵ�����ظ߶�
	unsigned short biPlanes;		//��ɫ�����,һ��Ϊ1
	unsigned short biBitCount;		//��ɫλ��,һ��Ϊ24
	unsigned int   biCompression;	//ѹ���㷨,��TCAVIReaderStreamHeader�е�fccHandlerһ��,�ɲ���
	unsigned int   biSizeImage;		//ͼ��Ĵ�С,����biWidth*biHeight*biBitCount/8
	unsigned int   biXPelsPerMeter; //���岻��,��0
	unsigned int   biYPelsPerMeter;	//���岻��,��0
	unsigned int   biClrUsed;		//���岻��,��0
	unsigned int   biClrImportant;	//���岻��,��0	
} TCAVIReaderBMPInfoHeader; 

//��Ƶ����ʽ����
typedef struct _TCAVIReaderWaveFormatEx {
	unsigned short wFormatTag;		//��Ƶ�ı����㷨
	unsigned short nChannels;		//������,һ����1
	unsigned int   nSamplesPerSec;	//������,һ����8000
	unsigned int   nAvgBytesPerSec;	//ƽ���ֽ���.������Լ������
	unsigned short nBlockAlign;		//ÿ֡���ݵĳ���
	unsigned short wBitsPerSample;	//����λ��,һ����16
	unsigned short cbSize;			//��չ���ݳ���,��0
	unsigned short rsv;				//��չ����,��0
} TCAVIReaderWaveFormatEx;

#ifdef __cplusplus
extern "C" {
#endif

/*  ��������: ��AVI�ļ�
 *  ����˵��:
 *      CAVIREADER_HANDLE *phReader [OUT]:		ģ����
 *      const char *cpszFileName [IN]:			�ļ���
 *  ����ֵ: CAVIREADER_E_OK��ʾ�ɹ�,������ʾʧ��,���嶨���ǰ��궨�� 
 */
int CAVIReader_OpenFile(CAVIREADER_HANDLE *phReader, const char *cpszFileName);

/*  ��������: �ر��ļ�
 *  ����˵��:
 *      CAVIREADER_HANDLE *phReader [OUT]:		ģ����
 */
void CAVIReader_Close(CAVIREADER_HANDLE hReader);

/*  ��������: ��ȡ�ļ���Ϣ
 *  ����˵��:
 *      CAVIREADER_HANDLE hReader [IN]:			ģ����
 *      TCAVIReaderMainHeader *pfinfo [OUT]:	�ļ���Ϣ
 *  ����ֵ: CAVIREADER_E_OK��ʾ�ɹ�,������ʾʧ��,���嶨���ǰ��궨��
 *	��ע:	�����Ϣ�ϲ�һ�㲻��Ҫʹ��,��������Ҳ����ô��ȷ
 */
int CAVIReader_GetFileInfo(CAVIREADER_HANDLE hReader, TCAVIReaderMainHeader *pfinfo);

/*  ��������: ��ȡ˽������
 *  ����˵��:
 *      CAVIREADER_HANDLE hReader [IN]:			ģ����
 *      char binData[CAVIREADER_PRIVATEDATA_LEN] [OUT]:	˽������
 *  ����ֵ: int 
 *  ��ע:	
 */
int CAVIReader_GetPrivateData(CAVIREADER_HANDLE hReader, char binData[CAVIREADER_PRIVATEDATA_LEN]);

/*  ��������: ��ȡ�ļ��е���
 *  ����˵��:
 *      CAVIREADER_HANDLE hReader [IN]:			ģ����
 *      CAVIREADER_STREAMHANDLE *phStream [OUT]:�����
 *      unsigned long fccType [IN]:				Ҫ��ȡ����������,
 *												�����ǰ��ĺ궨��,streamtypeVIDEO��
 *      long lParam [IN]:						�����ִ���
 *  ����ֵ: CAVIREADER_E_OK��ʾ�ɹ�,������ʾʧ��,���嶨���ǰ��궨�� 
 *  ��ע: ѭ�����ô˺���,ֱ������CAVIREADER_E_STREAMCOUNT���ɱ����ļ���������
 */
int CAVIReader_GetStream(CAVIREADER_HANDLE hReader, 
						 CAVIREADER_STREAMHANDLE *phStream, unsigned long fccType, long lParam);

/*  ��������: ��ȡ����Ϣ
 *  ����˵��:
 *      CAVIREADER_STREAMHANDLE hStream [IN]:	�����
 *      TCAVIReaderStreamHeader *psinfo [OUT]:	����Ϣ
 *  ����ֵ: CAVIREADER_E_OK��ʾ�ɹ�,������ʾʧ��,���嶨���ǰ��궨�� 
 */
int CAVIReader_GetStreamInfo(CAVIREADER_STREAMHANDLE hStream, TCAVIReaderStreamHeader *psinfo);

/*  ��������: ��ȡ����ʽ
 *  ����˵��:
 *      CAVIREADER_STREAMHANDLE hStream [IN]:	�����
 *      void *pFormat [OUT]:					���������Ͳ�ͬ����ͬ,��������:
 *													streamtypeVIDEOΪ�ṹTCAVIReaderBMPInfoHeader
 *													streamtypeAUDIOΪ�ṹTCAVIReaderWaveFormatEx
 *													streamtypeMIDI��streamtypeTEXT��֧��
 *      long cbFormat [IN]:						pFormat���ֽ���
 *  ����ֵ: CAVIREADER_E_OK��ʾ�ɹ�,������ʾʧ��,���嶨���ǰ��궨�� 
 *  ��ע: ���cbFormat�������Ͳ���Ӧ,��᷵��CAVIREADER_E_OVERFLOW����
 */
int CAVIReader_GetStreamFormat(CAVIREADER_STREAMHANDLE hStream, void *pFormat, long cbFormat);

/*  ��������: ��ȡ��������
 *  ����˵��:
 *      CAVIREADER_STREAMHANDLE hStream [IN]:	�����
 *      char szName[64] [OUT]:					��������
 *  ����ֵ: CAVIREADER_E_OK��ʾ�ɹ�,������ʾʧ��,���嶨���ǰ��궨�� 
 */
int CAVIReader_GetStreamName(CAVIREADER_STREAMHANDLE hStream, char szName[64]);

/*  ��������: ��ȡ����֡��
 *  ����˵��:
 *      CAVIREADER_STREAMHANDLE hStream [IN]:	�����
 *  ����ֵ: ����֡��
 */
int CAVIReader_GetLength(CAVIREADER_STREAMHANDLE hStream);

/*  ��������: ��ȡ֡����
 *  ����˵��:
 *      CAVIREADER_STREAMHANDLE hStream [IN]:	�����
 *      long lStart [IN]:						��ȡ֡�����
 *      long lSamples [IN]:						��ȡ��֡����Ŀ,Ŀǰ֧��1
 *      void *pBuffer [OUT]:					��ȡ����֡����
 *      long cbBuffer [IN]:						pBuffer�Ĵ�С,
 *												���С��֡��ʵ�ʴ�С������CAVIREADER_E_OVERFLOW
 *      long *plBytes [OUT]:					ʵ�ʶ�ȡ����֡��С
 *      long *plSamples [OUT]:					ʵ�ʶ�ȡ��֡��Ŀ,Ŀǰ����Ϊ1
 *		long *plFlag [OUT]:						��ȡ����֡������,�����ǰ����������ͺ궨��
 *  ����ֵ: CAVIREADER_E_OK��ʾ�ɹ�,������ʾʧ��,���嶨���ǰ��궨�� 
 */
int CAVIReader_ReadSample(CAVIREADER_STREAMHANDLE hStream, long lStart, long lSamples, 
						  void *pBuffer, long cbBuffer, long *plBytes, long *plSamples, long *plFlag);

/*  ��������: Ѱ��ǰһ��I֡
 *  ����˵��:
 *      CAVIREADER_STREAMHANDLE hStream [IN]:	�����
 *      long lStart [IN]:						��ʼѰ�ҵ�֡��(������һ֡)
 *  ����ֵ: �ҵ���I֡�����,-1��ʾû���ҵ�
 *  ��ע:	�ڳɹ��ҵ�֮��,�ϲ�Ҫ��ǰ��һ��֡�Ž���Ѱ��,�����ԭ����ѭ����.	
 */
long CAVIReader_FindPrevKeySample(CAVIREADER_STREAMHANDLE hStream, long lStart);

/*  ��������: Ѱ�Һ�һ��I֡
 *  ����˵��:
 *      CAVIREADER_STREAMHANDLE hStream [IN]:	�����
 *      long lStart [IN]:						��ʼѰ�ҵ�֡��(������һ֡)
 *  ����ֵ: �ҵ���I֡�����,-1��ʾû���ҵ�
 *  ��ע:	�ڳɹ��ҵ�֮��,�ϲ�Ҫ�����һ��֡�Ž���Ѱ��,�����ԭ����ѭ����.	
 */
long CAVIReader_FindNextKeySample(CAVIREADER_STREAMHANDLE hStream, long lStart);

#ifdef __cplusplus
}
#endif

#endif //__CAVIREADER_H__
