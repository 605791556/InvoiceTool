#include "stdafx.h"
#include "MoveCtrlHandler.h"
#include "cgloble.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// find the index of the element from a array
template<class T>
int FindArray(CArray<T, T>& arr, T t)
{
	for (int i = 0; i < arr.GetSize(); i++)
	{
		if (arr.GetAt(i) == t)
			return i;
	}
	return  -1;
}

// get the mouse point form a message structure.
static POINT  MakeMessagePoint(const MSG*  pMsg)
{
	POINT  pt;
	pt.x = (long)(short)LOWORD(pMsg->lParam);
	pt.y = (long)(short)HIWORD(pMsg->lParam);
	return pt;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMoveCtrlHandler::CMoveCtrlHandler()
{
	m_hParent = NULL;
}

CMoveCtrlHandler::~CMoveCtrlHandler()
{

}


void CMoveCtrlHandler::Initialize(HWND   hParent)
{
	ASSERT(hParent != NULL);
	m_hParent = hParent;
}

void CMoveCtrlHandler::AddCtrl(HWND  hCtrl)
{
	ASSERT(hCtrl != NULL);
	int id = FindArray(m_hWnds, hCtrl);
	if (id >= 0)  return;

	m_hWnds.Add(hCtrl);
}

BOOL CMoveCtrlHandler::HookMessage(MSG* pMsg)
{
	// drag process started when right click a contorl that contained in array.
	if (pMsg->message != WM_LBUTTONDOWN)
		return FALSE;

 	int id = FindArray(m_hWnds, pMsg->hwnd);
	if (id < 0) return FALSE;

	// capture the mouse point
	SetCapture(m_hParent);

	POINT  pt = MakeMessagePoint(pMsg);
	// mapping the point to parent window
	MapWindowPoints(m_hWnds[id], m_hParent, &pt, 1);

	// into dragging loop
	if (!HookLoop(m_hWnds[id], pt))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CMoveCtrlHandler::HookLoop(HWND hCtrl, POINT pt)
{
	POINT  point = pt;
	MSG    msg;
	POINT  ptNew;
	BOOL   bDrag = FALSE;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		switch (msg.message)
		{
		case WM_LBUTTONDOWN:
			break;
		case WM_MOUSEMOVE:
			// dragging the control.
			ptNew = MakeMessagePoint(&msg);
			if (msg.hwnd != m_hParent)
				MapWindowPoints(msg.hwnd, m_hParent, &ptNew, 1);
			if (ptNew.x != point.x || ptNew.y != point.y)
			{
				DoMoveCtrl(hCtrl, point, ptNew);
				point = ptNew;
				bDrag = TRUE;
			}
			break;
		case WM_LBUTTONUP:
			// leave the dragging mode.
			ReleaseCapture();
			::PostMessage(m_hParent, WM_UPDATEWND, NULL, NULL);
			// if the control isn't dragged. do default act and return
			return bDrag;
		default:
			// process the other message 
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			break;
		}
	}
	return bDrag;
}

void CMoveCtrlHandler::DoMoveCtrl(HWND hCtrl, POINT ptOrig, POINT ptNew)
{
	RECT  rc;

	// get the rectangle of the control
	GetWindowRect(hCtrl, &rc);
	ScreenToClient(m_hParent, (LPPOINT)&rc.left);
	ScreenToClient(m_hParent, (LPPOINT)&rc.right);

	// calculate the new rectangle of the control.
	OffsetRect(&rc, ptNew.x - ptOrig.x, ptNew.y - ptOrig.y);
	MoveWindow(hCtrl, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,TRUE);
	static int i = 0;
	i++;
	if(i== 5)
	{
		//::PostMessage(m_hParent, WM_UPDATEWND, NULL, NULL);
		i = 0;
	}
}