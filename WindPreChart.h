#if !defined(AFX_WINDPRECHART_H__EFCAAE5D_634E_42DE_90AD_306404521C9A__INCLUDED_)
#define AFX_WINDPRECHART_H__EFCAAE5D_634E_42DE_90AD_306404521C9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindPreChart.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWindPreChart dialog
#include "GridCtrl/GridCtrl.h"
class CWindPreChart : public CDialog
{
// Construction
public:
	CWindPreChart(CWnd* pParent = NULL);   // standard constructor

	void ShowData();
	BOOL ChangeRecord();
	BOOL ShowSelectRowInfo();
	void GridInit();

	CGridCtrl m_PreGrid;

	int m_nRows,m_nCols,m_nFixRows,m_nFixCols;

	//��λ
	int m_nTimeCol,m_nFrontWindCol,m_nIDCol,m_nBackWind1Col,m_nBackWind2Col;

	//����
	int m_nCurCol;

	//��ǰ������
	long m_nSelRow;


	//ADO connection
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;

	int count;
	
	//click or not
	int m_buttonlookbackClick;

// Dialog Data
	//{{AFX_DATA(CWindPreChart)
	enum { IDD = IDD_DIALOG_PRE_CHART };
	CString	m_strTitle;
	CString	m_range;
	CString	m_alarm;
	CString	m_nTime;
	CString	m_nFrontWind;
	CString	m_nBackWind1;
	CString	m_nBackWind2;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindPreChart)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWindPreChart)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonLookback();
	afx_msg void OnButtonOutputTable();
	afx_msg void OnButtonEditRange();
	afx_msg void OnButtonEditAlram();
	afx_msg void OnButtonConfirm();
	afx_msg void OnButtonShowSelect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDPRECHART_H__EFCAAE5D_634E_42DE_90AD_306404521C9A__INCLUDED_)
