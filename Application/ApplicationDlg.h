
// ApplicationDlg.h : header file
//

#pragma once

#include <GdiPlus.h>
#include <vector>

class CStaticImage : public CStatic
{
public:
	// Overridables (for owner draw only)
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) override;
};

class CStaticHist : public CStatic
{
public:
	// Overridables (for owner draw only)
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) override;
};

// CApplicationDlg dialog
class CApplicationDlg : public CDialogEx
{
// Construction
public:
	enum
	{
		WM_DRAW_IMAGE = (WM_USER + 1),
		WM_DRAW_HISTOGRAM // = (WM_USER + 1)
	};

	CApplicationDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_APPLICATION_DIALOG };
#endif

	protected:
	void DoDataExchange(CDataExchange* pDX) override;	// DDX/DDV support

	void OnOK() override {}
	void OnCancel() override {}


// Implementation
protected:
	HICON m_hIcon;
	CImage *m_pimg = nullptr;

	/*int *m_phistR[255] = {0};
	int *m_phistG[255] = {0};
	int *m_phistB[255] = {0};*/

	std::vector<int> Red = std::vector<int>(256,0);
	std::vector<int> Green = std::vector<int>(256, 0);
	std::vector<int> Blue = std::vector<int>(256, 0);

	int mine;
	int maxe;

//	ULONG_PTR(m_gdiplusToken);
//	Gdiplus::Bitmap bmp;
//	Gdiplus::Bitmap *newbmp; 
	// Generated message map functions
	BOOL OnInitDialog() override;
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnFileOpen();
	afx_msg void OnUpdateFileOpen(CCmdUI *pCmdUI);
	afx_msg void OnFileClose();
	afx_msg void OnUpdateFileClose(CCmdUI *pCmdUI);
	afx_msg void OnClose();
	afx_msg LRESULT OnDrawImage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDrawHist(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	float scale(CRect r, BITMAP  bi);
	void histogram();
	void draw_hist(CDC *pDC, CRect r,float scaleX, float scaleY, std::vector<int> vect, COLORREF col);
	void OnHistRed();
	void OnHistGreen();
	void OnHistBlue();

protected:
	CStaticImage m_ctrlImage;
	CStaticHist m_ctrlHist;
	CPoint m_ptImage;
	CPoint m_ptHist;
	bool check_r = FALSE;
	bool check_g = FALSE;
	bool check_b = FALSE;

};
