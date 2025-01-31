#pragma once
#include <vector>

// CDlgImage 대화 상자

class CDlgImage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgImage)

private:
	int nCircleCount = 0;
	int nCircleRadius = 10;

	std::pair<int, int> mouseXY[3];

public:
	CDlgImage(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgImage();

	CImage m_image;
	CImage m_image_rect;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgImage };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	void InitImage();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	BOOL validImgPos(int x, int y);

	void SetnCircleRadius(int val);
	void MakeCircle(int x, int y);
	void MakeCircle();//외접원 그리기
	void MakeRandomCircle(std::pair<int, int> p_array[]);
	int GetnCircleCount();
	void SetnCircleCount(int val);
};
