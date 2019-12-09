/*  
 *  Copyright (c) 2005, ���մ����Ƽ����޹�˾
 *  All rights reserved.
 *  
 *  �ļ�����: VARender.h
 *  �ļ���ʶ: 
 *  ժ    Ҫ:	
 *		�����Ƶ����Ƶ�Ľ�����ʾ���ź�ͬ������.
 *  ��ǰ�汾: 
 *  ��    ��: Tommy
 *  �������: 2007-7-25 11:31:37
 *  ������ע: 
 *  
 */

#ifndef __VARENDER_H__
#define __VARENDER_H__

//�����붨��
#define VARENDER_E_OK				0	//�ɹ�
#define VARENDER_E_WOULDBLOCK		1	//�����޷�����ִ��
#define VARENDER_E_UNKOWN			-1	//δ֪����
#define VARENDER_E_FRAMEINFOCHANGE	-2	//֡��Ϣ�����ı�
#define VARENDER_E_UNKOWNFRAMEINFO	-3	//��֧�ֵ�֡��Ϣ
#define VARENDER_E_TIMER			-4	//������ʱ��ʧ��
#define VARENDER_E_INVALID			-5	//��Դ������

//��Ƶ�����������
#define VARENDER_CHANNEL_DUAL		0	//˫�������
#define VARENDER_CHANNEL_LEFT		1	//���������
#define VARENDER_CHANNEL_RIGHT		2	//���������

//����Ƶ�㷨����
#define VARENDER_VIDEO_ALG_NULL		0
#define VARENDER_VIDEO_ALG_H264		1
#define VARENDER_AUDIO_ALG_G7231	2
#define VARENDER_VIDEO_ALG_S264		3
#define VARENDER_AUDIO_ALG_ADPCM	4
#define VARENDER_VIDEO_ALG_MPEG4	5
#define VARENDER_VIDEO_ALG_MJPEG	6	//��֧��DDraw
#define VARENDER_AUDIO_ALG_AMR		7
#define VARENDER_AUDIO_ALG_G711A	8
#define VARENDER_AUDIO_ALG_G711U	9
#define VARENDER_AUDIO_ALG_AAC		10

//��Ƶ�ֱ��ʶ���
#define VARENDER_VIDEO_SIZE_CIF		1
#define VARENDER_VIDEO_SIZE_QCIF	2
#define VARENDER_VIDEO_SIZE_D1		3

//����Ƶ���ݸ�ʽ
#define VARENDER_FORMAT_RGB			1	//RGB
#define VARENDER_FORMAT_YUV420		2	//YUV420,JPEG��֧�����ָ�ʽ�����������

//¼���ץ���ļ�������󳤶�
#define VARENDER_MAXLEN_FILENAME	256

//�ӿڽṹ����
typedef struct _TVARVideoFrameInfo {
	unsigned char ucAlg;			//�����㷨(1)
	unsigned char ucIFlag;			//I֡��ʶ(1)
	unsigned char ucSize;			//ͼ���С(1)
	unsigned char ucRsv;			//����(1)	���ڿ�ʼ����7.0�Ŀ��Ʊ�ʶ
									//��0λ		0��ʾͼ�񲻷�ת,1��ʾͼ��ת
									//��1~7λ	����λ,Ŀǰ��0
	unsigned int uiTimeStamp;		//ʱ���(4)
} TVARVideoFrameInfo;

typedef struct _TVARAudioFrameInfo {
	unsigned char ucAlg;			//�����㷨(1)
	unsigned char ucRsv;			//����(1)
	unsigned short usFrameNum;		//֡��Ŀ(2)
	unsigned int uiTimeStamp;		//ʱ���(4)
} TVARAudioFrameInfo;

typedef void* VARENDERUSER_HANDLE;
typedef void* VARENDER_HANDLE;
typedef void* VARENDER_HWND;
typedef void* VARENDER_HDC;

/*  ��������: ��ʾͼ���ʱ,�Զ����ͼ�Ļص������ӿ�
 *  ����˵��:
 *		VARENDERUSER_HANDLE hUser [IN]:			�Զ����û�����
 *		VARENDER_HDC hDC [IN]:					ͼ���DC
 *      int X [IN]:								ͼ�������X����
 *      int Y [IN]:								ͼ�������Y����
 *      int nWidth [IN]:						ͼ������Ŀ��
 *      int nHeight [IN]:						ͼ������ĸ߶�
 *      int X2 [IN]:							��ʾ�����X����
 *      int Y2 [IN]:							��ʾ�����Y����
 *      int nWidth2 [IN]:						��ʾ����Ŀ��
 *      int nHeight2 [IN]:						��ʾ����ĸ߶�
 */
