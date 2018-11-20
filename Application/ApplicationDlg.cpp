
// ApplicationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Application.h"
#include "ApplicationDlg.h"
#include "afxdialogex.h"
#include <utility>
#include <tuple>
#include<algorithm>
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

void CStaticHist::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	GetParent()->SendMessage(CApplicationDlg::WM_DRAW_HISTOGRAM, (WPARAM)lpDrawItemStruct);
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
	DDX_Control(pDX, HISTOGRAM, m_ctrlHist);
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
	ON_MESSAGE(WM_DRAW_HISTOGRAM, OnDrawHist)
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

		pOldbmp = bmDC.SelectObject(&bmp);
		bmp.GetBitmap(&bi);

		pDC->FillSolidRect(r.left, r.top, r.Width(), r.Height(), RGB(211, 211, 211));

		double dWtoH = (double)bi.bmWidth / (double)bi.bmHeight;
		UINT nHeight = r.Height();
		UINT nWidth = (UINT)(dWtoH * (double)nHeight);

		if (nWidth > (UINT)r.Width())
		{
			nWidth = r.Width();
			nHeight = (UINT)(nWidth / dWtoH);
			_ASSERTE(nHeight <= (UINT)r.Height());
		}

		pDC->SetStretchBltMode(HALFTONE);
		pDC->StretchBlt(r.left + (r.Width() - nWidth) / 2, r.top + (r.Height() - nHeight) / 2, nWidth, nHeight, &bmDC, 0, 0, bi.bmWidth, bi.bmHeight, SRCCOPY);
//		pDC->StretchBlt(0,0, bi.bmWidth, bi.bmHeight, &bmDC, 0, 0, bi.bmWidth, bi.bmHeight, SRCCOPY);
		bmDC.SelectObject(pOldbmp);

//		histogram(bi.bmWidth, bi.bmHeight, pDC);

/*		COLORREF col = 0;
//		BYTE bytecol;
		int rcol, gcol, bcol;
		int poc = 0;
	//	for (int i = (r.left + (r.Width() - nWidth) / 2) ; i < nWidth; i++)
		for (int i = 0; i < bi.bmWidth; i++)
		{
	//		for (int j = (r.top + (r.Height() - nHeight) / 2) ; j < nHeight; j++)
			for (int j = 0; j < bi.bmHeight; j++)
			{
				col = GetPixel(*pDC, i, j);

				rcol = (int)GetRValue(col);
				gcol = (int)GetGValue(col);
				bcol = (int)GetBValue(col);
				 
				Red[rcol] = Red[rcol] + 1;
				Green[gcol] = Green[gcol] + 1;
				Blue[bcol] = Blue[bcol] + 1;
			}
		}*/
		//::MessageBox(NULL, __T("vypocet done"), __T(" "), MB_OK | MB_SYSTEMMODAL);

		m_pimg->Attach((HBITMAP)bmp.Detach());
	}

	return S_OK;
}

