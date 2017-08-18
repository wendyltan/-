// MaximumChart.cpp : implementation file
//

#include "stdafx.h"
#include "�������⳵���ϵͳ.h"
#include "MaximumChart.h"
#include "excel/excel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaximumChart dialog


CMaximumChart::CMaximumChart(CWnd* pParent /*=NULL*/)
	: CDialog(CMaximumChart::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMaximumChart)
		// NOTE: the ClassWizard will add member initialization here
	m_strTitle = _T("");	
	m_nTime = _T("");
	m_nParameter = _T("");
	m_nCurMax = _T("");
	//}}AFX_DATA_INIT

	m_nRows = 30;
	m_nCols = 4;
	m_nFixRows = 1;
	m_nFixCols = 1;



	m_nIDCol = 0;
	m_nTimeCol = 1;
	m_nParameterCol = 2;
	m_nCurMaxCol = 3;

	m_nCurCol = 0;
	m_nSelRow = 0;

	

	m_buttonlookbackClick = 0;

	//}}AFX_DATA_INIT
}


void CMaximumChart::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaximumChart)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Text(pDX, IDC_TITLE, m_strTitle);
	DDX_Text(pDX, IDC_EDIT_TIME, m_nTime);
	DDX_Text(pDX, IDC_EDIT_PARAMETER, m_nParameter);
	DDX_Text(pDX, IDC_EDIT_MAXIMUM, m_nCurMax);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX,IDC_GRID_MAXIMUM,m_MaxGrid);

}


BEGIN_MESSAGE_MAP(CMaximumChart, CDialog)
	//{{AFX_MSG_MAP(CMaximumChart)
	ON_BN_CLICKED(IDC_BUTTON_LOOKBACK, OnButtonLookback)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT_TABLE, OnButtonOutputTable)
	ON_BN_CLICKED(IDC_BUTTON_CONFIRM, OnButtonConfirm)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_SELECT, OnButtonShowSelect)
	ON_BN_CLICKED(IDC_BUTTON_INSERT_CURRENT, OnButtonInsertCurrent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaximumChart message handlers
void CMaximumChart::GridInit()
{
	m_MaxGrid.EnableDragAndDrop(TRUE);
	m_MaxGrid.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));
	//m_Grid.SetEditable();
	

	m_MaxGrid.SetRowCount(m_nRows);
	m_MaxGrid.SetColumnCount(m_nCols);
	m_MaxGrid.SetFixedRowCount(m_nFixRows);
	m_MaxGrid.SetFixedColumnCount(m_nFixCols);
	char gridHeader[4][20]={"id","ʱ��","��������","��ǰ����ֵ"};

	for(int col=0;col<m_MaxGrid.GetColumnCount();col++)
	{
		GV_ITEM Item;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 0;
		Item.col = col;

		Item.nFormat = DT_LEFT|DT_WORDBREAK|DT_NOPREFIX;
		Item.strText = gridHeader[col];

		m_MaxGrid.SetItem(&Item);
		m_MaxGrid.SetColumnWidth(col,78);
	}
}

