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
    BYTE rxdata[2048]; //设置BYTE数组 An 8-bit integerthat is not signed.
    CString strtemp1;
	CString strtemp2;
	CString strtemp3;
    if(m_Comm.GetCommEvent()==2) //事件值为2表示接收缓冲区内有字符
    {             ////////以下你可以根据自己的通信协议加入处理代码
        variant_inp=m_Comm.GetInput(); //读缓冲区
        safearray_inp=variant_inp; //VARIANT型变量转换为ColeSafeArray型变量
        //len=safearray_inp.GetOneDimSize(); //得到有效数据长度
        for(k=0;k<2;k++)
            safearray_inp.GetElement(&k,rxdata+k);//转换为BYTE型数组
                
        BYTE bt=*(char*)(rxdata+0); //字符型
		if( bt == '1' )
		{
			bt = *(char*)(rxdata+1);
			m_RDZB1 = "";
			if( bt == '0' )
			{
				strtemp1.Format( "%s", "STOP" ); //将字符送入临时变量strtemp存放
				m_RDZB1+=strtemp1; //加入接收编辑框对应字符串 
				m_WTZB = "";
				m_WTZB += "21";
				m_Comm.SetOutput(COleVariant( m_WTZB ));//发送数据
			}
			else if( bt == '1' )
			{
				strtemp1.Format( "%s", "GO" ); //将字符送入临时变量strtemp存放
				m_RDZB1+=strtemp1; //加入接收编辑框对应字符串

			}
		}
		else if( bt == '2' )
		{
			bt = *(char*)(rxdata+1);
			m_RDZB2 = "";
			if( bt == '0' )
			{
				strtemp2.Format( "%s", "STOP" ); //将字符送入临时变量strtemp存放
				m_RDZB2+=strtemp2; //加入接收编辑框对应字符串 
			}
			else if( bt == '1' )
			{
				strtemp2.Format( "%s", "GO" ); //将字符送入临时变量strtemp存放
				m_RDZB2+=strtemp2; //加入接收编辑框对应字符串 
			}
		}
		else if( bt == '3' )
		{
			bt = *(char*)(rxdata+1);
			m_RDZB3 = "";
			if( bt == '0' )
			{
				strtemp3.Format( "%s", "STOP" ); //将字符送入临时变量strtemp存放
				m_RDZB3+=strtemp3; //加入接收编辑框对应字符串 
			}
			else if( bt == '1' )
			{
				strtemp3.Format( "%s", "GO" ); //将字符送入临时变量strtemp存放
				m_RDZB3+=strtemp3; //加入接收编辑框对应字符串 
			}
		}
    }
    //UpdateData(FALSE); //更新编辑框内容
	SetDlgItemText(IDC_RD_ZB1,m_RDZB1);//不使用UpdateData(FALSE);的原因是该函数会
	SetDlgItemText(IDC_RD_ZB2,m_RDZB2);
	SetDlgItemText(IDC_RD_ZB3,m_RDZB3);
	//刷新整个对话框的数据，而SetDlgItemText()
	//只更新接收编辑框的数据。	
}

void CSerialForZBDlg::OnOpenSerial() 
{
		// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	if(m_Comm.GetPortOpen())
	m_Comm.SetPortOpen(FALSE);

	m_Comm.SetCommPort(5); //选择com1，可根据具体情况更改
	m_Comm.SetInBufferSize(1024); //设置输入缓冲区的大小，Bytes
	m_Comm.SetOutBufferSize(1024); //设置输入缓冲区的大小，Bytes//
	m_Comm.SetSettings("115200,n,8,1"); //波特率9600，无校验，8个数据位，1个停止位
	m_Comm.SetInputMode(1); //1：表示以二进制方式检取数据
	m_Comm.SetRThreshold(1); 
	//参数1表示每当串口接收缓冲区中有多于或等于1个字符时将引发一个接收数据的OnComm事件
	m_Comm.SetInputLen(0); //设置当前接收区数据长度为0
	if( !m_Comm.GetPortOpen())
	m_Comm.SetPortOpen(TRUE);//打开串口
	else
	AfxMessageBox("cannot open serial port");
	m_Comm.GetInput();//先预读缓冲区以清除残留数据
	UpdateData(false);
}

void CSerialForZBDlg::OnSendData() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE); //读取编辑框内容
	m_Comm.SetOutput(COleVariant(m_WTZB));//发送数据
}
