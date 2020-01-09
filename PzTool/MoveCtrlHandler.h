#if !defined(AFX_MOVECTRLHANDLER_H__D2C119F0_5B9B_4759_A8EB_8B71F45402B6__INCLUDED_)
#define AFX_MOVECTRLHANDLER_H__D2C119F0_5B9B_4759_A8EB_8B71F45402B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>


typedef CArray<HWND, HWND>  WndowArray;


// for support move controls in the window
class CMoveCtrlHandler
{
public:
	CMoveCtrlHandler();
	virtual ~CMoveCtrlHandler();
	// set the parent window contained the controls.
	void Initialize(HWND  hParent);
	// add a control to moveable controls array.
	void AddCtrl(HWND hCtrl);
	// hook the message send to the parent window or contorls.
	BOOL HookMessage(MSG* pMsg);
private:
	BOOL HookLoop(HWND hCtrl, POINT  pt);
	void DoMoveCtrl(HWND hCtrl, POINT ptOrig, POINT ptNew);
private:

	WndowArray   m_hWnds;  // a array  contained all the moveable controls
	HWND         m_hParent; // parent window of the contorls.
};

#endif //