typedef void (*VARENDER_DISPLAY_CALLBACK)(VARENDERUSER_HANDLE hUser, VARENDER_HDC hDC, 
										  int X, int Y, int nWidth, int nHeight,
										  int X2, int Y2, int nWidth2, int nHeight2);

/*  ��������: ��ȡ����Ƶ���ݵĻص�����
 *  ����˵��:
 *		VARENDERUSER_HANDLE hUser [IN]:			�Զ����û�����
 *      int nWidth [IN]:						��Ƶ�Ŀ��
 *      int nHeight [IN]:						��Ƶ�ĸ߶�
 *      *pBuf [IN]:								��Ƶ���ݵĻ����ַ
 *      int iBufLen [IN]:						��Ƶ���ݵĳ���
 *      char cFormat [IN]:						��Ƶ���ݵĸ�ʽ,��ǰ��ĺ궨��
 */
typedef void (*VARENDER_RAWVIDEO_CALLBACK)(VARENDERUSER_HANDLE hUser, 
										   int nWidth, int nHeight, 
										   char *pBuf, int iBufLen, char cFormat);

/*  ��������: ��Ƶ���ݻص�����
 *  ����˵��:
 *      VARENDERUSER_HANDLE hUser [IN]:			�Զ����û�����
 *      short *pPCMBuf [IN]:					PCM��Ƶ���ݻ���
 *      int nSampleNum [IN]:					PCM��Ƶ������
 *  ����ֵ: typedef void 
 *  ��ע:	
 */
typedef void (*VARENDER_AUDIO_CALLBACK)(VARENDERUSER_HANDLE hUser, short *pPCMBuf, int nSampleNum);

#ifdef VARENDER_EXPORTS
#define VARENDER_API __declspec(dllexport)
#else
#define VARENDER_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*  ��������: ��ʼ��VAReander��
 *  ����ֵ: ������,��ǰ��ĺ궨��
 *  ��ע: ����������ʱ������ȳ�ʼ�����ܹ�������������,ȫ�ֵ���һ�μ���.
 */
VARENDER_API int VARender_Startup();

/*  ��������: ���VARender��
 *  ��ע: �����˳�֮ǰ���ô˺���,������������Դй©
 */
VARENDER_API void VARender_Cleanup();

/*  ��������: ����VARender����
 *  ����˵��:
 *      VARENDER_HANDLE *phVARender [OUT]:	�����õ��Ķ�����
 *  ����ֵ: ������,��ǰ��ĺ궨��
 *	��ע: ���������ľ������ʹ��VARender_Close�ر�,���������ڴ�й©
 */
VARENDER_API int VARender_Open(VARENDER_HANDLE *phVARender);

/*  ��������: �ر�VARender����
 */
VARENDER_API void VARender_Close(VARENDER_HANDLE hVARender);

/*  ��������: �����Զ����ͼ�ص�����
 *  ����˵��:
 *      VARENDER_CALLBACK pCallBack [IN]:	��ͼ�ص�����
 *      VARENDER_HANDLE hUser [IN]:			�Զ����û�����
 */
VARENDER_API void VARender_SetDisplayCallBack(VARENDER_HANDLE hVARender, 
								VARENDER_DISPLAY_CALLBACK pCallBack, VARENDERUSER_HANDLE hUser);

/*  ��������: ���û�ȡ����Ƶ���ݵĻص�����
 *  ����˵��:
 *      VARENDER_CALLBACK pCallBack [IN]:	��ȡ����Ƶ���ݵĻص�����
 *      VARENDER_HANDLE hUser [IN]:			�Զ����û�����
 *		char cFormat [IN]:					��Ҫ�������ݸ�ʽ,Ŀǰֵ֧��YUV420
 */
VARENDER_API void VARender_SetRawVideoCallBack(VARENDER_HANDLE hVARender, 
								VARENDER_RAWVIDEO_CALLBACK pCallBack, VARENDERUSER_HANDLE hUser, char cFormat);

/*  ��������: ������Ƶ���ݻص�����
 *  ����˵��:
 *      VARENDER_AUDIO_CALLBACK pCallBack [IN]:	
 *											��Ƶ���ݻص�����
 *      VARENDER_HANDLE hUser [IN]:			�Զ����û�����
 */
VARENDER_API void VARender_SetAudioCallBack(VARENDER_HANDLE hVARender, 
								VARENDER_AUDIO_CALLBACK pCallBack, VARENDERUSER_HANDLE hUser);

