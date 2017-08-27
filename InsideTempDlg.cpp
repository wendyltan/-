// InsideTempDlg.cpp : implementation file
//

#include "stdafx.h"
#include "�������⳵���ϵͳ.h"
#include "InsideTempDlg.h"
#include "TeeChart/tchart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsideTempDlg dialog


CInsideTempDlg::CInsideTempDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsideTempDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInsideTempDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
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
	m_pRecordset.CreateInstance(__uuidof(Recordset));

	_variant_t RecordsAffected; 
	m_pRecordset =m_pConnection->Execute("SELECT COUNT(*) FROM �¶ȱ�",&RecordsAffected,adCmdText); 
	_variant_t vIndex = (long)0; 
	_variant_t vCount = m_pRecordset->GetCollect(vIndex); //ȡ�õ�һ���ֶε�ֵ����vCount���� 
	count=vCount.lVal;										//��ȡ��¼���ļ�¼��
	m_pRecordset->Close();										//�رռ�¼�� 

	 m_pRecordset.CreateInstance(__uuidof(Recordset));
		
	CString strSql;

	strSql.Format("SELECT * FROM �¶ȱ�");

	hr = m_pRecordset->Open(strSql.AllocSysString(),
			m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);


}


void CInsideTempDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsideTempDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX,IDC_INSIDE_TEMP,m_Chart);
}


BEGIN_MESSAGE_MAP(CInsideTempDlg, CDialog)
	//{{AFX_MSG_MAP(CInsideTempDlg)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsideTempDlg message handlers




BOOL CInsideTempDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_RecordNum = 0;

	m_Chart.GetLegend().SetVisible(false);
	m_Chart.GetAspect().SetView3D(FALSE);  
	
	//m_Chart.GetHeader().GetText().SetItem(0,COleVariant("hello"));
 
     m_Chart.GetAxis().GetLeft().GetTitle().SetCaption("�����¶�");  
	 //��������ϵ�ռ�
	 CSeries mycs0 =(CSeries)m_Chart.Series(0);
     CAxes coord = (CAxes)m_Chart.GetAxis();
     CAxis left0 = (CAxis)coord.GetLeft();
	 left0.SetMinimum(0);
     left0.SetMaximum(40);
     left0.SetIncrement(1);
     left0.SetStartPosition(0);
     left0.SetEndPosition(100);
     left0.SetPositionPercent(0);


	 COleDateTime  CurTime;
	 COleDateTimeSpan  tmSpan;
	 CString csTime;

	 CurTime = COleDateTime::GetCurrentTime();  
	 tmSpan = COleDateTimeSpan(0,0,0,1); //1s    
 	 

	 
	 CString str;
			

	 m_pRecordset->MoveFirst();
	 //����ǰʮ����¼��ʼ��
	 for(int i =0;i<10;i++){
		str = (char*)(_bstr_t)m_pRecordset->GetCollect(_variant_t("�����¶�"));	
		csTime= CurTime.Format("%H:%M:%S");
		mycs0.Add(atof(str),csTime,RGB(255,0,0));
		CurTime+= tmSpan;
        m_pRecordset->MoveNext();
		m_RecordNum++;
		

	 }
	 //m_RecordNumΪ9,�Ѿ���ʼ��ǰʮ����¼

	
	
	SetTimer(1,1000,NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInsideTempDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	COleDateTime  CurTime = COleDateTime::GetCurrentTime();
	COleDateTimeSpan  tmSpan = COleDateTimeSpan(0,0,0,1); //1s 
	CString csTime = CurTime.Format("%H:%M:%S");
	CString str;
	if(m_RecordNum==9) m_pRecordset->Move((long)m_RecordNum++);	
	if(nIDEvent == 1){	
			
		//ʹ����������������ᶯ����
		str = (char*)(_bstr_t)m_pRecordset->GetCollect(_variant_t("�����¶�"));	
		csTime= CurTime.Format("%H:%M:%S");
     	m_Chart.Series(0).Add(atof(str),csTime,RGB(255,0,0));
		CurTime+= tmSpan;
		m_Chart.GetAxis().GetBottom().Scroll(1.0,TRUE);
        m_pRecordset->MoveNext();
		if(m_pRecordset->adoEOF) {KillTimer(1);m_pRecordset->Close();MessageBox("END OF RECORD!");}
		m_RecordNum++;
					
		  
	}
	
	
	CDialog::OnTimer(nIDEvent);
}



void CInsideTempDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	::CoUninitialize();

	
}