LRESULT CApplicationDlg::OnDrawHist(WPARAM wParam, LPARAM lParam)
{
	LPDRAWITEMSTRUCT lpDI = (LPDRAWITEMSTRUCT)wParam;

	CDC * pDC = CDC::FromHandle(lpDI->hDC);

	//DRAW BITMAP
	CRect r(lpDI->rcItem);

	if (m_pimg != nullptr)
	{

		FillRect(*pDC, &r, CreateSolidBrush(RGB(211, 211, 211)));

		CPoint DrawTo;
		int maxr = *std::max_element(Red.begin(),Red.end());
		int maxg = *std::max_element(Green.begin(), Green.end());
		int maxb = *std::max_element(Blue.begin(), Blue.end());

		std::vector<int> maxv{maxr,maxb,maxb};

		int maxe = *std::max_element(maxv.begin(), maxv.end());

		float scaleX = r.Width() / (float)256.;
		float scaleY = r.Height() / (float)(log10(maxe));
		
		draw_hist(pDC, r ,scaleX, scaleY, Red, RGB(255, 0, 0));
		draw_hist(pDC, r, scaleX, scaleY, Green, RGB(0, 255, 0));
		draw_hist(pDC, r, scaleX, scaleY, Blue, RGB(0, 0, 255));
		
		/*SetDCPenColor(lpDI->hDC, RGB(255, 0, 0));
		pDC->SelectStockObject(DC_PEN);

		for (int i = 0; i < Red.size(); i++)
		{
			pDC->MoveTo((int)(scaleX*(float)i), (int)((r.Height()) - scaleY*(float)Red[i]));
			pDC->LineTo((int)(scaleX*(float)i), (int)(r.Height()));
		}
		SetDCPenColor(lpDI->hDC, RGB(0, 255, 0));
		pDC->SelectStockObject(DC_PEN);
		for (int i = 0; i < Green.size(); i++)
		{
			pDC->MoveTo((int)(scaleX*(float)i), (int)((r.Height()) - scaleY*(float)Green[i]));
			pDC->LineTo((int)(scaleX*(float)i), (int)(r.Height()));
		}
		SetDCPenColor(lpDI->hDC, RGB(0, 0, 255));
		pDC->SelectStockObject(DC_PEN);
		for (int i = 0; i < Blue.size(); i++)
		{
			pDC->MoveTo((int)(scaleX*(float)i), (int)((r.Height()) - scaleY*(float)Blue[i]));
			pDC->LineTo((int)(scaleX*(float)i), (int)(r.Height()));
		}*/
	}
	else
		FillRect(*pDC, &r, CreateSolidBrush(RGB(255, 255, 255)));
	
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
	//OnFileClose();
	
	//GET FILE NAME AND CREATE GDIPLUS BITMAP
	CFileDialog jpgdlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Jpg Files (*.jpg)|*.jpg|Png Files (*.png)|*.png||"));

	//Gdiplus::GdiplusStartupInput gdiplusstartupinput;
	//GdiplusStartup(&m_gdiplusToken, &gdiplusstartupinput, NULL);

	if (jpgdlg.DoModal() == IDOK) 
	{
		//std::wstring cesta = jpgdlg.GetPathName();
		//Gdiplus::Bitmap bmp(cesta.c_str());
		CString cesta = jpgdlg.GetPathName();

		if (m_pimg == nullptr)
		{
			m_pimg = new CImage;
			if (m_pimg->Load(cesta))
			{
				delete m_pimg;
				m_pimg = nullptr;
			}
			else
			{
				histogram();
			}
			//::MessageBox(NULL, __T("nacitanie OK"), __T(" "), MB_OK | MB_SYSTEMMODAL);
			//prekleslenie vsetkeho
		}
		else
		{
			m_pimg->Detach();
			if (m_pimg->Load(cesta))
			{
				delete m_pimg;
				m_pimg = nullptr;
			}
			else
			{
				histogram();
			}
			//	::MessageBox(NULL, __T("nacitanie FAIL"), __T(" "), MB_OK | MB_SYSTEMMODAL);
		}

		Invalidate();
		
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
		m_ctrlImage.MoveWindow(0 + cx*0.2, 0, cx - 0.2*cx, cy);
	}

	if (::IsWindow(m_ctrlHist.GetSafeHwnd()))
	{
		m_ctrlHist.MoveWindow(0, (int)(cy*0.5), cx - 0.8*cx, (int)(cy*0.5));
	}

	Invalidate();
	__super::OnSize(nType, cx, cy);

}

float CApplicationDlg::scale(CRect r, BITMAP  bi)
{
	float f = 1.;
	if ((bi.bmHeight > r.Height())/* && (bi.bmWidth <= r.Width())*/)
		f = (float)bi.bmHeight / (float)r.Height();
	if ((bi.bmWidth > r.Width())/*&& (bi.bmHeight <= r.Height())*/)
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

void CApplicationDlg::histogram()
{

	COLORREF col = 0;
	int rcol, gcol, bcol;
	int poc = 0;
	
	for (int i = 0; i < Red.size(); i++)
	{
		Red[i] = 0;
		Green[i] = 0;
		Blue[i] = 0;
	}

	//	for (int i = (r.left + (r.Width() - nWidth) / 2) ; i < nWidth; i++)
	for (int i = 0; i < m_pimg->GetWidth(); i++)
	{
		//		for (int j = (r.top + (r.Height() - nHeight) / 2) ; j < nHeight; j++)
		for (int j = 0; j < m_pimg->GetHeight(); j++)
		{
			col = m_pimg->GetPixel(i, j);

			rcol = (int)GetRValue(col);
			gcol = (int)GetGValue(col);
			bcol = (int)GetBValue(col);

			Red[rcol] = Red[rcol] + 1;
			Green[gcol] = Green[gcol] + 1;
			Blue[bcol] = Blue[bcol] + 1;
		}
	}
}

void CApplicationDlg::draw_hist(CDC *pDC, CRect r, float scaleX, float scaleY , std::vector<int> vect, COLORREF col)
{
	
	/*SetDCPenColor(r., col);*/
//	pDC->SelectStockObject(DC_PEN);

	pDC->SelectStockObject(BLACK_PEN);
	for (int i = 0; i < vect.size(); i++)
	{
		
		pDC->FillSolidRect((int)(scaleX*(float)i), (int)((r.Height()) - scaleY*(float)log10(vect[i])), (int)(scaleX + 1), (int)(scaleY*(float)(log10(vect[i]))), col);
	
	/*	pDC->MoveTo((int)(scaleX*(float)i), (int)((r.Height()) - scaleY*(float)vect[i]));
		pDC->LineTo((int)(scaleX*(float)i), (int)(r.Height()));*/
	}

}