/*  
 *  Copyright (c) 2005, 安徽创世科技有限公司
 *  All rights reserved.
 *  
 *  文件名称: CAVIReader.h
 *  文件标识: 
 *  摘    要: 读AVI文件模块,支持大部分的AVI文件
 *  
 *  当前版本: 
 *  作    者: Tommy
 *  完成日期: 2009-11-6 10:15:59
 *  修正备注: 
 *		1.支持读取私有数据
 *		2.支持向前/向后寻找I帧
 */

#ifndef __CAVIREADER_H__
#define __CAVIREADER_H__

//错误码定义
#define CAVIREADER_E_OK					0
#define CAVIREADER_E_UNKNOWN			-1			//未定义错误
#define CAVIREADER_E_FILE_OPEN			-2			//打开文件错误
#define CAVIREADER_E_MEMORY				-3			//内存不足
#define CAVIREADER_E_FILE_READ			-4			//读取文件错误
#define CAVIREADER_E_FILE_FORMAT		-5			//文件格式错误
#define CAVIREADER_E_FILE_SEEK			-6			//定位文件失败
#define CAVIREADER_E_STREAMCOUNT		 -7			//流数目错误,
													//不能够超过CAVIREADER_MAX_STREAM_NUM
#define CAVIREADER_E_START				-8			//起始位置错误
#define CAVIREADER_E_OVERFLOW			-9			//缓冲不够,或者不对
#define CAVIREADER_E_STREAMTYPE			-10			//流类型错误

//支持的最大流数目
#define	CAVIREADER_MAX_STREAM_NUM		4		
//支持的私有数据长度
#define CAVIREADER_PRIVATEDATA_LEN		1024

//媒体类型宏定义
#define CAVIREADER_mmioFOURCC( ch0, ch1, ch2, ch3 ) \
	( (unsigned char)(ch0) | ( (unsigned char)(ch1) << 8 ) |	\
	( (unsigned char)(ch2) << 16 ) | ( (unsigned char)(ch3) << 24 ) )
#define CAVIREADER_streamtypeVIDEO	CAVIREADER_mmioFOURCC('v', 'i', 'd', 's')
#define CAVIREADER_streamtypeAUDIO	CAVIREADER_mmioFOURCC('a', 'u', 'd', 's')
#define CAVIREADER_streamtypeMIDI	CAVIREADER_mmioFOURCC('m', 'i', 'd', 's')
#define CAVIREADER_streamtypeTEXT	CAVIREADER_mmioFOURCC('t', 'x', 't', 's')

//句柄定义
typedef void* CAVIREADER_HANDLE;
typedef void* CAVIREADER_STREAMHANDLE;

//索引类型定义
#define CAVIREADER_AVIINDEX_FLAG_NULL			0x00	//非I帧
#define CAVIREADER_AVIINDEX_FLAG_KEYFRAME		0x10	//I帧

//AVI头定义,应用者一般不需要关心这个
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

//流信息头定义
typedef struct _TCAVIReaderStreamHeader {
	unsigned long fccType;				//流类型,见前面宏定义CAVIREADER_streamtypeVIDEO等
    unsigned long fccHandler;			//压缩算法
    unsigned long dwFlags;				//意义不明,一般为0
    unsigned long dwPriority;			//意义不明,一般为0
    unsigned long dwInitialFrames;		//意义不明,我们写的是0
    unsigned long dwScale;				//帧率的除因子,dwRate/dwScale就是实际的帧率
    unsigned long dwRate;				//帧率,dwRate/dwScale就是实际的帧率,音频和视频都有效
    unsigned long dwStart;				//意义不明,一般为0
    unsigned long dwLength;				//帧的总数
    unsigned long dwSuggestedBufferSize;//最大帧长度
    unsigned long dwQuality;			//意义不明,我们填的是0
    unsigned long dwSampleSize;			//视频为0,音频为帧长度
	unsigned long dwReserved2;			//保留,一般为0
	unsigned long dwReserved3;			//保留,一般为0
//	RECT   rcFrame;
} TCAVIReaderStreamHeader;

