// ParaCal.cpp : implementation file
//

#include "stdafx.h"
#include "�������⳵���ϵͳ.h"
#include "ParaCal.h"
#include<algorithm>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParaCal dialog
struct PTYPE{
	CString TableTypeName;
	int TableTypeID;
	CString TableRow[4];
}tabletype[8];


CParaCal::CParaCal(CWnd* pParent /*=NULL*/)
	: CDialog(CParaCal::IDD, pParent)
{
	//{{AFX_DATA_INIT(CParaCal)
	m_Average = _T("");
	m_Max = _T("");
	m_Min = _T("");
	//}}AFX_DATA_INIT

	curTableIndex = 0;
	curTableName = _T("");
	curRowIndex = 0;
	curRowName = _T("");

	for(int i=0;i<100;i++)
	{
		TempData[i] = 0;
	}

	//type init----------------------------------------------------------
	tabletype[0].TableTypeName = "��೵��γ�ȱ�"; //current position
	tabletype[0].TableTypeID = 0;
	tabletype[0].TableRow[0] = "��೵����";
	tabletype[0].TableRow[1] = "��೵γ��";


	tabletype[1].TableTypeName = "��೵λ�ñ�"; //position
	tabletype[1].TableTypeID = 1;
	tabletype[1].TableRow[0] = "����ǰλ��";
	tabletype[1].TableRow[1] = "����ǰλ��";
	tabletype[1].TableRow[2] = "�����λ��";
	tabletype[1].TableRow[3] = "���Һ�λ��";


	tabletype[2].TableTypeName = "��ˮ���"; //clean water box
	tabletype[2].TableTypeID = 2;
	tabletype[2].TableRow[0] = "��ˮ��ˮλ";

	tabletype[3].TableTypeName = "ˮ��ѹ����"; //water pressure
	tabletype[3].TableTypeID = 3;
	tabletype[3].TableRow[0] = "��ѹˮ��ѹ��";
	tabletype[3].TableRow[1] = "��ѹˮ��ѹ��";

	tabletype[4].TableTypeName = "�¶ȱ�"; //temperature
	tabletype[4].TableTypeID = 4;
	tabletype[4].TableRow[0] = "��������ˮ��";
	tabletype[4].TableRow[1] = "���⻷���¶�";
	tabletype[4].TableRow[2] = "�����¶�";

	tabletype[5].TableTypeName = "��ˮ���"; //dirty water box
	tabletype[5].TableTypeID = 5;
	tabletype[5].TableRow[0] = "��ˮ��ˮλ";

	tabletype[6].TableTypeName = "������ѹ����"; //wind pressure
	tabletype[6].TableTypeID = 6;
	tabletype[6].TableRow[0] = "ǰ������ѹ��";
	tabletype[6].TableRow[1] = "��������1ѹ��";
	tabletype[6].TableRow[2] = "��������2ѹ��";

	tabletype[7].TableTypeName = "�������ѹ";//volt
	tabletype[7].TableTypeID = 7;
	tabletype[7].TableRow[0] = "�������ѹ";


	DatabaseConnect();
}


void CParaCal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParaCal)
	DDX_Text(pDX, IDC_EDIT_AVG, m_Average);
	DDX_Text(pDX, IDC_EDIT_MAX, m_Max);
	DDX_Text(pDX, IDC_EDIT_MIN, m_Min);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParaCal, CDialog)
	//{{AFX_MSG_MAP(CParaCal)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_TABLE, OnSelchangeComboTable)
	ON_CBN_SELCHANGE(IDC_COMBO_COL, OnSelchangeComboCol)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParaCal message handlers
void CParaCal::DatabaseConnect()
{
	//��ʼ��com����
	::CoInitialize(NULL);
	//�������Ӷ���ͼ�¼������
	HRESULT hr;
	try
	{
		hr = m_pConnection.CreateInstance("ADODB.Connection");//����Connection����
		if(SUCCEEDED(hr))
		{
			hr = m_pConnection->Open(
				"Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=��೵;Data Source=WENDY-PC",
				"","",adModeUnknown);///�������ݿ�
		}
	}
	catch(_com_error e)///��׽�쳣
	{
		CString errormessage;
		errormessage.Format("�������ݿ�ʧ��!\r\n������Ϣ:%s",e.ErrorMessage());
		AfxMessageBox(errormessage);///��ʾ������Ϣ
	}
}


