
// ApplicationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Application.h"
#include "ApplicationDlg.h"
#include "afxdialogex.h"
#include <utility>
#include <tuple>
#include <algorithm>
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
//	ON_UPDATE_COMMAND_UI(ID_HISTOGRAM_RED, OnHistRed)
	ON_COMMAND(ID_HISTOGRAM_RED, OnHistRed)
	ON_COMMAND(ID_HISTOGRAM_GREEN, OnHistGreen)
	ON_COMMAND(ID_HISTOGRAM_BLUE, OnHistBlue)
	ON_COMMAND(ID_FORMAT_FONT, OnOilPaint)
	ON_WM_TIMER()
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

	if (m_pimg != nullptr && check_filter == FALSE)
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

		m_pimg->Attach((HBITMAP)bmp.Detach());
	}
	if (check_filter == TRUE && m_pimg != nullptr)
	{			
			CBitmap bmp;
			CDC bmDC;
			CBitmap *pOldbmp;
			BITMAP  bi;

			bmp.Attach(m_pimg1.Detach());
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
			pDC->StretchBlt((r.left + (r.Width() - nWidth) / 2), (r.top + (r.Height() - nHeight) / 2), nWidth, nHeight, &bmDC, 0, 0, bi.bmWidth, bi.bmHeight, SRCCOPY);
			//		pDC->StretchBlt(0,0, bi.bmWidth, bi.bmHeight, &bmDC, 0, 0, bi.bmWidth, bi.bmHeight, SRCCOPY);
			bmDC.SelectObject(pOldbmp);

			m_pimg1.Attach((HBITMAP)bmp.Detach());
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

		int minr = *std::min_element(Red.begin(), Red.end());
		int ming = *std::min_element(Green.begin(), Green.end());
		int minb = *std::min_element(Blue.begin(), Blue.end());

		std::vector<int> maxv{maxr,maxg,maxb};
		std::vector<int> minv{ minr,ming,minb };

		maxe = *std::max_element(maxv.begin(), maxv.end());
		mine = *std::min_element(minv.begin(), minv.end());

		float scaleX = r.Width() / (float)256.;
		float scaleY = r.Height() / (float)((maxe - mine));
		
		if(check_r)
			draw_hist(pDC, r ,scaleX, scaleY, Red, RGB(255, 0, 0));
		if (check_g)
			draw_hist(pDC, r, scaleX, scaleY, Green, RGB(0, 255, 0));
		if (check_b)
			draw_hist(pDC, r, scaleX, scaleY, Blue, RGB(0, 0, 255));
	}
	else
	{
		// FillRect(*pDC, &r, CreateSolidBrush(RGB(255, 255, 255)));
		std::vector<int> tmp;
		for (int i = 0; i < Green.size(); i++)
			tmp.push_back(i);
		float scaleX = r.Width() / (float)256.;
		float scaleY = r.Height() / (float)256.;
		for (int i = 0; i < tmp.size(); i++)
		{
			pDC->FillSolidRect((int)(scaleX*(float)i), (int)((r.Height()) - scaleY*(float)(tmp[i])), (int)(scaleX + 1), (int)(scaleY*(float)(tmp[i])), RGB(0,255,0));

		}

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
	//GET FILE NAME AND CREATE GDIPLUS BITMAP
	CFileDialog jpgdlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Jpg Files (*.jpg)|*.jpg|Png Files (*.png)|*.png||"));

	tmp_bool = false;
	tmp1_bool = false;
	if (jpgdlg.DoModal() == IDOK) 
	{
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
				bytePtr = (BYTE*)(m_pimg->GetBits());
				dlzka = m_pimg->GetPitch();
				sirka = m_pimg->GetWidth();
				vyska = m_pimg->GetHeight();

				//OilPainting();

				m_nTimerID = SetTimer(1, 200, nullptr);
				std::thread hist_thread(&CApplicationDlg::histogram, this);
				hist_thread.detach();
			
				std::thread oilpaint_thread(&CApplicationDlg::OilPainting, this);
				oilpaint_thread.detach();

			}
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
				bytePtr = (BYTE*)(m_pimg->GetBits());
				//bytePtrNew = (BYTE*)(m_pimg->GetBits());
				dlzka = m_pimg->GetPitch();
				sirka = m_pimg->GetWidth();
				vyska = m_pimg->GetHeight();

				//OilPainting();

				m_nTimerID = SetTimer(1, 200, nullptr);
				std::thread hist_thread(&CApplicationDlg::histogram, this);
				hist_thread.detach();

				std::thread oilpaint_thread(&CApplicationDlg::OilPainting, this);
				oilpaint_thread.detach(); 
				
			}
		}

		if(tmp_bool == true && tmp1_bool == true)
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
	
	for (int j = 0; j < vyska; j++)
	{
		for (int i = 0; i < sirka; i++)
		{
			// red color
			rcol =bytePtr[3*i + j*dlzka + 2];
			Red[rcol] = Red[rcol] + 1;
			// green color
			gcol = bytePtr[3 * (i) + j*dlzka + 1];
			Green[gcol] = Green[gcol] + 1;
			// blue color
			bcol = bytePtr[3 * (i) + j*dlzka];
			Blue[bcol] = Blue[bcol] + 1;
		}	
	}

	tmp_bool = true;
}

