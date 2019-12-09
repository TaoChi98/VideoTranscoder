/*  
 *  Copyright (c) 2005, 安徽创世科技有限公司
 *  All rights reserved.
 *  
 *  文件名称: VARender.h
 *  文件标识: 
 *  摘    要:	
 *		完成视频和音频的解码显示播放和同步功能.
 *  当前版本: 
 *  作    者: Tommy
 *  完成日期: 2007-7-25 11:31:37
 *  修正备注: 
 *  
 */

#ifndef __VARENDER_H__
#define __VARENDER_H__

//错误码定义
#define VARENDER_E_OK				0	//成功
#define VARENDER_E_WOULDBLOCK		1	//操作无法立即执行
#define VARENDER_E_UNKOWN			-1	//未知错误
#define VARENDER_E_FRAMEINFOCHANGE	-2	//帧信息发生改变
#define VARENDER_E_UNKOWNFRAMEINFO	-3	//不支持的帧信息
#define VARENDER_E_TIMER			-4	//创建定时器失败
#define VARENDER_E_INVALID			-5	//资源不可用

//音频输出声道定义
#define VARENDER_CHANNEL_DUAL		0	//双声道输出
#define VARENDER_CHANNEL_LEFT		1	//左声道输出
#define VARENDER_CHANNEL_RIGHT		2	//右声道输出

//音视频算法定义
#define VARENDER_VIDEO_ALG_NULL		0
#define VARENDER_VIDEO_ALG_H264		1
#define VARENDER_AUDIO_ALG_G7231	2
#define VARENDER_VIDEO_ALG_S264		3
#define VARENDER_AUDIO_ALG_ADPCM	4
#define VARENDER_VIDEO_ALG_MPEG4	5
#define VARENDER_VIDEO_ALG_MJPEG	6	//不支持DDraw
#define VARENDER_AUDIO_ALG_AMR		7
#define VARENDER_AUDIO_ALG_G711A	8
#define VARENDER_AUDIO_ALG_G711U	9
#define VARENDER_AUDIO_ALG_AAC		10

//视频分辨率定义
#define VARENDER_VIDEO_SIZE_CIF		1
#define VARENDER_VIDEO_SIZE_QCIF	2
#define VARENDER_VIDEO_SIZE_D1		3

//裸视频数据格式
#define VARENDER_FORMAT_RGB			1	//RGB
#define VARENDER_FORMAT_YUV420		2	//YUV420,JPEG不支持这种格式的裸数据输出

//录像和抓拍文件名的最大长度
#define VARENDER_MAXLEN_FILENAME	256

//接口结构定义
typedef struct _TVARVideoFrameInfo {
	unsigned char ucAlg;			//编码算法(1)
	unsigned char ucIFlag;			//I帧标识(1)
	unsigned char ucSize;			//图像大小(1)
	unsigned char ucRsv;			//保留(1)	现在开始用作7.0的控制标识
									//第0位		0表示图像不翻转,1表示图像翻转
									//第1~7位	保留位,目前填0
	unsigned int uiTimeStamp;		//时间戳(4)
} TVARVideoFrameInfo;

typedef struct _TVARAudioFrameInfo {
	unsigned char ucAlg;			//编码算法(1)
	unsigned char ucRsv;			//保留(1)
	unsigned short usFrameNum;		//帧数目(2)
	unsigned int uiTimeStamp;		//时间戳(4)
} TVARAudioFrameInfo;

typedef void* VARENDERUSER_HANDLE;
typedef void* VARENDER_HANDLE;
typedef void* VARENDER_HWND;
typedef void* VARENDER_HDC;

/*  功能描述: 显示图像的时,自定义绘图的回调函数接口
 *  参数说明:
 *		VARENDERUSER_HANDLE hUser [IN]:			自定义用户数据
 *		VARENDER_HDC hDC [IN]:					图像口DC
 *      int X [IN]:								图像区域的X坐标
 *      int Y [IN]:								图像区域的Y坐标
 *      int nWidth [IN]:						图像区域的宽度
 *      int nHeight [IN]:						图像区域的高度
 *      int X2 [IN]:							显示区域的X坐标
 *      int Y2 [IN]:							显示区域的Y坐标
 *      int nWidth2 [IN]:						显示区域的宽度
 *      int nHeight2 [IN]:						显示区域的高度
 */
