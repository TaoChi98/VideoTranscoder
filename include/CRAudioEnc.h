#ifndef __CR_AUDIO_ENC_H__
#define __CR_AUDIO_ENC_H__

#define  AE_EXPORTS		0x70000

#ifdef AE_EXPORTS
#define AENC_API __declspec(dllexport)
#else
#define AENC_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void* AENC_HANDLE;

// 创建音频编码模块
AENC_API AENC_HANDLE __stdcall AENC_Create(const char *pszProducerID, const char *pszAlgID, char *pDLLDirPath);
// 删除音频编码模块
AENC_API void __stdcall AENC_Delete(AENC_HANDLE hAENC);

// 输入一帧PCM数据
AENC_API int __stdcall AENC_PushPCMData(AENC_HANDLE hAENC, short *pPCM, int nPCMCnt, unsigned int uiTimeStamp);
// 输出一帧编码后的数据
AENC_API int __stdcall AENC_PopEncData(AENC_HANDLE hAENC, unsigned char *pData, int *nLen);

#ifdef __cplusplus
}
#endif

#endif/* __CR_AUDIO_ENC_H__ */
