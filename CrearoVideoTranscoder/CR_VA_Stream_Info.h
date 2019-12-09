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
	unsigned int LastTmStampValidFlg; //上一帧的时间戳是否存在的标记，1表明至少曾经来过一帧数据，LastTmStamp有效；0表明LastTmStamp无效
	unsigned int LastTmStamp; //上一帧的时间戳
	unsigned int LastFrmNO; //上一帧的帧序号
	unsigned int AudioFrmInterval; //每个音频帧的时间跨度，单位ms, 这个值仅仅对音频帧有效
	unsigned char *pEmptyFrm;
	int EmptyFrmLen;
	int NextSampleNO;  //下一帧要读取的Sample的索引
}STREAM_INFO;

typedef struct _STORAGE_READ_OBJ
{
	CAVIREADER_HANDLE hAVI;
	unsigned int BaseTmStamp; //基准时间戳
	STREAM_INFO Stream[PumpStreamNum];

	unsigned int VideoFrmRate; //图像帧率。对于PAL一般应该是25，对于NTSC一般应该是30	
}STORAGE_READ_OBJ;

//存储帧头
typedef struct _CR_STORAGE_HEAD
{
	unsigned int UTC;
	unsigned int TimeStamp;
	unsigned char KeyFrm;
	unsigned char Rsv[3];
}CR_STORAGE_HEAD;

//视频帧的头
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

//音频帧的头
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
	//	unsigned int Flags; //取值是CAVIWRITER_AVIINDEX_FLAG_XXX的宏
}PUMP_DATA; //记录各个Pump函数塞入的数据

//记录当前模块的状态
enum WorkStatus {
	Reset, //模块刚创建时的状态，AbsoluteBaseTmStamp还无效；
	Cache, //缓冲的数据间隔还没有达到CacheTime时，处于这个状态。这个状态时，AbsoluteBaseTmStamp有效。且缓冲的数据的时间戳间隔都是合法的。
	Write, //当缓冲了足够长的数据，并flush了缓冲区数据， 开始实时写入每一帧时处于这个状态
};

typedef struct _STORAGE_WRITE_OBJ
{
	CAVIWRITER_HANDLE hAVI;
	WorkStatus Status;

	unsigned int AbsoluteBaseTmStamp; //绝对基准时间戳，这个值只在第一帧来的时候记录一次，用来检查是否文件时间跨度超过预设值
	unsigned int CacheTime; //记录缓冲时间

	PUMP_DATA *pPumpData; //记录缓冲的，还没有写进文件的数据
	int MaxPumpDataNum; //最大有效数据节点的个数
	int PumpDataNum; //有效数据节点的个数

	unsigned int MaxTimeInterval; //记录文件的最大时间长度，塞入的时间戳跨度超过这个值就会出错
	STREAM_INFO Stream[PumpStreamNum];
	unsigned int VideoFrmRate; //图像采集帧率。对于PAL一般应该是25，对于NTSC一般应该是30。注意这里不是编码帧率。

	unsigned int LocalAudioTmOffset; //对本地音频进行校正的偏移，单位ms
	int LocalAudioTmOffsetFlg; //表明LocalAudioTmOffset是否有效。
	unsigned int ResetInterval; //前后两帧的视频时间戳超过这个值，就要提示换文件了，单位ms。

	HANDLE_V2C hRemoteAudioV2C;	// 远程音频变长转定长的句柄
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
	unsigned int Pos; //表明指向的偏移位置。
	unsigned int Idx; //表明当前指向第几个算法帧
	unsigned int AlgFrmInterval; //一个算法帧持续的时间, 单位ms
	unsigned int MaxCLSTGFrmLen; //最长的定长帧的长度.
	int VLFlg; //1 表明最后一次Push的是变长帧，0表明是定长帧

	//	CR_STORAGE_HEAD *pInSTGHead;
	//	CR_STORAGE_HEAD OutSTGHead;
	//	CR_AUDIO_FRM_HEAD ;
}AUDIO_V2C_OBJ;

#define VIDEO_FOURCC_CREARO_R264	0x34363252  //R264,所有的视频格式都是用R264的插件
#define AUDIO_FORMAT_CREARO_AAC		0x00008888	//所有的音频格式都是用这个插件