typedef void (*VARENDER_DISPLAY_CALLBACK)(VARENDERUSER_HANDLE hUser, VARENDER_HDC hDC, 
										  int X, int Y, int nWidth, int nHeight,
										  int X2, int Y2, int nWidth2, int nHeight2);

/*  功能描述: 获取裸视频数据的回调函数
 *  参数说明:
 *		VARENDERUSER_HANDLE hUser [IN]:			自定义用户数据
 *      int nWidth [IN]:						视频的宽度
 *      int nHeight [IN]:						视频的高度
 *      *pBuf [IN]:								视频数据的缓冲地址
 *      int iBufLen [IN]:						视频数据的长度
 *      char cFormat [IN]:						视频数据的格式,见前面的宏定义
 */
typedef void (*VARENDER_RAWVIDEO_CALLBACK)(VARENDERUSER_HANDLE hUser, 
										   int nWidth, int nHeight, 
										   char *pBuf, int iBufLen, char cFormat);

/*  功能描述: 音频数据回调函数
 *  参数说明:
 *      VARENDERUSER_HANDLE hUser [IN]:			自定义用户数据
 *      short *pPCMBuf [IN]:					PCM音频数据缓冲
 *      int nSampleNum [IN]:					PCM音频样本数
 *  返回值: typedef void 
 *  备注:	
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

/*  功能描述: 初始化VAReander库
 *  返回值: 错误码,见前面的宏定义
 *  备注: 程序启动的时候必须先初始化才能够调用其他函数,全局调用一次即可.
 */
VARENDER_API int VARender_Startup();

/*  功能描述: 清除VARender库
 *  备注: 程序退出之前调用此函数,否则可能造成资源泄漏
 */
VARENDER_API void VARender_Cleanup();

/*  功能描述: 创建VARender对象
 *  参数说明:
 *      VARENDER_HANDLE *phVARender [OUT]:	创建得到的对象句柄
 *  返回值: 错误码,见前面的宏定义
 *	备注: 创建出来的句柄必须使用VARender_Close关闭,否则会造成内存泄漏
 */
VARENDER_API int VARender_Open(VARENDER_HANDLE *phVARender);

/*  功能描述: 关闭VARender对象
 */
VARENDER_API void VARender_Close(VARENDER_HANDLE hVARender);

/*  功能描述: 设置自定义绘图回调函数
 *  参数说明:
 *      VARENDER_CALLBACK pCallBack [IN]:	绘图回调函数
 *      VARENDER_HANDLE hUser [IN]:			自定义用户数据
 */
VARENDER_API void VARender_SetDisplayCallBack(VARENDER_HANDLE hVARender, 
								VARENDER_DISPLAY_CALLBACK pCallBack, VARENDERUSER_HANDLE hUser);

/*  功能描述: 设置获取裸视频数据的回调函数
 *  参数说明:
 *      VARENDER_CALLBACK pCallBack [IN]:	获取裸视频数据的回调函数
 *      VARENDER_HANDLE hUser [IN]:			自定义用户数据
 *		char cFormat [IN]:					需要的裸数据格式,目前值支持YUV420
 */
VARENDER_API void VARender_SetRawVideoCallBack(VARENDER_HANDLE hVARender, 
								VARENDER_RAWVIDEO_CALLBACK pCallBack, VARENDERUSER_HANDLE hUser, char cFormat);

/*  功能描述: 设置音频数据回调函数
 *  参数说明:
 *      VARENDER_AUDIO_CALLBACK pCallBack [IN]:	
 *											音频数据回调函数
 *      VARENDER_HANDLE hUser [IN]:			自定义用户数据
 */
VARENDER_API void VARender_SetAudioCallBack(VARENDER_HANDLE hVARender, 
								VARENDER_AUDIO_CALLBACK pCallBack, VARENDERUSER_HANDLE hUser);

