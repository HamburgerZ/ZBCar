// SerialForZBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SerialForZB.h"
#include "SerialForZBDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialForZBDlg dialog

CSerialForZBDlg::CSerialForZBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialForZBDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSerialForZBDlg)
	m_RDZB1 = _T("");
	m_RDZB2 = _T("");
	m_RDZB3 = _T("");
	m_WTZB = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialForZBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSerialForZBDlg)
	DDX_Control(pDX, IDC_SEND_DATA, m_Send);
	DDX_Control(pDX, IDC_OPEN_SERIAL, m_Open);
	DDX_Text(pDX, IDC_RD_ZB1, m_RDZB1);
	DDX_Text(pDX, IDC_RD_ZB2, m_RDZB2);
	DDX_Text(pDX, IDC_RD_ZB3, m_RDZB3);
	DDX_Text(pDX, IDC_WT_ZB, m_WTZB);
	DDX_Control(pDX, IDC_MSCOMM, m_Comm);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSerialForZBDlg, CDialog)
	//{{AFX_MSG_MAP(CSerialForZBDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_SERIAL, OnOpenSerial)
	ON_BN_CLICKED(IDC_SEND_DATA, OnSendData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialForZBDlg message handlers

BOOL CSerialForZBDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSerialForZBDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSerialForZBDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSerialForZBDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BEGIN_EVENTSINK_MAP(CSerialForZBDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSerialForZBDlg)
	ON_EVENT(CSerialForZBDlg, IDC_MSCOMM, 1 /* OnComm */, OnOnCommMscomm, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CSerialForZBDlg::OnOnCommMscomm() 
{
	// TODO: Add your control notification handler code here
	VARIANT variant_inp;
    COleSafeArray safearray_inp;
    LONG len,k;
    BYTE rxdata[2048]; //����BYTE���� An 8-bit integerthat is not signed.
    CString strtemp1;
	CString strtemp2;
	CString strtemp3;
    if(m_Comm.GetCommEvent()==2) //�¼�ֵΪ2��ʾ���ջ����������ַ�
    {             ////////��������Ը����Լ���ͨ��Э����봦�����
        variant_inp=m_Comm.GetInput(); //��������
        safearray_inp=variant_inp; //VARIANT�ͱ���ת��ΪColeSafeArray�ͱ���
        //len=safearray_inp.GetOneDimSize(); //�õ���Ч���ݳ���
        for(k=0;k<2;k++)
            safearray_inp.GetElement(&k,rxdata+k);//ת��ΪBYTE������
                
        BYTE bt=*(char*)(rxdata+0); //�ַ���
		if( bt == '1' )
		{
			bt = *(char*)(rxdata+1);
			m_RDZB1 = "";
			if( bt == '0' )
			{
				strtemp1.Format( "%s", "STOP" ); //���ַ�������ʱ����strtemp���
				m_RDZB1+=strtemp1; //������ձ༭���Ӧ�ַ��� 
				m_WTZB = "";
				m_WTZB += "21";
				m_Comm.SetOutput(COleVariant( m_WTZB ));//��������
			}
			else if( bt == '1' )
			{
				strtemp1.Format( "%s", "GO" ); //���ַ�������ʱ����strtemp���
				m_RDZB1+=strtemp1; //������ձ༭���Ӧ�ַ���

			}
		}
		else if( bt == '2' )
		{
			bt = *(char*)(rxdata+1);
			m_RDZB2 = "";
			if( bt == '0' )
			{
				strtemp2.Format( "%s", "STOP" ); //���ַ�������ʱ����strtemp���
				m_RDZB2+=strtemp2; //������ձ༭���Ӧ�ַ��� 
			}
			else if( bt == '1' )
			{
				strtemp2.Format( "%s", "GO" ); //���ַ�������ʱ����strtemp���
				m_RDZB2+=strtemp2; //������ձ༭���Ӧ�ַ��� 
			}
		}
		else if( bt == '3' )
		{
			bt = *(char*)(rxdata+1);
			m_RDZB3 = "";
			if( bt == '0' )
			{
				strtemp3.Format( "%s", "STOP" ); //���ַ�������ʱ����strtemp���
				m_RDZB3+=strtemp3; //������ձ༭���Ӧ�ַ��� 
			}
			else if( bt == '1' )
			{
				strtemp3.Format( "%s", "GO" ); //���ַ�������ʱ����strtemp���
				m_RDZB3+=strtemp3; //������ձ༭���Ӧ�ַ��� 
			}
		}
    }
    //UpdateData(FALSE); //���±༭������
	SetDlgItemText(IDC_RD_ZB1,m_RDZB1);//��ʹ��UpdateData(FALSE);��ԭ���Ǹú�����
	SetDlgItemText(IDC_RD_ZB2,m_RDZB2);
	SetDlgItemText(IDC_RD_ZB3,m_RDZB3);
	//ˢ�������Ի�������ݣ���SetDlgItemText()
	//ֻ���½��ձ༭������ݡ�	
}

void CSerialForZBDlg::OnOpenSerial() 
{
		// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	if(m_Comm.GetPortOpen())
	m_Comm.SetPortOpen(FALSE);

	m_Comm.SetCommPort(5); //ѡ��com1���ɸ��ݾ����������
	m_Comm.SetInBufferSize(1024); //�������뻺�����Ĵ�С��Bytes
	m_Comm.SetOutBufferSize(1024); //�������뻺�����Ĵ�С��Bytes//
	m_Comm.SetSettings("115200,n,8,1"); //������9600����У�飬8������λ��1��ֹͣλ
	m_Comm.SetInputMode(1); //1����ʾ�Զ����Ʒ�ʽ��ȡ����
	m_Comm.SetRThreshold(1); 
	//����1��ʾÿ�����ڽ��ջ��������ж��ڻ����1���ַ�ʱ������һ���������ݵ�OnComm�¼�
	m_Comm.SetInputLen(0); //���õ�ǰ���������ݳ���Ϊ0
	if( !m_Comm.GetPortOpen())
	m_Comm.SetPortOpen(TRUE);//�򿪴���
	else
	AfxMessageBox("cannot open serial port");
	m_Comm.GetInput();//��Ԥ���������������������
	UpdateData(false);
}

void CSerialForZBDlg::OnSendData() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE); //��ȡ�༭������
	m_Comm.SetOutput(COleVariant(m_WTZB));//��������
}