/*  ��������: ������Ƶ�������
 *  ����˵��:
 *      int iLRChannel [IN]:			���������,ȡֵ��ǰ��ĺ궨��
 *  ��ע: ȱʡ���������������
 *		  ������Open֮��,��������֮ǰ�����������,֮���ܵ������������,���������ڴ���ʳ�ͻ
 */
VARENDER_API void VARender_SetAudioLRChannel(VARENDER_HANDLE hVARender, int iLRChannel);

/*  ��������: ������ʾ��
 *  ����˵��:
 *		int iNo [IN]:				��ʾ���ı��,��0(Ĭ����ʾ��)��ʼ,��ϵͳ֧�ֵ������ʾ����Ŀ
 *		int iX [IN]:				��ʾ������Ļ���Ͻ�X����
 *		int iY [IN]:				��ʾ������Ļ���Ͻ�Y����
 *  ����ֵ: ������,��ǰ��ĺ궨��
 *  ��ע: �����ʾ������ʹ��DDraw��ʱ��,��Ҫ�������.
 *		  �����ȵ���VARender_SetMonitor,�ٵ���VARender_SetWindow
 */
VARENDER_API int VARender_SetMonitor(VARENDER_HANDLE hVARender, int iNo, int iX, int iY);

/*  ��������: ������ʾ���ھ��
 *  ����˵��:
 *		VARENDER_HWND hWindow [IN]:		��ʾ���ھ��,����NULL��ʾ�ͷŴ���
 *  ����ֵ: ������,��ǰ��ĺ궨��
 *	��ע:
 *		�ڵ���VARender_PumpFrame����֮ǰ,�����ȵ��ô˺������ô��ھ��.
 *		���û�����þ��,����ʾ����ͼ��
 */
VARENDER_API int VARender_SetWindow(VARENDER_HANDLE hVARender, VARENDER_HWND hWnd);

/*  ��������: ������ʾ����
 *  ����˵��:
 *      int XDest [IN]:					x-coordinate of destination upper-left corner
 *      int YDest [IN]:					y-coordinate of destination upper-left corner
 *      int nDestWidth [IN]:			width of destination rectangle
 *      int nDestHeight [IN]:			height of destination rectangle
 *	��ע: Ĭ�ϵ�Ŀ��x,yΪ0,��Ⱥ͸߶�Ϊ���ڵĿ�Ⱥ͸߶�
 */
VARENDER_API void VARender_SetDestRect(VARENDER_HANDLE hVARender, int XDest, int YDest, int nDestWidth, int nDestHeight);

/*  ��������: ������ʾ����
 *  ����˵��:
 *      int XSrc [IN]:					x-coordinate of source upper-left corner
 *      int YSrc [IN]:					y-coordinate of source upper-left corner
 *      int nSrcWidth [IN]:				width of source rectangle
 *      int nSrcHeight [IN]:			height of source rectangle
 *	��ע: Ĭ�ϵ�Դx,yΪ0,��Ⱥ͸߶�Ϊͼ��ʵ�ʿ�Ⱥ͸߶�
 */
VARENDER_API void VARender_SetSrcRect(VARENDER_HANDLE hVARender, int XSrc, int YSrc, int nSrcWidth, int nSrcHeight);

/*  ��������: �����Ƿ�ʹ��DDraw
 *  ����˵��:
 *      int bDDraw [IN]:						�Ƿ�ʹDDraw,0��ʾ��ʹ��,1��ʾʹ��.
 *  ����ֵ: ������,��ǰ��ĺ궨��
 *  ��ע: ��������ô˺���,Ĭ��Ϊ��ʹ��DDraw,MJPEG�㷨��ʱ��֧��DDraw
 *		  �����ȵ���VARender_SetDDraw,�ٵ���VARender_SetWindow
 */
VARENDER_API int VARender_SetDDraw(VARENDER_HANDLE hVARender, int bDDraw);

/*  ��������: ��ȡ�Ƿ�ʹ��DDraw
 *  ����ֵ: �Ƿ�ʹDDraw,0��ʾ��ʹ��,1��ʾʹ��
 *  ��ע: Added by Terry.Wang, 2008/1/22 19:38:42
 */
VARENDER_API int VARender_IsDDraw(VARENDER_HANDLE hVARender);

/*  ��������: ʹ��ʹ��С�����,���������
 *	����˵��: �Ƿ�ʹ��,0��ʾ��ʹ��,1��ʾʹ��
 *			int bEnable[IN]:			�Ƿ�ʹ��ʹ�ñ������,0��ʾ��ʹ��,1��ʾʹ��
 *	��ע:	Ĭ�ϲ�ʹ�ñ������,
 *			���ӿ���Ҫ�ṩ��7.0���ʹ��,�ⲿ��������ô˽ӿ�,�ڲ������ж��Ƿ���Ҫ�����ڴ�
 *			�����Ӧ�õ����ĸ�¼��ӿ�
 */