void CApplicationDlg::draw_hist(CDC *pDC, CRect r, float scaleX, float scaleY , std::vector<int> vect, COLORREF col)
{
	
	/*SetDCPenColor(r., col);*/
//	pDC->SelectStockObject(DC_PEN);

	pDC->SelectStockObject(BLACK_PEN);
	for (int i = 0; i < vect.size(); i++)
	{
		// SKONTROLOVAT
		//pDC->FillSolidRect((int)(scaleX*(float)i), (int)(scaleY*(float)log10(vect[i] - mine)), (int)(scaleX + 1), (int)(scaleY*(float)(log10(vect[i] - mine))), col);
		pDC->FillSolidRect((int)(scaleX*(float)i), (int)(r.Height()- scaleY*(float)((vect[i]-mine))), (int)(scaleX + 1), (int)(scaleY*(float)((vect[i]-mine))), col);

/*		pDC->MoveTo((int)(scaleX*(float)i), (int)((r.Height()) - scaleY*(float)(log10(vect[i]-mine))));
		pDC->LineTo((int)(scaleX*(float)i), (int)(r.Height()));*/
	}
	//pDC->FillSolidRect(0, (int)(r.Height() - scaleY*(float)(log10(vect[0] - mine))),10, scaleY*(float)(log10(vect[0] - mine)), RGB(100,0,100));


}

void CApplicationDlg::OnHistRed()
{
	CMenu *pMenu = GetMenu();
	if (GetMenuState(*pMenu, ID_HISTOGRAM_RED, MF_CHECKED))
	{
		check_r = false;
		pMenu->GetSubMenu(1)->CheckMenuItem(ID_HISTOGRAM_RED, MF_UNCHECKED);
	}
	else
	{
		pMenu->GetSubMenu(1)->CheckMenuItem(ID_HISTOGRAM_RED, MF_CHECKED);
		check_r = true;
	}
	Invalidate();
}

void CApplicationDlg::OnHistGreen()
{
	CMenu *pMenu = GetMenu();
	if (GetMenuState(*pMenu, ID_HISTOGRAM_GREEN, MF_CHECKED))
	{
		check_g = false;
		pMenu->GetSubMenu(1)->CheckMenuItem(ID_HISTOGRAM_GREEN, MF_UNCHECKED);
	}
	else
	{
		pMenu->GetSubMenu(1)->CheckMenuItem(ID_HISTOGRAM_GREEN, MF_CHECKED);
		check_g = true;
	}
	Invalidate();
}

