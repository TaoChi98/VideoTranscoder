#pragma once
#include "stdafx.h"
#include <WinSock2.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <MMSYSTEM.H>

#include "../include/CAVIReader.h"
#include "../Include/CAVIWriter.h"
#include "../include/CRAudioEnc.h"
#include "../Include/XmVARender.h"
#include "../include/CR_AVDecoder.h"

#pragma  comment(lib,"../lib/CR_AVDecoder.lib")
#pragma  comment(lib,"../lib/XmVARender.lib")
#pragma  comment(lib,"../lib/CAVI2AVI.lib")
#pragma  comment(lib,"../lib/UtilitiesLib_VC60.lib")

enum PumpStreamType { PumpVideo, PumpRemoteAudio, PumpLocalAudio, PumpStreamNum };
typedef void* HANDLE_V2C;

typedef struct _STREAM_INFO
{
	CAVIWRITER_STREAMHANDLE hStream;
	unsigned int LastTmStampValidFlg; //��һ֡��ʱ����Ƿ���ڵı�ǣ�1����������������һ֡���ݣ�LastTmStamp��Ч��0����LastTmStamp��Ч
	unsigned int LastTmStamp; //��һ֡��ʱ���
	unsigned int LastFrmNO; //��һ֡��֡���
	unsigned int AudioFrmInterval; //ÿ����Ƶ֡��ʱ���ȣ���λms, ���ֵ��������Ƶ֡��Ч
	unsigned char *pEmptyFrm;
	int EmptyFrmLen;
	int NextSampleNO;  //��һ֡Ҫ��ȡ��Sample������
}STREAM_INFO;

typedef struct _STORAGE_READ_OBJ
{
	CAVIREADER_HANDLE hAVI;
	unsigned int BaseTmStamp; //��׼ʱ���
	STREAM_INFO Stream[PumpStreamNum];

	unsigned int VideoFrmRate; //ͼ��֡�ʡ�����PALһ��Ӧ����25������NTSCһ��Ӧ����30	
}STORAGE_READ_OBJ;

//�洢֡ͷ
typedef struct _CR_STORAGE_HEAD
{
	unsigned int UTC;
	unsigned int TimeStamp;
	unsigned char KeyFrm;
	unsigned char Rsv[3];
}CR_STORAGE_HEAD;

//��Ƶ֡��ͷ
typedef struct _CR_VIDEO_FRM_HEAD
{
	unsigned short Width;
	unsigned short Height;
	unsigned short ProducerID;
	unsigned char FrmRate;
	unsigned char Rsv1;
	unsigned int Rsv2;
	unsigned int Rsv3;
}CR_VIDEO_FRM_HEAD;

//��Ƶ֡��ͷ
typedef struct _CR_AUDIO_FRM_HEAD
{
	unsigned short BlockAlign;
	unsigned char Channels;
	unsigned char BitsPerSample;
	unsigned short SamplesPerSec;
	unsigned short AlgFrmNum;
	unsigned short ProducerID;
	unsigned short PCMLen;
	unsigned int Rsv1;
}CR_AUDIO_FRM_HEAD;

typedef struct _PUMP_DATA
{
	PumpStreamType StreamType;
	//	unsigned int TimeStamp;
	unsigned char *pData;
	int DataLen;
	//	unsigned int Flags; //ȡֵ��CAVIWRITER_AVIINDEX_FLAG_XXX�ĺ�
}PUMP_DATA; //��¼����Pump�������������

//��¼��ǰģ���״̬
enum WorkStatus {
	Reset, //ģ��մ���ʱ��״̬��AbsoluteBaseTmStamp����Ч��
	Cache, //��������ݼ����û�дﵽCacheTimeʱ���������״̬�����״̬ʱ��AbsoluteBaseTmStamp��Ч���һ�������ݵ�ʱ���������ǺϷ��ġ�
	Write, //���������㹻�������ݣ���flush�˻��������ݣ� ��ʼʵʱд��ÿһ֡ʱ�������״̬
};

typedef struct _STORAGE_WRITE_OBJ
{
	CAVIWRITER_HANDLE hAVI;
	WorkStatus Status;

	unsigned int AbsoluteBaseTmStamp; //���Ի�׼ʱ��������ֵֻ�ڵ�һ֡����ʱ���¼һ�Σ���������Ƿ��ļ�ʱ���ȳ���Ԥ��ֵ
	unsigned int CacheTime; //��¼����ʱ��

	PUMP_DATA *pPumpData; //��¼����ģ���û��д���ļ�������
	int MaxPumpDataNum; //�����Ч���ݽڵ�ĸ���
	int PumpDataNum; //��Ч���ݽڵ�ĸ���

	unsigned int MaxTimeInterval; //��¼�ļ������ʱ�䳤�ȣ������ʱ�����ȳ������ֵ�ͻ����
	STREAM_INFO Stream[PumpStreamNum];
	unsigned int VideoFrmRate; //ͼ��ɼ�֡�ʡ�����PALһ��Ӧ����25������NTSCһ��Ӧ����30��ע�����ﲻ�Ǳ���֡�ʡ�

	unsigned int LocalAudioTmOffset; //�Ա�����Ƶ����У����ƫ�ƣ���λms
	int LocalAudioTmOffsetFlg; //����LocalAudioTmOffset�Ƿ���Ч��
	unsigned int ResetInterval; //ǰ����֡����Ƶʱ����������ֵ����Ҫ��ʾ���ļ��ˣ���λms��

	HANDLE_V2C hRemoteAudioV2C;	// Զ����Ƶ�䳤ת�����ľ��
}STORAGE_WRITE_OBJ;

typedef struct _CR_AUDIO_FRM_PRIVATE_HEAD
{
	unsigned char AlgID;
	unsigned char Rsv1, Rsv2, Rsv3;
}CR_AUDIO_FRM_PRIVATE_HEAD;

typedef struct _CR_AUDIO_FRM_PRIVATE_VL_HEAD
{
	unsigned short AlgFrmLen;
	unsigned short Rsv1;
}CR_AUDIO_FRM_PRIVATE_VL_HEAD;

typedef struct _AUDIO_V2C_OBJ
{
	unsigned int MaxVLSTGFrmLen;
	unsigned char *pVLSTGFrm;
	unsigned int VLSTGFrmLen;
	unsigned int Pos; //����ָ���ƫ��λ�á�
	unsigned int Idx; //������ǰָ��ڼ����㷨֡
	unsigned int AlgFrmInterval; //һ���㷨֡������ʱ��, ��λms
	unsigned int MaxCLSTGFrmLen; //��Ķ���֡�ĳ���.
	int VLFlg; //1 �������һ��Push���Ǳ䳤֡��0�����Ƕ���֡

	//	CR_STORAGE_HEAD *pInSTGHead;
	//	CR_STORAGE_HEAD OutSTGHead;
	//	CR_AUDIO_FRM_HEAD ;
}AUDIO_V2C_OBJ;

#define VIDEO_FOURCC_CREARO_R264	0x34363252  //R264,���е���Ƶ��ʽ������R264�Ĳ��
#define AUDIO_FORMAT_CREARO_AAC		0x00008888	//���е���Ƶ��ʽ������������

