// DlgImage.cpp: 구현 파일
//

#include "pch.h"
#include "MfcStart.h"
#include "DlgImage.h"
#include "afxdialogex.h"
#include <iostream>

// CDlgImage 대화 상자

IMPLEMENT_DYNAMIC(CDlgImage, CDialogEx)

CDlgImage::CDlgImage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DlgImage, pParent)
{

}

CDlgImage::~CDlgImage()
{
}

void CDlgImage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgImage, CDialogEx)
	ON_WM_PAINT()
	ON_WM_MOUSEACTIVATE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CDlgImage 메시지 처리기


BOOL CDlgImage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	MoveWindow(0, 0, 640, 480);

	InitImage();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgImage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	if (m_image)
	{
		m_image.Draw(dc, 0, 0);
	}
}

void CDlgImage::InitImage()
{
	int nWidth = 640;
	int nHeight = 480;
	int nBpp = 8;

	m_image.Create(nWidth, -nHeight, nBpp);
	if (nBpp == 8)
	{
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;

		m_image.SetColorTable(0, 256, rgb);
	}

	int nPitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	memset(fm, 0xff, nWidth * nHeight);
}

int CDlgImage::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//if (nClickCount >= 3)
	//	return CDialogEx::OnMouseActivate(pDesktopWnd, nHitTest, message);

	//nClickCount++;

	//std::cout << nClickCount << std::endl;

	//CPoint point;

	//GetCursorPos(&point);

	//std::cout << point.x << " : " << point.y << std::endl;

	return CDialogEx::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


void CDlgImage::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (m_image_rect != NULL)
	{
		m_image_rect.Destroy();
	}
	
	if (nCircleCount < 3)
	{
		MakeCircle(point.x, point.y);
	}

	if (nCircleCount == 3)
	{
		MakeCircle();
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

BOOL CDlgImage::validImgPos(int x, int y)
{
	int nWidth = m_image_rect.GetWidth();
	int nHeight = m_image_rect.GetHeight();

	CRect rect(0, 0, nWidth, nHeight);

	return rect.PtInRect(CPoint(x, y));
}

void CDlgImage::SetnCircleRadius(int val)
{
	nCircleRadius = val;
}

void CDlgImage::MakeCircle(int x, int y)
{
	if (m_image_rect != NULL)
	{
		m_image_rect.Destroy();
	}

	int nWidth = nCircleRadius * 2;
	int nHeight = nCircleRadius * 2;
	int nBpp = 8;

	m_image_rect.Create(nWidth, -nHeight, nBpp);
	if (nBpp == 8)
	{
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;

		m_image_rect.SetColorTable(0, 256, rgb);
	}

	int nPitch = m_image_rect.GetPitch();
	unsigned char* fm = (unsigned char*)m_image_rect.GetBits();

	memset(fm, 0xff, nWidth * nHeight);

	for (int j = 0; j < nCircleRadius * 2; j++)
	{
		for (int i = 0; i < nCircleRadius * 2; i++)
		{
			int dx = nCircleRadius - i;
			int dy = nCircleRadius - j;

			if (validImgPos(i, j) && (dx * dx + dy * dy <= nCircleRadius * nCircleRadius))
			{
				fm[j*nPitch + i] = 0;
			}
		}
	}

	CClientDC dc(this);
	m_image_rect.Draw(dc, x - nCircleRadius, y - nCircleRadius);

	mouseXY[nCircleCount++] = { x, y };

	std::cout << x << " : " << y << "\n";
}

void CDlgImage::MakeCircle()
{
	std::pair<int, int> centerXY = { 0,0 };//중심좌표

	double A = (double)mouseXY[1].first - mouseXY[0].first;
	double B = (double)mouseXY[1].second - mouseXY[0].second;
	double C = (double)mouseXY[2].first - mouseXY[0].first;
	double D = (double)mouseXY[2].second - mouseXY[0].second;
	double E = A * (mouseXY[0].first + mouseXY[1].first) + B * (mouseXY[0].second + mouseXY[1].second);
	double F = C * (mouseXY[0].first + mouseXY[2].first) + D * (mouseXY[0].second + mouseXY[2].second);
	double G = 2.0 * (A * (mouseXY[2].second - mouseXY[1].second) - B * (mouseXY[2].first - mouseXY[1].first));

	if (G == 0.0)
	{
		centerXY = mouseXY[0];
	}
	else
	{
		int tempX, tempY;
		tempX = (D * E - B * F) / G;
		tempY = (A * F - C * E) / G;

		centerXY = { tempX, tempY };
	}

	int nRadius = sqrt(((double)mouseXY[0].first - centerXY.first)*((double)mouseXY[0].first - centerXY.first) + ((double)mouseXY[0].second - centerXY.second) * ((double)mouseXY[0].second - centerXY.second));

	//원그리기

	CClientDC dc(this);
	dc.Ellipse(centerXY.first - nRadius, centerXY.second - nRadius, centerXY.first + nRadius, centerXY.second + nRadius);
	Sleep(1000);
}

void CDlgImage::MakeRandomCircle(std::pair<int, int> p_array[])
{
	for (int i = 0; i < 3; i++)
	{
		MakeCircle(p_array[i].first, p_array[i].second);
	}
	MakeCircle();
}

int CDlgImage::GetnCircleCount()
{
	return nCircleCount;
}

void CDlgImage::SetnCircleCount(int val)
{
	nCircleCount = val;
}
