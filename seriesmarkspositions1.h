#if !defined(AFX_SERIESMARKSPOSITIONS1_H__9A99C156_A5B5_4C8D_9082_4A7AADE23BC7__INCLUDED_)
#define AFX_SERIESMARKSPOSITIONS1_H__9A99C156_A5B5_4C8D_9082_4A7AADE23BC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CSeriesMarkPosition;

/////////////////////////////////////////////////////////////////////////////
// CSeriesMarksPositions wrapper class

class CSeriesMarksPositions : public COleDispatchDriver
{
public:
	CSeriesMarksPositions() {}		// Calls COleDispatchDriver default constructor
	CSeriesMarksPositions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSeriesMarksPositions(const CSeriesMarksPositions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CSeriesMarkPosition GetPosition(long Index);
	void Automatic(long Index);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIESMARKSPOSITIONS1_H__9A99C156_A5B5_4C8D_9082_4A7AADE23BC7__INCLUDED_)