//视频流格式定义
typedef struct _TCAVIReaderBMPInfoHeader {
	unsigned int   biSize;			//这个结构的大小,sizeof(TCAVIReaderBMPInfoHeader)
	unsigned int   biWidth;			//图像的实际像素宽度
	unsigned int   biHeight;		//图像的实际像素高度
	unsigned short biPlanes;		//调色板个数,一般为1
	unsigned short biBitCount;		//颜色位数,一般为24
	unsigned int   biCompression;	//压缩算法,跟TCAVIReaderStreamHeader中的fccHandler一样,可不填
	unsigned int   biSizeImage;		//图像的大小,等于biWidth*biHeight*biBitCount/8
	unsigned int   biXPelsPerMeter; //含义不明,填0
	unsigned int   biYPelsPerMeter;	//含义不明,填0
	unsigned int   biClrUsed;		//含义不明,填0
	unsigned int   biClrImportant;	//含义不明,填0	
} TCAVIReaderBMPInfoHeader; 

//音频流格式定义
typedef struct _TCAVIReaderWaveFormatEx {
	unsigned short wFormatTag;		//音频的编码算法
	unsigned short nChannels;		//声道数,一般填1
	unsigned int   nSamplesPerSec;	//采样率,一般填8000
	unsigned int   nAvgBytesPerSec;	//平均字节率.这个可以计算出来
	unsigned short nBlockAlign;		//每帧数据的长度
	unsigned short wBitsPerSample;	//采样位宽,一般填16
	unsigned short cbSize;			//扩展数据长度,填0
	unsigned short rsv;				//扩展数据,填0
} TCAVIReaderWaveFormatEx;