BOOL CMaximumChart::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GridInit();
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
	m_pRecordset =m_pConnection->Execute("SELECT COUNT(*) FROM ���ޱ�",&RecordsAffected,adCmdText); 
	_variant_t vIndex = (long)0; 
	_variant_t vCount = m_pRecordset->GetCollect(vIndex); //ȡ�õ�һ���ֶε�ֵ����vCount���� 
	count=vCount.lVal;										//��ȡ��¼���ļ�¼��
	m_pRecordset->Close();										//�رռ�¼�� 

	m_pRecordset.CreateInstance(__uuidof(Recordset));					//������¼������
	CString strSQL="SELECT * FROM ���ޱ�";
	try
	{
		//�����ݿ��д򿪱�
		m_pRecordset->Open(_bstr_t(strSQL), m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	}
	catch (_com_error e)
	{
		CString strError;
		strError.Format("���棺�����ݱ�ʱ�����쳣��������Ϣ��%s",e.ErrorMessage());
		AfxMessageBox(strError);
		return false;
	}


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMaximumChart::OnButtonLookback() 
{
	// TODO: Add your control notification handler code here
		
	if(m_buttonlookbackClick==0){

		ShowData();
	}
	else
	{
		MessageBox("This button can't be click twice.");
	}
		

	
}

void CMaximumChart::OnButtonOutputTable() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	//change here
	CString Field[3]={"ʱ��","��������","��ǰ����"};//�еı���
	_Application exApp;//Ӧ�ó������
	_Workbook exBook;//����������
	Workbooks exBooks;//���������϶���
	Worksheets exSheets;//��������
	_Worksheet exSheet;//���������
	Range exRange;
	Font exFont;//����
	//����wordӦ�ö���
	if(!exApp.CreateDispatch(_T("Excel.Application"))) 
	{
		AfxMessageBox(_T(" Excel failed to start!"));
		return;
	}
	VARIANT varOptional;//��ѡ��������
	VariantInit(&varOptional);
	varOptional.vt=VT_ERROR;
	varOptional.scode=DISP_E_PARAMNOTFOUND;
	//��ӹ��鲾�͹�����
	exBooks=exApp.GetWorkbooks();
	exBook=exBooks.Add(varOptional);
	exSheets=exBook.GetWorksheets();
	exSheet=exSheets.Add(varOptional,varOptional,varOptional,varOptional);
	long nRows,nCols;//�������к���
	
	//change here
	nCols=3;
	nRows=count+1;

	long nstrLen;
	CString strField;
	VARIANT varCol;//���������п�
	VariantInit(&varCol);
	varCol.vt=VT_R4;
	char cCol='A';//A��
	CString strCol;
	for(int i=0;i<nCols;i++)
	{
		CString str=Field[i];
		nstrLen=str.GetLength();
		strCol=cCol;
		strCol=strCol+"1";
		exRange=exSheet.GetRange(COleVariant(strCol),varOptional);
		varCol.fltVal=nstrLen+10;
		exRange.SetColumnWidth(varCol);
		cCol=cCol++;//��A�п�ʼ��ÿ��
	}
	//��ȡ��Ԫ������
	char chr;
	chr='A'+nCols-1;
	CString strCh=chr;
	strCh=strCh+"2";
	VARIANT varUnit;
	VariantInit(&varUnit);
	varUnit.vt=VT_I4;
	varUnit.lVal=15;
	exRange=exSheet.GetRange(COleVariant("A1"),COleVariant(strCh));
	exRange.Merge(varOptional);//�ϲ���Ԫ��
	exFont=exRange.GetFont();
	exFont.SetName(COleVariant("����"));
	exFont.SetSize(varUnit);
	exRange.SetValue2(COleVariant(m_strTitle));//���ñ���
	varUnit.lVal=-4108;
	exRange.SetHorizontalAlignment(varUnit);//ˮƽ����
	exRange.SetVerticalAlignment(varUnit);//��ֱ����

	//������ȫ���ݣ���¼��ر������
	COleSafeArray sarry;
	DWORD ElementsNum[2];
	ElementsNum[0]=nRows;//����
	ElementsNum[1]=nCols;//����
	sarry.Create(VT_BSTR,2,ElementsNum);
	VARIANT varVal;//��ֵ����
	long Index[2];
	CString strvalue;
	m_pRecordset->MoveFirst();
	int k,l;
	for( k=0;k<nRows;k++)
	{
		if(k==0)//��һ��
		{
			for(l=0;l<nCols;l++)
			{
				Index[0]=k;
				Index[1]=l;
				VariantInit(&varVal);
				varVal.vt=VT_BSTR;
				varVal.bstrVal=Field[l].AllocSysString();
				sarry.PutElement(Index,varVal.bstrVal);//��ӵ�������
				SysFreeString(varVal.bstrVal);
				VariantClear(&varVal);
			}
		}
		else
		{
			for(l=0;l<nCols;l++)
			{
				Index[0]=k;
				Index[1]=l;
				//change here
				switch (l)
				{
				case 0:
					{
						strvalue=(char*)(_bstr_t)m_pRecordset->GetCollect("ʱ��");
						break;
					}
				case 1:
					{
						strvalue=(char*)(_bstr_t)m_pRecordset->GetCollect("��������");	
						break;
					}
				case 2:
					{
						strvalue=(char*)(_bstr_t)m_pRecordset->GetCollect("��ǰ����ֵ");	
						break;
					}


				}
				VariantInit(&varVal);
				varVal.vt=VT_BSTR;
				varVal.bstrVal=strvalue.AllocSysString();
				sarry.PutElement(Index,varVal.bstrVal);//��ӵ�������
				SysFreeString(varVal.bstrVal);//��ձ���
				VariantClear(&varVal);
			}
			m_pRecordset->MoveNext();
		}
	}
	strCh=chr;
	CString temp;
	temp.Format("%d",nRows+2);
	strCh=strCh+temp;
	//����ȫ�������ȸµ����ݸ���Excel�������е�ĳ������
	exRange=exSheet.GetRange(COleVariant("A3"),COleVariant(strCh));
	exFont=exRange.GetFont();
	exFont.SetName(COleVariant("����"));
	varUnit.lVal=12;
	exFont.SetSize(varUnit);
	exRange.SetValue2(COleVariant(sarry));
	varUnit.lVal=-4108;
	exRange.SetHorizontalAlignment(varUnit);//ˮƽ����
	exRange.SetVerticalAlignment(varUnit);//��ֱ����
	exApp.SetVisible(true);//��ʾ�ĵ�
}