VARENDER_API void VARender_EnableLitteDecodeLib(VARENDER_HANDLE hVARender, int bEnable);

/*  ��������: �����Ƿ�ʹ������Ӧ��볡
 *  ����˵��:
 *		int bEnable [IN]:					�Ƿ�ʹ������Ӧ��볡
 */
VARENDER_API void VARender_EnableHalfD1(VARENDER_HANDLE hVARender, int bEnable);

/*  ��������: ��ʼ/ֹͣ ����/��ʾ ��Ƶ/��Ƶ
 *  ��ע: 
 *		��ʼ��Ƶ����ʾ��Ƶ,������벻��ʾ��Ƶ
 *		��ʼ��Ƶ�򲥷���Ƶ,���򲻽��벥����Ƶ
 */
VARENDER_API void VARender_StartVideo(VARENDER_HANDLE hVARender);
VARENDER_API void VARender_StopVideo(VARENDER_HANDLE hVARender);
VARENDER_API void VARender_StartAudio(VARENDER_HANDLE hVARender);
VARENDER_API void VARender_StopAudio(VARENDER_HANDLE hVARender);

/*  ��������: ������Ƶ����/������7.0��Ƶ����֡
 *  ����˵��:
 *		char *pFrame [IN]:				����֡����.
 *		int iLen [IN]					����֡����
 *		TVARVideoFrameInfo *pvfinfo [IN]:��Ƶ֡��Ϣ
 *	����ֵ: VARENDER_E_OK			��ʾ�ɹ�
 *			VARENDER_E_WOULDBLOCK	��ʾ���ݻ�����,��Ҫ������������
 */
VARENDER_API int VARender_PumpVideoFrame(VARENDER_HANDLE hVARender, char *pFrame, int iLen, TVARVideoFrameInfo *pvfinfo);
VARENDER_API int VARender_PumpRawVideoFrame(VARENDER_HANDLE hVARender, char *pFrame, int iLen);

/*  ��������: ������Ƶ����/������7.0��Ƶ����֡
 *  ����˵��:
 *		char *pFrame [IN]:				����֡����.
 *		int iLen [IN]					����֡����
 *		TVARAudioFrameInfo *pafinfo [IN]:��Ƶ֡��Ϣ
 *	����ֵ: VARENDER_E_OK			��ʾ�ɹ�
 *			VARENDER_E_WOULDBLOCK	��ʾ���ݻ�����,��Ҫ������������
 */
VARENDER_API int VARender_PumpAudioFrame(VARENDER_HANDLE hVARender, char *pFrame, int iLen, TVARAudioFrameInfo *pafinfo);
VARENDER_API int VARender_PumpRawAudioFrame(VARENDER_HANDLE hVARender, char *pFrame, int iLen);

/*  ��������: ץͼ
 *  ����˵��:
 *      const char *pszFileName [IN]:	ץ���ļ�������ļ���
 */
VARENDER_API void VARender_Snapshot(VARENDER_HANDLE hVARender, const char *pszFileName);

/*  ��������: �ػ����һ��ͼ��
 *  ��ע: ����ʾ����ˢ�µ�ʱ������������,�����δ��ʾ����,���˺�������ֺ���
 */
VARENDER_API void VARender_Refresh(VARENDER_HANDLE hVARender);

/*  ��������: ��ȡ��ǰ��Ƶ��֡��
 *  ����ֵ: ��ǰ��Ƶ��֡��
 */
VARENDER_API int VARender_GetFrameRate(VARENDER_HANDLE hVARender);

/*  ��������: ��ȡ��ǰ��Ƶ������
 *  ����ֵ: ��ǰ��Ƶ������
 */
VARENDER_API int VARender_GetBitRate(VARENDER_HANDLE hVARender);

/*  ��������: ���õ�ǰ��Ƶ�Ƿ�ȷͬ������~
 */
VARENDER_API void VARender_SetbSync(VARENDER_HANDLE hVARender, int bSync);

/*  ��������: ��ȡ��ǰ��Ƶ�Ƿ�ȷͬ������
 *  ����ֵ: �Ƿ�ȷͬ������
 */
VARENDER_API int VARender_IsSync(VARENDER_HANDLE hVARender);

/*  ��������: �����Ƿ���ݻ�����ڲ����ٶ�
 *	��ע: ������ò����ݻ�����ڲ����ٶ�,����Ƶ����Ƶ���Զ���ȷͬ��
 */
