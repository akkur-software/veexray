#pragma once
#include <math.h>
#include "vfw.h"
#include "mmsystem.h"

typedef struct
{
	FOURCC fccType;
	FOURCC fccHandler;
	DWORD dwFlags;
	DWORD dwPriority;
	DWORD dwInitialFrames;
	DWORD dwScale;
	DWORD dwRate;
	DWORD dwStart;
	DWORD dwLength;
	DWORD dwSuggestedBufferSize;
	DWORD dwQuality;
	DWORD dwSampleSize;

	struct
	{
		WORD left;
		WORD top;
		WORD right;
		WORD bottom;
	};

	// RECT rcFrame; - ���� � MSDN
} AVIStreamHeaderCustom;

class CAviPlayerCustom
{
protected:
	CAviPlayerCustom* alpha;
protected:
	AVIINDEXENTRY* m_pMovieIndex;
	BYTE* m_pMovieData;
	HIC m_aviIC;
	BYTE* m_pDecompressedBuf;

	BITMAPINFOHEADER m_biOutFormat;
	BITMAPINFOHEADER m_biInFormat;

	float m_fRate; // ����������� ��������, fps
	float m_fCurrentRate; // ������� ��������, fps

	DWORD m_dwFrameTotal;
	DWORD m_dwFrameCurrent;
	u32 m_dwFirstFrameOffset;


	DWORD CalcFrame();

	BOOL DecompressFrame(DWORD dwFrameNum);
	VOID PreRoll(DWORD dwFrameNum);

public:
	CAviPlayerCustom();
	~CAviPlayerCustom();

	DWORD m_dwWidth, m_dwHeight;

	VOID GetSize(DWORD* dwWidth, DWORD* dwHeight);

	BOOL Load(char* fname);
	BOOL GetFrame(BYTE** pDest);

	BOOL NeedUpdate() { return CalcFrame() != m_dwFrameCurrent; }
	INT SetSpeed(INT nPercent);
};