#ifdef __cplusplus
extern "C" {
#endif

/*  功能描述: 打开AVI文件
 *  参数说明:
 *      CAVIREADER_HANDLE *phReader [OUT]:		模块句柄
 *      const char *cpszFileName [IN]:			文件名
 *  返回值: CAVIREADER_E_OK表示成功,其他表示失败,具体定义见前面宏定义 
 */
int CAVIReader_OpenFile(CAVIREADER_HANDLE *phReader, const char *cpszFileName);

/*  功能描述: 关闭文件
 *  参数说明:
 *      CAVIREADER_HANDLE *phReader [OUT]:		模块句柄
 */
void CAVIReader_Close(CAVIREADER_HANDLE hReader);

/*  功能描述: 读取文件信息
 *  参数说明:
 *      CAVIREADER_HANDLE hReader [IN]:			模块句柄
 *      TCAVIReaderMainHeader *pfinfo [OUT]:	文件信息
 *  返回值: CAVIREADER_E_OK表示成功,其他表示失败,具体定义见前面宏定义
 *	备注:	这个信息上层一般不需要使用,具体意义也不怎么明确
 */
int CAVIReader_GetFileInfo(CAVIREADER_HANDLE hReader, TCAVIReaderMainHeader *pfinfo);

/*  功能描述: 获取私有数据
 *  参数说明:
 *      CAVIREADER_HANDLE hReader [IN]:			模块句柄
 *      char binData[CAVIREADER_PRIVATEDATA_LEN] [OUT]:	私有数据
 *  返回值: int 
 *  备注:	
 */
int CAVIReader_GetPrivateData(CAVIREADER_HANDLE hReader, char binData[CAVIREADER_PRIVATEDATA_LEN]);

/*  功能描述: 获取文件中的流
 *  参数说明:
 *      CAVIREADER_HANDLE hReader [IN]:			模块句柄
 *      CAVIREADER_STREAMHANDLE *phStream [OUT]:流句柄
 *      unsigned long fccType [IN]:				要获取的流的类型,
 *												具体见前面的宏定义,streamtypeVIDEO等
 *      long lParam [IN]:						流出现次数
 *  返回值: CAVIREADER_E_OK表示成功,其他表示失败,具体定义见前面宏定义 
 *  备注: 循环调用此函数,直至返回CAVIREADER_E_STREAMCOUNT即可遍历文件的所有流
 */
int CAVIReader_GetStream(CAVIREADER_HANDLE hReader, 
						 CAVIREADER_STREAMHANDLE *phStream, unsigned long fccType, long lParam);

/*  功能描述: 获取流信息
 *  参数说明:
 *      CAVIREADER_STREAMHANDLE hStream [IN]:	流句柄
 *      TCAVIReaderStreamHeader *psinfo [OUT]:	流信息
 *  返回值: CAVIREADER_E_OK表示成功,其他表示失败,具体定义见前面宏定义 
 */
int CAVIReader_GetStreamInfo(CAVIREADER_STREAMHANDLE hStream, TCAVIReaderStreamHeader *psinfo);

/*  功能描述: 获取流格式
 *  参数说明:
 *      CAVIREADER_STREAMHANDLE hStream [IN]:	流句柄
 *      void *pFormat [OUT]:					根据流类型不同而不同,具体如下:
 *													streamtypeVIDEO为结构TCAVIReaderBMPInfoHeader
 *													streamtypeAUDIO为结构TCAVIReaderWaveFormatEx
 *													streamtypeMIDI和streamtypeTEXT不支持
 *      long cbFormat [IN]:						pFormat的字节数
 *  返回值: CAVIREADER_E_OK表示成功,其他表示失败,具体定义见前面宏定义 
 *  备注: 如果cbFormat和流类型不对应,则会返回CAVIREADER_E_OVERFLOW错误
 */
int CAVIReader_GetStreamFormat(CAVIREADER_STREAMHANDLE hStream, void *pFormat, long cbFormat);

/*  功能描述: 获取流的名字
 *  参数说明:
 *      CAVIREADER_STREAMHANDLE hStream [IN]:	流句柄
 *      char szName[64] [OUT]:					流的名字
 *  返回值: CAVIREADER_E_OK表示成功,其他表示失败,具体定义见前面宏定义 
 */
int CAVIReader_GetStreamName(CAVIREADER_STREAMHANDLE hStream, char szName[64]);

/*  功能描述: 获取流的帧数
 *  参数说明:
 *      CAVIREADER_STREAMHANDLE hStream [IN]:	流句柄
 *  返回值: 流的帧数
 */
int CAVIReader_GetLength(CAVIREADER_STREAMHANDLE hStream);

/*  功能描述: 读取帧数据
 *  参数说明:
 *      CAVIREADER_STREAMHANDLE hStream [IN]:	流句柄
 *      long lStart [IN]:						读取帧的序号
 *      long lSamples [IN]:						读取的帧的数目,目前支持1
 *      void *pBuffer [OUT]:					读取到的帧数据
 *      long cbBuffer [IN]:						pBuffer的大小,
 *												如果小于帧的实际大小将返回CAVIREADER_E_OVERFLOW
 *      long *plBytes [OUT]:					实际读取到的帧大小
 *      long *plSamples [OUT]:					实际读取的帧数目,目前总是为1
 *		long *plFlag [OUT]:						读取到的帧的类型,具体见前面的索引类型宏定义
 *  返回值: CAVIREADER_E_OK表示成功,其他表示失败,具体定义见前面宏定义 
 */
int CAVIReader_ReadSample(CAVIREADER_STREAMHANDLE hStream, long lStart, long lSamples, 
						  void *pBuffer, long cbBuffer, long *plBytes, long *plSamples, long *plFlag);

/*  功能描述: 寻找前一个I帧
 *  参数说明:
 *      CAVIREADER_STREAMHANDLE hStream [IN]:	流句柄
 *      long lStart [IN]:						开始寻找的帧号(包括这一帧)
 *  返回值: 找到的I帧的序号,-1表示没有找到
 *  备注:	在成功找到之后,上层要往前减一个帧号接着寻找,否则会原地死循环的.	
 */
long CAVIReader_FindPrevKeySample(CAVIREADER_STREAMHANDLE hStream, long lStart);

/*  功能描述: 寻找后一个I帧
 *  参数说明:
 *      CAVIREADER_STREAMHANDLE hStream [IN]:	流句柄
 *      long lStart [IN]:						开始寻找的帧号(包括这一帧)
 *  返回值: 找到的I帧的序号,-1表示没有找到
 *  备注:	在成功找到之后,上层要往后加一个帧号接着寻找,否则会原地死循环的.	
 */
long CAVIReader_FindNextKeySample(CAVIREADER_STREAMHANDLE hStream, long lStart);

#ifdef __cplusplus
}
#endif

#endif //__CAVIREADER_H__