/*  功能描述: 设置音频输出声道
 *  参数说明:
 *      int iLRChannel [IN]:			输出的声道,取值见前面的宏定义
 *  备注: 缺省是两个声道都输出
 *		  必须在Open之后,塞入数据之前调用这个函数,之后不能调用这个函数了,否则会出现内存访问冲突
 */
VARENDER_API void VARender_SetAudioLRChannel(VARENDER_HANDLE hVARender, int iLRChannel);

/*  功能描述: 设置显示器
 *  参数说明:
 *		int iNo [IN]:				显示器的编号,从0(默认显示器)开始,到系统支持的最大显示器数目
 *		int iX [IN]:				显示器的屏幕左上角X坐标
 *		int iY [IN]:				显示器的屏幕左上角Y坐标
 *  返回值: 错误码,见前面的宏定义
 *  备注: 多个显示器并且使用DDraw的时候,需要设置这个.
 *		  建议先调用VARender_SetMonitor,再调用VARender_SetWindow
 */
VARENDER_API int VARender_SetMonitor(VARENDER_HANDLE hVARender, int iNo, int iX, int iY);

/*  功能描述: 设置显示窗口句柄
 *  参数说明:
 *		VARENDER_HWND hWindow [IN]:		显示窗口句柄,传入NULL表示释放窗口
 *  返回值: 错误码,见前面的宏定义
 *	备注:
 *		在调用VARender_PumpFrame函数之前,必须先调用此函数设置窗口句柄.
 *		如果没有设置句柄,将显示不了图像
 */
VARENDER_API int VARender_SetWindow(VARENDER_HANDLE hVARender, VARENDER_HWND hWnd);

/*  功能描述: 设置显示区域
 *  参数说明:
 *      int XDest [IN]:					x-coordinate of destination upper-left corner
 *      int YDest [IN]:					y-coordinate of destination upper-left corner
 *      int nDestWidth [IN]:			width of destination rectangle
 *      int nDestHeight [IN]:			height of destination rectangle
 *	备注: 默认的目标x,y为0,宽度和高度为窗口的宽度和高度
 */
VARENDER_API void VARender_SetDestRect(VARENDER_HANDLE hVARender, int XDest, int YDest, int nDestWidth, int nDestHeight);

/*  功能描述: 设置显示区域
 *  参数说明:
 *      int XSrc [IN]:					x-coordinate of source upper-left corner
 *      int YSrc [IN]:					y-coordinate of source upper-left corner
 *      int nSrcWidth [IN]:				width of source rectangle
 *      int nSrcHeight [IN]:			height of source rectangle
 *	备注: 默认的源x,y为0,宽度和高度为图像实际宽度和高度
 */
VARENDER_API void VARender_SetSrcRect(VARENDER_HANDLE hVARender, int XSrc, int YSrc, int nSrcWidth, int nSrcHeight);

/*  功能描述: 设置是否使用DDraw
 *  参数说明:
 *      int bDDraw [IN]:						是否使DDraw,0表示不使用,1表示使用.
 *  返回值: 错误码,见前面的宏定义
 *  备注: 如果不调用此函数,默认为不使用DDraw,MJPEG算法暂时不支持DDraw
 *		  建议先调用VARender_SetDDraw,再调用VARender_SetWindow
 */
VARENDER_API int VARender_SetDDraw(VARENDER_HANDLE hVARender, int bDDraw);

/*  功能描述: 获取是否使用DDraw
 *  返回值: 是否使DDraw,0表示不使用,1表示使用
 *  备注: Added by Terry.Wang, 2008/1/22 19:38:42
 */
VARENDER_API int VARender_IsDDraw(VARENDER_HANDLE hVARender);

/*  功能描述: 使能使用小解码库,即本解码库
 *	参数说明: 是否使能,0表示不使用,1表示使用
 *			int bEnable[IN]:			是否使能使用本解码库,0表示不使用,1表示使用
 *	备注:	默认不使用本解码库,
 *			本接口主要提供给7.0插件使用,外部请谨慎调用此接口,内部用于判断是否需要分配内存
 *			和如何应该调用哪个录像接口
 */