void CApplicationDlg::OnHistBlue()
{
	CMenu *pMenu = GetMenu();
	if (GetMenuState(*pMenu, ID_HISTOGRAM_BLUE, MF_CHECKED))
	{
		check_b = false;
		pMenu->GetSubMenu(1)->CheckMenuItem(ID_HISTOGRAM_BLUE, MF_UNCHECKED);
	}
	else
	{
		pMenu->GetSubMenu(1)->CheckMenuItem(ID_HISTOGRAM_BLUE, MF_CHECKED);
		check_b = true;
	}
	Invalidate();
}

void CApplicationDlg::OnOilPaint()
{
	CMenu *pMenu = GetMenu();
	if (GetMenuState(*pMenu, ID_FORMAT_FONT, MF_CHECKED))
	{
		check_filter = false;
		pMenu->GetSubMenu(2)->CheckMenuItem(ID_FORMAT_FONT, MF_UNCHECKED);
	}
	else
	{
		pMenu->GetSubMenu(2)->CheckMenuItem(ID_FORMAT_FONT, MF_CHECKED);
		check_filter = true;
	}

	Invalidate();
}

void CApplicationDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (tmp_bool == true && tmp1_bool == true)
	{
		KillTimer(m_nTimerID);
		Invalidate();
	}
}

void CApplicationDlg::OilPainting()
{
	int radius = 5;
	float intenzita = 10.;
	
	std::vector<int> nSumR = std::vector<int>(256, 0);
	std::vector<int> nSumG = std::vector<int>(256, 0);
	std::vector<int> nSumB = std::vector<int>(256, 0);

	std::vector<int> nIntensityCount = std::vector<int>(256, 0);

	if (/*check_filter == TRUE && */m_pimg != nullptr)
	{
		m_pimg1.Create(sirka, vyska, m_pimg->GetBPP(), NULL);

		for (int nY = radius; nY < vyska - radius; nY++)
		{
			for (int nX = radius; nX < sirka - radius; nX++)
			{
				for (int nY_O = -radius; nY_O <= radius; nY_O++)
				{
					for (int nX_O = -radius; nX_O <= radius; nX_O++)
					{
					
						int nR = bytePtr[(nX + nX_O) * 3 + (nY + nY_O) * dlzka + 2];
						int nG = bytePtr[(nX + nX_O) * 3 + (nY + nY_O) * dlzka + 1];
						int nB = bytePtr[(nX + nX_O) * 3 + (nY + nY_O) * dlzka ];

						int nCurIntensity = (((nR + nG + nB) / 3.0) * intenzita) / 255.;
						if (nCurIntensity > 255)
							nCurIntensity = 255;
						int i = nCurIntensity;
						nIntensityCount[i] = nIntensityCount[i] + 1;

						nSumR[i] = nSumR[i] + nR;
						nSumG[i] = nSumG[i] + nG;
						nSumB[i] = nSumB[i] + nB;
					}
				}

				int nCurMax = 0;
				int nMaxIndex = 0;
				for (int nI = 0; nI < 256; nI++)
				{
					if (nIntensityCount[nI] > nCurMax)
					{
						nCurMax = nIntensityCount[nI];
						nMaxIndex = nI;
					}
				}

				BYTE r = (nSumR[nMaxIndex] / nCurMax);
				BYTE g = (nSumG[nMaxIndex] / nCurMax);
				BYTE b = (nSumB[nMaxIndex] / nCurMax);

				m_pimg1.SetPixel(nX, nY, RGB(r,g,b));

				for (int i = 0; i < nSumB.size(); i++)
				{
					nSumR[i] = 0;
					nSumG[i] = 0;
					nSumB[i] = 0;
					nIntensityCount[i] = 0;
				}
			}
		}

	}
	tmp1_bool = true;
	::MessageBox(NULL, __T("koniec vypoctu"), __T(" "), MB_OK | MB_SYSTEMMODAL);
}