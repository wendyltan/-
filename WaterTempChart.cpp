// WaterTempChart.cpp : implementation file
//

#include "stdafx.h"
#include "�������⳵���ϵͳ.h"
#include "WaterTempChart.h"
#include "excel/excel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaterTempChart dialog


CWaterTempChart::CWaterTempChart(CWnd* pParent /*=NULL*/)
	: CDialog(CWaterTempChart::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWaterTempChart)
	m_strTitle = _T("");	
	m_range = _T("");
	m_alarm = _T("");
	m_nTime = _T("");
	m_nEngine = _T("");
	m_nOutside = _T("");
	m_nInside = _T("");
	//}}AFX_DATA_INIT

	m_nRows = 30;
	m_nCols = 5;
	m_nFixRows = 1;
	m_nFixCols = 1;



	m_nIDCol = 0;
	m_nTimeCol = 1;
	m_nEngineCol = 2;
	m_nOutsideCol = 3;
	m_nInsideCol = 4;

	m_nCurCol = 0;
	m_nSelRow = 0;

	

	m_buttonlookbackClick = 0;

	//range and alarm data init
	m_range = "1.5";
	m_alarm = "2.0";

}


void CWaterTempChart::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaterTempChart)
	DDX_Text(pDX, IDC_TITLE, m_strTitle);
	DDX_Text(pDX, IDC_RANGE_CHANGE, m_range);
	DDX_Text(pDX, IDC_ALARM_CHANGE, m_alarm);
	DDX_Text(pDX, IDC_EDIT_TIME, m_nTime);
	DDX_Text(pDX, IDC_EDIT_ENGINE, m_nEngine);
	DDX_Text(pDX, IDC_EDIT_OUTSIDE, m_nOutside);
	DDX_Text(pDX, IDC_EDIT_INSIDE, m_nInside);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX,IDC_GRID_TEMP,m_TempGrid);
}


BEGIN_MESSAGE_MAP(CWaterTempChart, CDialog)
	//{{AFX_MSG_MAP(CWaterTempChart)
		// NOTE: the ClassWizard will add message map macros here
	ON_BN_CLICKED(IDC_BUTTON_LOOKBACK, OnButtonLookback)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT_TABLE, OnButtonOutputTable)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_RANGE, OnButtonEditRange)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_ALRAM, OnButtonEditAlram)
	ON_BN_CLICKED(IDC_BUTTON_CONFIRM, OnButtonConfirm)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_SELECT, OnButtonShowSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaterTempChart message handlers
void CWaterTempChart::GridInit()
{
	m_TempGrid.EnableDragAndDrop(TRUE);
	m_TempGrid.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));
	//m_Grid.SetEditable();
	

	m_TempGrid.SetRowCount(m_nRows);
	m_TempGrid.SetColumnCount(m_nCols);
	m_TempGrid.SetFixedRowCount(m_nFixRows);
	m_TempGrid.SetFixedColumnCount(m_nFixCols);
	char gridHeader[5][20]={"id","ʱ��","��������ˮ��","���⻷���¶�","���ڻ����¶�"};

	for(int col=0;col<m_TempGrid.GetColumnCount();col++)
	{
		GV_ITEM Item;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 0;
		Item.col = col;

		Item.nFormat = DT_LEFT|DT_WORDBREAK|DT_NOPREFIX;
		Item.strText = gridHeader[col];

		m_TempGrid.SetItem(&Item);
		m_TempGrid.SetColumnWidth(col,78);
	}
}

BOOL CWaterTempChart::OnInitDialog() 
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
	m_pRecordset =m_pConnection->Execute("SELECT COUNT(*) FROM �¶ȱ�",&RecordsAffected,adCmdText); 
	_variant_t vIndex = (long)0; 
	_variant_t vCount = m_pRecordset->GetCollect(vIndex); //ȡ�õ�һ���ֶε�ֵ����vCount���� 
	count=vCount.lVal;										//��ȡ��¼���ļ�¼��
	m_pRecordset->Close();										//�رռ�¼�� 

	m_pRecordset.CreateInstance(__uuidof(Recordset));					//������¼������
	CString strSQL="SELECT * FROM �¶ȱ�";
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

void CWaterTempChart::OnButtonLookback() 
{
	// TODO: Add your control notification handler code here
		
	if(m_buttonlookbackClick==0){

		CString str;

 		
 		//�õ��в��ܱ༭
 		for (int row = m_TempGrid.GetFixedRowCount(); row < m_TempGrid.GetRowCount(); row++)
		{
 			for (int col =m_TempGrid.GetFixedColumnCount(); col < m_TempGrid.GetColumnCount(); col++)
			{
 				m_TempGrid.SetItemState(row,col,m_TempGrid.GetItemState(row,col) | GVIS_READONLY);
 			}
 		}
 
 
 		int nRow = 1;
		while(!m_pRecordset->adoEOF)
 		{
			
			//���

			CString newstr,str;
			str.Format("%d",nRow);
		    m_TempGrid.SetItemText(nRow,m_nIDCol,str);
				
 			
			//ʱ��
 			str = (char*)(_bstr_t)m_pRecordset->GetCollect(_variant_t("ʱ��"));
 		    m_TempGrid.SetItemText(nRow,m_nTimeCol,str);
		
 			//��������ˮ��
			str = (char*)(_bstr_t)m_pRecordset->GetCollect(_variant_t("��������ˮ��"));
 			m_TempGrid.SetItemText(nRow,m_nEngineCol,str);

			//���⻷���¶�
			str = (char*)(_bstr_t)m_pRecordset->GetCollect(_variant_t("���⻷���¶�"));
 			m_TempGrid.SetItemText(nRow,m_nOutsideCol,str);
 

			//���ڻ����¶�
			str = (char*)(_bstr_t)m_pRecordset->GetCollect(_variant_t("���⻷���¶�"));
 			m_TempGrid.SetItemText(nRow,m_nInsideCol,str);
 
 
 			
 			nRow++;
			m_pRecordset->MoveNext();
 		}
		m_nCurCol = nRow;//���������

		m_pRecordset->Close();
	    m_TempGrid.Invalidate();
		m_buttonlookbackClick++;
	}
	else
	{
		MessageBox("This button can't be click twice.");
	}
		

	
}

