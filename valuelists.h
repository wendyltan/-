#if !defined(AFX_VALUELISTS_H__7FAC884C_F08C_49E8_B3AD_2475993C3754__INCLUDED_)
#define AFX_VALUELISTS_H__7FAC884C_F08C_49E8_B3AD_2475993C3754__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CValueList;

/////////////////////////////////////////////////////////////////////////////
// CValueLists wrapper class

class CValueLists : public COleDispatchDriver
{
public:
	CValueLists() {}		// Calls COleDispatchDriver default constructor
	CValueLists(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CValueLists(const CValueLists& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetCount();
	CValueList GetItems(long Index);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VALUELISTS_H__7FAC884C_F08C_49E8_B3AD_2475993C3754__INCLUDED_)