BOOL CMaximumChart::ShowSelectRowInfo()
{
	// TODO: Add your control notification handler code here
	

	m_nTime = m_MaxGrid.GetItemText(m_nSelRow,1);
	m_nParameter = m_MaxGrid.GetItemText(m_nSelRow,2);
	m_nCurMax = m_MaxGrid.GetItemText(m_nSelRow,3);


	UpdateData(FALSE);


	return TRUE;
	
}




BOOL CMaximumChart::ChangeRecord()
{
	CString strSQL="SELECT * FROM ���ޱ�";
	try
	{
		//�����ݿ��д򿪱�
		m_pRecordset->Open(_bstr_t(strSQL), m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	}
	catch (_com_error e)
	{
		CString strError;
		strError.Format("���棺�����ݱ�ʱ�����쳣��������Ϣ��%s",e.ErrorMessage());
		AfxMessageBox(strError);
		return FALSE;
	}


	
	 
	 if(m_nTime.IsEmpty() || m_nParameter.IsEmpty()|| m_nCurMax.IsEmpty()) 
	 {
		  MessageBox("values can not be empty!");    
		  return FALSE;
	 }
	 if(m_nSelRow == 0){
		 MessageBox("Please select a row first!");
		 return FALSE;
	 }
	 UpdateData(FALSE);

	 try
	 {
		  
		  
		  m_nSelRow--;
		  
		  m_pRecordset->Move(m_nSelRow);
		
		  m_pRecordset->PutCollect("ʱ��",_variant_t(m_nTime));
		  m_pRecordset->PutCollect("��������",_variant_t(m_nParameter));
		  m_pRecordset->PutCollect("��ǰ����ֵ",_variant_t(m_nCurMax));

		  m_nSelRow++;
		  
		  m_MaxGrid.SetItemText(m_nSelRow,m_nTimeCol,m_nTime);
		  m_MaxGrid.SetItemText(m_nSelRow,m_nParameterCol,m_nParameter);
		  m_MaxGrid.SetItemText(m_nSelRow,m_nCurMaxCol,m_nCurMax);
		
		  


		  m_pRecordset->Update();     
		  m_pRecordset->Close(); 
		  m_MaxGrid.Invalidate();
	 }
	 catch(...)             
	 {
	  MessageBox("����ʧ��");        
	  return FALSE;
	 }
	 MessageBox("�޸ĳɹ�");         
	return TRUE;
}

void CMaximumChart::OnButtonConfirm() 
{
	// TODO: Add your control notification handler code here
	
	if(AfxMessageBox("are u sure you want to change this row.",MB_OKCANCEL)==IDOK)
	{
		UpdateData();
		ChangeRecord();
	}
}

void CMaximumChart::OnButtonShowSelect() 
{
	// TODO: Add your control notification handler code here
	int nRow = m_MaxGrid.GetFocusCell().row;
	m_nSelRow = nRow;

	if(m_nSelRow == 0){
		MessageBox("Please select a row!");
	}
	else{
		ShowSelectRowInfo();
	}

}

void CMaximumChart::ShowData()
{
	CString str;

 		
 		//�õ��в��ܱ༭
 		for (int row = m_MaxGrid.GetFixedRowCount(); row < m_MaxGrid.GetRowCount(); row++)
		{
 			for (int col =m_MaxGrid.GetFixedColumnCount(); col < m_MaxGrid.GetColumnCount(); col++)
			{
 			m_MaxGrid.SetItemState(row,col,m_MaxGrid.GetItemState(row,col) | GVIS_READONLY);
 			}
 		}
 
 
 		int nRow = 1;
		while(!m_pRecordset->adoEOF)
 		{
			
			//���

			CString newstr,str;
			str.Format("%d",nRow);
		    m_MaxGrid.SetItemText(nRow,m_nIDCol,str);
				
 			
			//ʱ��
 			str = (char*)(_bstr_t)m_pRecordset->GetCollect(_variant_t("ʱ��"));
 		    m_MaxGrid.SetItemText(nRow,m_nTimeCol,str);
		
 			//��������
			str = (char*)(_bstr_t)m_pRecordset->GetCollect(_variant_t("��������"));
 			m_MaxGrid.SetItemText(nRow,m_nParameterCol,str);

			//��ǰ����ֵ
			str = (char*)(_bstr_t)m_pRecordset->GetCollect(_variant_t("��ǰ����ֵ"));
 			m_MaxGrid.SetItemText(nRow,m_nCurMaxCol,str);
 


 
 			
 			nRow++;
			m_pRecordset->MoveNext();
 		}
		m_nCurCol = nRow;//���������

		m_pRecordset->Close();
	    m_MaxGrid.Invalidate();
		m_buttonlookbackClick++;
}

void CMaximumChart::OnButtonInsertCurrent() 
{
	// TODO: Add your control notification handler code here
	COleDateTime CurTime = COleDateTime::GetCurrentTime(); 
	CString csTime = CurTime.Format("%Y-%m-%d %H:%M:%S");
	m_nTime = csTime;
	UpdateData(FALSE);
}