VARENDER_API void VARender_SetAdjustSpeedByBuffer(VARENDER_HANDLE hVARender, int bAdjustSpeedByBuffer);

/*  ��������: ��ȡ��ǰ�Ƿ���ݻ�����ڲ����ٶ�
 *  ����ֵ: �Ƿ���ݻ�����ڲ����ٶ�
 */
VARENDER_API int VARender_IsAdjustSpeedByBuffer(VARENDER_HANDLE hVARender);

/*  ��������: ���ò����ٶ�
 *  ����˵��:
 *      int iSpeed [IN]:	�����ٶ�,֧��1,2,4����ֵ����ص�Ĭ��ֵ1
 *	��ע: ֻ���ڲ����ݻ�����ڲ����ٶ���Ч���������Ч
 *		  �����ٶȲ���1��ʱ����Զ�ֹͣ��Ƶ�Ĳ���
 */
VARENDER_API void VARender_SetSpeed(VARENDER_HANDLE hVARender, int iSpeed);

/*  ��������: ��ȡ��ǰ�����ٶ�
 *  ����ֵ: ��ǰ�����ٶ�
 */
VARENDER_API int VARender_GetSpeed(VARENDER_HANDLE hVARender);

/*  ��������: ��ȡ��Ƶ������ֽ���
 *  ����˵��:
 *      int *piMaxBufferBytes [OUT]:	��󻺳���ֽ���,����NULL��ʾ����Ҫ�������ֵ.
 *  ����ֵ: �Ѿ�������ֽ���
 */
VARENDER_API int VARender_GetVideoBufferBytes(VARENDER_HANDLE hVARender, int *piMaxBufferBytes);

/*  ��������: ��ȡ��Ƶ�����֡��
 *  ����˵��:
 *      int *piMaxBufferFrames [OUT]:	��󻺳��֡��,����NULL��ʾ����Ҫ�������ֵ.
 *  ����ֵ: �Ѿ������֡��
 */
VARENDER_API int VARender_GetAudioBufferFrames(VARENDER_HANDLE hVARender, int *piMaxBufferFrames);

/*  ��������: ��ȡ/������Ƶ��������,��Χ0-100
 */
VARENDER_API int VARender_GetVolume(VARENDER_HANDLE hVARender);
VARENDER_API void VARender_SetVolume(VARENDER_HANDLE hVARender, int iVolume);

/*  ��������: ��ȡ������ŵ���Ƶ/��Ƶ֡��ʱ���
 *  ����˵��:
 *      unsigned int *puiTimeStamp [OUT]:	������ŵ�ʱ���
 *  ����ֵ: VARENDER_E_OK			��ʾ�ɹ�
 *			VARENDER_E_INVALID		��û�в���֡
 */
VARENDER_API int VARender_GetLatestPlayVideoTimeStamp(VARENDER_HANDLE hVARender, unsigned int *puiTimeStamp);
VARENDER_API int VARender_GetLatestPlayAudioTimeStamp(VARENDER_HANDLE hVARender, unsigned int *puiTimeStamp);

/*  ��������: ��ȡ��ǰ��Ƶ��Ƶ���������ȵȵ��Բ���
 */
VARENDER_API int VARender_GetVideoListLen(VARENDER_HANDLE hVARender);
VARENDER_API int VARender_GetAudioListLen(VARENDER_HANDLE hVARender);
VARENDER_API int VARender_GetVADelta(VARENDER_HANDLE hVARender);
VARENDER_API int VARender_GetSilenceAudioFrameNum(VARENDER_HANDLE hVARender);
VARENDER_API int VARender_GetDropAudioFrameNum(VARENDER_HANDLE hVARender);

/*  ��������: ��ʼ/ֹͣ¼��
 *  ����˵��:
 *      const char *lpszFileName [IN]:		�ļ���,��󳤶Ȳ��ɴ���VARENDER_MAXLEN_FILENAME.
 *      unsigned int uiMaxFileTime [IN]:	¼���ļ������ʱ�䳤��,��λ��.�������Ȼ��Զ����ļ�
 *  ����ֵ: ������궨��.
 *	��ע: ���ɵ��ļ����Զ��Ӻ�׺"_00",���ļ���ʱ��,����,"_01","_02",...
 */
VARENDER_API int VARender_StartRecord(VARENDER_HANDLE hVARender, const char *lpszFileName, unsigned int uiMaxFileTime);
VARENDER_API void VARender_CloseRecord(VARENDER_HANDLE hVARender);

#ifdef __cplusplus
}
#endif

#endif //__VARENDER_H__
