#ifndef __CR_AV_DECODER_H__
#define __CR_AV_DECODER_H__

#ifdef CR_AVDECODER_EXPORTS
#define CR_DEC_API __declspec(dllexport)
#else
#define CR_DEC_API __declspec(dllimport)
#endif

// 视频算法类型
#define VIDEO_FRAMEALG_JPEG	  0
#define VIDEO_FRAMEALG_MPEG1  1
#define VIDEO_FRAMEALG_MPEG2  2
#define VIDEO_FRAMEALG_MJPEG  3
#define VIDEO_FRAMEALG_MPEG4  4
#define VIDEO_FRAMEALG_H263	  5
#define VIDEO_FRAMEALG_H264	  6
#define VIDEO_FRAMEALG_AVS	  7
#define VIDEO_FRAMEALG_S264   8

// 视频解码库初始化参数
typedef struct _CR_VIDEODEC_INIT_PARAM {
	unsigned int MaxImgWidth;	// 显示图像的最大宽度
	unsigned int MaxImgHeight;	// 显示图像的最大高度
}CR_VIDEODEC_INIT_PARAM;

//视频帧参数
typedef struct _CR_VIDEODEC_FRAME_PARAM {
	unsigned int Alg;		// 视频采用的算法类型
	unsigned char *pBuffer; // 视频数据缓冲区指针
	unsigned int Length;	// 视频数据的长度
	bool bPostProcessing;	// 是否做后处理，0表示不做，1表示做后处理，这里一般为0
	//下面是输出参数
	unsigned char *pImg[3]; //分别指向YUV或RGB 3个分量, 输出图像的内存由解码器分配,
	//这段内存在下次调用VideoDec_XXX函数之前失效
	unsigned int ImgWidth;	//输出图像的尺寸
	unsigned int ImgHeight;
	unsigned int YStride; //对于YUV420的输出格式,允许有Stride的参数
	unsigned int UVStride; 
	bool bIFrame;			// 是否为I帧，0表示不是I帧，1表示是I帧
	unsigned char rsv;		// 保留字段，恒为1
}CR_VIDEODEC_FRAME_PARAM;

#define	AUDIO_FRAMEALG_G723A1				0
#define	AUDIO_FRAMEALG_G729AB				1
#define	AUDIO_FRAMEALG_G726					2
#define	AUDIO_FRAMEALG_G722					3
#define	AUDIO_FRAMEALG_MP3					4
#define	AUDIO_FRAMEALG_ADPCM				5
#define	AUDIO_FRAMEALG_PCM					6
#define	AUDIO_FRAMEALG_G711A				7
#define	AUDIO_FRAMEALG_G711U				8
#define AUDIO_FRAMEALG_AMR					9
#define AUDIO_FRAMEALG_AAC					10

// 音频解码库初始化参数
typedef struct _CR_AUDIODEC_INIT_PARAM {
	unsigned int MaxPCMLen;		//最长的解码输出数据长度，推荐值4096
}CR_AUDIODEC_INIT_PARAM;

//视频帧参数
typedef struct _CR_AUDIODEC_FRAME_PARAM {
	unsigned int Alg;		// 音频采用的算法类型
	unsigned char *pBuffer; // 音频数据缓冲区指针
	unsigned int Length;	// 音频数据的长度
	//下面是输出参数
	unsigned char *pPCM; //指向输出的音频数据，由解码器分配，这段内存在下次调用AudioDec_XXX函数之前失效
	unsigned int PCMLen;
}CR_AUDIODEC_FRAME_PARAM;

#ifdef __cplusplus
extern "C" {
#endif

// 创建一个音频频解码器实例
CR_DEC_API unsigned int CR_AudioDec_Init(CR_AUDIODEC_INIT_PARAM *pAudioDecInitParam);

// 删除一个音频解码器实例
CR_DEC_API void CR_AudioDec_Close(unsigned int Handle);

// 解一帧音频
CR_DEC_API int CR_AudioDec_DecFrm(unsigned int Handle, CR_AUDIODEC_FRAME_PARAM *pAudioDecParam);

// 创建一个视频解码器实例
CR_DEC_API unsigned int CR_VideoDec_Init(CR_VIDEODEC_INIT_PARAM *pVideoDecInitParam);

// 删除一个视频解码器实例
CR_DEC_API void CR_VideoDec_Close(unsigned int Handle);

// 解一帧视频
CR_DEC_API int CR_VideoDec_DecFrm(unsigned int Handle, CR_VIDEODEC_FRAME_PARAM *pVideoDecParam);

#ifdef __cplusplus
}
#endif

#endif /* __CR_AV_DECODER_H__ */
