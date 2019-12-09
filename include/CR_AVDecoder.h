#ifndef __CR_AV_DECODER_H__
#define __CR_AV_DECODER_H__

#ifdef CR_AVDECODER_EXPORTS
#define CR_DEC_API __declspec(dllexport)
#else
#define CR_DEC_API __declspec(dllimport)
#endif

// ��Ƶ�㷨����
#define VIDEO_FRAMEALG_JPEG	  0
#define VIDEO_FRAMEALG_MPEG1  1
#define VIDEO_FRAMEALG_MPEG2  2
#define VIDEO_FRAMEALG_MJPEG  3
#define VIDEO_FRAMEALG_MPEG4  4
#define VIDEO_FRAMEALG_H263	  5
#define VIDEO_FRAMEALG_H264	  6
#define VIDEO_FRAMEALG_AVS	  7
#define VIDEO_FRAMEALG_S264   8

// ��Ƶ������ʼ������
typedef struct _CR_VIDEODEC_INIT_PARAM {
	unsigned int MaxImgWidth;	// ��ʾͼ��������
	unsigned int MaxImgHeight;	// ��ʾͼ������߶�
}CR_VIDEODEC_INIT_PARAM;

//��Ƶ֡����
typedef struct _CR_VIDEODEC_FRAME_PARAM {
	unsigned int Alg;		// ��Ƶ���õ��㷨����
	unsigned char *pBuffer; // ��Ƶ���ݻ�����ָ��
	unsigned int Length;	// ��Ƶ���ݵĳ���
	bool bPostProcessing;	// �Ƿ�������0��ʾ������1��ʾ����������һ��Ϊ0
	//�������������
	unsigned char *pImg[3]; //�ֱ�ָ��YUV��RGB 3������, ���ͼ����ڴ��ɽ���������,
	//����ڴ����´ε���VideoDec_XXX����֮ǰʧЧ
	unsigned int ImgWidth;	//���ͼ��ĳߴ�
	unsigned int ImgHeight;
	unsigned int YStride; //����YUV420�������ʽ,������Stride�Ĳ���
	unsigned int UVStride; 
	bool bIFrame;			// �Ƿ�ΪI֡��0��ʾ����I֡��1��ʾ��I֡
	unsigned char rsv;		// �����ֶΣ���Ϊ1
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

// ��Ƶ������ʼ������
typedef struct _CR_AUDIODEC_INIT_PARAM {
	unsigned int MaxPCMLen;		//��Ľ���������ݳ��ȣ��Ƽ�ֵ4096
}CR_AUDIODEC_INIT_PARAM;

//��Ƶ֡����
typedef struct _CR_AUDIODEC_FRAME_PARAM {
	unsigned int Alg;		// ��Ƶ���õ��㷨����
	unsigned char *pBuffer; // ��Ƶ���ݻ�����ָ��
	unsigned int Length;	// ��Ƶ���ݵĳ���
	//�������������
	unsigned char *pPCM; //ָ���������Ƶ���ݣ��ɽ��������䣬����ڴ����´ε���AudioDec_XXX����֮ǰʧЧ
	unsigned int PCMLen;
}CR_AUDIODEC_FRAME_PARAM;

#ifdef __cplusplus
extern "C" {
#endif

// ����һ����ƵƵ������ʵ��
CR_DEC_API unsigned int CR_AudioDec_Init(CR_AUDIODEC_INIT_PARAM *pAudioDecInitParam);

// ɾ��һ����Ƶ������ʵ��
CR_DEC_API void CR_AudioDec_Close(unsigned int Handle);

// ��һ֡��Ƶ
CR_DEC_API int CR_AudioDec_DecFrm(unsigned int Handle, CR_AUDIODEC_FRAME_PARAM *pAudioDecParam);

// ����һ����Ƶ������ʵ��
CR_DEC_API unsigned int CR_VideoDec_Init(CR_VIDEODEC_INIT_PARAM *pVideoDecInitParam);

// ɾ��һ����Ƶ������ʵ��
CR_DEC_API void CR_VideoDec_Close(unsigned int Handle);

// ��һ֡��Ƶ
CR_DEC_API int CR_VideoDec_DecFrm(unsigned int Handle, CR_VIDEODEC_FRAME_PARAM *pVideoDecParam);

#ifdef __cplusplus
}
#endif

#endif /* __CR_AV_DECODER_H__ */