VARENDER_API void VARender_EnableLitteDecodeLib(VARENDER_HANDLE hVARender, int bEnable);

/*  功能描述: 设置是否使能自适应解半场
 *  参数说明:
 *		int bEnable [IN]:					是否使能自适应解半场
 */
VARENDER_API void VARender_EnableHalfD1(VARENDER_HANDLE hVARender, int bEnable);

/*  功能描述: 开始/停止 播放/显示 视频/音频
 *  备注: 
 *		开始视频则显示视频,否则解码不显示视频
 *		开始音频则播放音频,否则不解码播放音频
 */
VARENDER_API void VARender_StartVideo(VARENDER_HANDLE hVARender);
VARENDER_API void VARender_StopVideo(VARENDER_HANDLE hVARender);
VARENDER_API void VARender_StartAudio(VARENDER_HANDLE hVARender);
VARENDER_API void VARender_StopAudio(VARENDER_HANDLE hVARender);

/*  功能描述: 塞入视频数据/塞入裸7.0视频数据帧
 *  参数说明:
 *		char *pFrame [IN]:				数据帧缓冲.
 *		int iLen [IN]					数据帧长度
 *		TVARVideoFrameInfo *pvfinfo [IN]:视频帧信息
 *	返回值: VARENDER_E_OK			表示成功
 *			VARENDER_E_WOULDBLOCK	表示数据缓冲满,需要继续尝试塞入
 */
VARENDER_API int VARender_PumpVideoFrame(VARENDER_HANDLE hVARender, char *pFrame, int iLen, TVARVideoFrameInfo *pvfinfo);
VARENDER_API int VARender_PumpRawVideoFrame(VARENDER_HANDLE hVARender, char *pFrame, int iLen);

/*  功能描述: 塞入音频数据/塞入裸7.0音频数据帧
 *  参数说明:
 *		char *pFrame [IN]:				数据帧缓冲.
 *		int iLen [IN]					数据帧长度
 *		TVARAudioFrameInfo *pafinfo [IN]:视频帧信息
 *	返回值: VARENDER_E_OK			表示成功
 *			VARENDER_E_WOULDBLOCK	表示数据缓冲满,需要继续尝试塞入
 */
VARENDER_API int VARender_PumpAudioFrame(VARENDER_HANDLE hVARender, char *pFrame, int iLen, TVARAudioFrameInfo *pafinfo);
VARENDER_API int VARender_PumpRawAudioFrame(VARENDER_HANDLE hVARender, char *pFrame, int iLen);

/*  功能描述: 抓图
 *  参数说明:
 *      const char *pszFileName [IN]:	抓拍文件保存的文件名
 */
VARENDER_API void VARender_Snapshot(VARENDER_HANDLE hVARender, const char *pszFileName);

/*  功能描述: 重绘最后一幅图像
 *  备注: 在显示窗口刷新的时候调用这个函数,如果还未显示数据,调此函数会出现黑屏
 */
VARENDER_API void VARender_Refresh(VARENDER_HANDLE hVARender);

/*  功能描述: 获取当前视频的帧率
 *  返回值: 当前视频的帧率
 */
VARENDER_API int VARender_GetFrameRate(VARENDER_HANDLE hVARender);

/*  功能描述: 获取当前视频的码率
 *  返回值: 当前视频的码率
 */
VARENDER_API int VARender_GetBitRate(VARENDER_HANDLE hVARender);

/*  功能描述: 设置当前视频是否精确同步播放~
 */
VARENDER_API void VARender_SetbSync(VARENDER_HANDLE hVARender, int bSync);

/*  功能描述: 获取当前视频是否精确同步播放
 *  返回值: 是否精确同步播放
 */
VARENDER_API int VARender_IsSync(VARENDER_HANDLE hVARender);

/*  功能描述: 设置是否根据缓冲调节播放速度
 *	备注: 如果设置不根据缓冲调节播放速度,则音频和视频会自动精确同步
 */
