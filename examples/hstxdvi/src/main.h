
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#ifndef _MAIN_H
#define _MAIN_H

#ifdef __cplusplus
extern "C" {
#endif


#if USE_DRAW
extern FRAMETYPE *FrameBuf;
extern FRAMETYPE* pDrawBuf;	// current draw buffer
extern int DispStripInx;	// current index of back buffer strip (-1 = use full FrameBuf)
extern int DispMinY;		// minimal Y; base of back buffer strip
extern int DispMaxY;		// maximal Y + 1; end of back buffer strip

void DispLoad();

void DispDirtyAll();

void DispUpdate();

void DispSetStrip(int inx);

void DispDirtyPoint(int x, int y);

void DispDirtyRect(int x, int y, int w, int h);
#endif

#ifdef __cplusplus
}
#endif

#endif // _MAIN_H