void CWaterTempChart::OnButtonOutputTable() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	//change here
	CString Field[4]={"ʱ��","��������ˮ��","���⻷���¶�","���ڻ����¶�"};//�еı���
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
	nCols=4;
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
					strvalue=(char*)(_bstr_t)m_pRecordset->GetCollect("ʱ��");
					break;
				case 1:
					strvalue=(char*)(_bstr_t)m_pRecordset->GetCollect("��������ˮ��");	
					break;
				case 2:
					strvalue=(char*)(_bstr_t)m_pRecordset->GetCollect("���⻷���¶�");	
					break;
				case 3:
					strvalue=(char*)(_bstr_t)m_pRecordset->GetCollect("���⻷���¶�");	
					break;


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



BOOL CWaterTempChart::ShowSelectRowInfo()
{
	// TODO: Add your control notification handler code here
	

	m_nTime = m_TempGrid.GetItemText(m_nSelRow,1);
	m_nEngine = m_TempGrid.GetItemText(m_nSelRow,2);
	m_nOutside = m_TempGrid.GetItemText(m_nSelRow,3);
	m_nInside = m_TempGrid.GetItemText(m_nSelRow,4);


	UpdateData(FALSE);


	return TRUE;
	
}

void CWaterTempChart::OnButtonEditRange() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData();
	
	CString str;
	GetDlgItemText(IDC_RANGE_CHANGE,str);
	if(!str.IsEmpty()){

		UpdateData(FALSE);
		MessageBox("range change success!");
	}
	else{
		MessageBox("field is empty! try again.");
	}

}

void CWaterTempChart::OnButtonEditAlram() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	CString str;
	GetDlgItemText(IDC_ALARM_CHANGE,str);
	if(!str.IsEmpty()){

		UpdateData(FALSE);
		MessageBox("alarm change success!");
	}
	else{
		MessageBox("field is empty! try again.");
	}

	
}


BOOL CWaterTempChart::ChangeRecord()
{
	CString strSQL="SELECT * FROM �¶ȱ�";
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


	
	 
	 if(m_nTime.IsEmpty() || m_nEngine.IsEmpty()|| m_nOutside.IsEmpty()|| m_nInside.IsEmpty()) 
	 {
		  MessageBox("values can not be empty!");    
		  return FALSE;
	 }
	 if(m_nSelRow == 0){
		 MessageBox("Please select a row first!");
		 return FALSE;
	 }
	 UpdateData(FALSE);
	 /*
	 int pos   = m_Grid.GetSelectionMark();    
	 ADO m_Ado;         
	 m_Ado.OnInitADOConn();       
	 CString sql = "select * from employees";     
	 m_Ado.m_pRecordset = m_Ado.OpenRecordset(sql);  
	 */
	 try
	 {
		  m_pRecordset->Move(m_nSelRow,vtMissing); 
		
		  m_pRecordset->PutCollect("ʱ��",(_bstr_t)m_nTime);
		  m_pRecordset->PutCollect("��������ˮ��",(_bstr_t)m_nEngine);
		  m_pRecordset->PutCollect("���⻷���¶�",(_bstr_t)m_nOutside);
		  m_pRecordset->PutCollect("���⻷���¶�",(_bstr_t)m_nInside);
		  m_pRecordset->Update();     
		  m_pRecordset->Close(); 
		  m_TempGrid.Invalidate();
	 }
	 catch(...)             
	 {
	  MessageBox("����ʧ��");        
	  return FALSE;
	 }
	 MessageBox("�޸ĳɹ�");         
	return TRUE;
}

void CWaterTempChart::OnButtonConfirm() 
{
	// TODO: Add your control notification handler code here
	
	if(AfxMessageBox("are u sure you want to change this row.",MB_OKCANCEL)==IDOK)
	{
		UpdateData();
		ChangeRecord();
	}
}

void CWaterTempChart::OnButtonShowSelect() 
{
	// TODO: Add your control notification handler code here
	int nRow = m_TempGrid.GetFocusCell().row;
	m_nSelRow = nRow;

	if(m_nSelRow == 0){
		MessageBox("Please select a row!");
	}
	else{
		ShowSelectRowInfo();
	}

}