VARENDER_API void VARender_SetAdjustSpeedByBuffer(VARENDER_HANDLE hVARender, int bAdjustSpeedByBuffer);

/*  功能描述: 获取当前是否根据缓冲调节播放速度
 *  返回值: 是否根据缓冲调节播放速度
 */
VARENDER_API int VARender_IsAdjustSpeedByBuffer(VARENDER_HANDLE hVARender);

/*  功能描述: 设置播放速度
 *  参数说明:
 *      int iSpeed [IN]:	播放速度,支持1,2,4其他值都会回到默认值1
 *	备注: 只有在不根据缓冲调节播放速度无效的情况下有效
 *		  播放速度不是1的时候会自动停止音频的播放
 */
VARENDER_API void VARender_SetSpeed(VARENDER_HANDLE hVARender, int iSpeed);

/*  功能描述: 获取当前播放速度
 *  返回值: 当前播放速度
 */
VARENDER_API int VARender_GetSpeed(VARENDER_HANDLE hVARender);

/*  功能描述: 获取视频缓冲的字节数
 *  参数说明:
 *      int *piMaxBufferBytes [OUT]:	最大缓冲的字节数,传入NULL表示不需要带出这个值.
 *  返回值: 已经缓冲的字节数
 */
VARENDER_API int VARender_GetVideoBufferBytes(VARENDER_HANDLE hVARender, int *piMaxBufferBytes);

/*  功能描述: 获取音频缓冲的帧数
 *  参数说明:
 *      int *piMaxBufferFrames [OUT]:	最大缓冲的帧数,传入NULL表示不需要带出这个值.
 *  返回值: 已经缓冲的帧数
 */
VARENDER_API int VARender_GetAudioBufferFrames(VARENDER_HANDLE hVARender, int *piMaxBufferFrames);

/*  功能描述: 获取/设置音频播放音量,范围0-100
 */
VARENDER_API int VARender_GetVolume(VARENDER_HANDLE hVARender);
VARENDER_API void VARender_SetVolume(VARENDER_HANDLE hVARender, int iVolume);

/*  功能描述: 获取最近播放的视频/音频帧的时间戳
 *  参数说明:
 *      unsigned int *puiTimeStamp [OUT]:	最近播放的时间戳
 *  返回值: VARENDER_E_OK			表示成功
 *			VARENDER_E_INVALID		还没有播放帧
 */
VARENDER_API int VARender_GetLatestPlayVideoTimeStamp(VARENDER_HANDLE hVARender, unsigned int *puiTimeStamp);
VARENDER_API int VARender_GetLatestPlayAudioTimeStamp(VARENDER_HANDLE hVARender, unsigned int *puiTimeStamp);

/*  功能描述: 获取当前视频音频缓冲链表长度等调试参数
 */
VARENDER_API int VARender_GetVideoListLen(VARENDER_HANDLE hVARender);
VARENDER_API int VARender_GetAudioListLen(VARENDER_HANDLE hVARender);
VARENDER_API int VARender_GetVADelta(VARENDER_HANDLE hVARender);
VARENDER_API int VARender_GetSilenceAudioFrameNum(VARENDER_HANDLE hVARender);
VARENDER_API int VARender_GetDropAudioFrameNum(VARENDER_HANDLE hVARender);

/*  功能描述: 开始/停止录像
 *  参数说明:
 *      const char *lpszFileName [IN]:		文件名,最大长度不可大于VARENDER_MAXLEN_FILENAME.
 *      unsigned int uiMaxFileTime [IN]:	录像文件的最大时间长度,单位秒.超过长度会自动换文件
 *  返回值: 见上面宏定义.
 *	备注: 生成的文件名自动加后缀"_00",换文件的时候,递增,"_01","_02",...
 */
VARENDER_API int VARender_StartRecord(VARENDER_HANDLE hVARender, const char *lpszFileName, unsigned int uiMaxFileTime);
VARENDER_API void VARender_CloseRecord(VARENDER_HANDLE hVARender);

#ifdef __cplusplus
}
#endif

#endif //__VARENDER_H__
