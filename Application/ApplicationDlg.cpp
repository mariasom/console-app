
// ApplicationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Application.h"
#include "ApplicationDlg.h"
#include "afxdialogex.h"
#include <utility>
#include <tuple>
#include <vector>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef MIN_SIZE
#define MIN_SIZE 300
#endif

void CStaticImage::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	GetParent()->SendMessage( CApplicationDlg::WM_DRAW_IMAGE, (WPARAM)lpDrawItemStruct);
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {}

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override    // DDX/DDV support
	{
		CDialogEx::DoDataExchange(pDX);
	}

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};


BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CApplicationDlg dialog

CApplicationDlg::CApplicationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_APPLICATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE, m_ctrlImage);
}

BEGIN_MESSAGE_MAP(CApplicationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, OnUpdateFileClose)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_MESSAGE(WM_DRAW_IMAGE, OnDrawImage)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


void CApplicationDlg::OnDestroy()
{
	Default();
}

LRESULT CApplicationDlg::OnDrawImage(WPARAM wParam, LPARAM lParam)
{
	LPDRAWITEMSTRUCT lpDI = (LPDRAWITEMSTRUCT)wParam;

	CDC * pDC = CDC::FromHandle(lpDI->hDC);

	//DRAW BITMAP

	if (m_pimg != nullptr)
	{
		//::MessageBox(NULL, __T("tu som"), __T("test"), MB_OK);

		CBitmap bmp;
		CDC bmDC;
		CBitmap *pOldbmp;
		BITMAP  bi;
		

		bmp.Attach(m_pimg->Detach());
		bmDC.CreateCompatibleDC(pDC);	

		CRect r(lpDI->rcItem);

		pOldbmp = new CBitmap;
		pOldbmp->CreateCompatibleBitmap(pDC, r.Width(), r.Height());

		//SetBitmapDimensionEx((HBITMAP)bmp, r.Width(), r.Height(),NULL);
		
		pOldbmp = bmDC.SelectObject(&bmp);
		bmp.GetBitmap(&bi);

		float factorw = 1.;
		float factorh = 1.;

		factorh = (float)r.Height() / (float)bi.bmHeight;
		factorw = (float)r.Width() / (float)bi.bmWidth;
		
		//pDC->BitBlt(0, 0, r.Width(), r.Height(), &bmDC, 0, 0, SRCCOPY);
		pDC->StretchBlt(0, 0, r.Width(), r.Height(), &bmDC,0,0,bi.bmWidth*factorw*scale(r,bi), bi.bmHeight*factorh*scale(r, bi), SRCCOPY);
		bmDC.SelectObject(pOldbmp);

		m_pimg->Attach((HBITMAP)bmp.Detach());
	}

	return S_OK;
}

void CApplicationDlg::OnClose()
{
	EndDialog(0);
}

BOOL CApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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
	CRect rct;

	CRect rctClient;
	GetClientRect(&rctClient);
	
	m_ctrlImage.GetWindowRect(&rct);
	m_ptImage.x = rctClient.Width() - rct.Width();
	m_ptImage.y = rctClient.Height() - rct.Height();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CApplicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CApplicationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CApplicationDlg::OnFileOpen()
{
	//vymazanie predosleho obrazka
	OnFileClose();
	
	//GET FILE NAME AND CREATE GDIPLUS BITMAP
	CFileDialog jpgdlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Jpg Files (*.jpg)|*.jpg|Png Files (*.png)|*.png||"));

	//Gdiplus::GdiplusStartupInput gdiplusstartupinput;
	//GdiplusStartup(&m_gdiplusToken, &gdiplusstartupinput, NULL);

	if (jpgdlg.DoModal() == IDOK) 
	{
		//std::wstring cesta = jpgdlg.GetPathName();
		//Gdiplus::Bitmap bmp(cesta.c_str());
		CString cesta = jpgdlg.GetPathName();
		m_pimg = new CImage;
		m_pimg->Load(cesta);

		if (m_pimg != nullptr)
		{
			//::MessageBox(NULL, __T("nacitanie OK"), __T(" "), MB_OK | MB_SYSTEMMODAL);
			//prekleslenie vsetkeho
			Invalidate();
		}
		else
		{
			delete m_pimg;
			//	::MessageBox(NULL, __T("nacitanie FAIL"), __T(" "), MB_OK | MB_SYSTEMMODAL);
		}
		
	}
}


void CApplicationDlg::OnUpdateFileOpen(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void CApplicationDlg::OnFileClose()
{
	if (m_pimg != nullptr)
	{
		delete m_pimg;
		m_pimg = nullptr;
		Invalidate();
	}
}


void CApplicationDlg::OnUpdateFileClose(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CApplicationDlg::OnSize(UINT nType, int cx, int cy)
{
	if (::IsWindow(m_ctrlImage.GetSafeHwnd()))
	{
		m_ctrlImage.MoveWindow(0, 0, cx, cy);
		Invalidate();
		__super::OnSize(nType, cx, cy);
	}

}

float CApplicationDlg::scale(CRect r, BITMAP  bi)
{
	float f = 1.;
	if ((bi.bmHeight > r.Height()) && (bi.bmWidth <= r.Width()))
		f = (float)bi.bmHeight / (float)r.Height();
	if ((bi.bmWidth > r.Width()) && (bi.bmHeight <= r.Height()))
		f = (float)bi.bmWidth / (float)r.Width();
	if (((bi.bmWidth < r.Width()) && (bi.bmHeight < r.Height())) || ((bi.bmWidth > r.Width()) && (bi.bmHeight > r.Height())))
	{
		if (r.Height() > r.Width())
			f = (float)bi.bmWidth / (float)r.Width();
		else
		{
			f = (float)bi.bmHeight / (float)r.Height();
		}
	}
	return f;
}