void CParaCal::DatabaseDisconnect()
{
	::CoUninitialize();
	m_pRecordset->Close();

}

BOOL CParaCal::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	for(int i=0;i<8;i++)
	{
		m_pCurrentset[i] = JudgeType(i);
	}

	CString strTemp;
	((CComboBox*)GetDlgItem(IDC_COMBO_TABLE))->ResetContent(); //����������������
	for(int j=0;j<8;j++)
	{
	   strTemp = tabletype[j].TableTypeName;
	   ((CComboBox*)GetDlgItem(IDC_COMBO_TABLE))->InsertString(j,strTemp);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


_RecordsetPtr CParaCal::JudgeType(int type)
{
	
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	CString sqlstr;
	sqlstr.Format("SELECT * FROM %s",tabletype[type].TableTypeName);
		

	HRESULT hr = m_pRecordset->Open(sqlstr.AllocSysString(),
			m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
			
		m_pRecordset->MoveFirst();
		

	return m_pRecordset;
	
}

void CParaCal::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	DatabaseDisconnect();
}

void CParaCal::ifSelect()
{
	int iPos=((CComboBox*)GetDlgItem(IDC_COMBO_TABLE))->GetCurSel();

	((CComboBox*)GetDlgItem(IDC_COMBO_TABLE))->GetLBText(iPos,curTableName);

	for(int i=0;i<8;i++)
	{
		if(tabletype[i].TableTypeName == curTableName)
		{
			curTableIndex = i;
			break;
		}
	}


}



void CParaCal::OnSelchangeComboTable() 
{
	// TODO: Add your control notification handler code here
	ifSelect();
	CString strTemp;
	((CComboBox*)GetDlgItem(IDC_COMBO_COL))->ResetContent(); //����������������
	((CComboBox*)GetDlgItem(IDC_COMBO_COL))->EnableWindow(TRUE);
	for(int j=0;j<4;j++)
	{
		  strTemp = tabletype[curTableIndex].TableRow[j];
		  if(strTemp.IsEmpty())   {}//do nothing
		  else ((CComboBox*)GetDlgItem(IDC_COMBO_COL))->InsertString(j,strTemp);
		  
	}
	
}

void CParaCal::OnSelchangeComboCol() 
{
	// TODO: Add your control notification handler code here
	int iPos=((CComboBox*)GetDlgItem(IDC_COMBO_COL))->GetCurSel();
	curRowName = tabletype[curTableIndex].TableRow[iPos];
	MessageBox(curRowName);
	/*CString ipos;
	ipos.Format("%d",iPos);
	MessageBox(ipos);*/
	int i = 0;
	m_pCurrentset[curTableIndex]->MoveFirst();
	while(!m_pCurrentset[curTableIndex]->adoEOF)
	{

		//cal this col's sum,max and min

		TempData[i] = atof((char*)(_bstr_t)m_pCurrentset[curTableIndex]->GetCollect(_variant_t(curRowName)));
		m_pCurrentset[curTableIndex]->MoveNext();
		i++;
	}

	double tempSum = 0;
	double tempMax = 0;
	double tempMin = 0;
	double tempAve = 0;

	for(int j=0;j<i;j++)
	{
		tempSum += TempData[j];
	}

	tempMax = *max_element(TempData,TempData+i);
	tempMin = *min_element(TempData,TempData+i);

	CString maxString,minString,aveString;
	maxString.Format("%.3f",tempMax);
	minString.Format("%.3f",tempMin);

	m_Max = maxString;
	m_Min = minString;


	tempAve = tempSum/i;
	aveString.Format("%.3f",tempAve);

	m_Average = aveString;

	UpdateData(FALSE);

}
